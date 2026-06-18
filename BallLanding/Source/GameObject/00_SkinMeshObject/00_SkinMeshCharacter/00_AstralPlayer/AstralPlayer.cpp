#include "AstralPlayer.h"
#include "System/00_Manager/00_SkinMeshManager/SkinMeshManager.h"
#include "System/06_Camera/Camera.h"
#include "System/00_Manager/02_ImGuiManager/ImGuiManager.h"
#include "System/00_Manager/03_CollisionManager/CollisionManager.h"

#include <cmath>

using json = nlohmann::json;

//コンストラクタ.
AstralPlayer::AstralPlayer()
	: SkinMeshCharacter()

	, m_AnimList(enPlayerAnim::Idol)

	, m_ShortAttackList(enShort::None)

	, m_pCamera(nullptr)
	, m_AngleY(0.0f)      //旋回角度.
	, m_MoveSpeed(0.2f)   //移動速度.
	, m_RotSpeed(0.05f)   //旋回速度.

	, BlinkTimer(0.0f)
	, BlinkCoolDown(0.0f)
	, m_IsBlinking(false)
{
	// プレイヤーのスキンメッシュの設定
	SkinMesh* raw_mesh = SkinMeshManager::GetInstance()->GetSkinMeshInstance(SkinMeshManager::SkinList::Player);
	auto shared_mesh = std::shared_ptr<SkinMesh>(raw_mesh, [](SkinMesh*) {});
	AttachMesh(shared_mesh);

	//当たり判定の位置(体).
	m_pSphereBody = std::make_shared<BoundingSphere>();
	m_pSphereBody->SetTag(BoundingSphere::Tag::AstroPlayer);
	m_pSphereBody->SetRadius(1.2f);
	CollisionManager::GetInstance()->AddSphere(m_pSphereBody);
	//当たり判定の位置(頭).
	m_pSphereHead = std::make_shared<BoundingSphere>();
	m_pSphereHead->SetTag(BoundingSphere::Tag::AstroPlayer);
	m_pSphereHead->SetRadius(0.8f);
	CollisionManager::GetInstance()->AddSphere(m_pSphereHead);
	//当たり判定の位置(近距離攻撃).
	m_pSphereShort = std::make_shared<BoundingSphere>();
	m_pSphereShort->SetTag(BoundingSphere::Tag::AstralPlayerAttack);
	m_pSphereShort->SetRadius(1.5f);
	CollisionManager::GetInstance()->AddSphere(m_pSphereShort);


	//Playerのサイズの変更.
	SetScale(D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	//初期位置の設定.
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.f, 0.0f, 0.f);
	SetPosition(Pos);

	//ライト無効.
	this->SetLightEnable(false);

	//アニメーション設定.
	m_AnimSpeed = 0.01;
	m_Loop = true;

	//Playerの開始時は、Idolのアニメーションを再生させる.
	m_pMesh->SetAnimSpeed(m_AnimSpeed, m_pAnimCtrl);

}

//デストラクタ.
AstralPlayer::~AstralPlayer()
{
}

//動作関数.
void AstralPlayer::Update()
{
	float deltaTime = Timer::GetInstance().DeltaTime();

#if _DEBUG
	ImGui::Begin("AstralPlayer Debug");
	ImGui::Text("Attack Step: %d", (int)m_ShortAttackList);
	ImGui::End();
#endif

	// 攻撃中でない時だけ移動と旋回を許可
	if (m_ShortAttackList == enShort::None)
	{
		HandleMovement(); // 移動処理を切り出し（後述）

		// 右クリックで溜め攻撃開始
		if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		{
			m_ShortAttackList = enShort::AttackPreparation;
		}
	}
	else
	{
		// 攻撃中の更新処理（ここで溜めや終了判定を行う）
		PlayerShortAttack();
	}

	// 共通の更新
	UpdateCollisionPosition();
	SkinMeshCharacter::Update();
}

//描画関数.
void AstralPlayer::Draw()
{
#ifdef _DEBUG
	//プレイヤーのパラメータ設定・変更用のImGui.
	//jsonでも変更はできるがゲーム中に変更させたいときがあるから.
	ImGui::Begin(JAPANESE("プレイヤーのパラメータ変更"));
	if (ImGui::CollapsingHeader(JAPANESE("移動・回転速度・ブリンクの設定")))
	{
		ImGui::DragFloat(JAPANESE("速度"), &m_MoveSpeed, 0.001f, 0.0f, 10.0f);
		ImGui::DragFloat(JAPANESE("ブリンクの速度"), &m_BlinkParam.BlinkSpeed, 0.01f, 0.0f, 5.0f);
		ImGui::DragFloat(JAPANESE("ブリンクの持続時間"), &m_BlinkParam.duration, 0.01f, 0.0f, 2.0f);
		ImGui::DragFloat(JAPANESE("ブリンクのクールタイム"), &m_BlinkParam.BlinkCool, 0.01f, 0.0f, 5.0f);
		
		ImGui::Separator();

		//保存ボタン.
		if (ImGui::Button(JAPANESE("保存")))
		{
			//ボタンを押したら変更したPlayerパラメータを保存する.
			AstralPlayer_ParamSaveData("Data\\json\\AstralDuel_PlayerParam\\AstralDuel_PlayerParam.json");
		}
	}
	ImGui::End();
#endif
	SkinMeshCharacter::Draw();

//当たり判定の表示用Draw関数.
#ifdef _DEBUG
	//m_pSphereBody->Draw();
	//m_pSphereHead->Draw();
	//if (m_ShortAttackList == enShort::Attack)
	//{
	//	m_pSphereShort->Draw();
	//}
#endif // _DEBUG	
}

//初期化関数.
void AstralPlayer::Init()
{
	SetPosition(D3DXVECTOR3(0, 0, 0));
	m_AngleY = 0.0f;
}

//Playerの死んだときの判定.
bool AstralPlayer::GetDead() const
{
	return m_Dead;
}

//当たり判定のポジション設定.
void AstralPlayer::UpdateCollisionPosition()
{
	D3DXVECTOR3 pos = GetPosition();
	//当たり判定のつける位置.
	if (m_pSphereBody)
	{
		m_pSphereBody->SetPosition(pos + D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	}
	if (m_pSphereHead)
	{
		m_pSphereHead->SetPosition(pos + D3DXVECTOR3(0.0f, 3.2f, 0.0f));
	}
	if (m_pSphereShort)
	{
		//攻撃状態の時のみ当たり判定を表示する.
		if (m_ShortAttackList == enShort::Attack)
		{
			float AD = 4.5f;
			float Sin = sinf(m_AngleY);
			float Cos = cosf(m_AngleY);

			D3DXVECTOR3 AttackPos = pos;

			AttackPos.x += Sin * AD;
			AttackPos.z += Cos * AD;
			AttackPos.y += 1.5f;

			m_pSphereShort->SetPosition(AttackPos);
		}
		else
		{
			//攻撃していないときの当たり判定のいている所.
			m_pSphereShort->SetPosition(D3DXVECTOR3(0.0f, -100.0f, 0.0f));
		}
	}
}

void AstralPlayer::AstralPlayer_ParamLoadData(
	const std::string& FilePath)
{
	std::ifstream File(FilePath);
	if (!File.is_open())
	{
		// ここが出たら読み込めていない（パスが間違っている）
		OutputDebugStringA("### LOAD FAILED: File not found! ###\n");
		return;
	}

	nlohmann::json j;
	File >> j;

	//基本のパラメータ.
	//Playerの速度.
	m_MoveSpeed = j.value("MoveSpeed", 0.2f);
	//回転速度.
	m_RotSpeed = j.value("Rotiton", 0.05f);
	//ブリンクのパラメータ.
	if (j.contains("Blink"))
	{
		auto& b = j["Blink"];
		//ブリンクの速度.
		m_BlinkParam.BlinkSpeed = b.value("Speed", 3.0f);
		//ブリンクのクールタイム.
		m_BlinkParam.BlinkCool = b.value("CoolTime", 1.0f);
		//ブリンクの持続時間.
		m_BlinkParam.duration = b.value("Duration", 0.3f);
	}
}

void AstralPlayer::AstralPlayer_ParamSaveData(
	const std::string& FilePath)
{
	nlohmann::json j;

	//基本.
	j["MoveSpeed"] = m_MoveSpeed;
	j["RotSpeed"] = m_RotSpeed;

	//ブリンク.
	j["Blink"]["Speed"] = m_BlinkParam.BlinkSpeed;
	j["Blink"]["CoolTime"] = m_BlinkParam.BlinkCool;
	j["Blink"]["Duration"] = m_BlinkParam.duration;

	std::ofstream File(FilePath);
	if (File.is_open())
	{
		File << std::setw(4) << j << std::endl;
	}

}

//Playerの攻撃用の関数.
void AstralPlayer::PlayerShortAttack()
{
	//デルタタイム取得.
	float deltaTime = Timer::GetInstance().DeltaTime();

	switch (m_ShortAttackList)
	{
	case enShort::AttackPreparation:
		//設定したアニメーションの再生.
		m_pMesh->ChangeAnim(enPlayerAnim::Blade_1_2_3, m_pAnimCtrl);
		//次の状態へ遷移.
		m_ShortAttackList = enShort::Attack;
		m_HasHit = false;
		break;
	case enShort::Attack:
		//アニメーションの時間を設定.
		m_pMesh->SetAnimTime(m_pMesh->GetAnimTime() + deltaTime);
		//アニメーションが終了したときに.
		if (m_pMesh->IsAnimEnd(6))
		{
			//次の状態の遷移に入る.
			m_ShortAttackList = enShort::AttackEndIdol;
		}
		break;
	case enShort::AttackEndIdol:
		m_ShortAttackList = enShort::None;
		break;
	}
}

//WASDの移動の関数.
void AstralPlayer::HandleMovement()
{
	float deltaTime = Timer::GetInstance().DeltaTime();

	bool isMoving = false;
	D3DXVECTOR3 Pos = GetPosition();
	float SinY = sinf(m_AngleY);
	float CosY = cosf(m_AngleY);

	//クールタイムとブリンクの時間の更新.
	if (BlinkCoolDown > 0.0f)
	{
		BlinkCoolDown -= deltaTime;
	}
	//ブリンク.
	if (m_IsBlinking)
	{
		BlinkTimer -= deltaTime;
		if (BlinkTimer <= 0.0f)
		{
			m_IsBlinking = false;
		}
	}
	//ブリンク開始判定.
	if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) && BlinkCoolDown <= 0.0f)
	{
		m_IsBlinking = true;
		BlinkTimer = m_BlinkParam.duration;
		BlinkCoolDown = m_BlinkParam.BlinkCool;
	}

	//速度の決定.
	float Speed;
	if (m_IsBlinking)
	{
		//ブリンク時のPlayerの速度.
		Speed = m_BlinkParam.BlinkSpeed;
	}
	else
	{
		//ブリンク時ではないとき[通常時]の速度.
		Speed = m_MoveSpeed;
	}

	//旋回.
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_AngleY -= m_RotSpeed; isMoving = true;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_AngleY += m_RotSpeed; isMoving = true;
	}

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

	//移動アニメーションの切り替え.
	if (isMoving)
	{
		m_pMesh->ChangeAnim(enPlayerAnim::Move, m_pAnimCtrl);
	}
	else
	{
		m_pMesh->ChangeAnim(enPlayerAnim::Idol, m_pAnimCtrl);
	}
}