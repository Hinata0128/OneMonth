#include "AstroEvolution_LevelUp.h"

#include "SceneManager//SceneManager.h"
#include "GameObject//03_UIObject//UIObject.h"
#include "Sprite2D//Sprite2D.h"
#include "DirectX//DirectX11.h"
#include "System/02_Singleton/00_Timer/Timer.h"

#include "System/02_Singleton/02_SingletonManager/08_AstroEvolution_Rock_Manager/AstroEvolution_Rock_Manager.h"
#include "GameObject/03_UIObject/01_UIScoreNumber/UIScoreNumber.h"


AstroEvolution_LevelUp::AstroEvolution_LevelUp()
	: SceneBase()
	, m_Select(enSelectLevel::MoveUp)
	, m_RemainingUpgrades(3)
	, m_spNumRemaining(std::make_shared<UIScoreNumber>())

	, m_pSpriteBack(std::make_shared<Sprite2D>())
	, m_upBack(std::make_shared<UIObject>())

	, m_pSpritePlayerLevel(std::make_shared<Sprite2D>())
	, m_upPlayerLevel(std::make_shared<UIObject>())

	, m_pSpritePlayerMoveSpeed(std::make_shared<Sprite2D>())
	, m_upPlayerMoveSpeed(std::make_shared<UIObject>())

	, m_pSpritePlayerRotationUP(std::make_shared<Sprite2D>())
	, m_upPlayerRotationUP(std::make_shared<UIObject>())

	, m_pSpriteNextRound(std::make_shared<Sprite2D>())
	, m_upNextRound(std::make_shared<UIObject>())
	
	, m_pSpriteSelectBack(std::make_shared<Sprite2D>())
	, m_upSelectBack(std::make_shared<UIObject>())

	, m_pSpriteSelectFrame(std::make_shared<Sprite2D>())
	, m_upSelectFrame(std::make_shared<UIObject>())

	, m_pSpriteArrowLeft(std::make_shared<Sprite2D>())
	, m_pSpriteArrowRight(std::make_shared<Sprite2D>())

	// UIObjectも同様に生成が必要
	, m_upArrowShotL(std::make_shared<UIObject>())
	, m_upArrowShotR(std::make_shared<UIObject>())
	, m_upArrowMoveL(std::make_shared<UIObject>())
	, m_upArrowMoveR(std::make_shared<UIObject>())
	, m_upArrowRotL(std::make_shared<UIObject>())
	, m_upArrowRotR(std::make_shared<UIObject>())

	, m_spNumShot(std::make_shared<UIScoreNumber>())
	, m_spNumMove(std::make_shared<UIScoreNumber>())
	, m_spNumRot(std::make_shared<UIScoreNumber>())
{
}

AstroEvolution_LevelUp::~AstroEvolution_LevelUp()
{
}

void AstroEvolution_LevelUp::Initialize()
{
}

void AstroEvolution_LevelUp::Create()
{
	// --- 1. 背景・共通サイズの定義 ---
	const float WND_W = 1280.0f;
	const float WMD_H = 720.0f;
	const float Level_W = 430.0f;
	const float Level_H = 210.0f;

	int currentRound = AstroEvolution_Rock_Manager::GetInstance()->GetCurrentRound();
	if (currentRound <= 2) {
		m_RemainingUpgrades = 3;
	}
	else {
		m_RemainingUpgrades = 2; // 3ラウンド目以降は厳しくするなど
	}

	// 背景画像の設定
	Sprite2D::SPRITE_STATE SSBack = { WND_W, WMD_H, WND_W, WMD_H, WND_W, WMD_H };
	m_pSpriteBack->Init(_T("Data\\Image\\Setting\\Win.png"), SSBack);
	m_upBack->AttachSprite(m_pSpriteBack);
	m_upBack->SetPosition(0.0f, 0.0f, 0.0f);

	// --- 2. 各強化項目のテキスト画像設定 ---
	Sprite2D::SPRITE_STATE SSLevel = { Level_W, Level_H, Level_W, Level_H, Level_W, Level_H };

	// プレイヤーレベル（ショット）
	m_pSpritePlayerLevel->Init(_T("Data\\Image\\Setting\\PlayerLevel.png"), SSLevel);
	m_upPlayerLevel->AttachSprite(m_pSpritePlayerLevel);
	m_upPlayerLevel->SetPosition(150.0f, 100.0f, 0.0f);

	// 移動速度
	m_pSpritePlayerMoveSpeed->Init(_T("Data\\Image\\Setting\\MoveUp.png"), SSLevel);
	m_upPlayerMoveSpeed->AttachSprite(m_pSpritePlayerMoveSpeed);
	m_upPlayerMoveSpeed->SetPosition(150.0f, 200.0f, 0.0f);

	// 回転速度
	m_pSpritePlayerRotationUP->Init(_T("Data\\Image\\Setting\\RotationUp.png"), SSLevel);
	m_upPlayerRotationUP->AttachSprite(m_pSpritePlayerRotationUP);
	m_upPlayerRotationUP->SetPosition(150.0f, 300.0f, 0.0f);

	// 次のラウンドへ
	m_pSpriteNextRound->Init(_T("Data\\Image\\Setting\\NextRound.png"), SSLevel);
	m_upNextRound->AttachSprite(m_pSpriteNextRound);
	m_upNextRound->SetPosition(150.0f, 400.0f, 0.0f);

	// --- 3. 矢印（ボタン）の設定と配置 ---
	const float Arrow_Size = 32.0f;
	Sprite2D::SPRITE_STATE SSArrow = { Arrow_Size, Arrow_Size, Arrow_Size, Arrow_Size, Arrow_Size, Arrow_Size };

	m_pSpriteArrowLeft->Init(_T("Data\\Image\\Setting\\LeftArrow.png"), SSArrow);
	m_pSpriteArrowRight->Init(_T("Data\\Image\\Setting\\RightArrow.png"), SSArrow);

	// ラムダ関数を使って左右の矢印を一気に配置
	auto SetupArrows = [&](std::shared_ptr<UIObject>& L, std::shared_ptr<UIObject>& R, float y) {
		L->AttachSprite(m_pSpriteArrowLeft);
		L->SetPosition(100.0f, y + 90.0f, 0.0f); // 文字画像の中心付近に合わせる
		R->AttachSprite(m_pSpriteArrowRight);
		R->SetPosition(580.0f, y + 90.0f, 0.0f);
		};

	SetupArrows(m_upArrowShotL, m_upArrowShotR, 100.0f); // ショット強化矢印
	SetupArrows(m_upArrowMoveL, m_upArrowMoveR, 200.0f); // 移動速度矢印
	SetupArrows(m_upArrowRotL, m_upArrowRotR, 300.0f); // 回転速度矢印

	// --- 4. 🌟 数字（UIScoreNumber）の表示位置設定 ---
	// 矢印(x=100, 580)の間の、見やすい位置に配置
	m_spNumShot->SetPosition(D3DXVECTOR2(350.0f, 190.0f));
	m_spNumMove->SetPosition(D3DXVECTOR2(350.0f, 290.0f));
	m_spNumRot->SetPosition(D3DXVECTOR2(350.0f, 390.0f));

	m_spNumRemaining->SetPosition(D3DXVECTOR2(1100.0f, 50.0f));

	// --- 5. 🌟 初期データの読み込みと反映 ---
	// シーンに入った瞬間に正しい数字が出るように、ここで一度JSONを読む
	auto tempPlayer = std::make_unique<AstroEvolution_Player>();
	tempPlayer->LoadData("Data/json/Player/AstroEvolution_Player_Param/AstroEvolution_Player_Param.json");

	// UIScoreNumberに値をセット
	m_spNumShot->SetDisplayValue(tempPlayer->GetShotLevel());
	m_spNumMove->SetDisplayValue((int)(tempPlayer->GetMoveSpeed() * 10.0f));
	m_spNumRot->SetDisplayValue((int)(tempPlayer->GetRotSpeed() * 100.0f));
	m_spNumRemaining->SetDisplayValue(m_RemainingUpgrades);

	// 🌟 内部のスプライトを生成・更新
	m_spNumShot->Update();
	m_spNumMove->Update();
	m_spNumRot->Update();
	m_spNumRemaining->Update();
}

void AstroEvolution_LevelUp::Update()
{
	// --- 1. マウス・クリック判定用の座標計算 ---
	HWND hWnd = DirectX11::GetInstance()->GethWnd();
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(hWnd, &mousePos);

	RECT rc;
	GetClientRect(hWnd, &rc);
	float windowW = (float)(rc.right - rc.left);
	float windowH = (float)(rc.bottom - rc.top);

	// 1280x720 の仮想解像度にマウス座標を変換
	float mouseX = (float)mousePos.x * (1280.0f / (windowW > 0 ? windowW : 1));
	float mouseY = (float)mousePos.y * (720.0f / (windowH > 0 ? windowH : 1));

	// クリックの瞬間（トリガー）を判定
	static bool isPrevClicked = false;
	bool isCurrentClicked = (GetAsyncKeyState(VK_LBUTTON) & 0x8000);
	bool isClickTrigger = (isCurrentClicked && !isPrevClicked);
	isPrevClicked = isCurrentClicked;

	// --- 2. プレイヤーデータの準備 ---
	const char* JSON_PATH = "Data/json/Player/AstroEvolution_Player_Param/AstroEvolution_Player_Param.json";
	auto tempPlayer = std::make_unique<AstroEvolution_Player>();
	tempPlayer->LoadData(JSON_PATH); // 現在のパラメータをロード

	bool isChanged = false;

	// --- 3. 判定用ラムダ関数 ---
	auto IsClicked = [&](std::shared_ptr<UIObject> ui, float width = 32.0f, float height = 32.0f) {
		if (!isClickTrigger) return false;
		D3DXVECTOR3 pos = ui->GetPosition();
		// 座標が未設定の場合は無視
		if (pos.x == 0.0f && pos.y == 0.0f) return false;
		// 当たり判定チェック
		return (mouseX >= pos.x && mouseX <= pos.x + width &&
			mouseY >= pos.y && mouseY <= pos.y + height);
		};

	// --- 4. 強化ボタンの判定（残り回数がある場合のみ実行） ---
	if (m_RemainingUpgrades > 0)
	{
		// ショット強化 (右矢印)
		if (IsClicked(m_upArrowShotR)) {
			if (tempPlayer->GetShotLevel() < 3) {
				tempPlayer->UpgradeShotLevel();
				isChanged = true;
				m_RemainingUpgrades--; // 強化回数を消費
			}
		}
		// 移動速度強化 (右矢印)
		if (IsClicked(m_upArrowMoveR)) {
			if (tempPlayer->GetMoveSpeed() < 1.0f) {
				tempPlayer->UpgradeMoveSpeed();
				isChanged = true;
				m_RemainingUpgrades--; // 強化回数を消費
			}
		}
		// 回転速度強化 (右矢印)
		if (IsClicked(m_upArrowRotR)) {
			if (tempPlayer->GetRotSpeed() < 0.15f) {
				tempPlayer->UpgradeRotationSpeed();
				isChanged = true;
				m_RemainingUpgrades--; // 強化回数を消費
			}
		}
	}

	// --- 5. 変更があった場合の保存とUI数値の更新 ---
	if (isChanged) {
		tempPlayer->SaveData(JSON_PATH); // ファイルへ保存

		// UI表示用の数値を最新状態に更新
		m_spNumShot->SetDisplayValue(tempPlayer->GetShotLevel());
		m_spNumMove->SetDisplayValue((int)(tempPlayer->GetMoveSpeed() * 10.0f));
		m_spNumRot->SetDisplayValue((int)(tempPlayer->GetRotSpeed() * 100.0f));

		// 残り強化回数の表示も更新
		if (m_spNumRemaining) {
			m_spNumRemaining->SetDisplayValue(m_RemainingUpgrades);
		}
	}

	// 各数字UIの内部更新（アニメーション等がある場合のため毎フレーム実行）
	m_spNumShot->Update();
	m_spNumMove->Update();
	m_spNumRot->Update();
	if (m_spNumRemaining) {
		m_spNumRemaining->Update();
	}

	// --- 6. 次のラウンドへ遷移する判定 ---
	// スペースキーが押されるか、「次のラウンドへ」の画像(430x210)がクリックされたら遷移
	if ((GetAsyncKeyState(VK_SPACE) & 0x8000) || IsClicked(m_upNextRound, 430.0f, 210.0f))
	{
		// 内部のラウンドカウントを1進める
		AstroEvolution_Rock_Manager::GetInstance()->NextRound();

		// ゲームメイン（Astro）シーンへ戻る
		SceneManager::GetInstance()->LoadScene(SceneManager::Astro);
	}
}

void AstroEvolution_LevelUp::Draw()
{
	DirectX11::GetInstance()->SetDepth(false);
	DirectX11::GetInstance()->SetAlphaBlend(true);
	m_upBack->Draw();
	m_upPlayerLevel->Draw();
	m_upPlayerMoveSpeed->Draw();
	m_upPlayerRotationUP->Draw();
	m_upNextRound->Draw();
	m_upArrowShotL->Draw();
	m_upArrowShotR->Draw();
	m_upArrowMoveL->Draw();
	m_upArrowMoveR->Draw();
	m_upArrowRotL->Draw();
	m_upArrowRotR->Draw();
	m_spNumShot->Draw();
	m_spNumMove->Draw();
	m_spNumRot->Draw();
	m_spNumRemaining->Draw(); // 🌟 残り回数を描画
	DirectX11::GetInstance()->SetAlphaBlend(false);
	DirectX11::GetInstance()->SetDepth(true);
}

