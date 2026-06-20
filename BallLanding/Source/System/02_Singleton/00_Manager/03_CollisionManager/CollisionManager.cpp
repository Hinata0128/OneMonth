#include "CollisionManager.h"

#include "Effect/Effect.h"

#include <algorithm>

#include "System//02_Singleton//01_Timer//Timer.h"

#include "Sound//SoundManager.h"

#include "System/02_Singleton/02_Score/Score.h"

#include "System/02_Singleton//00_Manager/02_ImGuiManager/ImGuiManager.h"


#include "GameObject/00_SkinMeshObject/00_SkinMeshCharacter/00_AstralPlayer/AstralPlayer.h"
#include "GameObject/00_SkinMeshObject/00_SkinMeshCharacter/01_AstralBoss/AstralBoss.h"
#include "..//..//..//06_Camera/Camera.h"

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

