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
	// 🌟 すでに死亡している（または非アクティブ）なら一切の更新処理をスキップ
	if (m_IsDead) return;

	// ShotBase側の移動処理や、m_Lifeの減少処理などを呼び出す
	ShotBase::Update();

	// 🌟【寿命判定の連動】
	// ShotBase側の寿命が尽きた（Active()がfalseになった）場合
	if (!Active())
	{
		ShotKill(); // 確実に m_IsDead = true にする
		return;     // 死亡したので以降の処理はスキップ
	}

	if (m_pCollider != nullptr)
	{
		// 座標をコライダーに同期
		m_pCollider->SetPosition(m_Position);

		// CollisionManager側でコライダーが死んだ（プレイヤーに当たった）時
		if (m_pCollider->IsDead())
		{
			ShotKill();
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
	// 🌟 1. 独自死亡フラグを false にリセット（これで再発射可能になります）
	m_IsDead = false;

	// 2. 基底クラスの Launch を呼んで、m_Life に寿命（3.0fなど）を設定させる
	ShotBase::Launch(Pos, Vel, Radius, Life);

	m_Position = Pos;

	// 3. 自機狙い計算
	Player* pPlayer = CollisionManager::GetInstance()->GetPlayer();
	if (pPlayer != nullptr)
	{
		D3DXVECTOR3 playerPos = pPlayer->GetPosition();

		// 敵（Pos）からプレイヤー（playerPos）への方向ベクトルを計算
		D3DXVECTOR3 dir = playerPos - Pos;
		D3DXVec3Normalize(&dir, &dir);

		// 引数の Vel から移動スピードを割り出す
		float speed = D3DXVec3Length(&Vel);
		if (speed < 0.001f) speed = 5.0f;

		// 計算した自機狙いベクトルを速度に設定
		m_Velocity = dir * speed;
	}
	else
	{
		m_Velocity = Vel;
	}
	// コライダーの生成と登録
	//auto pStaticMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::PlayerShot);
	//m_pCollider = std::make_shared<BoundingSphere>();
	//m_pCollider->SetTag(BoundingSphere::Tag::EnemyShot); // 🌟コライダー側に「敵の弾」のタグが必要な場合は要確認
	//m_pCollider->CreateSphereForMesh(*pStaticMesh);
	//
	//// スケールを考慮した半径の設定
	//float adjustedRadius = Radius * GetScale().x;
	//m_pCollider->SetRadius(adjustedRadius);
	//
	//// コライダーマネージャーに登録する
	//CollisionManager::GetInstance()->AddSphere(m_pCollider);
}
