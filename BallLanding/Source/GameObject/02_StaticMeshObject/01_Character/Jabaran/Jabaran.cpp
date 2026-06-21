#include "Jabaran.h"

#include "System//02_Singleton//00_Manager//01_StaticMeshManager//StaticMeshManager.h"
#include "..//..//..//..//System/02_Singleton/00_Manager/03_CollisionManager/CollisionManager.h"
#include "..//..//ShotBase/JabaranShot/JabaranShot.h"

namespace ColliderPosNumber
{
	float ColliderSettingPos = 1.5f
}

Jabaran::Jabaran()
	: Character			()
	, m_pCollider		( nullptr )
	, m_upJabaranShot	( nullptr )
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
	m_pCollider->SetTag(BoundingSphere::Tag::Jabaran);
	m_pCollider->CreateSphereForMesh(*pStaticMesh);

	float adjustedRadius = m_pCollider->GetRadius() * 0.005f;
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


	if (m_pCollider != nullptr)
	{
		//敵の座標を常にコライダーに同期.
		D3DXVECTOR3 ColliderPos = m_Position;
		ColliderPos.y += ColliderPosNumber::ColliderSettingPos;
		m_pCollider->SetPosition(ColliderPos);

		if (m_pCollider->IsDead())
		{
			// TODO: 敵の死亡フラグを立てる（例：m_isDead = true; など）
			// ここで敵自身を消滅、または死亡アニメーションに移行させてください
		}
	}

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		// すでに弾が存在していて活動中なら新しく撃たない（単体テスト用制限）
		if (m_upJabaranShot == nullptr || !m_upJabaranShot->IsActive())
		{
			m_upJabaranShot = std::make_unique<JabaranShot>();

			D3DXVECTOR3 baseVel = { 0.0f, 0.0f, 5.0f }; // スピード5.0f
			m_upJabaranShot->Launch(m_Position, baseVel, 1.0f, 3.0f);
		}
	}

	if (m_upJabaranShot != nullptr)
	{
		m_upJabaranShot->Update();
	}

	Character::Update();
}

void Jabaran::Draw()
{
	//死んでいたら描画しない.
	if (GetDead())
	{
		return;
	}

	Character::Draw();

	if (m_upJabaranShot != nullptr && m_upJabaranShot->IsActive())
	{
		m_upJabaranShot->Draw();
	}
#ifdef _DEBUG
	if (m_pCollider)
	{
		m_pCollider->Draw();
	}
#endif
}

void Jabaran::Init()
{
}

bool Jabaran::GetDead() const
{
	return false;
}
