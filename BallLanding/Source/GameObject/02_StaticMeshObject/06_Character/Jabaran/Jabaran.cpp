#include "Jabaran.h"

#include "System//00_Manager//01_StaticMeshManager//StaticMeshManager.h"

Jabaran::Jabaran()
	: Character	()
{
	//敵のスタティックメッシュを呼び込む.
	auto pStaticMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Jabaran);
	//設定.
	//ポインタで取得します.
	AttachMesh(*pStaticMesh);
	
	//ポジションの設定.
	D3DXVECTOR3 Pos = { 0.0f,0.0f,10.0f };
	SetPosition(Pos);

	//サイズの変更.
	D3DXVECTOR3 Scale = { 0.01f,0.01f,0.01f };
	SetScale(Scale);
	//光遮断.
	this->SetLightEnable(false);
	Init();


}

Jabaran::~Jabaran()
{
}

void Jabaran::Update()
{
	Character::Update();
}

void Jabaran::Draw()
{
	Character::Draw();
}

void Jabaran::Init()
{
}

bool Jabaran::GetDead() const
{
	return false;
}
