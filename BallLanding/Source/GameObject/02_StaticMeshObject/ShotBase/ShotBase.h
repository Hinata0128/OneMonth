#pragma once
#include "..//StaticMeshObject.h"

/*********************************************************
*	弾の基底クラス.
*	これを親クラスに弾の管理をする.
**/

class Timer;

class ShotBase
	: public StaticMeshObject
{
public:
	ShotBase();
	virtual ~ShotBase() override;

	virtual void Update() override;
	virtual void Draw() override;

	//弾の発射する関数(初期化).
	virtual void Launch(const D3DXVECTOR3& Pos,
		const D3DXVECTOR3& Vel,
		float Radius,
		float Life);

public:
	//ゲット・セット関数.
	bool Active() const;
	void Deactivate();
	//弾の半径を取得する関数.
	float GetRadius() const;

protected:
	//派生先のクラスでもアクセスできる変数設定.
	//弾の速度(方向*スピード).
	D3DXVECTOR3 m_Velocity;
	//当たり判定の半径.
	float		m_Radius;
	//弾の寿命.
	float		m_LifeTime;
	//弾の生存フラグ.
	bool		m_Active;
};