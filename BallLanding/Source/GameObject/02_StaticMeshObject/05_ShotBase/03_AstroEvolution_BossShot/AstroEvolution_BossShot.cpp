#include "AstroEvolution_BossShot.h"
#include "System/00_Manager/03_CollisionManager/CollisionManager.h"

AstroEvolution_BossShot::AstroEvolution_BossShot()
{
    auto pMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Ball);
    if (pMesh)
    {
        AttachMesh(*pMesh);
    }

    D3DXVECTOR3 Scale = D3DXVECTOR3(10.0f, 10.0f, 10.0f);
    SetScale(Scale);


    m_BSphere = std::make_shared<BoundingSphere>();
    m_BSphere->SetRadius(1.5f); // 避けやすさを考慮して少し小さめに設定

    // 🌟 敵の弾として登録（CollisionManagerでプレイヤーと判定させるため）
    m_BSphere->SetTag(BoundingSphere::Tag::AstroBossShot);

    CollisionManager::GetInstance()->AddSphere(m_BSphere);
    Init();
}

AstroEvolution_BossShot::~AstroEvolution_BossShot()
{
    // 破棄時にエフェクトが残らないようにする
    if (m_EffectHandle != -1) {
        Effect::GetInstance()->Stop(m_EffectHandle);
    }
}

void AstroEvolution_BossShot::Init() {
    if (m_EffectHandle != -1) {
        Effect::GetInstance()->Stop(m_EffectHandle);
        m_EffectHandle = -1;
    }
    m_IsActive = false;
    m_LifeTime = 0.0f;

}

void AstroEvolution_BossShot::Reload(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir, float speed) {
    m_Position = pos;
    m_Direction = dir;
    m_Speed = speed;
    m_IsActive = true;
    m_LifeTime = 0.0f;

    if (m_BSphere) {
        m_BSphere->SetPosition(m_Position);
    }

    // 🌟 敵らしいエフェクト（例：Laser02 や Explosion系）を再生
    // お持ちのエフェクトリストに合わせて変更してください
    m_EffectHandle = Effect::GetInstance()->Play(Effect::Laser01, m_Position);
}

void AstroEvolution_BossShot::Update() {
    if (!m_IsActive) return;

    // 1. 移動
    m_Position += m_Direction * m_Speed;

    // 2. エフェクト更新
    if (m_EffectHandle != -1) {
        Effect::SetLocation(m_EffectHandle, m_Position);
        Effect::SetRotation(m_EffectHandle, m_Direction);
    }

    // 3. 当たり判定更新
    if (m_BSphere) {
        m_BSphere->SetPosition(m_Position);
    }

    // 4. 寿命管理
    m_LifeTime += 1.0f;
    if (m_LifeTime > MAX_LIFE_TIME) {
        Init();
    }

    StaticMeshObject::Update();
}

void AstroEvolution_BossShot::OnHit()
{
    // 火花などのヒット演出を入れてもOK
    // Effect::GetInstance()->Play(Effect::Explosion01, m_Position);
    Init();
}

void AstroEvolution_BossShot::Draw() 
{
    StaticMeshObject::Draw();
#if _DEBUG
    if (m_IsActive && m_BSphere) m_BSphere->Draw();
#endif
}