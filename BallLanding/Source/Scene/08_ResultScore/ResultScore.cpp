#include "ResultScore.h"

#include "GameObject//03_UIObject//01_UIScoreNumber//UIScoreNumber.h"
#include "System//02_Singleton//03_Score//ScoreManager.h"

#include "SceneManager/SceneManager.h"

ResultScore::ResultScore()
{
	m_pCurrentScoreUI = std::make_unique<UIScoreNumber>();
	m_pHighScoreUI = std::make_unique<UIScoreNumber>();
}

ResultScore::~ResultScore()
{
}

void ResultScore::Initialize() 
{
    auto& sm = ScoreManager::GetInstance();
    std::string id = sm.GetCurrentGameID(); // "GameMain" が取得できる

    // ファイル名をIDに基づいて自動生成する
    std::string fileName = "Data\\json\\ScoreData\\ScoreData_" + id + ".json";

    // そのファイルをロード
    sm.LoadFromFile(fileName);

    auto current = sm.GetScore(id + "_Current");
    auto high = sm.GetScore(id + "_High");

    // ハイスコア更新判定
    if (current->GetValue() > high->GetValue())
    {
        high->Set(current->GetValue());
        // 保存時も同じ専用パスを使う
        sm.SaveToFail(fileName);
    }

    // 3. 表示用UIのセットアップ
    m_pCurrentScoreUI = std::make_unique<UIScoreNumber>();
    m_pCurrentScoreUI->SetDisplayValue(current->GetValue());
    m_pCurrentScoreUI->SetPosition(D3DXVECTOR2(400, 300));

    m_pHighScoreUI = std::make_unique<UIScoreNumber>();
    m_pHighScoreUI->SetDisplayValue(high->GetValue());
    m_pHighScoreUI->SetPosition(D3DXVECTOR2(400, 500));
}
void ResultScore::Create()
{
	Initialize();
}

void ResultScore::Update()
{
	m_pCurrentScoreUI->Update();
	m_pHighScoreUI->Update();
	if (GetAsyncKeyState(VK_RETURN) & 0x0001)
	{
		SceneManager::GetInstance()->LoadScene(SceneManager::OP);
	}
}

void ResultScore::Draw()
{
	m_pCurrentScoreUI->Draw();
	m_pHighScoreUI->Draw();
}
