#include "Jabaran.h"

#include "System//02_Singleton//00_Manager//01_StaticMeshManager//StaticMeshManager.h"
#include "..//..//..//..//System/02_Singleton/00_Manager/03_CollisionManager/CollisionManager.h"
#include "..//..//..//..//System/02_Singleton/00_Manager/05_JabaranShotManager/JabaranShotManager.h"

namespace ColliderPosNumber
{
	float ColliderSettingPos = 1.5f;
}

Jabaran::Jabaran()
	: Character			()
	, m_pCollider		( nullptr )
	, m_Dead			( false )
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
			m_Dead = true;
		}
	}

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		//初期パラメータの設定.
		//敵の位置取得.
		D3DXVECTOR3 StartPos = m_Position;
		//弾の速度の計算.
		D3DXVECTOR3 JabaranTargetDir = { 0.0f, 0.0f, 0.0f };
		D3DXVECTOR3 Velocity = JabaranTargetDir * 50.0f;
		//弾の大きさ.
		float Radius = 1.0f;
		//弾の寿命.
		float Life = 3.0f;
		//生成直後弾の設定可能.
		JabaranShotManager::GetInstance()->Launch(StartPos, Velocity, Radius, Life);
	}

	JabaranShotManager::GetInstance()->Update();

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

	JabaranShotManager::GetInstance()->Draw();

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
	return m_Dead;
}
