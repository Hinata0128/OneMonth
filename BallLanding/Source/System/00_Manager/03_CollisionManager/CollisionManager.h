#pragma once
#include "System//02_Singleton//00_Timer//Timer.h"
#include "Collision//BoundingBox//BoundingBox.h"
#include "Collision//BoundingSphere//BoundingSphere.h"

//当たり判定に使用するクラスをインクルード.
#include "System//00_Manager//ManagerBase.h"

#include "GameObject/02_StaticMeshObject/01_Ball/Ball.h"
#include "GameObject/02_StaticMeshObject/02_WallBase/00_Wall/Wall.h"

#include "System/02_Singleton/03_Score/Score.h"

#include "GameObject/02_StaticMeshObject/06_Character/00_Player/Player.h"
#include "GameObject/02_StaticMeshObject/05_ShotBase/01_EnemyShot/EnemyShot.h"

#include "GameObject/02_StaticMeshObject/07_Ring/Ring.h"

#include "GameObject/02_StaticMeshObject/06_Character/02_AstroEvolution_Player/AstroEvolution_Player.h"
#include "GameObject/02_StaticMeshObject/06_Character/03_AstroEvolution_Rock/AstroEvolution_Rock.h"
#include "System/02_Singleton/02_SingletonManager/09_AstroEvolution_PlayerShot_Manager/AstroEvolution_PlayerShot_Manager.h"
#include "GameObject/02_StaticMeshObject/06_Character/04_AstroEvoultion_Boss/AstroEvoultion_Boss.h"
#include "System/02_Singleton/02_SingletonManager/10_AstroEvolution_BossShot_Manager/AstroEvolution_BossShot_Manager.h"

/******************************************************************************
*	当たり判定マネージャークラス.
**/

class Camera;
class AstralPlayer;
class AstralBoss;

//衝突判定の詳細結果を格納する.
struct CollisionResult
{
	bool IsHit;		//当たったか?
	D3DXVECTOR3 PushVector; //押し戻すためのベクトル.
};

class CollisionManager
	: public ManagerBase
{
public:
	CollisionManager();
	~CollisionManager() override;

	static CollisionManager* GetInstance() {
		static CollisionManager instance; // 🌟ここで実体が作られる（1回だけ）
		return &instance;
	}

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

	void AddRing(Ring* pRing) { m_pRings.push_back(pRing); }
	void ClearRings() { m_pRings.clear(); }

	void AddRock(AstroEvolution_Rock* pRock) { m_pRocks.push_back(pRock); }
	void ClearRocks() { m_pRocks.clear(); }

	void SetAstralPlayer(AstralPlayer* p) { m_pAstralPlayer = p; }
	void SetAstralBoss(AstralBoss* p) { m_pAstralBoss_New = p; }
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

	Player* m_pPlayer = nullptr;
	AstroEvolution_Player* m_pAstroPlayer = nullptr;
	AstroEvoultion_Boss* m_pAstroBoss = nullptr;
	std::vector<EnemyShot*> m_EnemyShots;

	std::vector<Ring*> m_pRings;
	std::vector<AstroEvolution_Rock*> m_pRocks;

	bool m_isPlayerHit = false;

	bool IsPlayerHit() const { return m_isPlayerHit; }

	//エフェクトのクールタイム.
	float m_BossHitEffectCooldown = 0.0f;

	Camera* m_pCamera = nullptr;

	AstralPlayer* m_pAstralPlayer = nullptr;
	AstralBoss* m_pAstralBoss_New = nullptr;

	float m_PlayerAttackEffectTimer = 0.0f;
public:

	void SetPlayer(Player* p) { m_pPlayer = p; }
	void SetAstro_Player(AstroEvolution_Player* Ap) { m_pAstroPlayer = Ap; }
	void SetAstro_Boss(AstroEvoultion_Boss* Ab) { m_pAstroBoss = Ab; }
	void AddEnemyShot(EnemyShot* shot) { m_EnemyShots.push_back(shot); }

	void SetCamera(Camera* pCamera) { m_pCamera = pCamera; }
};