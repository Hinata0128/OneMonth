#include "Ground.h"
#include "System//02_Singleton//00_Manager//01_StaticMeshManager//StaticMeshManager.h"

Ground::Ground()
{
	AttachMesh(*StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Ground));

	D3DXVECTOR3 Pos = { 0.0f,-1.0f,0.f };
	SetPosition(Pos);
}

Ground::~Ground()
{
}

void Ground::Update()
{
}
