#include "Enemy.h"
#include "System//00_Manager//01_StaticMeshManager//StaticMeshManager.h"
#include "System/02_Singleton/02_SingletonManager/05_EnemyShotManager/EnemyShotManager.h"
#include "GameObject/02_StaticMeshObject/06_Character/00_Player/Player.h"

#include "System/02_Singleton/00_Timer/Timer.h"

#include "DirectX/DirectX11.h"

#include <random>

Enemy::Enemy()
	: Character()

	, m_pPlayer(nullptr)

	, m_ShotCoolDown(0.0f)
	, m_CoolTime(2.0f)

	, m_IsDead(false)

	, m_HP(30)
	, m_MaxHP(30)

	, m_MoveType(MoveType::None)
	, m_MoveSpeed(0.0f)
	, m_MoveAngle(0.0f)
	, m_MoveRange(0.0f)
	, m_BasePos(0.0f, 0.0f, 0.0f)
{
	//読み込み.
	auto pMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Enemy);
	//設定.
	AttachMesh(*pMesh);

	//ポジションの設定.
	D3DXVECTOR3 Pos = { 0.0f,0.0f,-4.0f };
	SetPosition(Pos);

	//サイズの変更.
	D3DXVECTOR3 Scale = { 0.5f,0.5f,0.5f };
	SetScale(Scale);

	//光遮断.
	this->SetLightEnable(true);

	m_BSphere = std::make_unique<BoundingSphere>();
	m_BSphere->SetRadius(1.0f);   // 敵の大きさに合わせて調整
	m_BSphere->SetPosition(GetPosition());

	Init();

}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	float deltaTime = Timer::GetInstance().DeltaTime();

	UpdateMovement(deltaTime);

	m_ShotCoolDown -= deltaTime;

	if (m_BSphere)
	{
		m_BSphere->SetPosition(GetPosition());
	}

	if (m_ShotCoolDown < 0.0f)
	{
		m_ShotCoolDown = 0.0f;
	}
	//弾の発射.
	AutoShot();

	Character::Update();

	if (m_upHPBar != nullptr)
	{
		m_upHPBar->Update();
	}
}

void Enemy::UpdateMovement(float deltaTime)
{
	// 移動しないタイプなら何もしない
	if (m_MoveType == MoveType::None) return;

	// --- 修正ポイント：基準座標の確定 ---
	// コンストラクタ時点では座標が確定していないため、
	// 最初のUpdateが呼ばれた瞬間の座標を基準点(m_BasePos)として保存する。
	if (m_BasePos.x == 0.0f && m_BasePos.y == 0.0f && m_BasePos.z == 0.0f)
	{
		m_BasePos = GetPosition();
	}

	// 時間経過で角度（フェーズ）を進める
	m_MoveAngle += m_MoveSpeed * deltaTime;

	D3DXVECTOR3 currentPos = m_BasePos;
	float offset = sinf(m_MoveAngle) * m_MoveRange;

	// タイプ別に座標をオフセットさせる
	switch (m_MoveType)
	{
	case MoveType::Horizontal:
		currentPos.x += offset; // 左右に揺れる
		break;
	case MoveType::Vertical:
		currentPos.y += offset; // 上下に揺れる
		break;
	case MoveType::Random:
		// X軸はサイン波、Y軸はコサイン波で少し周期をずらして8の字に近い動きにする
		currentPos.x += offset;
		currentPos.y += cosf(m_MoveAngle * 0.7f) * m_MoveRange;
		break;
	default:
		break;
	}

	// 最終的な座標をセット
	SetPosition(currentPos);
}

void Enemy::Draw()
{
	Character::Draw();

#ifdef _DEBUG
	m_BSphere->Draw();
#endif
	if (m_upHPBar != nullptr)
	{
		m_upHPBar->Draw();
	}
}

void Enemy::Init()
{
	m_upHPBar = std::make_unique<EnemyHealthBar>();
	m_upHPBar->SetTarget(this);

	// 乱数生成器の準備
	std::random_device rd;
	std::mt19937 gen(rd());

	// 1. 移動パターンの決定
	// static_cast<int>(MoveType::Max) を使うことで、定義が増えても対応可能
	std::uniform_int_distribution<int> dist(0, static_cast<int>(MoveType::Max) - 1);
	m_MoveType = static_cast<MoveType>(dist(gen));

	// 2. 移動速度（周波数）の決定 (1.0 ～ 3.0)
	std::uniform_real_distribution<float> speedDist(1.0f, 3.0f);
	m_MoveSpeed = speedDist(gen);

	// 3. 移動範囲（振幅）の決定 (1.0 ～ 4.0)
	std::uniform_real_distribution<float> rangeDist(1.0f, 4.0f);
	m_MoveRange = rangeDist(gen);

	// 4. 開始角度の決定 (0 ～ 2π)
	// これにより、同じタイミングで生成された敵も動きが同期しない
	std::uniform_real_distribution<float> angleDist(0.0f, 6.283185f);
	m_MoveAngle = angleDist(gen);
}

bool Enemy::GetDead() const
{
	return m_IsDead;
}

void Enemy::AutoShot()
{
	// プレイヤーがいない、またはクールタイム中なら何もしない
	if (!m_pPlayer || m_ShotCoolDown > 0.0f) return;

	if (!IsVisibleOnScreen())
	{
		return;
	}

	// 1. プレイヤーの位置を取得
	D3DXVECTOR3 playerPos = m_pPlayer->GetPosition();

	// 2. 発射位置（自分の位置から少し上）
	D3DXVECTOR3 shotPos = GetPosition() + D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 3. 方向ベクトルの計算
	D3DXVECTOR3 finalDir = playerPos - shotPos;
	float len = D3DXVec3Length(&finalDir);

	// ゼロ除算防止
	if (len <= 0.001f) return;

	// 正規化（長さを1にする）
	D3DXVec3Normalize(&finalDir, &finalDir);

	// 4. 弾マネージャーを使って発射
	// インクルードしている EnemyShotManager を使用
	EnemyShotManager::GetInstance()->AddEnemyNomalShot(shotPos, finalDir);

	// 5. クールタイムをリセット
	m_ShotCoolDown = m_CoolTime;
}

BoundingSphere& Enemy::GetBoundingSphere()
{
	return *m_BSphere;
}

void Enemy::OnHit(int damege)
{
	m_HP -= damege;
	if (m_HP <= 0)
	{
		m_HP = 0;
		m_IsDead = true;
	}
}

bool Enemy::IsDead() const
{
	return m_IsDead;
}

bool Enemy::IsVisibleOnScreen() const
{
	// HPバークラスが持っている WorldToScreen を利用して座標変換
		// （m_upHPBarが作成されていない場合は安全のために true を返して攻撃を許可するか、falseにするか選べます）
	if (!m_upHPBar) return true;

	D3DXVECTOR3 screenPos = m_upHPBar->WorldToScreen(GetPosition());

	// 1. カメラの前後判定 (0.0f ～ 1.0f の間ならカメラの前方にいる)
	if (screenPos.z < 0.0f || screenPos.z > 1.0f) return false;

	// 2. 画面の上下左右に収まっているか判定
	D3D11_VIEWPORT vp = DirectX11::GetInstance()->GetViewport();

	// 少し余裕（マージン）を持たせたい場合は、数値を足し引きしてください
	if (screenPos.x < 0.0f || screenPos.x > vp.Width)  return false;
	if (screenPos.y < 0.0f || screenPos.y > vp.Height) return false;

	return true;
}
