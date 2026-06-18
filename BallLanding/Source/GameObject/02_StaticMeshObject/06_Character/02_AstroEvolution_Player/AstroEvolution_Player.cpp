#include "AstroEvolution_Player.h"

#include "System/00_Manager/03_CollisionManager/CollisionManager.h"
#include "System/02_Singleton/02_SingletonManager/08_AstroEvolution_Rock_Manager/AstroEvolution_Rock_Manager.h"

namespace {
	const float DEFAULT_SPEED = 0.1f;
	const float DEFAULT_ROT = 0.05f;
	const char* JSON_PATH = "Data/json/Player/AstroEvolution_Player_Param/AstroEvolution_Player_Param.json";
}

AstroEvolution_Player::AstroEvolution_Player()
	: Character()
	
	, m_AngleY(0.0f)
	, m_MoveSpeed(0.1f)
	, m_RotSpeed(0.05f)

	, m_pCamera(nullptr)
	, m_HP(0)
	, m_MaxHP(100)

{
	//読み込み.
	auto pMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Fighter);
	//設定.
	AttachMesh(*pMesh);
	//ポジションの設定.
	D3DXVECTOR3 Pos = { 0.0f,0.0f,-4.0f };
	SetPosition(Pos);
	//光遮断.
	this->SetLightEnable(false);


	m_pSphere = std::make_shared<BoundingSphere>();
	m_pSphere->SetTag(BoundingSphere::Tag::AstroPlayer);
	if (pMesh)
	{
		//メッシュから半径を計算.
		m_pSphere->CreateSphereForMesh(*pMesh);
		//当たり判定のサイズの設定.
		m_pSphere->SetRadius(1.0f);
	}
	CollisionManager::GetInstance()->SetAstro_Player(this);
	CollisionManager::GetInstance()->AddSphere(m_pSphere);

	Init();

}

AstroEvolution_Player::~AstroEvolution_Player()
{
}

void AstroEvolution_Player::Update()
{
#if _DEBUG
	ImGui::Begin("Player Debug");
	ImGui::DragFloat("Move Speed", &m_MoveSpeed, 0.001f, 0.0f, 1.0f);
	ImGui::DragFloat("Rotation Speed", &m_RotSpeed, 0.001f, 0.0f, 0.5f);

	ImGui::Separator();

	// --- 攻撃・耐久（今回追加） ---
	ImGui::Text(JAPANESE("--- 戦闘性能 ---"));
	// ShotLevelを 1〜3 でスライダー調整
	ImGui::SliderInt("Shot Level", &m_ShotLevel, 1, 3);
	ImGui::DragInt("Attack Power", &m_AttackPower, 1, 0, 100);

	// シールド設定
	if (ImGui::DragFloat("Max Shield", &m_MaxShield, 1.0f, 10.0f, 500.0f)) {
		// 最大値を変えたら現在値も全回復させる
		m_CurrentShield = m_MaxShield;
	}
	ImGui::ProgressBar(m_CurrentShield / m_MaxShield, ImVec2(0.0f, 0.0f), "Shield HP");

	ImGui::Separator();

	if (ImGui::Button("Save to JSON")) {
		SaveData(JSON_PATH);
	}

	// デバッグ用にリセットボタンもあると便利！
	if (ImGui::Button("Reset to Default")) {
		ResetStatus();
	}
	ImGui::End();
#endif

	if (m_InvincibleTimer > 0.0f) {
		m_InvincibleTimer -= 1.0f / 60.0f; // デルタタイム
		if (m_InvincibleTimer < 0.0f) m_InvincibleTimer = 0.0f;
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_AngleY -= m_RotSpeed;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_AngleY += m_RotSpeed;
	}

	//移動
	D3DXVECTOR3 Pos = GetPosition();

	float SinY = sinf(m_AngleY);
	float CosY = cosf(m_AngleY);

	if (GetAsyncKeyState('W') & 0x8000)
	{
		Pos.x += SinY * m_MoveSpeed;
		Pos.z += CosY * m_MoveSpeed;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		Pos.x -= SinY * m_MoveSpeed;
		Pos.z -= CosY * m_MoveSpeed;
	}

	// --- 攻撃タイマー更新 ---
	if (m_ShotTimer > 0.0f) {
		m_ShotTimer -= 1.0f / 60.0f;
	}

	// --- 攻撃処理 (修正点: マネージャーに丸投げ) ---
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (m_ShotTimer <= 0.0f)
		{
			// 位置、角度、レベルを渡すだけでOK
			AstroEvolution_PlayerShot_Manager::GetInstance()->PlayerFire(m_Position, m_AngleY, m_ShotLevel);
			m_ShotTimer = SHOT_INTERVAL;
		}
	}

	//反映.
	SetPosition(Pos);
	//回転.
	SetRotationY(m_AngleY);

	m_pSphere->SetPosition(m_Position);

	Character::Update();
}

void AstroEvolution_Player::Draw()
{
#ifdef _DEBUG
	// ImGuiのウィンドウを表示
	if (ImGui::Begin("Player Debug"))
	{
		// HPをバーで表示
		float hpRate = static_cast<float>(m_HP) / static_cast<float>(m_MaxHP);
		ImGui::Text("HP: %d / %d", m_HP, m_MaxHP);
		ImGui::ProgressBar(hpRate, ImVec2(-1, 0), "Current HP");

		ImGui::Separator(); // 区切り線

		// 座標情報
		ImGui::Text("Pos: (%.2f, %.2f, %.2f)", m_Position.x, m_Position.y, m_Position.z);

		// デバッグ用操作：ボタン一つで初期位置に戻すテスト
		if (ImGui::Button("Reset Position")) {
			Init();
		}
	}
	ImGui::End();
#endif
	if (m_InvincibleTimer > 0.0f) {
		// 0.05秒間隔くらいで高速点滅
		if (static_cast<int>(m_InvincibleTimer * 20) % 2 == 0) return;
	}
	//m_pSphere->Draw();
	Character::Draw();
}

void AstroEvolution_Player::Init()
{
	LoadData(JSON_PATH);
	m_MaxHP = 100;
	m_HP = m_MaxHP;

}

bool AstroEvolution_Player::GetDead() const
{
	return false;
}

void AstroEvolution_Player::LoadData(const std::string& FilePath)
{
	std::ifstream File(FilePath);
	if (!File.is_open())
	{
		// ここが出たら読み込めていない（パスが間違っている）
		OutputDebugStringA("### LOAD FAILED: File not found! ###\n");
		return;
	}

	json Data;
	try {
		File >> Data;
		if (Data.contains("MoveSpeed")) m_MoveSpeed = Data["MoveSpeed"];
		if (Data.contains("RotSpeed"))  m_RotSpeed = Data["RotSpeed"];
		if (Data.contains("ShotLevel"))   m_ShotLevel = Data["ShotLevel"];
		if (Data.contains("AttackPower")) m_AttackPower = Data["AttackPower"];
		if (Data.contains("MaxShield"))   m_MaxShield = Data["MaxShield"];

		m_CurrentShield = m_MaxShield;

		// ここが出れば成功！
		OutputDebugStringA("### LOAD SUCCESS: Parameters updated! ###\n");
	}
	catch (const json::exception& e) {
		OutputDebugStringA(e.what());
	}
}

void AstroEvolution_Player::SaveData(const std::string& FilePath)
{
	json Data;
	// 現在のメンバ変数をJSON構造に入れる
	Data["MoveSpeed"] = m_MoveSpeed;
	Data["RotSpeed"] = m_RotSpeed;
	Data["ShotLevel"] = m_ShotLevel;
	Data["AttackPower"] = m_AttackPower;
	Data["MaxShield"] = m_MaxShield;

	std::ofstream File(FilePath);
	if (File.is_open()) 
	{
		File << std::setw(4) << Data << std::endl;
	}
}

void AstroEvolution_Player::ResetStatus()
{
	m_MoveSpeed = DEFAULT_SPEED;
	m_RotSpeed = DEFAULT_ROT;

	m_ShotLevel = 1;
	m_AttackPower = 10;
	m_MaxShield = 100.0f;
	m_CurrentShield = m_MaxShield;
	m_ShotTimer = 0.0f;

	OutputDebugStringA("### Player Status Reset to Default ###\n");
}

void AstroEvolution_Player::Hit(float damage)
{
	// 🌟 無敵時間中ならダメージを無効化
	if (m_InvincibleTimer > 0.0f) return;

	// HPを減らす
	m_HP -= damage;

	// 🌟 被弾したら無敵タイマーをセット
	m_InvincibleTimer = INVINCIBLE_DURATION;

	// HPが0以下になったら
	if (m_HP <= 0)
	{
		m_HP = 0;
		// 今は Init で復活する設定ですね
		Init();
		m_Dead = false;
	}
}
