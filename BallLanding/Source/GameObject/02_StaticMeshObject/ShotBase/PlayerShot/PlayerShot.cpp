#include "PlayerShot.h"
#include "System\02_Singleton//00_Manager\01_StaticMeshManager\StaticMeshManager.h"
#include "..//..//..//..//System/02_Singleton/00_Manager/03_CollisionManager/CollisionManager.h"

PlayerShot::PlayerShot()
	: ShotBase()
	, m_pCollider(nullptr)
	, m_IsDead(false) // 初期化
{
	auto pStaticMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::PlayerShot);
	AttachMesh(*pStaticMesh);

	D3DXVECTOR3 Scale = { 0.5f,0.5f,0.5f };
	SetScale(Scale);
	this->SetLightEnable(false);
}

PlayerShot::~PlayerShot()
{
	if (m_pCollider != nullptr)
	{
		m_pCollider->SetDead(true);
	}
}

void PlayerShot::Update()
{
	//コライダー側が死んでいたら、この弾自身も死亡状態にする.
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

void PlayerShot::Draw()
{
	//死んでいるなら絶対にメッシュを描画しない.
	if (!IsActive())
	{
		return;
	}

	ShotBase::Draw();

#ifdef _DEBUG
	//if (m_pCollider != nullptr && !m_pCollider->IsDead())
	//{
	//	m_pCollider->Draw();
	//}
#endif
}

void PlayerShot::Launch(const D3DXVECTOR3& Pos, const D3DXVECTOR3& Vel, float Radius, float Life)
{
	ShotBase::Launch(Pos, Vel, Radius, Life);

	m_IsDead = false; 

	m_pCollider = std::make_shared<BoundingSphere>();
	m_pCollider->SetTag(BoundingSphere::Tag::PlayerShot);
	m_pCollider->SetPosition(Pos);
	m_pCollider->SetRadius(Radius);

	CollisionManager::GetInstance()->AddSphere(m_pCollider);
}