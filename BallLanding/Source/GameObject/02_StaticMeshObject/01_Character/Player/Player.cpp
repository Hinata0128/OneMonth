#include "Player.h"

#include "System//02_Singleton//00_Manager//01_StaticMeshManager//StaticMeshManager.h"
#include "System/06_Camera/Camera.h"
#include "System\02_Singleton\00_Manager\04_PlayerShotManager\PlayerShotManager.h"

Player::Player()
	: Character()
	, m_pCamera(nullptr)
	, m_AngleY(0.0f)      //旋回角度.
	, m_MoveSpeed(0.2f)   //移動速度.
	, m_RotSpeed(0.05f)   //旋回速度.

{
	//敵のスタティックメッシュを呼び込む.
	auto pStaticMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Fighter);
	//設定.
	//ポインタで取得します.
	AttachMesh(*pStaticMesh);

	//ポジションの設定.
	D3DXVECTOR3 Pos = { 0.0f,0.0f,0.0f };
	SetPosition(Pos);

	//サイズの変更.
	D3DXVECTOR3 Scale = { 5.0f,5.0f,5.0f };
	SetScale(Scale);
	//光遮断.
	this->SetLightEnable(false);
	Init();


}

Player::~Player()
{
}

void Player::Update()
{
	HandleMovement();

#if 1
	//ボタンが押されたときに弾を発射する.
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{

		//初期パラメータ.
		D3DXVECTOR3 StartPos = m_Position;
		//プレイヤーの向いている角度から弾が発射するように設定.
		float sinY = sinf(m_AngleY);
		float cosY = cosf(m_AngleY);
		D3DXVECTOR3 PlayerTargetDir = D3DXVECTOR3(sinY, 0.0f, cosY);
		D3DXVECTOR3 Velocity = PlayerTargetDir * 80.0f;
		float Radius = 1.0f;
		float Life = 3.0f;

		//生成直後弾の設定可能.
		//m_upPlayerShot->Launch(StartPos, Velocity, Radius, Life);
		PlayerShotManager::GetInstance()->Launch(StartPos, Velocity, Radius, Life);
	}
#else

	static bool bPrevSpacePressed = false;
	bool bCurrentSpacePressed = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		//弾の数が 0（画面内に弾がない）のときだけ発射する！
		if (PlayerShotManager::GetInstance()->GetShotCount() == 0)
		{
			//初期パラメータ.
			D3DXVECTOR3 StartPos = m_Position;

			//プレイヤーの向いている角度から弾が発射するように設定.
			float sinY = sinf(m_AngleY);
			float cosY = cosf(m_AngleY);
			D3DXVECTOR3 PlayerTargetDir = D3DXVECTOR3(sinY, 0.0f, cosY);
			D3DXVECTOR3 Velocity = PlayerTargetDir * 80.0f;
			float Radius = 1.0f;
			float Life = 3.0f;

			//弾を発射
			PlayerShotManager::GetInstance()->Launch(StartPos, Velocity, Radius, Life);
		}
	}
#endif
	//弾が存在している時.
	PlayerShotManager::GetInstance()->Update();
	
	Character::Update();
}

void Player::Draw()
{
	PlayerShotManager::GetInstance()->Draw();
	Character::Draw();
}

void Player::Init()
{
}

bool Player::GetDead() const
{
	return false;
}

void Player::HandleMovement()
{
	bool isMoving = false;
	D3DXVECTOR3 Pos = GetPosition();
	float SinY = sinf(m_AngleY);
	float CosY = cosf(m_AngleY);


	//旋回.
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_AngleY -= m_RotSpeed; isMoving = true;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_AngleY += m_RotSpeed; isMoving = true;
	}
	
	float Speed = 0.5f;

	//前後移動.
	if (GetAsyncKeyState('W') & 0x8000)
	{
		Pos.x += SinY * Speed;
		Pos.z += CosY * Speed;
		isMoving = true;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		Pos.x -= SinY * Speed;
		Pos.z -= CosY * Speed;
		isMoving = false;
	}

	SetPosition(Pos);
	SetRotation(D3DXVECTOR3(0.0f, m_AngleY, 0.0f));
}
