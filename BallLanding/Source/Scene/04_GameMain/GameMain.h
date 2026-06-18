#pragma once

#include "Scene//SceneBase.h"
#include "GameObject//01_SpriteObject//SpriteObject.h"
#include "SceneManager//SceneManager.h"

#include "System/00_Manager/03_CollisionManager/CollisionManager.h"

#include "Sprite3D//Sprite3D.h"

#include "GameObject/02_StaticMeshObject/StaticMeshObject.h"

#include "GameObject/02_StaticMeshObject/00_Ground/Ground.h"

#include "GameObject/03_UIObject/00_LimitTime/LimitTime.h"

#include "System/06_Camera/Camera.h"
#include "System/02_Singleton/03_Score/Score.h"


class Timer;



/*********************************************
*	ゲームメイン画像クラス.
**/

class GameMain
	: public SceneBase
{
public:
	GameMain();
	~GameMain() override;

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


	//地面.
	std::unique_ptr<Ground> m_pGround;
	//制限時間の設定.
	std::shared_ptr<LimitTime> m_pLimitTime;

	
	std::shared_ptr<Score> m_pScore;


	std::unique_ptr<Camera> m_pCamera;
};