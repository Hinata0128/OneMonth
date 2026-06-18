#include "Needle.h"

#include "System//00_Manager//01_StaticMeshManager//StaticMeshManager.h"
#include "System/00_Manager/03_CollisionManager/CollisionManager.h"
#include "System/00_Manager/02_ImGuiManager/ImGuiManager.h"
	
Needle::Needle()
	: StaticMeshObject	()
	, m_DegRota			{ 90.0f, 0.0f, 0.0f }
{
	//読み込み.
	auto pMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Needle);
	//設定.
	AttachMesh(*pMesh);

	//ポジションの設定.
	D3DXVECTOR3 Pos = { 0.0f,5.0f,-4.5f };
	SetPosition(Pos);

	//サイズの設定.
	D3DXVECTOR3 Scale = { 0.005f ,0.005f , 0.005f };
	SetScale(Scale);

	D3DXVECTOR3 Rota = { D3DXToRadian(90.0f), 0.0f, 0.0f };
	SetRotation(Rota);

	m_pSphere = std::make_shared<BoundingSphere>();
	if (pMesh)
	{
		//メッシュから半径を計算.
		m_pSphere->CreateSphereForMesh(*pMesh);

		m_pSphere->SetRadius(0.7f);
	}

	m_pSphere->SetPosition(Pos);

	CollisionManager::GetInstance()->AddSphere(m_pSphere);

}

Needle::~Needle()
{
}

void Needle::Update()
{
	// 基本的なトランスフォーム更新（行列計算など）
	StaticMeshObject::Update();

	if (m_pSphere)
	{
		m_pSphere->SetPosition(GetPosition());
	}
}

void Needle::Draw()
{
	StaticMeshObject::Draw();
}
