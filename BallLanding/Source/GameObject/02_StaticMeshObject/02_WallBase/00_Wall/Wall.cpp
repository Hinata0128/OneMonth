#include "Wall.h"

#include "System//00_Manager//01_StaticMeshManager//StaticMeshManager.h"

Wall::Wall()
{
    auto pMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Wall);
    AttachMesh(*pMesh);

    // --- 当たり判定の実装 ---
    m_pBoundingBox = std::make_shared<BoundingBox>();
    if (pMesh)
    {
        // 1. メッシュからローカルのサイズを計算
        m_pBoundingBox->CreateBoxForMesh(*pMesh);
    }

}

Wall::~Wall()
{
}

void Wall::Update()
{
	StaticMeshObject::Update();
    if (m_pBoundingBox)
    {
        m_pBoundingBox->UpdateWorldAABB(GetPosition(), GetScale());        // AABBなので回転(GetRotation)は適用しないのが安全
    }
}

void Wall::Draw()
{
    if (!m_isVisible) return;
	StaticMeshObject::Draw();
}
