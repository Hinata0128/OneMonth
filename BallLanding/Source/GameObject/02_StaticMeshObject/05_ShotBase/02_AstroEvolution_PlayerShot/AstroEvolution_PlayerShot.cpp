#include "AstroEvolution_PlayerShot.h"
#include "System/00_Manager/03_CollisionManager/CollisionManager.h"

AstroEvolution_PlayerShot::AstroEvolution_PlayerShot() 
{
    auto pMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Ball);
    if (pMesh)
    {
        AttachMesh(*pMesh);
    }
    //見た目の大きさ（プレイヤーの弾）
    m_Scale = D3DXVECTOR3(5.0f, 5.0f, 5.0f);
    SetScale(m_Scale);
    m_BSphere = std::make_shared<BoundingSphere>();
    m_BSphere->SetRadius(2.0f); // 当たり判定の大きさ
    m_BSphere->SetTag(BoundingSphere::Tag::AstroPlayerShot);
    CollisionManager::GetInstance()->AddSphere(m_BSphere);
    Init();
}

AstroEvolution_PlayerShot::~AstroEvolution_PlayerShot() 
{
}

void AstroEvolution_PlayerShot::Init() {
    // すでにエフェクトが再生中なら停止させる
    if (m_EffectHandle != -1) {
        Effect::GetInstance()->Stop(m_EffectHandle);
        m_EffectHandle = -1;
    }
    m_IsActive = false;
    m_LifeTime = 0.0f;
}

void AstroEvolution_PlayerShot::Reload(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir, float speed) {
    m_Position = pos;
    m_Direction = dir;
    m_Speed = speed;
    m_IsActive = true;
    m_LifeTime = 0.0f;

    // ★ エフェクト再生開始
    // 例: Laser01 エフェクトを再生し、そのハンドルを保存
    m_EffectHandle = Effect::GetInstance()->Play(Effect::Laser01, m_Position);
}

void AstroEvolution_PlayerShot::Update() {
    if (!m_IsActive) return;

    // 1. 座標移動
    m_Position += m_Direction * m_Speed;

    // 2. ★ エフェクトの位置を更新 (SetPosition ではなく SetLocation を使う)
    if (m_EffectHandle != -1) {
        // static関数として定義されているので、Effect::SetLocation(handle, pos) でOKです
        Effect::SetLocation(m_EffectHandle, m_Position);

        // 向きも同様に static関数の SetRotation を使います
        Effect::SetRotation(m_EffectHandle, m_Direction);
    }

    // 3. 当たり判定の位置を更新
    if (m_BSphere) {
        m_BSphere->SetPosition(m_Position + m_Direction * 1.0f);
    }

    // 4. 寿命消去
    m_LifeTime += 1.0f;
    if (m_LifeTime > 180.0f) Init();

    StaticMeshObject::Update();
}

void AstroEvolution_PlayerShot::OnHit() 
{
    if (m_BSphere) {
        m_BSphere->SetDead(true);
    }
    Init();
}

void AstroEvolution_PlayerShot::Draw() 
{
    StaticMeshObject::Draw();

#if _DEBUG
    if (m_IsActive && m_BSphere) m_BSphere->Draw();
#endif
}