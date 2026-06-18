#pragma once
#include "Scene/SceneBase.h"

#include "GameObject/02_StaticMeshObject/06_Character/02_AstroEvolution_Player/AstroEvolution_Player.h"

class UIObject;
class Sprite2D;
class DirectX11;
class Timer;
class UIScoreNumber;

/**************************************************************
*	AstroEvolutionのレベルアップ用のシーンクラス.
**/

class AstroEvolution_LevelUp final
	: public SceneBase
{
public:
	enum class enSelectLevel : byte
	{
		MoveUp = 0,
		PlayerLevel,
		Rotation,
		NextRound
	};
public:
	AstroEvolution_LevelUp();
	~AstroEvolution_LevelUp() override;

	void Initialize() override;
	void Create() override;
	void Update() override;
	void Draw() override;

	//カメラ関数.
	void ApplyCamera() override {}
	//プロジェクション関数.
	void Projection() override {}


private:
	enSelectLevel m_Select;

	//それぞれの生成位置の設定.
	//残りのラウンド数取得.
	int m_RemainingUpgrades;
	std::shared_ptr<UIScoreNumber> m_spNumRemaining;

#pragma region smartpointer
	//背景画像.
	std::shared_ptr<Sprite2D> m_pSpriteBack;
	std::shared_ptr<UIObject> m_upBack;
	//PlayerLevelUp.
	std::shared_ptr<Sprite2D> m_pSpritePlayerLevel;
	std::shared_ptr<UIObject> m_upPlayerLevel;
	//移動速度強化.
	std::shared_ptr<Sprite2D> m_pSpritePlayerMoveSpeed;
	std::shared_ptr<UIObject> m_upPlayerMoveSpeed;
	//回転速度強化.
	std::shared_ptr<Sprite2D> m_pSpritePlayerRotationUP;
	std::shared_ptr<UIObject> m_upPlayerRotationUP;
	//次のラウンドへ遷移用.
	std::shared_ptr<Sprite2D> m_pSpriteNextRound;
	std::shared_ptr<UIObject> m_upNextRound;
	//選択肢.
	std::shared_ptr<Sprite2D> m_pSpriteSelectBack;
	std::shared_ptr<UIObject> m_upSelectBack;
	//選択肢の枠.
	std::shared_ptr<Sprite2D> m_pSpriteSelectFrame;
	std::shared_ptr<UIObject> m_upSelectFrame;

	// --- 矢印テクスチャ ---
	std::shared_ptr<Sprite2D> m_pSpriteArrowLeft;
	std::shared_ptr<Sprite2D> m_pSpriteArrowRight;

	// --- 各項目のボタン (ショット、移動、回転) ---
	std::shared_ptr<UIObject> m_upArrowShotL, m_upArrowShotR;
	std::shared_ptr<UIObject> m_upArrowMoveL, m_upArrowMoveR;
	std::shared_ptr<UIObject> m_upArrowRotL, m_upArrowRotR;

	std::shared_ptr<UIScoreNumber> m_spNumShot;
	std::shared_ptr<UIScoreNumber> m_spNumMove;
	std::shared_ptr<UIScoreNumber> m_spNumRot;
#pragma endregion
};