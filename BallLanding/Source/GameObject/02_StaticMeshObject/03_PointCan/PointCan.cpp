#include "PointCan.h"

#include "System//00_Manager//01_StaticMeshManager//StaticMeshManager.h"
#include "System/00_Manager/03_CollisionManager/CollisionManager.h"

#include "System/02_Singleton/02_SingletonManager/02_WallManager/WallManager.h"

PointCan::PointCan()
{
	//読み込み.
	auto pMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::PointCan);
	//設定.
	AttachMesh(*pMesh);

	//ポジションの設定.
	D3DXVECTOR3 Pos = { 0.0f,0.0f,-5.0f };
	SetPosition(Pos);

	//サイズの変更.
	D3DXVECTOR3 Scale = { 0.008f,0.008f,0.008f };
	SetScale(Scale);

	m_pSphere = std::make_shared<BoundingSphere>();
	m_pSphere->SetTag(BoundingSphere::Tag::Goal);
	if (pMesh)
	{
		//メッシュから半径を計算.
		m_pSphere->CreateSphereForMesh(*pMesh);
		//当たり判定のサイズの設定.
		m_pSphere->SetRadius(1.2f);
	}
	CollisionManager::GetInstance()->AddSphere(m_pSphere);
}

PointCan::~PointCan()
{
}

void PointCan::Update()
{
	if (GetAsyncKeyState('A') & 0x8000) { m_Position.x -= 0.1f; }
	if (GetAsyncKeyState('D') & 0x8000) { m_Position.x += 0.1f; }

	//当たり判定と座標修正
	if (m_pSphere)
	{
		m_pSphere->SetPosition(m_Position);

		// --- 壁（ボックス）との衝突判定と押し戻し ---
		auto& walls = WallManager::GetInstance()->GetWalls();
		for (auto& wall : walls)
		{
			auto pWallBox = wall->GetBoundingBox();
			if (!pWallBox) continue;

			// CollisionManagerの機能を使って壁との押し戻し計算
			CollisionResult result = CollisionManager::GetInstance()->CheckSphereBoxDetailed(*m_pSphere, *pWallBox);
			if (result.IsHit)
			{
				// Y軸の押し戻しは無効化（地面に埋まったり浮いたりしないように）
				result.PushVector.y = 0.0f;
				// 押し戻しを適用
				m_Position += result.PushVector;
				m_pSphere->SetPosition(m_Position);
			}
		}
	}

	StaticMeshObject::Update();
}

void PointCan::Draw()
{
	m_pSphere->Draw();
	StaticMeshObject::Draw();
}
