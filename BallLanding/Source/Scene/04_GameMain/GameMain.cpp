#include "GameMain.h"
#include "Sound//SoundManager.h"
#include "Effect//Effect.h"

#include "System//02_Singleton//00_Timer//Timer.h"
#include "System//00_Manager//02_ImGuiManager//ImGuiManager.h"

#include "System/00_Manager/03_CollisionManager/CollisionManager.h" 


#include "System/02_Singleton/02_SingletonManager/02_WallManager/WallManager.h"
#include "System/02_Singleton/03_Score/ScoreManager.h"

GameMain::GameMain()
	: SceneBase()
	, m_pStcMeshObj(std::make_unique<StaticMeshObject>())



	, m_pGround(std::make_unique<Ground>())

	, m_pLimitTime(std::make_shared<LimitTime>(50.0))
	
	, m_pPointCan(nullptr)
	, m_pSkyBox(std::make_shared<SkyBox>())
	, m_pCamera(std::make_unique<Camera>())

{
	m_pDx11 = DirectX11::GetInstance();
	m_pDx9 = DirectX9::GetInstance();

	m_pDx11 = DirectX11::GetInstance();
	m_pDx9 = DirectX9::GetInstance();

	// --- 1. まず当たり判定マネージャーを真っさらにする ---
	CollisionManager::GetInstance()->Release();
	CollisionManager::GetInstance()->Create();

	// --- 2. その後に「かご」を生成する（これで正しく登録される） ---
	m_pPointCan = std::make_unique<PointCan>();


	BallManager::GetInstance()->Release(); // まだ無ければ作成してください
	WallManager::GetInstance()->Release();

	WallManager::GetInstance()->LoadData("Data\\json\\WallPosition\\WallPosition.json");
	NeedleManager::GetInstance()->LoadData("Data\\json\\Needle\\Needel.json");

	if (BallManager::GetInstance()) {
		BallManager::GetInstance()->SetLimitTime(m_pLimitTime);
	}

	auto& sm = ScoreManager::GetInstance();
	sm.SetCurrentGameID("GameMain");
	sm.LoadFromFile("Data\\json\\ScoreData\\ScoreData_GameMain.json");
	m_pScore = sm.GetScore(sm.GetCurrentGameID() + "_Current");

	if (m_pScore) {
		m_pScore->Clear();
		m_pScore->SetPosition(D3DXVECTOR2(50.0f, 50.0f));

		// スコアをマネージャーに紐付け
		CollisionManager::GetInstance()->SetTargetScore(m_pScore);
		CollisionManager::GetInstance()->SetScoreValue(100);
	}

	m_Camera.vPosition = D3DXVECTOR3(0.0f, 5.0f, -5.0f);
	m_Camera.vLook = D3DXVECTOR3(0.0f, 2.0f, 5.0f);
	m_Light.vDirection = D3DXVECTOR3(1.5f, 1.f, -1.f);

	m_pSkyBox->SetCamera(m_pCamera.get());

	Initialize();


}
GameMain::~GameMain()
{
	//外部で作成しているので、ここでは破棄しない.
	m_hWnd = nullptr;
	m_pDx11 = nullptr;
	m_pDx9 = nullptr;
}

void GameMain::Initialize()
{
	m_pCamera->SetMode(Camera::CameraMode::ThirdPerson);
	m_pCamera->SetOffset(D3DXVECTOR3(0.0f, 10.0f, -30.0f));
	m_pCamera->SetAngleX(D3DXToRadian(5.0f));
	m_pCamera->SetAngleY(0.0f);
}

void GameMain::Create()
{
	m_pLimitTime->Create();
}

void GameMain::Update()
{
	float cameraAngle = m_pCamera->GetAngleX();
	m_pCamera->SetAngleX(cameraAngle);

	//カメラのターゲットの位置は固定.
	D3DXVECTOR3 targetPos{ 0.0f,0.0f,0.0f };
	m_pCamera->Update(targetPos);
	{
		static ::EsHandle hEffect = -1;
		if (GetAsyncKeyState('Y') & 0x0001) {
			hEffect = Effect::Play(Effect::LookPortal, D3DXVECTOR3(0.f, 1.f, 0.f));
			Effect::SetScale(hEffect, D3DXVECTOR3(0.8f, 0.8f, 0.8f));
			Effect::SetRotation(hEffect, D3DXVECTOR3(0.f, D3DXToRadian(90.0f), 0.f));
			Effect::SetLocation(hEffect, D3DXVECTOR3(0.0f, 7.0f, 5.0f));
		}
		if (GetAsyncKeyState('T') & 0x0001) {
			Effect::Stop(hEffect);
		}
	}

	CollisionManager::GetInstance()->Update();


	m_pLimitTime->Update();

	WallManager::GetInstance()->Update();
	NeedleManager::GetInstance()->Update();

	m_pPointCan->Update();
	BallManager::GetInstance()->Update();
	m_pScore->Update();
	m_pSkyBox->Update();

	if (m_pLimitTime->IsTimeUp())
	{
		SceneManager::GetInstance()->LoadScene(SceneManager::Score);
		return; 
	}

}

void GameMain::Draw()
{
	ApplyCamera();
	Projection();

	PreDraw();
	m_pSkyBox->Draw();

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
	//Effectクラス
	Effect::GetInstance()->Draw();

	m_pGround->Draw();
	

	BallManager::GetInstance()->Draw();
	WallManager::GetInstance()->Draw();
	NeedleManager::GetInstance()->Draw();

	m_pLimitTime->Draw();


	m_pPointCan->Draw();
	m_pScore->Draw();

}

HRESULT GameMain::LoadData()
{
	return S_OK;
}

void GameMain::ApplyCamera()
{
	m_mView = m_pCamera->GetViewMatrix();
}

//プロジェクション関数.
void GameMain::Projection()
{
	float fov_y = static_cast<FLOAT>(D3DXToRadian(45.0));
	float aspect = static_cast<FLOAT>(WND_W) / static_cast<FLOAT>(WND_H);
	float near_z = 0.1f;
	float far_z = 50000.0;

	D3DXMatrixPerspectiveFovLH(&m_mProj, fov_y, aspect, near_z, far_z);
}

