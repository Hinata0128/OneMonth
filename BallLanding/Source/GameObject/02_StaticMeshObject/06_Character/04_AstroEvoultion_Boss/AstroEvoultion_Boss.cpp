#include "AstroEvoultion_Boss.h"

#include "System/00_Manager/03_CollisionManager/CollisionManager.h"
#include "System/02_Singleton/02_SingletonManager/08_AstroEvolution_Rock_Manager/AstroEvolution_Rock_Manager.h"
#include "System/02_Singleton/02_SingletonManager/10_AstroEvolution_BossShot_Manager/AstroEvolution_BossShot_Manager.h"

AstroEvoultion_Boss::AstroEvoultion_Boss()
    : Character()


    , m_State(BossState::Chase)
{
    //読み込み.
    auto pMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Fighter);
    //設定.
    AttachMesh(*pMesh);
    //ポジションの設定.
    D3DXVECTOR3 Pos = { 0.0f,0.0f,15.0f };
    SetPosition(Pos);
    //サイズ変更.
    D3DXVECTOR3 Scale = { 10.0f,10.0f,10.0f };
    SetScale(Scale);
    //向きの設定.
    //最終的にはPlayerを注視するボスを作成します.
    D3DXVECTOR3 Rotation = { 0.0f,D3DXToRadian(180.0f),0.0f };
    SetRotation(Rotation);

    //光遮断.
    this->SetLightEnable(false);

    m_pSphere = std::make_shared<BoundingSphere>();
    m_pSphere->SetTag(BoundingSphere::Tag::AstroBoss);
    if (pMesh)
    {
        //メッシュから半径を計算.
        m_pSphere->CreateSphereForMesh(*pMesh);
        //当たり判定のサイズの設定.
        m_pSphere->SetRadius(5.0f);
    }
    CollisionManager::GetInstance()->SetAstro_Boss(this);

    Init();

}

AstroEvoultion_Boss::~AstroEvoultion_Boss()
{
}

void AstroEvoultion_Boss::Update()
{
    if (!m_pTargetPlayer) return;

    if (m_InvincibleTimer > 0.0f) {
        m_InvincibleTimer -= 0.016f;
        if (m_InvincibleTimer < 0.0f) m_InvincibleTimer = 0.0f;
    }

    CollisionManager::GetInstance()->AddSphere(m_pSphere);

    // 🌟 現在のラウンド数を取得（調整のベースにする）
    int round = AstroEvolution_Rock_Manager::GetInstance()->GetCurrentRound();

    D3DXVECTOR3 targetPos = m_pTargetPlayer->GetPosition();
    D3DXVECTOR3 myPos = GetPosition();
    D3DXVECTOR3 diff = targetPos - myPos;
    float dist = D3DXVec3Length(&diff);

    float dt = 0.016f;
    m_StateTimer += dt;

    switch (m_State)
    {
    case BossState::Chase:
        // 15mより遠ければ追いかける
        if (dist > 15.0f) {
            D3DXVec3Normalize(&diff, &diff);
            // 🌟 追跡速度もラウンドで少しアップ (0.04f -> 0.06f -> 0.08f)
            float chaseSpeed = 0.02f + (round * 0.02f);
            myPos += diff * chaseSpeed;
            SetRotationY(atan2f(diff.x, diff.z));
        }
        else {
            if (rand() % 2 == 0) m_State = BossState::Charge;
            else m_State = BossState::Barrage;

            m_StateTimer = 0.0f;
            D3DXVec3Normalize(&m_DashDir, &diff);
        }
        break;

    case BossState::Charge:
        if (m_StateTimer < 0.5f) {
            D3DXVec3Normalize(&diff, &diff);
            SetRotationY(atan2f(diff.x, diff.z));
        }
        else {
            // 🌟 突進速度の調整 (Round1:1.2f, Round2:1.5f, Round3:1.8f)
            float dashSpeed = 0.9f + (round * 0.3f);
            myPos += m_DashDir * dashSpeed;

            if (fabsf(myPos.x) >= 70.0f || fabsf(myPos.z) >= 70.0f) {
                if (myPos.x > 70.0f)  myPos.x = 70.0f;
                if (myPos.x < -70.0f) myPos.x = -70.0f;
                if (myPos.z > 70.0f)  myPos.z = 70.0f;
                if (myPos.z < -70.0f) myPos.z = -70.0f;

                m_State = BossState::Cooldown;
                m_StateTimer = 0.0f;
            }

            if (m_StateTimer > 1.0f) {
                m_State = BossState::Cooldown;
                m_StateTimer = 0.0f;
            }
        }
        break;

    case BossState::Barrage:
        if (m_StateTimer < 1.0f) {
            D3DXVec3Normalize(&diff, &diff);
            SetRotationY(atan2f(diff.x, diff.z));
        }
        else {
            D3DXVECTOR3 dirToPlayer;
            D3DXVec3Normalize(&dirToPlayer, &diff);

            // 🌟 弾数の調整 (Round1:3Way, Round2:5Way, Round3:7Way)
            int shotCount = 1 + (round * 2);
            // 🌟 拡散角度の調整 (Roundが上がると少し広角にする)
            float angle = D3DXToRadian(10.0f + (round * 5.0f));

            AstroEvolution_BossShot_Manager::GetInstance()->BossFireWay(
                myPos,
                dirToPlayer,
                shotCount,
                angle
            );

            m_State = BossState::Cooldown;
            m_StateTimer = 0.0f;
        }
        break;

    case BossState::Cooldown:
        // 🌟 クールダウン（隙）をラウンドが進むと短くする (2.0s -> 1.5s -> 1.0s)
        float cooldownTime = 2.5f - (round * 0.5f);
        if (m_StateTimer > cooldownTime) {
            m_State = BossState::Chase;
        }
        break;
    }

    SetPosition(myPos);
    if (m_pSphere) m_pSphere->SetPosition(m_Position);

    Character::Update();
}

void AstroEvoultion_Boss::Draw()
{
    if (!m_IsActive) return;
    if (m_InvincibleTimer > 0.0f) {
        // 0.06秒ごとに表示・非表示を切り替え
        if (static_cast<int>(m_InvincibleTimer * 20) % 2 == 0) return;
    }
    //m_pSphere->Draw();
    Character::Draw();
}

void AstroEvoultion_Boss::Init()
{
    int round = AstroEvolution_Rock_Manager::GetInstance()->GetCurrentRound();

    // Round1: 100, Round2: 250, Round3: 450 (少しずつ倍率を上げる)
    m_MaxHP = 50.0f + (static_cast<float>(round * round) * 50.0f);
    m_HP = m_MaxHP;

    m_Dead = false;
    m_InvincibleTimer = 0.0f;
    m_State = BossState::Chase; // ステートも初期化

    Character::Init();
}

bool AstroEvoultion_Boss::GetDead() const
{
    return m_Dead;
}

void AstroEvoultion_Boss::AstroEvolution_BossParamLoadData(const std::string& FilePath)
{
}

void AstroEvoultion_Boss::AstroEvolution_BossParamSaveData(const std::string& FilePath)
{
}

void AstroEvoultion_Boss::ShootBarrage()
{
    D3DXVECTOR3 myPos = GetPosition();
    D3DXVECTOR3 targetPos = m_pTargetPlayer->GetPosition();
    D3DXVECTOR3 dir = targetPos - myPos;
    D3DXVec3Normalize(&dir, &dir);

    // マネージャーを呼んで 5-Way ショット！
    auto* pShotManager = AstroEvolution_BossShot_Manager::GetInstance();
    pShotManager->BossFireWay(myPos, dir, 5, D3DXToRadian(15.0f));
}
