#pragma once
#pragma once

#include "Scene//SceneBase.h"
#include "GameObject//01_SpriteObject//SpriteObject.h"
#include "SceneManager//SceneManager.h"

#include "System/00_Manager/03_CollisionManager/CollisionManager.h"

#include "Sprite3D//Sprite3D.h"

#include "GameObject/02_StaticMeshObject/StaticMeshObject.h"

#include "GameObject/03_UIObject/00_LimitTime/LimitTime.h"


#include "System/06_Camera/Camera.h"

#include "GameObject/02_StaticMeshObject/00_Ground/Ground.h"

#include "GameObject/00_SkinMeshObject/00_SkinMeshCharacter/00_AstralPlayer/AstralPlayer.h"
#include "GameObject/00_SkinMeshObject/00_SkinMeshCharacter/01_AstralBoss/AstralBoss.h"

class Timer;



/*********************************************
*	ゲームメイン画像クラス.
**/

class AstralDuel final
	: public SceneBase
{
public:
	AstralDuel();
	~AstralDuel() override;

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
	std::unique_ptr<Ground> m_pGround;


	std::unique_ptr<Camera> m_pCamera;

	std::unique_ptr<AstralPlayer> m_pAstralPlayer;
	std::unique_ptr<AstralBoss> m_pAstralBoss;
};