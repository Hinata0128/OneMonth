#pragma once

#include "Scene//SceneBase.h"
#include "GameObject//01_SpriteObject//SpriteObject.h"
#include "SceneManager//SceneManager.h"

#include "System/00_Manager/03_CollisionManager/CollisionManager.h"

#include "Sprite3D//Sprite3D.h"

#include "GameObject/02_StaticMeshObject/StaticMeshObject.h"

#include "GameObject/03_UIObject/00_LimitTime/LimitTime.h"


#include "System/06_Camera/Camera.h"

#include "GameObject/02_StaticMeshObject/06_Character/02_AstroEvolution_Player/AstroEvolution_Player.h"


#include "GameObject/02_StaticMeshObject/04_SkyBox/SkyBox.h"

#include "GameObject/02_StaticMeshObject/08_SkyDome/00_AstroEvolution_SkyDome/AstroEvolution_SkyDome.h"

#include "System/02_Singleton/02_SingletonManager/07_AstroEvolution_WallManager/AstroEvolution_WallManager.h"
#include "System/02_Singleton/02_SingletonManager/09_AstroEvolution_PlayerShot_Manager/AstroEvolution_PlayerShot_Manager.h"
#include "GameObject/02_StaticMeshObject/06_Character/04_AstroEvoultion_Boss/AstroEvoultion_Boss.h"

#include "GameObject/03_UIObject/00_HPBar/HPBar.h"
#include "GameObject/03_UIObject/03_AstroBoss_HPBar/AstroBoss_HPBar.h"

class Timer;



/*********************************************
*	ゲームメイン画像クラス.
**/

class AstroEvolution
	: public SceneBase
{
public:
	enum class GameState : byte
	{
		Play,    // ラウンド（岩）
		Boss,    // ボス戦
		Result,  // ← 強化（ここが重要）
		GameClear,
		GameOver
	};
	//最終privateに書く.
	GameState m_State;
	int m_CurrentRound;
public:
	AstroEvolution();
	~AstroEvolution() override;

	void Initialize() override;
	void Create() override;
	void Update() override;
	void Draw() override;

	HRESULT LoadData();

	//カメラ関数.
	void ApplyCamera() override;
	//プロジェクション関数.
	void Projection() override;

private:
	//ゲームで扱うスプライトデータ(使いまわす資源).
	std::unique_ptr<Sprite3D> m_pSp3D;

	//スタティックメッシュオブジェクトクラス.
	std::unique_ptr<StaticMeshObject> m_pStcMeshObj;


	//制限時間の設定.
	std::shared_ptr<LimitTime> m_pLimitTime;


	std::unique_ptr<Camera> m_pCamera;

	std::unique_ptr<AstroEvolution_Player> m_pPlayer;

	std::unique_ptr<AstroEvolution_SkyDome> m_pSkyDome;
	std::unique_ptr<AstroEvoultion_Boss> m_pBoss;

	std::unique_ptr<HPBar> m_pPlayerHp;
	std::unique_ptr<AstroBoss_HPBar> m_pAstroBossHp;

	bool m_IsBossHandled = false;
};