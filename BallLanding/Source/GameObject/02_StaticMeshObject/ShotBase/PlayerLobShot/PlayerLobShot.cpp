#include "PlayerLobShot.h"
#include "System//02_Singleton//00_Manager//01_StaticMeshManager//StaticMeshManager.h"
#include "..//..//..//..//System//02_Singleton//00_Manager//03_CollisionManager//CollisionManager.h"
#include "..//..//..//..//System//02_Singleton//01_Timer//Timer.h"

PlayerLobShot::PlayerLobShot()
	: ShotBase			()
	, m_pCollider		( nullptr )
	, m_IsDead			( false )
	, m_P0				( 0.0f, 0.0f, 0.0f )
	, m_P1				( 0.0f, 0.0f, 0.0f )
	, m_P2				( 0.0f, 0.0f, 0.0f )
	, m_TimeProgress	( 0.0f )
{
	auto pStaticMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::SphereCollision);
	AttachMesh(*pStaticMesh);

	D3DXVECTOR3 Scale = { 0.5f, 0.5f , 0.5f };
	SetScale(Scale);
	this->SetLightEnable(false);
}

PlayerLobShot::~PlayerLobShot()
{
	if (m_pCollider != nullptr)
	{
		m_pCollider->SetDead(true);
	}
}

void PlayerLobShot::Update()
{
#if 0 
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
	//毎フレーム経過時間を蓄積し、0.0f(始点) ~ 1.0f(終点)までの進行度を作成.
	if (m_LifeTime > 0.0f)
	{
		m_TimeProgress += Timer::GetInstance().DeltaTime() / m_LifeTime;
	}

	//二次ベジェ曲線から現在のフレームの正確な座標を割り出して反映する.
	D3DXVECTOR3 CurrentPos = CalculateBezier2D(m_P0, m_P1, m_P2, m_TimeProgress);
	SetPosition(CurrentPos);

	//進行度が1.0(着弾点)に達したら寿命切れとする.
	if (m_TimeProgress >= 1.0f)
	{
		m_IsDead = true;
		//爆発用のコライダーの生成.
		auto Explosion = std::make_shared<BoundingSphere>();
		Explosion->SetTag(BoundingSphere::Tag::PlayerLobExplosion);
		Explosion->SetPosition(m_P2);
		Explosion->SetRadius(15.0f);

		Explosion->SetLifeTime(0.5f);
		CollisionManager::GetInstance()->AddSphere(Explosion);
	}
	//コライダーの位置を弾のグラフィックの座標に同期.
	if (m_pCollider != nullptr)
	{
		m_pCollider->SetPosition(m_Position);
		if (m_IsDead)
		{
			m_pCollider->SetDead(true);
		}
	}
#else
	// コライダー側が死んでいたら、この弾自身も死亡状態にする.
	if (m_pCollider && m_pCollider->IsDead())
	{
		m_IsDead = true;
	}
	// すでに死亡しているならこれ以降の前進処理をさせず終了.
	if (!IsActive())
	{
		return;
	}

	// 進行度が1.0未満（まだ飛行中）の場合
	if (m_TimeProgress < 1.0f)
	{
		// 毎フレーム経過時間を蓄積し、0.0f(始点) ~ 1.0f(終点)までの進行度を作成.
		if (m_LifeTime > 0.0f)
		{
			m_TimeProgress += Timer::GetInstance().DeltaTime() / m_LifeTime;
		}

		// 二次ベジェ曲線から現在のフレームの正確な座標を割り出して反映する.
		D3DXVECTOR3 CurrentPos = CalculateBezier2D(m_P0, m_P1, m_P2, m_TimeProgress);
		SetPosition(CurrentPos);

		// コライダーの位置を弾のグラフィックの座標に同期.
		if (m_pCollider != nullptr)
		{
			m_pCollider->SetPosition(m_Position);
		}
	}
	// ★進行度が1.0(着弾点)に達した後の「爆発デバッグ表示」処理
	else
	{
		if (m_pCollider != nullptr)
		{
			// 着弾位置（床）にコライダーを固定
			m_pCollider->SetPosition(m_P2);
			// 爆発確認用に、コライダーの半径を「15.0f」に巨大化！
			m_pCollider->SetRadius(15.0f);
		}

		// デバッグ表示用に30フレーム（約0.5秒）その場に維持する
		static int debugDisplayFrame = 0;
		debugDisplayFrame++;

		if (debugDisplayFrame >= 30)
		{
			m_IsDead = true;
			if (m_pCollider != nullptr)
			{
				m_pCollider->SetDead(true); // 30フレーム経ったらコライダーを消去
			}
			debugDisplayFrame = 0; // カウンターリセット
		}
	}
#endif
	ShotBase::Update();
}

void PlayerLobShot::Draw()
{
	//死んでいるなら絶対にメッシュを描画させない.
	if (!IsActive())
	{
		return;
	}
	ShotBase::Draw();
#ifdef _DEBUG
	//スフィア描画
	m_pCollider->Draw();
#endif
}

void PlayerLobShot::Launch(const D3DXVECTOR3& Pos, const D3DXVECTOR3& Vel, float Radius, float Life)
{
	ShotBase::Launch(Pos, Vel, Radius, Life);

	m_IsDead = false;
	//進行度をリセット.
	m_TimeProgress = 0.0f;

	//ベジェ曲線の3つの制御点を決定する.
	//始点はプレイヤーの位置から.
	m_P0 = Pos;
	//終点(着弾地点).
	m_P2 = Pos + Vel * Life;
	//制御点.
	m_P1 = (m_P0 + m_P2) * 0.5f;
	//高さの設定.
	m_P1.y = 6.0f;
	//当たり判定の生成と登録.
	m_pCollider = std::make_shared<BoundingSphere>();
	m_pCollider->SetTag(BoundingSphere::Tag::PlayerLobShot);
	m_pCollider->SetPosition(Pos);
	m_pCollider->SetRadius(Radius);

	CollisionManager::GetInstance()->AddSphere(m_pCollider);
}

//二次ベジェ曲線の座標計算関数.
D3DXVECTOR3 PlayerLobShot::CalculateBezier2D(const D3DXVECTOR3& p0, const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, float t)
{
	//安全対策.
	if (t < 0.0f)
	{
		t = 0.0f;
	}
	if (t > 1.0f)
	{
		t = 1.0f;
	}

	float U = 1.0f - t;
	float TT = t * t;
	float UU = U * U;

	//数式通りの補間計算.
	D3DXVECTOR3 P = UU * p0 + 2.0f * U * t * p1 + TT * p2;
	return P;
}
