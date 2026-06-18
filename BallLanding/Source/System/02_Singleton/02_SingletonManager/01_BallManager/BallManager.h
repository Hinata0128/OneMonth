#pragma once
#include "System//00_Manager//ManagerBase.h"
#include <algorithm>

class Ball;
class Timer;
class LimitTime;

/**************************************************************
*	ボールマネージャクラス.
**/

class BallManager final
	: public ManagerBase
{
public:
	//シングルトンインスタンスを作成.
	static BallManager* GetInstance()
	{
		static BallManager instance;
		return &instance;
	}

	~BallManager() override;

	void Update() override;
	void Draw() override;
	void Create() override;
	void Release() override;

	//ボールを新しく生成.
	void AddBall();
public:
	void SetLimitTime(std::shared_ptr<LimitTime> pLimitTime)
	{
		m_pLimitTimer = pLimitTime;
	}
private:
	BallManager();
	//コピー禁止.
	BallManager(const BallManager& rhs) = delete;
	BallManager& operator = (const BallManager& rhs) = delete;
private:
	std::vector<std::unique_ptr<Ball>> m_pBalls;
	std::weak_ptr<LimitTime> m_pLimitTimer;

	//ボールの生成時間の設定.
	float m_SpewnNextTime;
	float m_CurrentInterval;
	float m_MinInteval;
	//deltaTime計算用.
	float m_SpawnAccmulator;
	float m_SpawnInterval;
};
