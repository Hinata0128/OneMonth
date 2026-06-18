#include "CollisionManager.h"

#include "Effect/Effect.h"

#include <algorithm>

#include "System//02_Singleton//00_Timer//Timer.h"

#include "Sound//SoundManager.h"

#include "System/02_Singleton/03_Score/Score.h"

#include "System/00_Manager/02_ImGuiManager/ImGuiManager.h"

#include "System/02_Singleton/02_SingletonManager/05_EnemyShotManager/EnemyShotManager.h"
#include "System/02_Singleton/02_SingletonManager/04_EnemyManager/EnemyManager.h"

#include "GameObject/00_SkinMeshObject/00_SkinMeshCharacter/00_AstralPlayer/AstralPlayer.h"
#include "GameObject/00_SkinMeshObject/00_SkinMeshCharacter/01_AstralBoss/AstralBoss.h"

CollisionManager::CollisionManager()
    : m_pTargetScore(nullptr)
{
}

CollisionManager::~CollisionManager()
{
    Release();
}

void CollisionManager::Update()
{

    // 🌟 クールタイム減少
    if (m_BossHitEffectCooldown > 0.0f)
    {
        m_BossHitEffectCooldown -= Timer::GetInstance().DeltaTime();
        if (m_BossHitEffectCooldown < 0.0f)
            m_BossHitEffectCooldown = 0.0f;
    }

    AllCollider();
    //死んだ子リジョンをまとめて削除.
    m_pBSphere.erase(
        std::remove_if(m_pBSphere.begin(), m_pBSphere.end(),
            [](const std::shared_ptr<BoundingSphere>& s) {
                return s->IsDead(); // IsDeadがtrueのものをリストから外す
            }),
        m_pBSphere.end()
    );

    m_pRocks.erase(
        std::remove_if(m_pRocks.begin(), m_pRocks.end(),
            [](AstroEvolution_Rock* rock) {
                return !rock || rock->IsDead(); // 死んでいる岩をリストから消す
            }),
        m_pRocks.end()
    );
}

void CollisionManager::Draw()
{
#ifdef _DEBUG
    //スフィア描画
    for (const auto& sphere : m_pBSphere)
    {
        if (sphere) sphere->Draw();	
    }


    //ボックス描画
    for (const auto& box : m_pBBox)
    {
        if (box) box->Draw();		
    }
#endif
}

void CollisionManager::Create()
{
    m_pBSphere.clear();	
    m_pBBox.clear();	
    m_pRings.clear();
    m_pRocks.clear();

    m_EnemyShots.clear();

    m_pPlayer = nullptr;
    m_pAstroPlayer = nullptr;
}

void CollisionManager::Release()
{
    m_pBSphere.clear();
    m_pBBox.clear();
}

void CollisionManager::AddSphere(const std::shared_ptr<BoundingSphere>& sphere)
{
    if (sphere) m_pBSphere.push_back(sphere); //スフィア登録.
}

void CollisionManager::AddBox(const std::shared_ptr<BoundingBox>& box)
{
    if (box) m_pBBox.push_back(box); //ボックス登録.
}

void CollisionManager::Clear()
{
}

void CollisionManager::AllCollider()
{
    //球と球の当たり判定
    for (size_t i = 0; i < m_pBSphere.size(); ++i)
    {
        for (size_t j = i + 1; j < m_pBSphere.size(); ++j)
        {
            auto& sphereA = m_pBSphere[i];
            auto& sphereB = m_pBSphere[j];

            if (sphereA->IsDead() || sphereB->IsDead()) continue;

            if (CheckSphereSphere(*sphereA, *sphereB))
            {

                // 各タグごとの衝突処理
                if ((sphereA->GetTag() == BoundingSphere::Tag::Ball && sphereB->GetTag() == BoundingSphere::Tag::Goal) ||
                    (sphereB->GetTag() == BoundingSphere::Tag::Ball && sphereA->GetTag() == BoundingSphere::Tag::Goal))
                {
                    HandleGoal(sphereA, sphereB);
                }

            }
        }
    }

    //かごと壁の当たり判定.
    for (auto& sphere : m_pBSphere)
    {
        if (sphere->GetTag() != BoundingSphere::Tag::Goal || sphere->IsDead()) continue;

        for (auto& box : m_pBBox)
        {
            CollisionResult result = CheckSphereBoxDetailed(*sphere, *box);
            if (result.IsHit)
            {
                sphere->SetPosition(sphere->GetPostion() + result.PushVector);

            }
        }
    }

    //それぞれの情報[シングルトン].
    auto enemyShots = EnemyShotManager::GetInstance()->GetShots();
    auto enemyManager = EnemyManager::GetInstance();

    const auto& enemies = enemyManager->GetEnemies();

    //プレイヤーと敵の弾の当たり判定.
    if (m_pPlayer)
    {
        auto playerSphere = m_pPlayer->GetBoundingSphere();
        if (playerSphere)
        {
            for (auto* eShot : enemyShots)
            {
                if (eShot->IsActive() && playerSphere->IsHit(*eShot->GetSphere()))
                {
                    m_pPlayer->OnHit(5);
                    eShot->Init();
                }
            }
        }
    }

    if (m_pPlayer) {
        auto playerSphere = m_pPlayer->GetBoundingSphere();
        if (playerSphere) {
            for (auto* ring : m_pRings) {
                // Ringクラスからコライダーを取得して判定
                auto ringSphere = ring->GetBoundingSphere();
                if (ringSphere && playerSphere->IsHit(*ringSphere)) {

                    // 当たった時の処理をここで直接呼ぶ
                    ring->TeleportRandom();

                    // スコア加算
                    if (m_pTargetScore) {
                        m_pTargetScore->Add(m_ScoreValue);
                    }
                }
            }
        }
    }

    if (m_pAstroPlayer)
    {
        auto playerSphere = m_pAstroPlayer->GetBoundingSphere();
        if (playerSphere)
        {
            for (auto& box : m_pBBox)
            {
                CollisionResult result = CheckSphereBoxDetailed(*playerSphere, *box);
                if (result.IsHit)
                {
                    // 押し戻しを適用
                    D3DXVECTOR3 pos = m_pAstroPlayer->GetPosition();
                    m_pAstroPlayer->SetPosition(pos + result.PushVector);
                    m_pCamera->SetShake(0.2f, 0.15f);

                    // 衝突エフェクト
                    Effect::GetInstance()->Play(
                        Effect::Laser01,
                        playerSphere->GetPostion()
                    );
                }
            }
        }
    }

    if (m_pAstroPlayer)
    {
        auto playerSphere = m_pAstroPlayer->GetBoundingSphere();
        if (playerSphere)
        {
            for (auto* rock : m_pRocks)
            {
                if (!rock) continue;

                auto rockSphere = rock->GetBoundingSphere();
                if (rockSphere && playerSphere->IsHit(*rockSphere))
                {
                    // --- 衝突時の押し戻し処理 ---
                    D3DXVECTOR3 pPos = m_pAstroPlayer->GetPosition();
                    D3DXVECTOR3 rPos = rockSphere->GetPostion();
                    D3DXVECTOR3 diff = pPos - rPos;

                    float dist = D3DXVec3Length(&diff);
                    float minRadii = playerSphere->GetRadius() + rockSphere->GetRadius();

                    if (dist < minRadii) {
                        if (dist < 0.001f) diff = D3DXVECTOR3(0, 1, 0); // 重なり対策
                        D3DXVec3Normalize(&diff, &diff);

                        // プレイヤーを押し戻す
                        m_pAstroPlayer->SetPosition(pPos + diff * (minRadii - dist));
                    }


                }
            }
        }
    }

    // プレイヤーの弾 vs 岩 の判定
    auto& astroPlayerShots = AstroEvolution_PlayerShot_Manager::GetInstance()->GetShots();

    for (auto& pShotPtr : astroPlayerShots)
    {
        // unique_ptrから生のポインタを取り出す
        AstroEvolution_PlayerShot* pShot = pShotPtr.get();

        // 弾が存在しない、または非アクティブ（画面外など）ならスキップ
        if (!pShot || !pShot->GetIsActive()) continue;

        auto pShotSphere = pShot->GetSphere();
        if (!pShotSphere) continue;

        // 登録されている岩のリストを回す
        for (auto* pRock : m_pRocks)
        {
            // 岩が存在しない、または既に破壊されていたらスキップ
            if (!pRock || pRock->IsDead()) continue;

            auto pRockSphere = pRock->GetBoundingSphere();
            if (!pRockSphere) continue;

            // 球体同士の当たり判定（既存の CheckSphereSphere を使用）
            if (CheckSphereSphere(*pShotSphere, *pRockSphere))
            {
                // --- 衝突時の処理 ---

                // 1. 弾側のヒット処理（消滅エフェクトや非アクティブ化）
                pShot->OnHit();

                // 2. 岩側のダメージ処理（HPを減らす、0なら死ぬ）
                pRock->OnHit(10);

                // 3. ヒット演出（火花やレーザー着弾エフェクト）
                Effect::GetInstance()->Play(
                    Effect::Laser01,
                    pRockSphere->GetPostion()
                );

                // 4. 岩がこの攻撃で死んだ場合、スコアを加算
                if (pRock->IsDead() && m_pTargetScore)
                {
                    m_pTargetScore->Add(m_ScoreValue);
                }
            }
        }
    }


    // --- プレイヤー(AstroPlayer) vs ボス(AstroBoss) の押し出し判定 ---
    if (m_pAstroPlayer && m_pAstroBoss)
    {
        auto playerSphere = m_pAstroPlayer->GetBoundingSphere();
        auto bossSphere = m_pAstroBoss->GetBoundingSphere(); // Bossクラスにこの関数があると仮定

        if (playerSphere && bossSphere)
        {
            D3DXVECTOR3 pPos = m_pAstroPlayer->GetPosition();
            D3DXVECTOR3 bPos = m_pAstroBoss->GetPosition();
            D3DXVECTOR3 diff = pPos - bPos;

            float dist = D3DXVec3Length(&diff);
            float minRadii = playerSphere->GetRadius() + bossSphere->GetRadius();

            // 距離が半径の合計より小さい＝めり込んでいる
            if (dist < minRadii)
            {
                // 完全に重なっている場合のゼロ除算対策
                if (dist < 0.001f) diff = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

                D3DXVec3Normalize(&diff, &diff);

                // 押し戻しベクトルを計算 (めり込んでいる距離分だけ離す)
                D3DXVECTOR3 pushVec = diff * (minRadii - dist);

                // プレイヤーの位置を修正
                m_pAstroPlayer->SetPosition(pPos + pushVec);

                // 判定用スフィアの位置も同期（これ重要！）
                playerSphere->SetPosition(m_pAstroPlayer->GetPosition());

                // 必要であれば：衝突エフェクトなどを出す
                // Effect::GetInstance()->Play(Effect::Laser01, pPos);
            }
        }
    }

    if (m_pAstroPlayer)
    {
        auto playerSphere = m_pAstroPlayer->GetBoundingSphere();
        // ボスの弾マネージャーからアクティブな弾のリストを取得
        auto& bossShots = AstroEvolution_BossShot_Manager::GetInstance()->GetShots();

        if (playerSphere)
        {
            for (auto& bShotPtr : bossShots)
            {
                AstroEvolution_BossShot* bShot = bShotPtr.get();
                if (!bShot || !bShot->GetIsActive()) continue;

                auto bShotSphere = bShot->GetSphere();
                if (bShotSphere && CheckSphereSphere(*playerSphere, *bShotSphere))
                {

                    // 弾側のヒット処理（消滅・エフェクト）
                    bShot->OnHit();
                    m_pAstroPlayer->Hit(10);
                    m_pCamera->SetShake(0.6f, 0.3f);

                    // 被弾エフェクト
                    Effect::GetInstance()->Play(Effect::Laser01, playerSphere->GetPostion());
                }
            }
        }
    }

    // --- プレイヤーの弾 vs ボス ---
    if (m_pAstroBoss && m_pAstroBoss->IsActive())
    {
        auto bossSphere = m_pAstroBoss->GetBoundingSphere();
        auto& playerShots = AstroEvolution_PlayerShot_Manager::GetInstance()->GetShots();

        if (bossSphere && !m_pAstroBoss->GetDead())
        {
            for (auto& pShotPtr : playerShots)
            {
                AstroEvolution_PlayerShot* pShot = pShotPtr.get();
                if (!pShot || !pShot->GetIsActive()) continue;

                auto pShotSphere = pShot->GetSphere();
                if (pShotSphere && CheckSphereSphere(*bossSphere, *pShotSphere))
                {
                    pShot->OnHit();
                    m_pAstroBoss->Hit(50);

                    if (m_BossHitEffectCooldown <= 0.0f)
                    {
                        Effect::GetInstance()->Play(Effect::Laser01, pShotSphere->GetPostion());

                        m_BossHitEffectCooldown = 0.5f; // 0.1秒に1回
                    }
                }
            }
        }
    }

    if (m_pAstralPlayer && m_pAstralBoss_New)
    {
        // 1. プレイヤー側の球体リストを取得
        std::vector<std::shared_ptr<BoundingSphere>> playerSpheres = {
            m_pAstralPlayer->GetSphereBody(),
            m_pAstralPlayer->GetSphereHead()
        };

        // 2. ボス側の球体リストを取得 (AstralBossにも同様のGetterがあると仮定)
        std::vector<std::shared_ptr<BoundingSphere>> bossSpheres = {
            m_pAstralBoss_New->GetSpheres(),
        };

        for (auto& pSphere : playerSpheres)
        {
            if (!pSphere) continue;

            for (auto& bSphere : bossSpheres)
            {
                if (!bSphere) continue;

                // 球体同士の衝突判定
                if (CheckSphereSphere(*pSphere, *bSphere))
                {
                    // 座標と押し出し方向の計算
                    D3DXVECTOR3 pPos = m_pAstralPlayer->GetPosition();
                    D3DXVECTOR3 bPos = m_pAstralBoss_New->GetPosition();
                    D3DXVECTOR3 diff = pPos - bPos; // ボスからプレイヤーへのベクトル

                    float dist = D3DXVec3Length(&diff);
                    float minRadii = pSphere->GetRadius() + bSphere->GetRadius();

                    if (dist < 0.001f) diff = D3DXVECTOR3(0, 0, 1);
                    D3DXVec3Normalize(&diff, &diff);

                    // 押し戻し量（ボスは重いので、プレイヤー側を多めに押し戻すとそれっぽくなります）
                    float overlap = minRadii - dist;

                    // 例: プレイヤー 0.8 : ボス 0.2 の割合で分配
                    m_pAstralPlayer->SetPosition(pPos + diff * (overlap * 0.8f));
                    m_pAstralBoss_New->SetPosition(bPos - diff * (overlap * 0.2f));

                    // 各クラスの内部スフィア座標を即座に更新
                    m_pAstralPlayer->UpdateCollisionPosition();
                    m_pAstralBoss_New->UpdateCollisionPosition();

                }
            }
        }
    }

    // --- 【修正】プレイヤーの攻撃 vs ボスの当たり判定 ---
    if (m_pAstralPlayer && m_pAstralPlayer->IsAttacking() && m_pAstralBoss_New)
    {
        // タイマーをデルタタイム分減らす
        if (m_PlayerAttackEffectTimer > 0.0f)
        {
            m_PlayerAttackEffectTimer -= Timer::GetInstance().DeltaTime();
        }

        auto attackSphere = m_pAstralPlayer->GetAttackSphere();
        auto bossSpheres = m_pAstralBoss_New->GetSpheres();

        if (attackSphere)
        {
            for (auto& bSphere : bossSpheres)
            {
                if (bSphere && CheckSphereSphere(*attackSphere, *bSphere))
                {
                    // ボスにダメージ（ダメージは毎フレーム与えても、エフェクトだけ制限する）
                    m_pAstralBoss_New->OnHit(10.0f);

                    // --- 🌟 0.5秒経過しているかチェック ---
                    if (m_PlayerAttackEffectTimer <= 0.0f)
                    {
                        // エフェクト再生
                        Effect::GetInstance()->Play(Effect::Laser01, bSphere->GetPostion());

                        // タイマーを0.5秒にリセット
                        m_PlayerAttackEffectTimer = 0.5f;

                        if (m_pCamera) m_pCamera->SetShake(0.5f, 0.2f);
                    }

                    break; // 1つの球体に当たったらループを抜ける
                }
            }
        }
    }
}

CollisionResult CollisionManager::CheckSphereBoxDetailed(const BoundingSphere& sphere, const BoundingBox& box)
{
    CollisionResult result = { false, D3DXVECTOR3(0.0f, 0.0f, 0.0f) };

    D3DXVECTOR3 sphereCenter = sphere.GetPostion();
    D3DXVECTOR3 boxMin = box.GetMin();
    D3DXVECTOR3 boxMax = box.GetMax();

    //ボックス内の球体に最も近い点を求める.
    D3DXVECTOR3 closest;
    closest.x = std::max<float>(boxMin.x, std::min<float>(sphereCenter.x, boxMax.x));
    closest.y = std::max<float>(boxMin.y, std::min<float>(sphereCenter.y, boxMax.y));
    closest.z = std::max<float>(boxMin.z, std::min<float>(sphereCenter.z, boxMax.z));

    D3DXVECTOR3 diff = sphereCenter - closest;
    float distanceSq = D3DXVec3LengthSq(&diff);
    float radius = sphere.GetRadius();

    if (distanceSq > 0.00001f)
    {
        float distance = sqrtf(distanceSq);
        if (distance < radius)
        {
            result.IsHit = true;
            D3DXVECTOR3 normal = diff / distance;
            result.PushVector = normal * (radius - distance);
        }
    }
    //球の中心がボックスに完全に埋まっている場合.
    else
    {
        result.IsHit = true;
        //各面までの距離を計算.
        float distMinX = sphereCenter.x - boxMin.x;
        float distMaxX = boxMax.x - sphereCenter.x;
        float distMinY = sphereCenter.y - boxMin.y;
        float distMaxY = boxMax.y - sphereCenter.y;
        float distMinZ = sphereCenter.z - boxMin.z;
        float distMaxZ = boxMax.z - sphereCenter.z;

        //最も近い面を探す.
        float minDist = distMaxZ;
        result.PushVector = D3DXVECTOR3(0, 0, distMaxZ + radius);

        if (distMinZ < minDist) { minDist = distMinZ; result.PushVector = D3DXVECTOR3(0, 0, -(distMinZ + radius)); }
        if (distMaxX < minDist) { minDist = distMaxX; result.PushVector = D3DXVECTOR3(distMaxX + radius, 0, 0); }
        if (distMinX < minDist) { minDist = distMinX; result.PushVector = D3DXVECTOR3(-(distMinX + radius), 0, 0); }
        if (distMaxY < minDist) { minDist = distMaxY; result.PushVector = D3DXVECTOR3(0, distMaxY + radius, 0); }
        if (distMinY < minDist) { minDist = distMinY; result.PushVector = D3DXVECTOR3(0, -(distMinY + radius), 0); }
    }

    return result;
}

//衝突した際の具体的な処理.
void CollisionManager::HandleGoal(std::shared_ptr<BoundingSphere> a, std::shared_ptr<BoundingSphere> b)
{
    //スコア加算.
    //Todo : スコアクラスを作成していないので今はGameMainで呼んでいる.
    m_pTargetScore->Add(m_ScoreValue);

    //ボールを消去対象にする.
    if (a->GetTag() == BoundingSphere::Tag::Ball) a->SetDead(true);
    if (b->GetTag() == BoundingSphere::Tag::Ball) b->SetDead(true);
}


bool CollisionManager::CheckSphereSphere(const BoundingSphere& a, const BoundingSphere& b)
{
    return a.IsHit(b);
}

bool CollisionManager::CheckBoxBox(const BoundingBox& a, const BoundingBox& b)
{
    return a.IsHit(b);
}

bool CollisionManager::CheckSphereBox(const BoundingSphere& sphere, const BoundingBox& box)
{
    return CheckSphereBoxDetailed(sphere, box).IsHit;
}

