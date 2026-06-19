#pragma once
#include "Scene/SceneBase.h"
#include "Scene/00_Title/Title.h"
#include "Scene/06_GameOver/GameOver.h"
#include "Scene/05_Ending/Ending.h"
#include "Scene/07_Credit/Credit.h"
#include "Scene/08_ResultScore/ResultScore.h"
#include "Scene/12_AstralDuel/AstralDuel.h"



/********************************************
*	シーンマネージャークラス.
**/

class SceneManager
{
public:
	//シーンリストの列挙型.
	enum List
	{
		OP,			//タイトル.
		Main,		//メイン.
		Lose,		//敗北.
		Win,		//勝利.
		CCredit,	//クレジット.
		Score,		//スコア.
		Astral,		//一対一の対決.

		max,	//何も書かない.
	};

public:
	//シングルトンパターン
	static SceneManager* GetInstance()
	{
		static SceneManager s_Instance;
		return &s_Instance;
	}
	~SceneManager();

	HRESULT Create(HWND hWnd);

	void Update();
	void Draw();

	//シーン読み込み.
	void LoadScene(List Scene);

	//HWNDを取得.
	HWND GetHWND() const;

public:
	void SetPause(bool pause);
	bool IsPause() const;
public:
	void SetDx11(DirectX11* Dx11) { m_pDx11 = Dx11; }
	void SetDx9(DirectX9* pDx9) { m_pDx9 = pDx9; }
private:
	//シーン作成.
	void MakeScene(List Scene);
private:
	SceneManager();
	SceneManager(const SceneManager& rhs) = delete;
	SceneManager& operator = (const SceneManager& rhs) = delete;

private:
	std::unique_ptr<SceneBase> m_pScene;
	HWND m_hWnd;

	DirectX11* m_pDx11;
	DirectX9*	m_pDx9;

	bool m_IsPause;

};
