#pragma once
#include "..//ShotBase.h"
#include "Collision//BoundingSphere//BoundingSphere.h"

/************************************************************
*	プレイヤーの放物攻撃.
**/

class StaticMeshManager;
class CollisionManager;
class Timer;

class PlayerLobShot final
	: public ShotBase
{
public:
	PlayerLobShot();
	~PlayerLobShot() override;

	//動作関数.
	void Update() override;
	//描画関数.
	void Draw() override;
	//弾の発射関数(初期化).
	void Launch(const D3DXVECTOR3& Pos,
		const D3DXVECTOR3& Vel,
		float Radius,
		float Life) override;

	//弾を強制的に消す.
	void ShotKill()
	{
		//独自フラグを確実に死亡にする.
		m_IsDead = true; 
		if (m_pCollider) m_pCollider->SetDead(true);
	}

	bool IsActive() const { return !m_IsDead; }
	std::shared_ptr<BoundingSphere> GetCollider() const { return m_pCollider; }

	//二次ベジェ曲線の座標を計算する関数.
	D3DXVECTOR3 CalculateBezier2D(const D3DXVECTOR3& p0, const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, float t);

private:
	//当たり判定のコライダー.
	std::shared_ptr<BoundingSphere> m_pCollider;
	bool m_IsDead;

	//二次ベジェ曲線に必要になる点と時間進行の管理変数.
	D3DXVECTOR3 m_P0;		//始点(プレイヤー位置).
	D3DXVECTOR3 m_P1;		//制御点(頂点).
	D3DXVECTOR3 m_P2;		//終点(目的着弾地点).
	float m_TimeProgress;	//0.0f ~ 1.0fまで進む進行度.
};