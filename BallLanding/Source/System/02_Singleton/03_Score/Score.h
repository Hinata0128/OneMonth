#pragma once

#include "System/02_Singleton/Singleton.h"

/***************************************************************
*	スコアクラス.
*	ToDo : マネージャクラスに渡す.
*		   スコアナンバークラスを呼んでスコアの表示をしている.
**/

class UIScoreNumber;

class Score final
	: public Singleton
{
public:
	Score();
	~Score() override;

	//スコアの加算.
	void Add(int val);

	//スコアの設定.
	void Set(int val);

	//スコアの取得.
	int GetValue() const;

	//リセット処理.
	void Clear() override;

	void SetPosition(const D3DXVECTOR2& pos);

	void Update();
	void Draw();

private:
	//今の得点.
	int m_CurrentScore;
	std::unique_ptr<UIScoreNumber> m_pUIScoreNumber;
};