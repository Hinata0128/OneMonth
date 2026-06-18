#include "PlayerShot.h"
#include "System/00_Manager/01_StaticMeshManager/StaticMeshManager.h"

PlayerShot::PlayerShot()
	: ShotBase()

	, m_BSphere ( std::make_unique<BoundingSphere>())

	, m_Target(nullptr)
	, m_HomingPower(0.1f)
{
	AttachMesh(*StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Ball));

	this->SetLightEnable(false);

	Init();
}

PlayerShot::~PlayerShot()
{

}

void PlayerShot::Update()
{
	if (!m_IsActive)
		return;

	// ターゲットが存在していて生きているならホーミング
	if (m_Target && !m_Target->IsDead())
	{
		D3DXVECTOR3 toTarget = m_Target->GetPosition() - m_Position;
		D3DXVec3Normalize(&toTarget, &toTarget);

		m_Direction += (toTarget - m_Direction) * m_HomingPower;
		D3DXVec3Normalize(&m_Direction, &m_Direction);
	}
	else
	{
		// ターゲットが消えたらロック解除
		m_Target = nullptr;
	}

	// ★ 常に移動（ここが重要）
	m_Position += m_Direction * m_Speed;

	if (m_BSphere)
		m_BSphere->SetPosition(m_Position);
}

void PlayerShot::Draw()
{
	if (m_Disp)
	{
		ShotBase::Draw();
		m_BSphere->Draw();
	}
}

void PlayerShot::Init()
{
	//親クラスの基本初期化
	ShotBase::Init();

	m_Disp = false;
	m_IsActive = false;

	//見た目の大きさ（プレイヤーの弾）
	m_Scale = D3DXVECTOR3(5.0f, 5.0f, 5.0f);

	//当たり判定の大きさ（スケールに合わせて）
	if (m_BSphere)
	{
		m_BSphere->SetRadius(0.5f); 
	}
}

void PlayerShot::Reload(
	const D3DXVECTOR3& pos,
	Enemy* target,
	float speed,
	const D3DXVECTOR3& forwardDir)
{
	m_Position = pos;
	m_Target = target;
	m_Speed = speed;

	if (m_Target)
	{
		// 敵方向へ
		m_Direction = m_Target->GetPosition() - m_Position;
	}
	else
	{
		// ★ カメラ方向へ
		m_Direction = forwardDir;
	}

	D3DXVec3Normalize(&m_Direction, &m_Direction);

	m_Disp = true;
	m_IsActive = true;

	if (m_BSphere)
	{
		m_BSphere->SetPosition(m_Position);
		m_BSphere->SetRadius(0.5f);
	}
}

BoundingSphere* PlayerShot::GetSphere() const
{
	return m_BSphere.get();
}

void PlayerShot::OnHit()
{
	Init();	// 非表示＆非アクティブに戻す
}