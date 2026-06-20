#include "PlayerShot.h"
#include "System\02_Singleton//00_Manager\01_StaticMeshManager\StaticMeshManager.h"

PlayerShot::PlayerShot()
	: ShotBase()
{
	//
	auto pStaticMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::PlayerShot);
	//設定.
	//ポインタで取得します.
	AttachMesh(*pStaticMesh);

	//サイズの変更.
	D3DXVECTOR3 Scale = { 0.5f,0.5f,0.5f };
	SetScale(Scale);
	//光遮断.
	this->SetLightEnable(false);

}

PlayerShot::~PlayerShot()
{
}

void PlayerShot::Update()
{
	ShotBase::Update();
}

void PlayerShot::Draw()
{
	ShotBase::Draw();
}

void PlayerShot::Launch(const D3DXVECTOR3& Pos, const D3DXVECTOR3& Vel, float Radius, float Life)
{
	ShotBase::Launch(Pos, Vel, Radius, Life);
}
