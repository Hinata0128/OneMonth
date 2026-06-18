#pragma once
#include "Scene/SceneBase.h"

class UIScoreNumber;
class ScoreManager;

class ResultScore final
	: public SceneBase
{
public:
	ResultScore();
	~ResultScore() override;

	void Initialize() override;
	void Create() override;
	void Update() override;
	void Draw() override;

	//カメラ関数.
	void ApplyCamera() override {}
	//プロジェクション関数.
	void Projection() override {}

private:
	//表示用のクラス.
	std::unique_ptr<UIScoreNumber> m_pCurrentScoreUI;
	std::unique_ptr<UIScoreNumber> m_pHighScoreUI;
};