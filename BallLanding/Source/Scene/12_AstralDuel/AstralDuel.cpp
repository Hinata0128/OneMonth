#include "AstralDuel.h"
#include "Sound//SoundManager.h"
#include "Effect//Effect.h"

#include "System//02_Singleton//01_Timer//Timer.h"
#include "System//02_Singleton//00_Manager//02_ImGuiManager//ImGuiManager.h"


#include "System/02_Singleton//00_Manager/03_CollisionManager/CollisionManager.h" 

#include "SceneManager/SceneManager.h"


AstralDuel::AstralDuel()
	: SceneBase()


	, m_pStcMeshObj(std::make_unique<StaticMeshObject>())


	, m_pGround(std::make_unique<Ground>())


	, m_pLimitTime(std::make_shared<LimitTime>(100000.0))

	, m_pCamera(std::make_unique<Camera>())

	, m_pAstralPlayer(std::make_unique<AstralPlayer>())
	, m_pAstralBoss(std::make_unique<AstralBoss>())

{
	m_pDx11 = DirectX11::GetInstance();
	m_pDx9 = DirectX9::GetInstance();



	m_Camera.vPosition = D3DXVECTOR3(0.0f, 5.0f, -5.0f);
	m_Camera.vLook = D3DXVECTOR3(0.0f, 2.0f, 5.0f);
	m_Light.vDirection = D3DXVECTOR3(1.5f, 1.f, -1.f);


	Initialize();
}
AstralDuel::~AstralDuel()
{
	//外部で作成しているので、ここでは破棄しない.
	m_hWnd = nullptr;
	m_pDx11 = nullptr;
	m_pDx9 = nullptr;
}

void AstralDuel::Initialize()
{
	m_pCamera->SetMode(Camera::CameraMode::ThirdPerson);
	//Game用のカメラの位置(仮).
	//m_pCamera->SetOffset(D3DXVECTOR3(0.0f, 15.5f, -30.0f));
	m_pCamera->SetOffset(D3DXVECTOR3(0.0f, 8.50f, -30.0f));
	m_pCamera->SetAngleX(D3DXToRadian(5.0f));
	m_pCamera->SetAngleY(0.0f);

	m_pAstralPlayer->SetCamera_Player_Back(m_pCamera.get());

	Effect::GetInstance()->Clear();


	auto colManager = CollisionManager::GetInstance();
	colManager->SetCamera(m_pCamera.get());
	colManager->SetAstralPlayer(m_pAstralPlayer.get()); // プレイヤーを登録
	colManager->SetAstralBoss(m_pAstralBoss.get());     // ボスを登録
}

void AstralDuel::Create()
{
	m_pLimitTime->Create();
}

void AstralDuel::Update()
{
	CollisionManager::GetInstance()->Update();
	Effect::GetInstance()->Update();
	
	m_pAstralPlayer->Update();
	m_pAstralBoss->Update();

	// カメラ
	m_pCamera->SetAngleY(m_pAstralPlayer->GetAngleY());
	m_pCamera->Update(m_pAstralPlayer->GetPosition());
}

void AstralDuel::Draw()
{
	ApplyCamera();
	Projection();

	PreDraw();


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


	m_pGround->Draw();

	m_pAstralBoss->Draw();
	m_pAstralPlayer->Draw();
	m_pLimitTime->Draw();
	Effect::GetInstance()->Draw();

}
HRESULT AstralDuel::LoadData()
{
	return S_OK;
}

void AstralDuel::ApplyCamera()
{
	m_mView = m_pCamera->GetViewMatrix();
}

//プロジェクション関数.
void AstralDuel::Projection()
{
	float fov_y = static_cast<FLOAT>(D3DXToRadian(45.0));
	float aspect = static_cast<FLOAT>(WND_W) / static_cast<FLOAT>(WND_H);
	float near_z = 0.1f;
	float far_z = 50000.0;

	D3DXMatrixPerspectiveFovLH(&m_mProj, fov_y, aspect, near_z, far_z);
	m_pDx11->SetProjectionMatrix(m_mProj);
}

