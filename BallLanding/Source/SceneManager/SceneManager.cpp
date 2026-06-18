#include "SceneManager.h"
#include "Sound/SoundManager.h"

#include "..//System/00_Manager/02_ImGuiManager/ImGuiManager.h"

SceneManager::SceneManager()
	: m_pScene			( nullptr )
	, m_hWnd			()
	, m_pDx11			()
	, m_pDx9			()

	, m_IsPause(false)
{
}

SceneManager::~SceneManager()
{
}

HRESULT SceneManager::Create(HWND hWnd)
{
	m_hWnd = hWnd;
	LoadScene(List::Astral);
	return S_OK;
}

void SceneManager::Update()
{


#ifdef _DEBUG
	//それぞれのシーンへの移動ができる
	ImGui::Begin("Scene Debug");

	ImGui::Text("Current Scene Control");
	ImGui::Separator();

	if (ImGui::Button("Title"))
	{
		LoadScene(SceneManager::OP);
	}

	if (ImGui::Button("Game Main"))
	{
		LoadScene(SceneManager::Main);
	}

	if (ImGui::Button("First Round"))
	{
		LoadScene(SceneManager::First);
	}

	if (ImGui::Button("Second Round"))
	{
		LoadScene(SceneManager::Second);
	}
	
	if (ImGui::Button("Final Round"))
	{
		LoadScene(SceneManager::Final);
	}

	if (ImGui::Button("Win"))
	{
		LoadScene(SceneManager::Win);
	}

	if (ImGui::Button("Lose"))
	{
		LoadScene(SceneManager::Lose);
	}

	if (ImGui::Button("Score"))
	{
		LoadScene(SceneManager::Score);
	}
	if (ImGui::Button("Universe"))
	{
		LoadScene(SceneManager::Universe);
	}
	if (ImGui::Button("Astro"))
	{
		LoadScene(SceneManager::Astro);
	}	
	if (ImGui::Button("Level"))
	{
		LoadScene(SceneManager::AstroLevel);
	}
	if (ImGui::Button("Astral"))
	{
		LoadScene(SceneManager::Astral);
	}




	ImGui::End();
#endif
	m_pScene->Update();

}

void SceneManager::Draw()
{
	m_pScene->Draw();
}

void SceneManager::LoadScene(List Scene)
{
	m_pScene.reset();

	//シーン作成.
	MakeScene(Scene);
	m_pScene->Create();
}

HWND SceneManager::GetHWND() const
{
	return m_hWnd;
}

void SceneManager::SetPause(bool pause)
{
	m_IsPause = pause;
}

bool SceneManager::IsPause() const
{
	return m_IsPause;
}

//シーン作成.
void SceneManager::MakeScene(List Scene)
{
	switch (Scene)
	{
	case SceneManager::OP:
		//ToDo : クレジットから戻ってきたときにクレジットの音楽を停止してタイトル用の曲に戻す.
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Title);
		SoundManager::GetInstance()->PlayLoop(SoundManager::BGM_Title);
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Ending);
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Over);
		m_pScene = std::make_unique<Title>();
		break;
	case SceneManager::First:
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Title);
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Ending);
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Over);
		SoundManager::GetInstance()->PlaySE(SoundManager::SE_Round);
		m_pScene = std::make_unique<FirstRound>();
		break;
	case SceneManager::Second:
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Main);
		SoundManager::GetInstance()->PlaySE(SoundManager::SE_Round);
		m_pScene = std::make_unique<SecondRound>();
		break;
	case SceneManager::Final:
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Main);
		SoundManager::GetInstance()->PlaySE(SoundManager::SE_Round);
		m_pScene = std::make_unique<FinalRound>();
		break;
	case SceneManager::Main:
		//SoundManager::GetInstance()->PlayLoop(SoundManager::BGM_Main);
		m_pScene = std::make_unique<GameMain>();
		break;
	case SceneManager::Lose:
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Main);
		SoundManager::GetInstance()->PlayLoop(SoundManager::BGM_Over);
		m_pScene = std::make_unique<GameOver>();
		break;
	case SceneManager::Win:
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Main);
		SoundManager::GetInstance()->PlayLoop(SoundManager::BGM_Ending);
		m_pScene = std::make_unique<Ending>();
		break;
	case SceneManager::CCredit:
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Title);
		//ToDo : 今クレジットの音楽をTitleにしているけど最終的には変更する.
		SoundManager::GetInstance()->PlayLoop(SoundManager::BGM_Title);
		m_pScene = std::make_unique<Credit>();
		break;
	case SceneManager::Score:
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Title);
		//ToDo : 今クレジットの音楽をTitleにしているけど最終的には変更する.
		SoundManager::GetInstance()->PlayLoop(SoundManager::BGM_Title);
		m_pScene = std::make_unique<ResultScore>();
		break;
	case SceneManager::Universe:
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Title);
		m_pScene = std::make_unique<UniverseShooter>();
		break;
	case SceneManager::Astro:
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Title);
		m_pScene = std::make_unique<AstroEvolution>();
		break;
	case SceneManager::AstroLevel:
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Title);
		m_pScene = std::make_unique<AstroEvolution_LevelUp>();
		break;
	case SceneManager::Astral:
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Title);
		m_pScene = std::make_unique<AstralDuel>();
		break;
	default:
		break;
	}
}
