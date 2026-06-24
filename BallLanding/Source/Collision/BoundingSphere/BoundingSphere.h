#pragma once

#include "StaticMash//StaticMesh.h"

/**********************************************************************************
*	バウンディングスフィアクラス(当たり判定クラス).
**/

class BoundingSphere
{
public:
	//当たり判定をしようするクラスの当たり判定名をここに登録する.
	enum class Tag : byte
	{
		None,
		Player,			//プレイヤー.
		PlayerShot,		//プレイヤーの弾.
		PlayerLobShot,	//プレイヤーの弾の放物線攻撃.
		PlayerLobExplosion,	//プレイヤーの弾の爆発
		Jabaran,		//敵.
		JabaranShot,	//敵の弾.

	};
public:
	BoundingSphere();
	~BoundingSphere();

	//モデルに合わせたバウンディングスフィアを作成.
	//引数に&がある時はその時のアドレスを手に入れることが確実にできる.
	HRESULT CreateSphereForMesh(const StaticMesh& pMesh);

	//他の球体との当たり判定.
	bool IsHit(const BoundingSphere& pBSphere) const;

	//中心座標を取得する.
	const D3DXVECTOR3& GetPostion() const { return m_Position; }
	//半径(長さ)を取得する.
	float GetRadius() const { return m_Radius; }

	//中心座標を設定する.
	void SetPosition(const D3DXVECTOR3& Pos) { m_Position = Pos; }
	//半径(長さ)を設定する.
	void SetRadius(float Radius) { m_Radius = Radius; }
	
	void Update();
	void Draw();

	//ターゲット.
	void SetTag(Tag tag) { m_Tag = tag; }
	Tag GetTag() const { return m_Tag; }

	//スコア加算済みフラグ
	void SetProcessed(bool flag) { m_isProcessed = flag; }
	bool IsProcessed() const { return m_isProcessed; }
	//玉を消すときに使用.
	void SetDead(bool dead) { m_isDead = dead; }
	bool IsDead() const { return m_isDead; }

	void OnHitResponse(const D3DXVECTOR3& pushVec) 
	{
		m_Position += pushVec;
	}

	void SetLifeTime(float duration) { m_LifeTime = duration; }
	float GetLifeTime() const { return m_LifeTime; }
	void DecreaseLifeTime(float dt) { if (m_LifeTime > 0.0f) m_LifeTime -= dt; }

private:
	StaticMesh*		m_pMesh;
	D3DXVECTOR3		m_Position;	//中心座標.
	float			m_Radius;	//半径(長さ).

	Tag m_Tag;
	bool m_isProcessed;
	bool m_isDead;
	float m_LifeTime;
};