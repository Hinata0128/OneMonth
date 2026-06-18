#include "BallManager.h"

#include "GameObject//02_StaticMeshObject//01_Ball//Ball.h"
#include "System//02_Singleton//00_Timer//Timer.h"
#include "GameObject/03_UIObject/00_LimitTime/LimitTime.h"

BallManager::BallManager()
	: ManagerBase		()
	, m_SpewnNextTime	(0.0f)
	, m_CurrentInterval	(2.0f)
	, m_MinInteval		(0.2f)

	, m_SpawnAccmulator	(0.0f)
	, m_SpawnInterval	(2.0f)
{
}

BallManager::~BallManager()
{
	Release();
}

void BallManager::Update()
{
	//deltaTimeの呼び出し.
	float deltaTime = Timer::GetInstance().DeltaTime();
	m_SpawnAccmulator += deltaTime;

	//ゲームの残り時間を取得.
	float TimeLeft = 30.0f;
	if (auto pLimitTime = m_pLimitTimer.lock())
	{
		TimeLeft = pLimitTime->GetRemainTime();
	}

	//残り時間に基づいて生成間隔を切り替える.
	if (TimeLeft <= 0.0f) {
		m_SpawnInterval = 999999.0f;
	}
	//10秒からは間隔が速い.
	else if (TimeLeft <= 10.0f) {
		m_SpawnInterval = 0.2f;
	}
	else {
		m_SpawnInterval = 1.0f;
	}

	if (m_SpawnAccmulator >= m_SpawnInterval)
	{
		AddBall();
		m_SpawnAccmulator -= m_SpawnInterval;
		if (m_SpawnAccmulator < 0.0f) m_SpawnAccmulator = 0.0f;
	}

	//ボールの更新と削除判定.
	for (auto it = m_pBalls.begin(); it != m_pBalls.end();)
	{
		(*it)->Update();

		//Y座標が一定以下に落ちた.
		bool isOutOfBounds = ((*it)->GetPosition().y < -2.0f);

		//箱に入ってDeadフラグが立っている.
		bool isGoalIn = (*it)->IsDead();

		if (isOutOfBounds || isGoalIn)
		{
			//リストから削除.
			it = m_pBalls.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void BallManager::Draw()
{
	for (auto& Ball : m_pBalls)
	{
		Ball->Draw();
	}
}

void BallManager::Create()
{
}

//ボールの破棄.
void BallManager::Release()
{
	m_pBalls.clear();
}

//ボールを新しく生成.
void BallManager::AddBall()
{
	m_pBalls.push_back(std::make_unique<Ball>());
}
