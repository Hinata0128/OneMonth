#include "GameMain.h"
#include "Sound//SoundManager.h"
#include "Effect//Effect.h"

#include "System//02_Singleton//01_Timer//Timer.h"
#include "System//02_Singleton//00_Manager//02_ImGuiManager//ImGuiManager.h"


#include "System/02_Singleton//00_Manager/03_CollisionManager/CollisionManager.h" 

#include "SceneManager/SceneManager.h"


GameMain::GameMain()
	: SceneBase()


	, m_pStcMeshObj(std::make_unique<StaticMeshObject>())


	, m_pGround(std::make_unique<Ground>())


	, m_pLimitTime(std::make_shared<LimitTime>(100000.0))

	, m_pCamera(std::make_unique<Camera>())

	, m_pAstralPlayer(std::make_unique<AstralPlayer>())

	, m_upPlayer(std::make_unique<Player>())
	, m_upJabaran(std::make_unique<Jabaran>())

{
	m_pDx11 = DirectX11::GetInstance();
	m_pDx9 = DirectX9::GetInstance();



	m_Camera.vPosition = D3DXVECTOR3(0.0f, 5.0f, -5.0f);
	m_Camera.vLook = D3DXVECTOR3(0.0f, 2.0f, 5.0f);
	m_Light.vDirection = D3DXVECTOR3(1.5f, 1.f, -1.f);


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
	//Game用のカメラの位置(仮).
	//m_pCamera->SetOffset(D3DXVECTOR3(0.0f, 15.5f, -30.0f));
	m_pCamera->SetOffset(D3DXVECTOR3(0.0f, 8.50f, -30.0f));
	m_pCamera->SetAngleX(D3DXToRadian(5.0f));
	m_pCamera->SetAngleY(0.0f);

	m_upPlayer->SetCamera_Player_Back(m_pCamera.get());

	Effect::GetInstance()->Clear();


	auto colManager = CollisionManager::GetInstance();
	colManager->SetCamera(m_pCamera.get());
	colManager->SetPlayer(m_upPlayer.get()); // プレイヤーを登録
}

void GameMain::Create()
{
	m_pLimitTime->Create();
}

void GameMain::Update()
{
	CollisionManager::GetInstance()->Update();
	Effect::GetInstance()->Update();

	m_pAstralPlayer->Update();
	m_upPlayer->Update();
	//m_upJabaran->Update();

	// カメラ
	m_pCamera->SetAngleY(m_upPlayer->GetAngleY());
	m_pCamera->Update(m_upPlayer->GetPosition());
}

void GameMain::Draw()
{
	ApplyCamera();
	Projection();

	PreDraw();



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


	m_pGround->Draw();

	m_upJabaran->Draw();
	m_upPlayer->Draw();
	//m_pAstralPlayer->Draw();
	m_pLimitTime->Draw();
	Effect::GetInstance()->Draw();

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
	m_pDx11->SetProjectionMatrix(m_mProj);
}

