#include "AstralBoss.h"
#include "System/00_Manager/00_SkinMeshManager/SkinMeshManager.h"
#include "System/06_Camera/Camera.h"
#include "System/00_Manager/02_ImGuiManager/ImGuiManager.h"
#include "System/00_Manager/03_CollisionManager/CollisionManager.h"

#include <cmath> // atan2f, sinf, cosf 用

AstralBoss::AstralBoss()
	: SkinMeshCharacter()
	, m_pCamera(nullptr)
	, m_AngleY(0.0f)      // 追加: 旋回角度
	, m_MoveSpeed(0.2f)   // 追加: 移動速度
	, m_RotSpeed(0.05f)   // 追加: 旋回速度
{
	// プレイヤーのスキンメッシュの設定
	SkinMesh* raw_mesh = SkinMeshManager::GetInstance()->GetSkinMeshInstance(SkinMeshManager::SkinList::Enemy);
	auto shared_mesh = std::shared_ptr<SkinMesh>(raw_mesh, [](SkinMesh*) {});
	AttachMesh(shared_mesh);

	m_pSphereBody = std::make_shared<BoundingSphere>();
	m_pSphereBody->SetTag(BoundingSphere::Tag::AstroBoss);
	m_pSphereBody->SetRadius(2.5f);
	CollisionManager::GetInstance()->AddSphere(m_pSphereBody);

	m_pSphereBody_2 = std::make_shared<BoundingSphere>();
	m_pSphereBody_2->SetTag(BoundingSphere::Tag::AstroBoss);
	m_pSphereBody_2->SetRadius(3.0f);
	CollisionManager::GetInstance()->AddSphere(m_pSphereBody_2);

	m_pSphereBody_3 = std::make_shared<BoundingSphere>();
	m_pSphereBody_3->SetTag(BoundingSphere::Tag::AstroBoss);
	m_pSphereBody_3->SetRadius(1.5f);
	CollisionManager::GetInstance()->AddSphere(m_pSphereBody_3);

	m_pSphereHead = std::make_shared<BoundingSphere>();
	m_pSphereHead->SetTag(BoundingSphere::Tag::AstralBoss);
	m_pSphereHead->SetRadius(0.8f);
	CollisionManager::GetInstance()->AddSphere(m_pSphereHead);


	//Bossのサイズの変更
	SetScale(D3DXVECTOR3(7.0f, 7.0f, 7.0f));

	// 初期位置の設定
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.f, 0.0f, 10.f);
	SetPosition(Pos);

	// ライト無効（必要に応じてtrueに変更してください）
	this->SetLightEnable(false);

	// アニメーション設定
	m_AnimSpeed = 0.01f;
	m_Loop = true;
}

AstralBoss::~AstralBoss()
{
}

void AstralBoss::Update()
{
	if (m_Dead)
	{
		// 死んでいるときは当たり判定を遠くに飛ばすか、マネージャーから除外する処理が必要
		// 簡易的には位置を遥か彼方に飛ばす方法があります
		if (m_pSphereBody) m_pSphereBody->SetPosition(D3DXVECTOR3(0, -1000.0f, 0));
		if (m_pSphereBody_2) m_pSphereBody_2->SetPosition(D3DXVECTOR3(0, -1000.0f, 0));
		if (m_pSphereBody_3) m_pSphereBody_3->SetPosition(D3DXVECTOR3(0, -1000.0f, 0));
		if (m_pSphereHead) m_pSphereHead->SetPosition(D3DXVECTOR3(0, -1000.0f, 0));
		return;
	}

	if (m_InvincibleTimer > 0.0f)
	{
		m_InvincibleTimer -= 1.0f / 60.0f; // デルタタイムがあればそれを使用
		if (m_InvincibleTimer < 0.0f) m_InvincibleTimer = 0.0f;
	}
	// --- デバッグ表示 (AstroEvolution風) ---
#if _DEBUG
	ImGui::Begin("AstralBoss Debug");

	// HPの数値を表示
	ImGui::Text("HP: %.1f / 100.0", m_HP);

	// HPバーの表示 (現在値 / 最大値)
	float hpRatio = m_HP / 100.0f; // 0.0〜1.0の範囲にする
	// バーの色を体力に合わせて変える（オプション）
	ImVec4 color = ImVec4(1.0f - hpRatio, hpRatio, 0.0f, 1.0f); // 赤 → 緑に変化
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, color);
	ImGui::ProgressBar(hpRatio, ImVec2(-1.0f, 0.0f));
	ImGui::PopStyleColor();

	ImGui::Separator(); // 区切り線

	ImGui::DragFloat("Move Speed", &m_MoveSpeed, 0.01f, 0.0f, 2.0f);
	ImGui::DragFloat("Rotation Speed", &m_RotSpeed, 0.01f, 0.0f, 0.5f);
	ImGui::Text("AngleY: %.2f", m_AngleY);

	// デバッグ用にHPを減らすボタン（動作テスト用）
	if (ImGui::Button("Damage 10")) {
		OnHit(10.0f);
	}

	ImGui::End();
#endif

	SetRotation(D3DXVECTOR3(0.0f, m_AngleY, 0.0f)); // 旋回角度を反映

	UpdateCollisionPosition();

	// 4. 親クラスのUpdate（アニメーション更新）
	SkinMeshCharacter::Update();
}

void AstralBoss::Draw()
{
	if (m_Dead) return;
	SkinMeshCharacter::Draw();
	//m_pSphereBody->Draw();
	//m_pSphereBody_2->Draw();
	//m_pSphereBody_3->Draw();
	//m_pSphereHead->Draw();
}

void AstralBoss::Init()
{
	// 必要に応じて初期座標リセットなどを記述
	SetPosition(D3DXVECTOR3(0, 0, 0));
	m_AngleY = 0.0f;
	m_HP = 100.0f;
	m_Dead = false;
	m_InvincibleTimer = 0.0f;
}

bool AstralBoss::GetDead() const
{
	return m_Dead;
}

void AstralBoss::UpdateCollisionPosition()
{
	D3DXVECTOR3 pos = GetPosition();

	// コンストラクタやUpdateで決めたオフセットと同じ値を指定
	if (m_pSphereBody) m_pSphereBody->SetPosition(pos + D3DXVECTOR3(0.0f, 7.0, 0.5f));
	if (m_pSphereBody) m_pSphereBody_2->SetPosition(pos + D3DXVECTOR3(0.0f, 3.0, 0.5f));
	if (m_pSphereBody) m_pSphereBody_3->SetPosition(pos + D3DXVECTOR3(0.0f, 10.0, 0.5f));
	if (m_pSphereHead) m_pSphereHead->SetPosition(pos + D3DXVECTOR3(0.0f, 12.0f, 0.0f));
}
