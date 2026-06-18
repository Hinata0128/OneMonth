#pragma once
#include "Scene/SceneBase.h"
#include "SceneManager//SceneManager.h"

#include "System/00_Manager/03_CollisionManager/CollisionManager.h"

#include "GameObject/02_StaticMeshObject/StaticMeshObject.h"

#include "GameObject/03_UIObject/00_LimitTime/LimitTime.h"
#include "GameObject/02_StaticMeshObject/04_SkyBox/SkyBox.h"
#include "GameObject/02_StaticMeshObject/03_PointCan/PointCan.h"

#include "GameObject/02_StaticMeshObject/06_Character/00_Player/Player.h"
#include "GameObject/02_StaticMeshObject/00_Ground/Ground.h"

#include "System/06_Camera/Camera.h"

#include "GameObject/03_UIObject/00_HPBar/HPBar.h"
#include "System/02_Singleton/03_Score/Score.h"

class Timer;
class EnemyManager;

class UniverseShooter final
	: public SceneBase
{
public:
	UniverseShooter();
	~UniverseShooter() override;

	void Initialize() override;
	void Create() override;
	void Update() override;
	void Draw() override;

	//カメラ関数.
	void ApplyCamera() override;
	//プロジェクション関数.
	void Projection() override;

private:

	std::shared_ptr<SkyBox> m_pSkyBox;
	std::unique_ptr<Player> m_pPlayer;
	std::unique_ptr<Ground> m_pGround;

	std::unique_ptr<Camera> m_pCamera;

	std::unique_ptr<HPBar> m_upPlayerHPBar;

	std::shared_ptr<Score> m_pScore;
	//制限時間の設定.
	std::shared_ptr<LimitTime> m_pLimitTime;


};