#include "Ball.h"

#include "System//00_Manager//01_StaticMeshManager//StaticMeshManager.h"
#include "System/00_Manager/03_CollisionManager/CollisionManager.h"

#include <random>

Ball::Ball()
	: StaticMeshObject	()
	
	, m_Velocity		{0.0f,0.0f,0.0f}
	, m_Gravity			(-0.01f)
{
	//ボールの呼び出し.
	auto pMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Ball);

	AttachMesh(*pMesh);

    this->SetLightEnable(false);


	//X座標の指定範囲内のどこかにボールが生成されるように設定.
	std::random_device Rd;
	std::mt19937 Get(Rd());
	//ボールの生成位置は、-9～9の間で生成される.
	std::uniform_real_distribution<float> Dis(-9.0f, 9.0f);

	//ポジションの設定.
	float RandomX = Dis(Get);
	D3DXVECTOR3 Pos = { RandomX,20.0f,0.0f };
	SetPosition(Pos);
	//サイズの設定.
	D3DXVECTOR3 Scale = { 5.0f,5.0f, 5.0f };
	SetScale(Scale);

	m_pSphere = std::make_shared<BoundingSphere>();
	m_pSphere->SetTag(BoundingSphere::Tag::Ball);
	if (pMesh)
	{
		//メッシュから半径を計算.
		m_pSphere->CreateSphereForMesh(*pMesh);

		float baseRadius = m_pSphere->GetRadius();
		m_pSphere->SetRadius(baseRadius * Scale.x);
	}

	m_pSphere->SetPosition(Pos);

	CollisionManager::GetInstance()->AddSphere(m_pSphere);
}

Ball::~Ball()
{
}

void Ball::Update()
{
    // 1. 重力と進行方向への加速を先に適用
    m_Velocity.y += m_Gravity;
    // Z方向の微加速をここで行うことで、壁に当たった後に「壁を突き抜ける力」として
    // 判定に悪影響を与えるのを防ぎます。
    m_Velocity.z -= 0.0005f;

    // 2. 現在の速度に基づいた移動先の仮計算
    D3DXVECTOR3 nextPos = GetPosition() + m_Velocity;

    // 判定用のスフィアを移動先の位置に仮置きする
    m_pSphere->SetPosition(nextPos);

    auto pCM = CollisionManager::GetInstance();
    bool isOnGround = false;

    if (pCM)
    {
        // --- ボックス（壁）との判定 ---
        auto& boxes = pCM->GetBoxes();
        for (auto& box : boxes)
        {
            // 移動先の位置にあるスフィアで判定
            CollisionResult result = pCM->CheckSphereBoxDetailed(*m_pSphere, *box);
            if (result.IsHit)
            {
                isOnGround = true;

                // 押し戻しを nextPos に適用
                nextPos += result.PushVector;

                // 反射ベクトルの計算
                D3DXVECTOR3 normal;
                D3DXVec3Normalize(&normal, &result.PushVector);
                float dot = D3DXVec3Dot(&m_Velocity, &normal);
                if (dot < 0.0f)
                {
                    // 反射（滑り）計算
                    m_Velocity -= dot * normal;
                }

                // ★重要: ここで SetPosition をせず、スフィアの位置だけ現在の nextPos に更新
                // これにより、次の壁の判定が「押し戻された後の位置」で行われます
                m_pSphere->SetPosition(nextPos);
            }
        }

        // --- 他のスフィアとの判定 ---
        auto& spheres = pCM->GetSpheres();
        for (auto& otherSphere : spheres)
        {
            if (otherSphere == m_pSphere) continue;
            if (otherSphere->GetTag() == BoundingSphere::Tag::Ball) continue;
            if (otherSphere->GetTag() == BoundingSphere::Tag::Goal) continue;

            D3DXVECTOR3 diff = m_pSphere->GetPostion() - otherSphere->GetPostion();
            float dist = D3DXVec3Length(&diff);
            float minDist = m_pSphere->GetRadius() + otherSphere->GetRadius();

            if (dist < minDist)
            {
                // 押し戻し
                D3DXVECTOR3 normal;
                D3DXVec3Normalize(&normal, &diff);
                float overlap = minDist - dist;
                nextPos += normal * overlap;

                // 跳ね返し
                float bounce = 0.6f;
                float dot = D3DXVec3Dot(&m_Velocity, &normal);
                if (dot < 0.0f)
                {
                    m_Velocity -= (1.0f + bounce) * dot * normal;
                }

                // スフィアの位置を最新の補正位置に更新
                m_pSphere->SetPosition(nextPos);
            }
        }
    }

    // 3. 地面にいる時の摩擦処理
    if (isOnGround)
    {
        m_Velocity.x *= 0.98f;
    }

    // 4. 全ての補正が終わった最終座標を確定させる
    SetPosition(nextPos);
    m_pSphere->SetPosition(nextPos);

    StaticMeshObject::Update();
}

void Ball::Draw()
{
    m_pSphere->Draw();
	StaticMeshObject::Draw();
}
