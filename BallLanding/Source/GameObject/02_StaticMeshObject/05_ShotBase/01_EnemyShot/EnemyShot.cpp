#include "EnemyShot.h"
#include "System//00_Manager//01_StaticMeshManager//StaticMeshManager.h"
#include "System/00_Manager/03_CollisionManager/CollisionManager.h"

EnemyShot::EnemyShot()
    : ShotBase          ()
{
    AttachMesh(*StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::EnemyBullet));

    m_pSphere = std::make_shared<BoundingSphere>();
    m_pSphere->SetRadius(0.3f);

    CollisionManager::GetInstance()->AddSphere(m_pSphere);

    Init();
}

EnemyShot::~EnemyShot() 
{
}

void EnemyShot::Update()
{
    constexpr float MaxRange = 9999.0f;
    if (!m_IsActive) return;

    if (m_Disp)
    {
        m_Position += m_Direction * m_Speed;

        // 当たり判定の位置を同期
        if (m_pSphere) {
            m_pSphere->SetPosition(m_Position);
        }

        // 弾が一定距離飛ぶか、コライダー側で「死亡(当たった)」と判定されたら消す
        if (D3DXVec3Length(&m_Position) >= MaxRange || (m_pSphere && m_pSphere->IsDead()))
        {
            Init();
        }
    }
}

void EnemyShot::Draw()
{
    ShotBase::Draw();
#ifdef _DEBUG
    m_pSphere->Draw();
#endif
}

void EnemyShot::Init()
{
    ShotBase::Init();
    m_Disp = false;
    m_IsActive = false;
    m_pSphere->SetDead(true);
    m_Scale = D3DXVECTOR3(3.0f, 3.0f, 3.0f);
}

void EnemyShot::Reload(const D3DXVECTOR3& pos, const D3DXVECTOR3& direction, float speed)
{
    m_Position = pos;
    m_Direction = direction;
    m_Speed = speed;
    m_Disp = true;
    m_IsActive = true;

    if (m_pSphere)
    {
        m_pSphere->SetPosition(m_Position);
        m_pSphere->SetDead(false); // 弾が再利用されるときに判定を復活させる
    }
}

