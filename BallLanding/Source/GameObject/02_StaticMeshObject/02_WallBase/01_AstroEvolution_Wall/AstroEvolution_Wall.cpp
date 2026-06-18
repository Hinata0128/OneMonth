#include "AstroEvolution_Wall.h"

#include "System//00_Manager//01_StaticMeshManager//StaticMeshManager.h"

AstroEvolution_Wall::AstroEvolution_Wall()
{
	auto pMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Wall);
	AttachMesh(*pMesh);

    m_pBoundingBox = std::make_shared<BoundingBox>();
    if (pMesh)
    {
        m_pBoundingBox->CreateBoxForMesh(*pMesh);
    }

}

AstroEvolution_Wall::~AstroEvolution_Wall()
{
}

void AstroEvolution_Wall::Update()
{
    StaticMeshObject::Update();
    if (m_pBoundingBox)
    {
        m_pBoundingBox->UpdateWorldAABB(GetPosition(), GetScale());        // AABB‚È‚Ì‚Å‰ñ“](GetRotation)‚Í“K—p‚µ‚È‚¢‚Ì‚ªˆÀ‘S
    }
}

void AstroEvolution_Wall::Draw()
{
    if (!m_isVisible)
    {
        return;
    }
    StaticMeshObject::Draw();
}
