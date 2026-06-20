#include "Jabaran.h"

#include "System//02_Singleton//00_Manager//01_StaticMeshManager//StaticMeshManager.h"
#include "..//..//..//..//System/02_Singleton/00_Manager/03_CollisionManager/CollisionManager.h"

Jabaran::Jabaran()
	: Character	()
{
	//敵のスタティックメッシュを呼び込む.
	auto pStaticMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Jabaran);
	//設定.
	//ポインタで取得します.
	AttachMesh(*pStaticMesh);
	
	//ポジションの設定.
	D3DXVECTOR3 Pos = { 0.0f,0.0f,10.0f };
	SetPosition(Pos);

	//サイズの変更.
	D3DXVECTOR3 Scale = { 0.01f,0.01f,0.01f };
	SetScale(Scale);
	//光遮断.
	this->SetLightEnable(false);
	Init();

	m_pCollider = std::make_shared<BoundingSphere>();
	m_pCollider->SetTag(BoundingSphere::Tag::Enemy);
	m_pCollider->CreateSphereForMesh(*pStaticMesh);

	float adjustedRadius = m_pCollider->GetRadius() * Scale.x;
	m_pCollider->SetRadius(adjustedRadius);

	CollisionManager::GetInstance()->AddSphere(m_pCollider);
}

Jabaran::~Jabaran()
{
}

void Jabaran::Update()
{
	//もし敵自体が既に死亡しているなら処理しない.
	if (GetDead())
	{
		if (m_pCollider) m_pCollider->SetDead(true); //コライダーも消す.
		return;
	}

	Character::Update();

	if (m_pCollider != nullptr)
	{
		//敵の座標を常にコライダーに同期.
		m_pCollider->SetPosition(m_Position);

		if (m_pCollider->IsDead())
		{
			// TODO: 敵の死亡フラグを立てる（例：m_isDead = true; など）
			// ここで敵自身を消滅、または死亡アニメーションに移行させてください
		}
	}
}

void Jabaran::Draw()
{
	//死んでいたら描画しない.
	if (GetDead())
	{
		return;
	}

	Character::Draw();
#ifdef _DEBUG
	/*if (m_pCollider)
	{
		m_pCollider->Draw();
	}*/
#endif
}

void Jabaran::Init()
{
}

bool Jabaran::GetDead() const
{
	return false;
}
