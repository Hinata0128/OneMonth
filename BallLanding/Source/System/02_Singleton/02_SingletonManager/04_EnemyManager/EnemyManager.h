#pragma once
#include "System/00_Manager/ManagerBase.h"
#include "GameObject/02_StaticMeshObject/06_Character/01_Enemy/Enemy.h"

//ToDo : 敵の生成位置をランダムに設定する.
#include <random>

class Timer;
class PlayerShot;
class PlayerShotManager;

/************************************************************************
*	敵マネージャクラス.
*	ToDo : 敵の数を増やすのを目的としているクラス.
*          シングルトンで実装する.
**/

class EnemyManager final
	: public ManagerBase
{
public:
	//インスタンス取得.
	static EnemyManager* GetInstance()
	{
		//唯一のインスタンスを作成する.
		static EnemyManager s_Instance;
		return &s_Instance;
	}

	~EnemyManager() override;

	void Update() override;
	void Draw() override;
	void Create() override;
	void Release() override;

	//敵を生成する.
	void AddEnemy(const D3DXVECTOR3& Pos);
	//敵の衛星位置の関数.
	void SpawnRandom();

	void SetPlayer(Player* p) { m_pTargetPlayer = p; }

public:
	//生成範囲の設定.
	D3DXVECTOR3 m_MinRange = D3DXVECTOR3(-100.0f, 0.0f, 100.0f);
	//Y座標も生成位置として設定する.
	D3DXVECTOR3 m_MaxRange = D3DXVECTOR3(100.0f, 30.0f, 100.0f);
	//乱数生成器.
	std::mt19937 m_RandomEngine{ std::random_device{}() };

	void SetPlayerShots(const std::vector<std::unique_ptr<PlayerShot>>* shots);

	void CheckCollisionPlayerShot();

	void SetPlayerShotManager(PlayerShotManager* p);
	// 敵のリストを取得する関数を追加
	const std::vector<std::unique_ptr<Enemy>>& GetEnemies() const { return m_pEnemies; }

	Enemy* GetNearestEnemy(const D3DXVECTOR3& fromPos);
private:
	EnemyManager();
	//コピーを禁止している.
	EnemyManager(const EnemyManager& rhs) = delete;
	EnemyManager& operator = (const EnemyManager& rhs) = delete;

private:
	//可変長で敵を増やしていく.
	std::vector<std::unique_ptr<Enemy>> m_pEnemies;

	//ToDo : 敵の生成を時間管理で作成していく.
	//カウント用.
	float m_SpawnTimer;
	//難病で敵を生成させるか.
	float m_SpawnInterval;

	Player* m_pTargetPlayer = nullptr;

	const std::vector<std::unique_ptr<PlayerShot>>* m_pPlayerShots = nullptr;
	PlayerShotManager* m_pPlayerShotManager = nullptr;
};
