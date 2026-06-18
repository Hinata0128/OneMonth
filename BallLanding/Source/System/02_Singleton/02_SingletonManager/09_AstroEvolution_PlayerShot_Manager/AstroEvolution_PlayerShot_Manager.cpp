#include "AstroEvolution_PlayerShot_Manager.h"

AstroEvolution_PlayerShot_Manager::AstroEvolution_PlayerShot_Manager()
    : ManagerBase()
{
}

AstroEvolution_PlayerShot_Manager::~AstroEvolution_PlayerShot_Manager()
{
    Release();
}

void AstroEvolution_PlayerShot_Manager::Update()
{
    // 1. 各弾の更新
    for (auto& shot : m_pShots)
    {
        if (shot->GetIsActive()) {
            shot->Update();
        }
    }

    // 2. 非アクティブ（画面外や着弾済み）になった弾をリストから削除
    m_pShots.erase(
        std::remove_if(m_pShots.begin(), m_pShots.end(),
            [](const std::unique_ptr<AstroEvolution_PlayerShot>& s) {
                return !s->GetIsActive();
            }),
        m_pShots.end()
    );
}

void AstroEvolution_PlayerShot_Manager::Draw()
{
    for (auto& shot : m_pShots)
    {
        if (shot->GetIsActive()) {
            shot->Draw();
        }
    }
}

void AstroEvolution_PlayerShot_Manager::Create()
{
    m_pShots.clear();
}

void AstroEvolution_PlayerShot_Manager::Release()
{
    m_pShots.clear();
}

void AstroEvolution_PlayerShot_Manager::PlayerFire(const D3DXVECTOR3& pos, float angleY, int shotLevel)
{
    // 弾を生成して発射するラムダ関数
    auto Spawn = [&](float offsetAngle, D3DXVECTOR3 offsetPos = { 0,0,0 }) 
        {
        auto newShot = std::make_unique<AstroEvolution_PlayerShot>();

        // 🌟 正しい向きベクトルを計算 (X=sin, Z=cos)
        float finalAngle = angleY + offsetAngle;
        D3DXVECTOR3 finalDir(sinf(finalAngle), 0.0f, cosf(finalAngle));

        // 弾を初期化 (引数: 発射位置, 向き, 速度)
        newShot->Reload(pos + offsetPos, finalDir, 0.5f);

        m_pShots.push_back(std::move(newShot));
        };

    switch (shotLevel)
    {
    case 1: Spawn(0.0f); break;

    case 2: // 2連装：左右に並行
    {
        float dist = 0.3f;
        // プレイヤーの右方向ベクトルを計算
        D3DXVECTOR3 rightDir(cosf(angleY), 0.0f, -sinf(angleY));
        Spawn(0.0f, rightDir * dist);
        Spawn(0.0f, -rightDir * dist);
    }
    break;

    case 3:
    default: // 3-Way
        Spawn(0.0f);
        Spawn(D3DXToRadian(15.0f)); // 右斜め
        Spawn(-D3DXToRadian(15.0f)); // 左斜め
        break;
    }
}