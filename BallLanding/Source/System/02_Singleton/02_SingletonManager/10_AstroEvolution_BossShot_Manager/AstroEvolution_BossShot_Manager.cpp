#include "AstroEvolution_BossShot_Manager.h"

AstroEvolution_BossShot_Manager::AstroEvolution_BossShot_Manager()
    : ManagerBase()
{
}

AstroEvolution_BossShot_Manager::~AstroEvolution_BossShot_Manager()
{
    Release();
}

void AstroEvolution_BossShot_Manager::Update()
{
    // 1. 各弾の更新
    for (auto& shot : m_pShots)
    {
        if (shot->GetIsActive()) {
            shot->Update();
        }
    }

    // 2. 非アクティブになった弾を削除（寿命切れや着弾）
    m_pShots.erase(
        std::remove_if(m_pShots.begin(), m_pShots.end(),
            [](const std::unique_ptr<AstroEvolution_BossShot>& s) {
                return !s->GetIsActive();
            }),
        m_pShots.end()
    );
}

void AstroEvolution_BossShot_Manager::Draw()
{
    for (auto& shot : m_pShots)
    {
        if (shot->GetIsActive()) {
            shot->Draw();
        }
    }
}

void AstroEvolution_BossShot_Manager::Create()
{
    m_pShots.clear();
}

void AstroEvolution_BossShot_Manager::Release()
{
    m_pShots.clear();
}

void AstroEvolution_BossShot_Manager::BossFireWay(const D3DXVECTOR3& pos, const D3DXVECTOR3& baseDir, int numShots, float angleStep)
{
    // 中心から左右に広げるための開始オフセット角度
    float startAngle = -(numShots - 1) * angleStep / 2.0f;

    for (int i = 0; i < numShots; ++i)
    {
        float currentAngle = startAngle + (i * angleStep);

        // Y軸回転行列を作成して方向を計算
        D3DXMATRIX matRot;
        D3DXMatrixRotationY(&matRot, currentAngle);

        D3DXVECTOR3 shotDir;
        D3DXVec3TransformNormal(&shotDir, &baseDir, &matRot);

        // 弾を生成
        auto newShot = std::make_unique<AstroEvolution_BossShot>();

        // 発射位置はボスの座標 + 少し前方にオフセット
        D3DXVECTOR3 spawnPos = pos + shotDir * 2.0f;

        // 弾を初期化 (引数: 発射位置, 向き, 速度)
        newShot->Reload(spawnPos, shotDir, 0.4f);

        m_pShots.push_back(std::move(newShot));
    }
}