#include "AstralDuel_SkyDome.h"

#include "System//00_Manager//01_StaticMeshManager//StaticMeshManager.h"
#include "DirectX/DirectX11.h"
#include "System/06_Camera/Camera.h"

AstralDuel_SkyDome::AstralDuel_SkyDome()
    : m_pCamera(nullptr)
{
    auto pMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::SkyDome_No4);
    AttachMesh(*pMesh);

    //ŒõŽÕ’f.
    this->SetLightEnable(false);

    D3DXVECTOR3 Scale = { 5000.0f, 5000.0f, 5000.0f };
    SetScale(Scale);

}

AstralDuel_SkyDome::~AstralDuel_SkyDome()
{
}

void AstralDuel_SkyDome::Update()
{
    D3DXVECTOR3 camPos = m_pCamera->GetPosition();
    SetPosition(camPos);

    StaticMeshObject::Update();
}

void AstralDuel_SkyDome::Draw()
{
    auto dx11 = DirectX11::GetInstance();
    auto context = dx11->GetContext();

    dx11->SetDepth(DEPTH_MODE::SKYBOX);

    StaticMeshObject::Draw();

    dx11->SetDepth(DEPTH_MODE::ON);
}

void AstralDuel_SkyDome::SetCamera(Camera* pCamera)
{
    m_pCamera = pCamera;
}
