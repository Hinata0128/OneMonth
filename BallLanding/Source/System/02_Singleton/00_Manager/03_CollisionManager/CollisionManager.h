#pragma once
#include "System//02_Singleton//01_Timer//Timer.h"
#include "Collision//BoundingBox//BoundingBox.h"
#include "Collision//BoundingSphere//BoundingSphere.h"

//当たり判定に使用するクラスをインクルード.
#include "System//02_Singleton//00_Manager//ManagerBase.h"


#include "System/02_Singleton/02_Score/Score.h"
#include "GameObject\02_StaticMeshObject\01_Character\Player\Player.h"

/******************************************************************************
*	当たり判定マネージャークラス.
**/

class Camera;

//衝突判定の詳細結果を格納する.
struct CollisionResult
{
	bool IsHit;		//当たったか?
	D3DXVECTOR3 PushVector; //押し戻すためのベクトル.
};

class CollisionManager final
	: public ManagerBase<CollisionManager>
{
public:
	//基底クラスの GetInstance()にだけ生成を許可する.
	friend class ManagerBase<CollisionManager>;

	CollisionManager();
	~CollisionManager() override;


	void Update() override;
	void Draw() override;
	void Create() override;
	void Release() override;

	//コライダーの登録関数.
	void AddSphere(const std::shared_ptr<BoundingSphere>& sphere);
	void AddBox(const std::shared_ptr<BoundingBox>& box);
	//登録されているすべてのコライダーを削除する.
	void Clear();

	//当たり判定を常に起動しておく関数.
	void AllCollider();

	const std::vector<std::shared_ptr<BoundingBox>>& GetBoxes() const { return m_pBBox; }
	const std::vector<std::shared_ptr<BoundingSphere>>& GetSpheres() const { return m_pBSphere; }

	void RemoveSphere(const std::shared_ptr<BoundingSphere>& sphere) {
		auto it = std::find(m_pBSphere.begin(), m_pBSphere.end(), sphere);
		if (it != m_pBSphere.end()) {
			m_pBSphere.erase(it);
		}
	}
public:

	void SetShortAttackSphere(const std::shared_ptr<BoundingSphere>& sphere) { m_pShortAttackSphere = sphere; }

	CollisionResult CheckSphereBoxDetailed(const BoundingSphere& sphere, const BoundingBox& box);

	void HandleGoal(std::shared_ptr<BoundingSphere> a, std::shared_ptr<BoundingSphere> b);

	void SetTargetScore(std::shared_ptr<Score> score) { m_pTargetScore = score; }
	// 点数を設定するための関数を追加
	void SetScoreValue(int value) { m_ScoreValue = value; }

	void SetPlayer(Player* p) { m_upPlayer = p; }
	Player* GetPlayer() const { return m_upPlayer; }
private:
	//当たり判定の関数をここに書きます
	bool CheckSphereSphere(const BoundingSphere& a, const BoundingSphere& b);	//スフィア同士の判定. 
	bool CheckBoxBox(const BoundingBox& a, const BoundingBox& b);				//AABB同士の判定. 
	bool CheckSphereBox(const BoundingSphere& sphere, const BoundingBox& box);	//スフィアとAABBの判定. 

private:

	std::vector<std::shared_ptr<BoundingSphere>>	m_pBSphere;
	std::vector<std::shared_ptr<BoundingBox>>		m_pBBox;
	
	std::shared_ptr<BoundingSphere> m_pShortAttackSphere;
	std::shared_ptr<Score> m_pTargetScore;
	int m_ScoreValue = 0;
private:

	bool m_isPlayerHit = false;

	bool IsPlayerHit() const { return m_isPlayerHit; }

	//エフェクトのクールタイム.
	float m_BossHitEffectCooldown = 0.0f;

	Camera* m_pCamera = nullptr;

	//初期化をいったんここでしている.
	Player* m_upPlayer = nullptr;

	float m_PlayerAttackEffectTimer = 0.0f;
public:

	void SetCamera(Camera* pCamera) { m_pCamera = pCamera; }
};