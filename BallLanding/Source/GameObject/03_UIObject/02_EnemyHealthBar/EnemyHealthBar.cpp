#include "EnemyHealthBar.h"

#include "Sprite2D//Sprite2D.h"
#include "GameObject//02_StaticMeshObject//06_Character//01_Enemy//Enemy.h"

#include "DirectX//DirectX11.h"

//コンストラクタ.
EnemyHealthBar::EnemyHealthBar()
	: UIObject			()

	, m_pTargetEnemy	( nullptr )
	, m_MaxBarScale		( 0.0f )
	
{
	//各パーツのインスタンス生成.
	m_upBase			= std::make_shared<UIObject>();
	m_upBack			= std::make_shared<UIObject>();
	m_upDamage			= std::make_shared<UIObject>();
	m_upGauge			= std::make_shared<UIObject>();

	m_spBaseSprite		= std::make_shared<Sprite2D>();
	m_spBackSprite		= std::make_shared<Sprite2D>();
	m_spDamageSprite	= std::make_shared<Sprite2D>();
	m_spGaugeSprite		= std::make_shared<Sprite2D>();

	Create();
}

//デストラクタ.
EnemyHealthBar::~EnemyHealthBar()
{
}

//動作関数.
#if 0
void EnemyHealthBar::Update()
{
	//ターゲットがいない、または死んでいたら何もしない.
	if (m_pTargetEnemy == nullptr)
	{
		return;
	}
	if (m_pTargetEnemy->IsDead() == true)
	{
		return;
	}
	//座標の更新.
	//敵の位置を取得.
	D3DXVECTOR3 WorldPos = m_pTargetEnemy->GetPosition();
	//敵の少し上にUIを表示する.
	WorldPos.y += 2.0f;

	//3Dの座標を2Dピクセル座標に変換する.
	D3DXVECTOR3 ScreenPos = WorldToScreen(WorldPos);

	//カメラの裏側に敵がいる場合座標返還の結果が画面内に収まっていても、
	//Z値がおかしくなるので、描画をしないようにする.
	if (ScreenPos.z < 0.0f)
	{
		return;
	}
	if (ScreenPos.z > 1.0f)
	{
		return;
	}

	//メインの枠の位置を決定.
	m_upBase->SetPosition(ScreenPos.x, ScreenPos.y, 0.0f);

	//整数同士の割り算は0か1になってしまうのでfloatに変換をして計算する.
	float HpRate = static_cast<float>(m_pTargetEnemy->GetHP()) / static_cast<float>(m_pTargetEnemy->GetMaxHP());
	//計算結果が0.0~1.0の範囲に収まるように制限.
	HpRate = max(0.0f, min(1.0f, HpRate));
	//画像の元の幅に対してHPの割合を掛けてスケールを出させる.
	float TargetScaleX = m_MaxBarScale * HpRate;

	//緑色のゲージ.
	if (m_upGauge != nullptr)
	{
		D3DXVECTOR3 GreenScale = m_upGauge->GetScale();
		//計算した幅をセット.
		GreenScale.x = TargetScaleX;
		m_upGauge->SetScale(GreenScale);
		//枠の位置に合わせたバーの入りも追従させる.
		m_upGauge->SetPosition(ScreenPos.x, ScreenPos.y, 0.0f);
	}
	//赤色のゲージ.
	if (m_upDamage != nullptr)
	{
		D3DXVECTOR3 RedScale = m_upDamage->GetScale();
		//現在の赤い幅とターゲットの幅の差分に0.05を掛けて少しずつ近づけていく.
		//ダメージを受けたときにバーが追いかけるようにする.
		//補間速度.
		float InterpolationSpeed = 0.05f;
		RedScale.x += (TargetScaleX - RedScale.x) * InterpolationSpeed;
		//赤いバーが緑バーより短くならないようにする.
		if (RedScale.x < TargetScaleX)
		{
			RedScale.x = TargetScaleX;
		}
		m_upDamage->SetScale(RedScale);
		//位置を追従.
		m_upDamage->SetPosition(ScreenPos.x, ScreenPos.y, 0.0f);
	}
	//背景バーも敵の位置に追従させる.
	if (m_upBack != nullptr)
	{
		m_upBack->SetPosition(ScreenPos.x, ScreenPos.y, 0.0f);
	}
	m_upBase->Update();
	m_upBack->Update();
	m_upDamage->Update();
	m_upGauge->Update();
}
#else
void EnemyHealthBar::Update()
{
	if (m_pTargetEnemy == nullptr || m_pTargetEnemy->IsDead()) return;

	// 1. 座標計算
	D3DXVECTOR3 WorldPos = m_pTargetEnemy->GetPosition();
	WorldPos.y += 1.8f;

	D3DXVECTOR3 ScreenPos = WorldToScreen(WorldPos);
	if (ScreenPos.z < 0.0f || ScreenPos.z > 1.0f) return;

	// --- 全体の位置調整 ---
	float barOffsetX = -30.0f; // 全体を左にずらす量
	float finalX = ScreenPos.x + barOffsetX;

	// 2. 枠（Base）の位置
	m_upBase->SetPosition(finalX, ScreenPos.y, 0.0f);

	// --- 内部パーツの微調整（ここが修正ポイント！） ---
	// 枠の中心に対して、中身が左にズレるのを防ぐための補正値です。
	// 画像を見ながら 1.0f や 2.0f 程度で調整してみてください。
	float innerAdjustX = 2.0f;
	float partsX = finalX + innerAdjustX;

	// 3. HP割合計算
	float HpRate = static_cast<float>(m_pTargetEnemy->GetHP()) / static_cast<float>(m_pTargetEnemy->GetMaxHP());
	HpRate = max(0.0f, min(1.0f, HpRate));
	float TargetScaleX = m_MaxBarScale * HpRate;

	// 4. 各パーツの更新
	if (m_upGauge)
	{
		m_upGauge->SetScale(D3DXVECTOR3(TargetScaleX, m_upGauge->GetScale().y, 1.0f));
		m_upGauge->SetPosition(partsX, ScreenPos.y, 0.0f); // 補正後の座標
	}
	if (m_upDamage)
	{
		D3DXVECTOR3 RedScale = m_upDamage->GetScale();
		RedScale.x += (TargetScaleX - RedScale.x) * 0.05f;
		if (RedScale.x < TargetScaleX) RedScale.x = TargetScaleX;
		m_upDamage->SetScale(RedScale);
		m_upDamage->SetPosition(partsX, ScreenPos.y, 0.0f); // 補正後の座標
	}
	if (m_upBack)
	{
		m_upBack->SetPosition(partsX, ScreenPos.y, 0.0f); // 補正後の座標
	}

	m_upBase->Update();
	m_upBack->Update();
	m_upDamage->Update();
	m_upGauge->Update();
}
#endif
//描画関数.
void EnemyHealthBar::Draw()
{
	// ターゲットがいない、または死んでいたら描画しない
	if (m_pTargetEnemy == nullptr || m_pTargetEnemy->IsDead() == true)
	{
		return;
	}

	// UI（2D）として最前面に描画するため、一時的に深度テストを無効にする
	DirectX11::GetInstance()->SetDepth(false);

	// 背面から順に描画（背景 -> 赤バー -> 緑バー -> 枠）
	if (m_upBack)   m_upBack->Draw();
	if (m_upDamage) m_upDamage->Draw();
	if (m_upGauge)  m_upGauge->Draw();
	if (m_upBase)   m_upBase->Draw();

	// 他の3Dオブジェクトの描画に影響しないよう深度テストを元に戻す
	DirectX11::GetInstance()->SetDepth(true);
}

void EnemyHealthBar::Create()
{
	// --- パラメータ調整 ---
	const float baseW = 60.0f;  // さらに一回り小さく(一般敵用)
	const float baseH = 8.0f;
	const float barW = 1000.0f;
	const float barH = 34.0f;

	// 枠の厚みに合わせて調整（枠が細いなら 2.0f くらいにする）
	// ここを大きくすると、中身のバーが短くなります
	const float paddingX = 2.0f;
	const float innerBarW = baseW - (paddingX * 2.0f);

	m_MaxBarScale = innerBarW / barW;

	// 枠
	Sprite2D::SPRITE_STATE ssBase = { baseW, baseH, baseW, baseH, baseW, baseH };
	m_spBaseSprite->Init(_T("Data\\Image\\Buttle\\HPBase.png"), ssBase);
	m_upBase->AttachSprite(m_spBaseSprite);

	// 中身
	Sprite2D::SPRITE_STATE ssBar = { barW, barH, barW, barH, barW, barH };
	// 縦幅を枠の高さに対して 0.4～0.5 倍程度に
	D3DXVECTOR3 initialScale(m_MaxBarScale, 0.2f, 1.0f);

	m_spBackSprite->Init(_T("Data\\Image\\Buttle\\GaugeBack.png"), ssBar);
	m_upBack->AttachSprite(m_spBackSprite);
	m_upBack->SetScale(initialScale);

	m_spDamageSprite->Init(_T("Data\\Image\\Buttle\\HPDamage.png"), ssBar);
	m_upDamage->AttachSprite(m_spDamageSprite);
	m_upDamage->SetScale(initialScale);

	m_spGaugeSprite->Init(_T("Data\\Image\\Buttle\\HPGauge.png"), ssBar);
	m_upGauge->AttachSprite(m_spGaugeSprite);
	m_upGauge->SetScale(initialScale);
}



//どの敵のHPを参照するか.
void EnemyHealthBar::SetTarget(Enemy* pEnemy)
{
	m_pTargetEnemy = pEnemy;
}

//3D座標を2Dスクリーン座標に変換する.
D3DXVECTOR3 EnemyHealthBar::WorldToScreen(const D3DXVECTOR3 WorldPos)
{
	D3DXVECTOR3 ScreenPos;
	D3D11_VIEWPORT Viewport = DirectX11::GetInstance()->GetViewport();
	D3DXMATRIX View = DirectX11::GetInstance()->GetViewMatrix();
	D3DXMATRIX Proj = DirectX11::GetInstance()->GetProjectionMatrix();

	//ワールド行列は単位行列で計算(移動・回転は計算しない).
	D3DXMATRIX World;
	D3DXMatrixIdentity(&World);

	D3DVIEWPORT9 vp9;
	vp9.X = static_cast<DWORD>(Viewport.TopLeftX);
	vp9.Y = static_cast<DWORD>(Viewport.TopLeftY);
	vp9.Width = static_cast<DWORD>(Viewport.Width);
	vp9.Height = static_cast<DWORD>(Viewport.Height);
	vp9.MinZ = Viewport.MinDepth;
	vp9.MaxZ = Viewport.MaxDepth;

	// 3. 変換したvp9を渡して計算
	D3DXVec3Project(
		&ScreenPos,
		&WorldPos,
		&vp9,       // ここで詰め替えたvp9を渡す
		&Proj,
		&View,
		&World
	);

	return ScreenPos;
}
