#include "UniverseShooter.h"

#include "System/02_Singleton/00_Timer/Timer.h"
#include "System/02_Singleton/02_SingletonManager/03_PlayerShotManager/PlayerShotManager.h"
#include "System/02_Singleton/02_SingletonManager/04_EnemyManager/EnemyManager.h"
#include "System/02_Singleton/02_SingletonManager/05_EnemyShotManager/EnemyShotManager.h"

#include "System/02_Singleton/03_Score/ScoreManager.h"
#include "System/02_Singleton/02_SingletonManager/06_RingManager/RingManager.h"

UniverseShooter::UniverseShooter()
	: SceneBase()

	, m_pSkyBox(std::make_shared<SkyBox>())
	, m_pPlayer(nullptr)

	, m_pGround(std::make_unique<Ground>())

	, m_pCamera(std::make_unique<Camera>())

	, m_upPlayerHPBar(std::make_unique<HPBar>())

	, m_pLimitTime(std::make_shared<LimitTime>(500.0f))

	//, m_pRing(std::make_unique<Ring>())
{
	m_pDx11 = DirectX11::GetInstance();
	m_pDx9 = DirectX9::GetInstance();

	CollisionManager::GetInstance()->Release();
	CollisionManager::GetInstance()->Create();
	EnemyManager::GetInstance()->Release();
	RingManager::GetInstance()->Release();

	m_pPlayer = std::make_unique<Player>();

	m_Light.vDirection = D3DXVECTOR3(1.5f, 1.f, -1.f);

	m_pSkyBox->SetCamera(m_pCamera.get());

	auto& sm = ScoreManager::GetInstance();
	sm.SetCurrentGameID("UniverseShooter");
	sm.LoadFromFile("Data\\json\\ScoreData\\ScoreData_UniverseShooter.json");

	m_pScore = sm.GetScore(sm.GetCurrentGameID() + "_Current");

	if (m_pScore) {
		m_pScore->Clear();
		m_pScore->SetPosition(D3DXVECTOR2(50.0f, 50.0f));

		// コリジョンマネージャにセット
			CollisionManager::GetInstance()->SetTargetScore(m_pScore);
			CollisionManager::GetInstance()->SetScoreValue(100);
	}



	Initialize();

}

UniverseShooter::~UniverseShooter()
{
}

void UniverseShooter::Initialize()
{
	//カメラの位置・プレイヤーの位置・敵の位置を初期化.
	m_pCamera->SetMode(Camera::CameraMode::MouseCamera);
	m_pCamera->SetAngleX(0.0f);
	m_pCamera->SetAngleY(0.0f);
	m_pCamera->SetOffset(D3DXVECTOR3(0.0f, 3.0f, -12.0f));

	m_pPlayer->Init();
	m_pPlayer->SetAngleY(0.0f);

	m_pPlayer->SetCamera(m_pCamera.get());

	m_pCamera->ResetInitFlg();

	m_pCamera->Update(m_pPlayer->GetPosition());

	EnemyManager::GetInstance()->Release();
	PlayerShotManager::GetInstance()->Init();
	EnemyShotManager::GetInstance()->Clear();
}

void UniverseShooter::Create()
{
	EnemyManager::GetInstance()->SetPlayer(m_pPlayer.get());

	auto enemyMgr = EnemyManager::GetInstance();
	auto shotMgr = PlayerShotManager::GetInstance();

	enemyMgr->SetPlayer(m_pPlayer.get());
	enemyMgr->SetPlayerShotManager(shotMgr);

	RingManager::GetInstance()->SetPlayer(m_pPlayer.get());
	RingManager::GetInstance()->Create();
}

void UniverseShooter::Update()
{
	m_pLimitTime->Update();

	// --- マウス操作・360度旋回のための新フロー ---

	// 1. まずカメラを更新（マウスの移動量を角度に変換）
	// 引数には移動前のプレイヤー座標を渡す
	m_pCamera->Update(m_pPlayer->GetPosition());

	// 2. プレイヤーを更新
	// Player::Updateの中で「m_pCamera->GetAngleY/X」を参照して移動させる
	m_pPlayer->Update();

	// 3. プレイヤーが移動した後の位置に、カメラを最終配置
	// これをしないと、移動した分だけカメラと機体が離れてしまいます
	m_pCamera->Update(m_pPlayer->GetPosition());

	// --- 以下、マネージャー類の更新（変更なし） ---
	auto Enemy = EnemyManager::GetInstance();
	m_upPlayerHPBar->Update();
	//Enemy->Update();
	PlayerShotManager::GetInstance()->Update();
	//EnemyShotManager::GetInstance()->Update();
	CollisionManager::GetInstance()->Update();
	m_pSkyBox->Update();
	m_pScore->Update();

	RingManager::GetInstance()->Update();


	if (m_pLimitTime->IsTimeUp()) {
		SceneManager::GetInstance()->LoadScene(SceneManager::Score);
		return;
	}
}

void UniverseShooter::Draw()
{
	ApplyCamera(); 
	Projection(); 
	PreDraw();


	//スカイボックス描画.
	m_pSkyBox->Draw();
	//m_pGround->Draw();

	m_pPlayer->Draw();
	auto Enemy = EnemyManager::GetInstance();
	//Enemy->Draw();
	PlayerShotManager::GetInstance()->Draw(m_mView, m_mProj);
	//EnemyShotManager::GetInstance()->Draw(m_mView, m_mProj);
	m_upPlayerHPBar->Draw();
	m_pScore->Draw();
	m_pLimitTime->Draw();
	RingManager::GetInstance()->Draw();

#ifdef _DEBUG
	// デバッグ描画（CollisionManagerが登録された全Boxを表示）
	//m_pCollisionManager->Draw();
#endif

}

void UniverseShooter::ApplyCamera()
{
	m_mView = m_pCamera->GetViewMatrix();
}

void UniverseShooter::Projection()
{
	float fov_y = static_cast<FLOAT>(D3DXToRadian(45.0));
	float aspect = static_cast<FLOAT>(WND_W) / static_cast<FLOAT>(WND_H);
	float near_z = 0.1f;
	float far_z = 50000.0;

	D3DXMatrixPerspectiveFovLH(&m_mProj, fov_y, aspect, near_z, far_z);

	m_pDx11->SetProjectionMatrix(m_mProj);
}
