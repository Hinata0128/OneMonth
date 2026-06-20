#include "CollisionManager.h"
#include "Effect/Effect.h"
#include <algorithm>
#include "System//02_Singleton//01_Timer//Timer.h"
#include "Sound//SoundManager.h"
#include "System/02_Singleton/02_Score/Score.h"
#include "System/02_Singleton//00_Manager/02_ImGuiManager/ImGuiManager.h"
#include "..//..//00_Manager/04_PlayerShotManager/PlayerShotManager.h"
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
                return s->IsDead(); //IsDeadがtrueのものをリストから外す.
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
    //敵とプレイヤーの弾の当たり判定.
    for (auto& shot : m_pBSphere)
    {
        //既に死んでいる、またはプレイヤーの弾でなければスキップ.
        if (!shot || shot->IsDead() || shot->GetTag() != BoundingSphere::Tag::PlayerShot) continue;

        for (auto& enemy : m_pBSphere)
        {
            if (!enemy || enemy->IsDead() || enemy->GetTag() != BoundingSphere::Tag::Enemy) continue;

            //スフィア同士の当たり判定.
            if (CheckSphereSphere(*shot, *enemy))
            {
                //着弾エフェクト再生.
                Effect::GetInstance()->Play(
                    Effect::Laser01,
                    shot->GetPostion()
                );

                //コライダーを死亡状態にする.
                shot->SetDead(true);

                //弾本体を即座に死亡状態にする.
                auto shotManager = PlayerShotManager::GetInstance();
                if (shotManager)
                {
                    shotManager->KillShotByCollider(shot);
                }

                //この弾は既に消滅したので、他の敵との判定をスキップして次の弾へ.
                break;
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

void CollisionManager::HandleGoal(std::shared_ptr<BoundingSphere> a, std::shared_ptr<BoundingSphere> b)
{
    m_pTargetScore->Add(m_ScoreValue);
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