#include "JabaranShot.h"
#include "System\02_Singleton//00_Manager\01_StaticMeshManager\StaticMeshManager.h"
#include "..//..//..//..//System/02_Singleton/00_Manager/03_CollisionManager/CollisionManager.h"
#include "..//..//01_Character/Player/Player.h"


JabaranShot::JabaranShot()
	: ShotBase		()
	, m_pCollider	( nullptr )
	, m_IsDead		( false )
{
	auto pStaticMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::SphereCollision);

	AttachMesh(*pStaticMesh);

	D3DXVECTOR3 Scale = { 0.5f, 0.5f, 0.5f };
	SetScale(Scale);

	this->SetLightEnable(false);
}

JabaranShot::~JabaranShot()
{
}

void JabaranShot::Update()
{
	if (m_pCollider && m_pCollider->IsDead())
	{
		m_IsDead = true;
	}

	//すでに死亡しているならこれ以降の前進処理をさせず終了.
	if (!IsActive())
	{
		return;
	}

	//ここで初めて親クラスの前進処理や寿命カウントが走る.
	ShotBase::Update();

	//時間経過による親クラス側の寿命切れをここでキャッチしてフラグを倒す.
	if (ShotBase::Active() == false)
	{
		m_IsDead = true;
	}

	if (m_pCollider != nullptr)
	{
		m_pCollider->SetPosition(m_Position);
		if (m_IsDead)
		{
			m_pCollider->SetDead(true);
		}
	}
}

void JabaranShot::Draw()
{
	if (m_IsDead) return;

	// 親クラスの描画処理を呼ぶ
	ShotBase::Draw();
}

void JabaranShot::Launch(const D3DXVECTOR3& Pos, const D3DXVECTOR3& Vel, float Radius, float Life)
{
	m_IsDead = false;

	ShotBase::Launch(Pos, Vel, Radius, Life);

	m_Position = Pos;

	Player* pPlayer = CollisionManager::GetInstance()->GetPlayer();
	if (pPlayer != nullptr)
	{
		D3DXVECTOR3 playerPos = pPlayer->GetPosition();

		D3DXVECTOR3 dir = playerPos - Pos;
		D3DXVec3Normalize(&dir, &dir);

		float speed = D3DXVec3Length(&Vel);
		if (speed < 0.001f) speed = 5.0f;

		m_Velocity = dir * speed;
	}
	else
	{
		m_Velocity = Vel;
	}
	// コライダーの生成と登録
	auto pStaticMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::PlayerShot);
	m_pCollider = std::make_shared<BoundingSphere>();
	m_pCollider->SetTag(BoundingSphere::Tag::JabaranShot); 
	m_pCollider->CreateSphereForMesh(*pStaticMesh);
	
	// スケールを考慮した半径の設定
	float adjustedRadius = Radius * GetScale().x;
	m_pCollider->SetRadius(adjustedRadius);
	
	// コライダーマネージャーに登録する
	CollisionManager::GetInstance()->AddSphere(m_pCollider);
}
