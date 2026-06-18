#include "AstroEvolution_Rock.h"

#include "System/00_Manager/03_CollisionManager/CollisionManager.h"


namespace {
	const float DEFAULT_SPEED = 0.1f;
	const float DEFAULT_ROT = 0.05f;
}

AstroEvolution_Rock::AstroEvolution_Rock()
	: Character()

	, m_AngleY(0.0f)
	, m_MoveSpeed(0.1f)
	, m_RotSpeed(0.05f)

	, m_MaxHP(100)  
	, m_HP(100)  
	, m_Phase(0)

	, m_Timer(0.0f)

{
	//読み込み.
	auto pMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Rock_OneHundred);
	//設定.
	AttachMesh(*pMesh);
	//光遮断.
	this->SetLightEnable(false);


	m_pSphere = std::make_shared<BoundingSphere>();

	if (pMesh) 
	{
		m_pSphere->CreateSphereForMesh(*pMesh);
		//当たり判定のサイズの設定.
		m_pSphere->SetRadius(1.0f);

	}
	Init();

}

AstroEvolution_Rock::~AstroEvolution_Rock()
{
}

void AstroEvolution_Rock::Update()
{
	// 死んでいたら描画しない
	if (IsDead()) return;

	auto* pMeshManager = StaticMeshManager::GetInstance();
	D3DXVECTOR3 fixedScale = { 0.09f, 0.09f, 0.09f }; // 固定したいサイズ

	// Phase 0 -> 1 (HP 50以下)
	if (m_Phase == 0 && m_HP <= 50) {
		m_Phase = 1;
		auto pMesh = pMeshManager->GetMeshInstance(StaticMeshManager::CMeshList::Rock_Fifty);
		if (pMesh) {
			AttachMesh(*pMesh);
			SetScale(fixedScale); // 🌟 貼り直し後にサイズを再設定
		}
	}
	// Phase 1 -> 2 (HP 20以下)
	else if (m_Phase == 1 && m_HP <= 20) {
		m_Phase = 2;
		auto pMesh = pMeshManager->GetMeshInstance(StaticMeshManager::CMeshList::Rock_Ten);
		if (pMesh) {
			AttachMesh(*pMesh);
			SetScale(fixedScale); // 🌟 貼り直し後にサイズを再設定
		}
	}
	m_Timer += 0.05f;

	D3DXVECTOR3 currentPos = GetPosition();

	switch (m_Pattern)
	{
	case MovePattern::UpDown:
		currentPos.y += sinf(m_Timer) * m_MoveSpeed; // サイン波でふわふわ上下
		break;

	case MovePattern::LeftRight:
		currentPos.x += cosf(m_Timer) * m_MoveSpeed; // コサイン波で左右
		break;

	case MovePattern::Wait:
		// その場待機なので何もしない（あるいは自転させるだけ）
		break;
	}

	SetPosition(currentPos);

	Character::Update();
	//岩に当たり判定を引っ付ける.
	m_pSphere->SetPosition(m_Position);

	//m_AngleY += m_RotSpeed;
	//SetRotation(D3DXVECTOR3(0, m_AngleY, 0));
}

void AstroEvolution_Rock::Draw()
{
	// 死んでいたら描画しない
	if (IsDead()) return;
	Character::Draw();
	//m_pSphere->Draw();
}

void AstroEvolution_Rock::Init()
{
	D3DXVECTOR3 Pos = { 0.0f,0.0f,1.0f };
	SetPosition(Pos);

	D3DXVECTOR3 Scale = { 10.0f,10.0f,10.0f };
	SetScale(Scale);
	m_Pattern = static_cast<MovePattern>(rand() % static_cast<int>(MovePattern::Max));
	m_Timer = (float)(rand() % 100); // 動き出しをバラけさせるためにタイマーもランダム
	m_MoveSpeed = 0.05f;

	m_MaxHP = 100;
	m_HP = m_MaxHP;
	m_Phase = 0;
}

bool AstroEvolution_Rock::GetDead() const
{
	return IsDead();
}

void AstroEvolution_Rock::OnHit(int damage)
{
	if (IsDead()) return; // すでに死んでいたら何もしない

	// 1. 体力を減らす
	m_HP -= damage;

	// 2. 体力が0以下になったかチェック
	if (m_HP <= 0)
	{
		m_HP = 0; // マイナスにならないように固定
		if (m_pSphere)
		{
			// ここで初めて「死亡」状態にする
			m_pSphere->SetDead(true);
		}
	}
}

bool AstroEvolution_Rock::IsDead() const
{
	// スフィアが存在して、かつ死んでいれば「岩は死んでいる」とみなす
	if (m_pSphere)
	{
		return m_pSphere->IsDead();
	}
	return true;
}