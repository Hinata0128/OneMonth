#include "Ring.h"

#include "System/00_Manager/01_StaticMeshManager/StaticMeshManager.h"

#include "GameObject/02_StaticMeshObject/06_Character/00_Player/Player.h"
#include "System/00_Manager/03_CollisionManager/CollisionManager.h"
#include <cmath>
#include <ctime>

Ring::Ring()
	: StaticMeshObject()
{
    auto pMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Ball);
    AttachMesh(*pMesh);

    D3DXVECTOR3 Pos = { 0.0f, 0.0f, -4.0f };
    SetPosition(Pos);

    D3DXVECTOR3 Scale = { 100.0f, 100.0f, 100.0f };
    SetScale(Scale);
    this->SetLightEnable(false);

    m_pSphere = std::make_shared<BoundingSphere>();
    m_pSphere->SetRadius(8.0f);

    TeleportRandom();
}

Ring::~Ring()
{
}

void Ring::Update()
{
	StaticMeshObject::Update();
	if (m_pSphere) m_pSphere->SetPosition(m_Position);
}

void Ring::Draw()
{
	//m_pSphere->Draw();
	StaticMeshObject::Draw();
}

void Ring::TeleportRandom()
{
    float rangeXZ = 1000.0f;
    float rangeY = 200.0f;
    float x = (float)(rand() % (int)(rangeXZ + 1)) - (rangeXZ * 0.5f);
    float y = (float)(rand() % (int)(rangeY + 1)) - (rangeY * 0.5f);
    float z = (float)(rand() % (int)(rangeXZ + 1)) - (rangeXZ * 0.5f);

    D3DXVECTOR3 newPos = { x, y, z };
    SetPosition(newPos);

    if (m_pSphere) {
        m_pSphere->SetPosition(newPos);
    }
}
