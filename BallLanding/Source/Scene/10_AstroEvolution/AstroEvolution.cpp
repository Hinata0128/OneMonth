#include "AstroEvolution.h"
#include "Sound//SoundManager.h"
#include "Effect//Effect.h"

#include "System//02_Singleton//00_Timer//Timer.h"
#include "System//00_Manager//02_ImGuiManager//ImGuiManager.h"

#include "System/02_Singleton/02_SingletonManager/08_AstroEvolution_Rock_Manager/AstroEvolution_Rock_Manager.h"

#include "System/00_Manager/03_CollisionManager/CollisionManager.h" 

#include "SceneManager/SceneManager.h"

#include "System//02_Singleton/02_SingletonManager/10_AstroEvolution_BossShot_Manager/AstroEvolution_BossShot_Manager.h"

AstroEvolution::AstroEvolution()
	: SceneBase()

	, m_State(GameState::Play)

	, m_pStcMeshObj(std::make_unique<StaticMeshObject>())




	, m_pLimitTime(std::make_shared<LimitTime>(100000.0))

	, m_pCamera(std::make_unique<Camera>())

	, m_pPlayer(std::make_unique<AstroEvolution_Player>())

	, m_pSkyDome(std::make_unique<AstroEvolution_SkyDome>())

	, m_pBoss(std::make_unique<AstroEvoultion_Boss>())

	, m_pPlayerHp(std::make_unique<HPBar>())
	, m_pAstroBossHp(std::make_unique<AstroBoss_HPBar>())

{
	m_pPlayer->LoadData("Data\\json\\Player\\AstroEvolution_Player_Param\\AstroEvolution_Player_Param.json");
	m_pDx11 = DirectX11::GetInstance();
	m_pDx9 = DirectX9::GetInstance();

	AstroEvolution_WallManager::GetInstance()->Release();
	AstroEvolution_WallManager::GetInstance()->LoadData("Data\\json\\AstroEvolution_WallPosition\\AstroEvolution_WallPosition.json");

	int round = AstroEvolution_Rock_Manager::GetInstance()->GetCurrentRound();
	AstroEvolution_Rock_Manager::GetInstance()->Release();
	AstroEvolution_Rock_Manager::GetInstance()->LoadData_Astro_Rock("Data\\json\\Astro_Rock\\Astro_Rock_Param.json", round);

	m_pSkyDome->SetCamera(m_pCamera.get());

	m_Camera.vPosition = D3DXVECTOR3(0.0f, 5.0f, -5.0f);
	m_Camera.vLook = D3DXVECTOR3(0.0f, 2.0f, 5.0f);
	m_Light.vDirection = D3DXVECTOR3(1.5f, 1.f, -1.f);

	m_pPlayerHp->SetTargetPlayer(m_pPlayer.get());
	m_pAstroBossHp->SetTargetBoss(m_pBoss.get());
	m_pBoss->SetTargetPlayer(m_pPlayer.get());

	Initialize();
}
AstroEvolution::~AstroEvolution()
{
	AstroEvolution_BossShot_Manager::GetInstance()->Release();
	//外部で作成しているので、ここでは破棄しない.
	m_hWnd = nullptr;
	m_pDx11 = nullptr;
	m_pDx9 = nullptr;
}

void AstroEvolution::Initialize()
{
	m_pCamera->SetMode(Camera::CameraMode::ThirdPerson);
	m_pCamera->SetOffset(D3DXVECTOR3(0.0f, 0.0f, -12.0f));
	m_pCamera->SetAngleX(D3DXToRadian(5.0f));
	m_pCamera->SetAngleY(0.0f);

	m_pPlayer->SetCamera_Player_Back(m_pCamera.get());
	m_pBoss->SetActive(false);

	Effect::GetInstance()->Clear();

	AstroEvolution_BossShot_Manager::GetInstance()->Create();   // ボスの弾
	AstroEvolution_PlayerShot_Manager::GetInstance()->Create(); // プレイヤーの弾

	CollisionManager::GetInstance()->SetCamera(m_pCamera.get());

}

void AstroEvolution::Create()
{
	m_pLimitTime->Create();
}

void AstroEvolution::Update()
{
	CollisionManager::GetInstance()->Update();
	AstroEvolution_PlayerShot_Manager::GetInstance()->Update();
	AstroEvolution_BossShot_Manager::GetInstance()->Update();
	auto* RockManager = AstroEvolution_Rock_Manager::GetInstance();

	m_pPlayerHp->Update();

	switch (m_State)
	{
	case GameState::Play:
	{
		m_pPlayer->Update();
		AstroEvolution_Rock_Manager::GetInstance()->Update();

		// 岩全部破壊
		if (AstroEvolution_Rock_Manager::GetInstance()->IsAllDestroyed())
		{
			m_State = GameState::Boss;

			// ボス開始
			m_pBoss->SetActive(true);
		}

		// 🌟 死亡チェック
		if (m_pPlayer->GetDead())
		{
			m_State = GameState::GameOver;
		}

		break;
	}
	case GameState::Boss:
	{
		m_pPlayer->Update();
		m_pBoss->Update();
		m_pAstroBossHp->Update();

		if (m_pPlayer->GetDead()) {
			m_State = GameState::GameOver;
			break;
		}

		if (m_pBoss->GetDead() && !m_IsBossHandled)
		{
			m_IsBossHandled = true;
			int currentRound = AstroEvolution_Rock_Manager::GetInstance()->GetCurrentRound();

			// ラウンド3のボスを倒したら即クリア
			if (currentRound >= 3)
			{
				m_State = GameState::GameClear;
			}
			else
			{
				// ラウンド1, 2なら強化シーンへ
				m_State = GameState::Result;
			}
		}
		break;
	}

	case GameState::Result:
	{

		m_IsBossHandled = false;
		SceneManager::GetInstance()->LoadScene(SceneManager::AstroLevel);
		return;
	}
	case GameState::GameOver:
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			AstroEvolution_Rock_Manager::GetInstance()->ResetRound();
			SceneManager::GetInstance()->LoadScene(SceneManager::Win);
		}
		break;
	}
	case GameState::GameClear:
	{
		AstroEvolution_Rock_Manager::GetInstance()->ResetRound();
		SceneManager::GetInstance()->LoadScene(SceneManager::OP);
		return;
	}
	}

	// カメラ
	m_pCamera->SetAngleY(m_pPlayer->GetAngleY());
	m_pCamera->Update(m_pPlayer->GetPosition());
}

void AstroEvolution::Draw()
{
	ApplyCamera();
	Projection();

	PreDraw();

	m_pSkyDome->Draw();

#ifdef _DEBUG
	ImGui::Begin(JAPANESE("タイマー情報"));
	ImGui::Text(JAPANESE("デルタタイム: %.4f 秒"), Timer::GetInstance().DeltaTime());
	ImGui::Text(JAPANESE("総経過時間: %.2f 秒"), Timer::GetInstance().ElapsedTime());
	ImGui::End();
#endif

#ifdef _DEBUG
	ImGui::Begin("Camera Debug");

	D3DXVECTOR3 offset = m_pCamera->GetOffset();
	if (ImGui::DragFloat3("Offset (X/Y/Z)", (float*)&offset, 0.1f)) {
		m_pCamera->SetOffset(offset);
	}

	float angleX = m_pCamera->GetAngleX();
	if (ImGui::SliderAngle("Angle X", &angleX)) {
		m_pCamera->SetAngleX(angleX);
	}

	ImGui::Separator();

	if (ImGui::Button("Reset Camera")) {
		m_pCamera->SetOffset(D3DXVECTOR3(1.5f, 5.0f, -15.0f));
		m_pCamera->SetAngleX(0.0f);
	}

	ImGui::End();
#endif


	Effect::GetInstance()->Draw();

	AstroEvolution_WallManager::GetInstance()->Draw();
	AstroEvolution_Rock_Manager::GetInstance()->Draw();
	AstroEvolution_PlayerShot_Manager::GetInstance()->Draw();
	AstroEvolution_BossShot_Manager::GetInstance()->Draw();

	m_pPlayer->Draw();

	// 🌟 Bossのときだけ描画
	if (m_State == GameState::Boss)
	{
		m_pBoss->Draw();
	}

	m_pPlayerHp->Draw();
	m_pAstroBossHp->Draw();
	m_pLimitTime->Draw();
}
HRESULT AstroEvolution::LoadData()
{
	return S_OK;
}

void AstroEvolution::ApplyCamera()
{
	m_mView = m_pCamera->GetViewMatrix();
}

//プロジェクション関数.
void AstroEvolution::Projection()
{
	float fov_y = static_cast<FLOAT>(D3DXToRadian(45.0));
	float aspect = static_cast<FLOAT>(WND_W) / static_cast<FLOAT>(WND_H);
	float near_z = 0.1f;
	float far_z = 50000.0;

	D3DXMatrixPerspectiveFovLH(&m_mProj, fov_y, aspect, near_z, far_z);
	m_pDx11->SetProjectionMatrix(m_mProj);
}

