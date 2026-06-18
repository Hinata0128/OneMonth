#include "Player.h"

#include "System//00_Manager//01_StaticMeshManager//StaticMeshManager.h"
#include "System/02_Singleton/02_SingletonManager/03_PlayerShotManager/PlayerShotManager.h"

#include "System/00_Manager/03_CollisionManager/CollisionManager.h"

#include "System/06_Camera/Camera.h"


#include "System/02_Singleton/02_SingletonManager/04_EnemyManager/EnemyManager.h"

#include "System/00_Manager/02_ImGuiManager/ImGuiManager.h"

Player::Player()
    : Character()

    , m_angleY(0.0f)
    , m_Dead(false)

    , m_pCamera(nullptr)

    , m_ShotTimer(0.0f)
    , m_ShotInteval(0.5f)


    , m_pitch(0.0f)
    , m_roll(0.0f)
{
    SetSpeed(5.0f);
	//読み込み.
	auto pMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Fighter);
	//設定.
	AttachMesh(*pMesh);
	//ポジションの設定.
	D3DXVECTOR3 Pos = { 0.0f,0.0f,-4.0f };
	SetPosition(Pos);

	//サイズの変更.
	D3DXVECTOR3 Scale = { 1.5f,1.5f,1.5f };
	SetScale(Scale);
    this->SetLightEnable(false);


    m_pSphere = std::make_shared<BoundingSphere>();
    m_pSphere->SetTag(BoundingSphere::Tag::Player);
    if (pMesh)
    {
        //メッシュから半径を計算.
        m_pSphere->CreateSphereForMesh(*pMesh);
        //当たり判定のサイズの設定.
        m_pSphere->SetRadius(1.0f);
    }
    CollisionManager::GetInstance()->SetPlayer(this);
    CollisionManager::GetInstance()->AddSphere(m_pSphere);

    Init();

}

Player::~Player()
{
}
#if 0
void Player::Update()
{
    if (!m_pCamera) return;

    //==============================
    // カメラ更新（視点操作）
    //==============================
    m_pCamera->Update(m_Position);
    float deltaTime = Timer::GetInstance().DeltaTime();

    // カメラYaw取得
    float camYaw = m_pCamera->GetAngleY();

    float cosY = cosf(camYaw);
    float sinY = sinf(camYaw);

    // 前・右方向ベクトル
    D3DXVECTOR3 forward(sinY, 0.0f, cosY);
    D3DXVECTOR3 right(cosY, 0.0f, -sinY);

    D3DXVec3Normalize(&forward, &forward);
    D3DXVec3Normalize(&right, &right);

    //==============================
    // 移動処理
    //==============================
    D3DXVECTOR3 moveVec(0, 0, 0);

    if (GetAsyncKeyState('W') & 0x8000) moveVec += forward;
    if (GetAsyncKeyState('S') & 0x8000) moveVec -= forward;
    if (GetAsyncKeyState('D') & 0x8000) moveVec += right;
    if (GetAsyncKeyState('A') & 0x8000) moveVec -= right;

    float speed = 0.2f;

    if (D3DXVec3LengthSq(&moveVec) > 0.0f)
    {
        D3DXVec3Normalize(&moveVec, &moveVec);
        m_Position += moveVec * speed;

        // 移動方向にモデル回転
        m_Rotation.y = atan2f(moveVec.x, moveVec.z);
    }

    //=========================
    // 攻撃（スペースキー）
    //=========================
    m_ShotTimer += deltaTime;

    if (GetAsyncKeyState(VK_SPACE) & 0x8000 &&
        m_ShotTimer >= m_ShotInteval)
    {
        m_ShotTimer = 0.0f;

        D3DXVECTOR3 shotPos = m_Position;
        //弾の発射位置の設定.
        shotPos.z += 0.5f;

        // 視野角内ターゲット取得（約30度）
        Enemy* target = FindAimTarget(0.965f);

        D3DXVECTOR3 forward = m_pCamera->GetForward();
        D3DXVec3Normalize(&forward, &forward);

        PlayerShotManager::GetInstance()->AddHomingShot(
            shotPos,
            target,
            forward
        );
    }

    //==============================
    // 当たり判定用Sphere更新
    //==============================
    if (m_pSphere)
        m_pSphere->SetPosition(m_Position);

    Character::Update();
}
#else
#if 0
void Player::Update()
{
    float dt = Timer::GetInstance().DeltaTime();
    if (!m_pCamera) return;

    // --- パラメータ設定 ---
    const float rollLimit = D3DXToRadian(45.0f);   // 見た目の傾き制限
    const float lerpSpeed = 5.0f;                  // 傾きの戻り速さ
    const float sideMoveSpeed = 30.0f;             // 横移動の速さ
    const float forwardSpeed = 60.0f;              // 前進速度

    // 1. 【ロール（見た目だけの傾き）】
    // 角度を「加算」せず、入力がある時だけ目標角度を設定する
    float targetRoll = 0.0f;
    float sideInput = 0.0f; // 横移動量

    if (GetAsyncKeyState('A') & 0x8000) {
        targetRoll = rollLimit;  // 左に傾く
        sideInput = -1.0f;       // 左へ
    }
    if (GetAsyncKeyState('D') & 0x8000) {
        targetRoll = -rollLimit; // 右に傾く
        sideInput = 1.0f;        // 右へ
    }
    // 姿勢だけを滑らかに補間（離すと0に戻る）
    m_roll += (targetRoll - m_roll) * lerpSpeed * dt;

    // 2. 【ピッチ（上下移動）】
    float targetPitch = 0.0f;
    float upDownInput = 0.0f;
    if (GetAsyncKeyState('W') & 0x8000) {
        targetPitch = -D3DXToRadian(30.0f);
        upDownInput = 1.0f;  // 上へ
    }
    if (GetAsyncKeyState('S') & 0x8000) {
        targetPitch = D3DXToRadian(30.0f);
        upDownInput = -1.0f; // 下へ
    }
    m_pitch += (targetPitch - m_pitch) * lerpSpeed * dt;

    // 3. 【移動計算：回転を増やさず、ベクトルで動かす】
    // カメラの現在の向き（Yaw）を基準にする
    float yaw = m_pCamera->GetAngleY();

    D3DXMATRIX matYaw;
    D3DXMatrixRotationY(&matYaw, yaw);

    // 前方向・横方向のベクトルを算出
    D3DXVECTOR3 forward(0, 0, 1);
    D3DXVECTOR3 right(1, 0, 0);
    D3DXVec3TransformNormal(&forward, &forward, &matYaw);
    D3DXVec3TransformNormal(&right, &right, &matYaw);

    // 座標更新：前進 + 横移動
    m_Position += forward * forwardSpeed * dt;
    m_Position += right * (sideInput * sideMoveSpeed) * dt;
    // 上下移動（必要であれば）
    m_Position.y += upDownInput * sideMoveSpeed * dt;

    // 4. 【見た目の回転反映】
    // モデルの描画角度。 Yawはカメラに合わせ、Roll/Pitchは入力状態を反映
    m_Rotation.x = m_pitch;
    m_Rotation.y = yaw; // 向きはカメラと同じ
    m_Rotation.z = m_roll;

    // 当たり判定更新
    if (m_pSphere) m_pSphere->SetPosition(m_Position);

    Character::Update();
}
#endif
void Player::Update()
{
    // タイマーから経過時間を取得
    float dt = Timer::GetInstance().DeltaTime();

    // カメラがない場合は処理しない
    if (!m_pCamera) return;

    // --- 1. カメラと入力情報の取得 ---
    float targetPitch = m_pCamera->GetAngleX();
    float targetYaw = m_pCamera->GetAngleY();

    // --- 2. ロール(Roll)の計算 ---
    float targetRoll = 0.0f;
    const float rollLimit = D3DXToRadian(45.0f);
    const float rollLerpSpeed = 5.0f;

    if (GetAsyncKeyState('A') & 0x8000) {
        targetRoll = rollLimit;  // 左に傾く
    }
    else if (GetAsyncKeyState('D') & 0x8000) {
        targetRoll = -rollLimit; // 右に傾く
    }

    m_roll += (targetRoll - m_roll) * rollLerpSpeed * dt;

    // --- 3. 移動用行列の計算（航空機としての正しい計算順序） ---
    D3DXMATRIX matYaw, matPitch, matRoll, matTotal;

    // 正しい姿勢の合成: 自転(Roll)してから上下(Pitch)、最後に向き(Yaw)を決める
    D3DXMatrixRotationZ(&matRoll, m_roll);
    D3DXMatrixRotationX(&matPitch, targetPitch);
    D3DXMatrixRotationY(&matYaw, targetYaw);

    matTotal = matRoll * matPitch * matYaw;

    // 行列から正面方向を取得
    D3DXVECTOR3 forward;
    forward.x = matTotal._31;
    forward.y = matTotal._32;
    forward.z = matTotal._33;

    // 移動
    float moveSpeed = 60.0f;
    m_Position += forward * moveSpeed * dt;

    // --- 4. 【ここを修正】StaticMeshへの角度受け渡し（ねじれ補正） ---
    m_Rotation.y = targetYaw;
    m_Rotation.x = targetPitch;

    // StaticMesh::Renderが「Yaw * Pitch * Roll」で計算している場合、
    // 真上を向くとロール軸が世界軸と重なって「ねじれ」が発生します。
    // これを打ち消すため、機体がどの角度でも「自機から見た横」に傾くように
    // 角度を逆算して補正します。

    float cosPitch = cosf(targetPitch);
    // ジンバルロック（垂直時）の計算破綻を防ぐため、ごく僅かな値を確保
    if (fabsf(cosPitch) < 0.001f) cosPitch = (cosPitch >= 0) ? 0.001f : -0.001f;

    // [修正ポイント]
    // StaticMesh側の合成順序の矛盾を、この除算で打ち消します。
    // これにより、真上や真下を向いてもA/Dキーでの傾きが一定に保たれます。
    m_Rotation.z = m_roll / cosPitch;

    // --- 5. 各種コンポーネントの更新 ---
    m_ShotTimer += dt;
    if (GetAsyncKeyState(VK_SPACE) & 0x8000 && m_ShotTimer >= m_ShotInteval)
    {
        m_ShotTimer = 0.0f;
        D3DXVECTOR3 shotPos = m_Position + forward * 2.0f;
        Enemy* target = FindAimTarget(0.965f);
        PlayerShotManager::GetInstance()->AddHomingShot(shotPos, target, forward);
    }

    if (m_pSphere) {
        m_pSphere->SetPosition(m_Position);
    }

    Character::Update();
}
#endif

void Player::Draw()
{
	Character::Draw();
}

void Player::Init()
{
    m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_angleY = 0.0f;
    m_pitch = 0.0f;
    m_roll = 0.0f;
    m_Dead = false;
   
}

bool Player::GetDead() const
{
    return false;
}

void Player::OnHit(int damage)
{
}

void Player::SetCamera(Camera* pCamera)
{
    m_pCamera = pCamera;
}

Enemy* Player::FindAimTarget(float cosThreshold)
{
    Enemy* bestEnemy = nullptr;
    float bestDot = cosThreshold;

    // カメラ前方向
    D3DXVECTOR3 camForward = m_pCamera->GetForward();
    D3DXVec3Normalize(&camForward, &camForward);

    // 自分の位置
    D3DXVECTOR3 camPos = m_pCamera->GetPosition();

    auto& enemies = EnemyManager::GetInstance()->GetEnemies();

    for (auto& enemy : enemies)
    {
        if (!enemy) continue;

        D3DXVECTOR3 toEnemy = enemy->GetPosition() - camPos;
        D3DXVec3Normalize(&toEnemy, &toEnemy);

        float dot = D3DXVec3Dot(&camForward, &toEnemy);

        // 視野角内 & より正面
        if (dot > bestDot)
        {
            bestDot = dot;
            bestEnemy = enemy.get();
        }
    }

    return bestEnemy;
}

D3DXVECTOR3 Player::GetStraightForward() const
{
    float yaw = m_pCamera->GetAngleY();

    // 水平方向のみ（上下なし）
    D3DXVECTOR3 dir(
        sinf(yaw),
        0.0f,
        cosf(yaw)
    );

    D3DXVec3Normalize(&dir, &dir);
    return dir;
}
