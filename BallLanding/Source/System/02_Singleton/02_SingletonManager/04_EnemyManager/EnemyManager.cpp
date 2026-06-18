#include "EnemyManager.h"

#include "System/02_Singleton/00_Timer/Timer.h"
#include "GameObject/02_StaticMeshObject/05_ShotBase/00_PlayerShot/PlayerShot.h"

#include "System/02_Singleton/02_SingletonManager/03_PlayerShotManager/PlayerShotManager.h"


void EnemyManager::SetPlayerShots(const std::vector<std::unique_ptr<PlayerShot>>* shots)
{
	m_pPlayerShots = shots;
}

void EnemyManager::CheckCollisionPlayerShot()
{
	if (!m_pPlayerShots) return;

	for (auto& enemy : m_pEnemies)
	{
		if (enemy->GetDead()) continue;

		for (auto& shot : *m_pPlayerShots)
		{
			if (!shot->IsActive()) continue;

			if (enemy->GetBoundingSphere().IsHit(
				shot->GetBoundingSphere()))
			{
				// ヒット処理
				enemy->OnHit(10);
				shot->OnHit();

				break;	// 1発で敵消えるので次の敵へ
			}
		}
	}
}

void EnemyManager::SetPlayerShotManager(PlayerShotManager* p)
{
	m_pPlayerShotManager = p;
}


Enemy* EnemyManager::GetNearestEnemy(const D3DXVECTOR3& fromPos)
{
	Enemy* nearest = nullptr;
	float minDistSq = FLT_MAX;

	for (auto& enemy : m_pEnemies)
	{
		if (!enemy || enemy->IsDead())
			continue;

		D3DXVECTOR3 diff = enemy->GetPosition() - fromPos;

		float distSq = D3DXVec3LengthSq(&diff); // √しない（軽い）

		if (distSq < minDistSq)
		{
			minDistSq = distSq;
			nearest = enemy.get();
		}
	}

	return nearest;
}

EnemyManager::EnemyManager()
	: ManagerBase	()
	, m_SpawnTimer	(0.0f)
	, m_SpawnInterval(3.0f)
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Update()
{
	float deltaTime = Timer::GetInstance().DeltaTime();
	m_SpawnTimer -= deltaTime;

	if (m_SpawnTimer <= 0.0f)
	{
		SpawnRandom();
		m_SpawnTimer = m_SpawnInterval;
	}

	// 敵の更新と削除フラグのチェックのみを行う
	for (auto enemyIt = m_pEnemies.begin(); enemyIt != m_pEnemies.end(); )
	{
		(*enemyIt)->Update();

		// 当たり判定マネージャー側で OnHit() が呼ばれ、
		// IsDead が true になったものを削除する
		if ((*enemyIt)->IsDead())
		{
			enemyIt = m_pEnemies.erase(enemyIt);
		}
		else
		{
			++enemyIt;
		}
	}
}


void EnemyManager::Draw()
{
	for (auto& enemy : m_pEnemies)
	{
		enemy->Draw();
	}
}

void EnemyManager::Create()
{
}

void EnemyManager::Release()
{
	//削除.
	m_pEnemies.clear();
}

//敵を生成する.
void EnemyManager::AddEnemy(const D3DXVECTOR3& Pos)
{
	//敵を生成して所有権をmoveでEnemyに返す.
	auto NewEnemy = std::make_unique<Enemy>();
	//ポジション設定.
	NewEnemy->SetPosition(Pos);

	NewEnemy->SetPlayer(m_pTargetPlayer);
	m_pEnemies.push_back(std::move(NewEnemy));
}

void EnemyManager::SpawnRandom()
{
	//各軸の乱数分布を作成する.
	std::uniform_real_distribution<float> DistX(m_MinRange.x, m_MaxRange.x );
	std::uniform_real_distribution<float> DistY(m_MinRange.y, m_MaxRange.y );
	std::uniform_real_distribution<float> DistZ(m_MinRange.z, m_MaxRange.z );

	//3軸すべてランダムな座標を生成.
	D3DXVECTOR3 RandomPos(DistX(m_RandomEngine), DistY(m_RandomEngine), DistZ(m_RandomEngine));
	//敵の生成.
	AddEnemy(RandomPos);
}
