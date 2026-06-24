#pragma once
#include "..//ShotBase.h"
#include "Collision//BoundingSphere//BoundingSphere.h"

/**********************************************************
*	プレイヤーの通常攻撃.
**/

class StaticMeshManager;
class CollisionManager;

class PlayerShot
	: public ShotBase
{
public:
	//コンストラクタ.
	PlayerShot();
	//デストラクタ.
	~PlayerShot() override;

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
		m_IsDead = true; //独自フラグを確実に死亡にする
		if (m_pCollider) m_pCollider->SetDead(true);
	}

	bool IsActive() const 
	{ 
		return !m_IsDead; 
	}
	std::shared_ptr<BoundingSphere> GetCollider() const 
	{ 
		return m_pCollider; 
	}
private:
	//当たり判定のコライダー.
	std::shared_ptr<BoundingSphere> m_pCollider;
	bool m_IsDead;
};