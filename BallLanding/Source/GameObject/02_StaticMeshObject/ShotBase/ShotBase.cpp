#include "ShotBase.h"
#include "System//02_Singleton//01_Timer//Timer.h"

namespace number
{
	//数字の数値を設定する
	const float MIN_LIFE_TIME = 0.0f;
}

ShotBase::ShotBase()
	: StaticMeshObject	()
	, m_Velocity		(0.0f, 0.0f, 0.0f)
	, m_Radius			(1.0f)
	, m_LifeTime		(0.0f)
	, m_Active			(false)
{
}

ShotBase::~ShotBase()
{
}

void ShotBase::Update()
{
	//非表示中なら何もしない.
	if (m_Active == false)
	{
		return;
	}
	//弾の移動処理.
	//デルタタイムの取得.
	float deltaTime = Timer::GetInstance().DeltaTime();
	AddPosition(m_Velocity * deltaTime);
	//弾の寿命のカウントダウン.
	m_LifeTime -= deltaTime;
	//寿命が尽きたら非アクティブ(消滅)にする.
	if (m_LifeTime <= number::MIN_LIFE_TIME)
	{
		m_Active = false;
	}

	StaticMeshObject::Update();
}

void ShotBase::Draw()
{
	StaticMeshObject::Draw();
}

void ShotBase::Launch(const D3DXVECTOR3& Pos, const D3DXVECTOR3& Vel, float Radius, float Life)
{
	//座標のセット.
	SetPosition(Pos);
	//飛ぶ方向と速度の設定.
	m_Velocity	= Vel;
	//当たり判定の大きさを設定.
	m_Radius	= Radius;
	//弾の寿命の設定.
	m_LifeTime	= Life;
	//弾の表示非表示を設定.
	m_Active	= true;
}

bool ShotBase::Active() const
{
	return m_Active;
}

void ShotBase::Deactivate()
{
	m_Active = false;
}

float ShotBase::GetRadius() const
{
	return m_Radius;
}
