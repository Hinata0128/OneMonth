#include "SkyBox.h"

#include "System//00_Manager//01_StaticMeshManager//StaticMeshManager.h"
#include "DirectX/DirectX11.h"
#include "System/06_Camera/Camera.h"

SkyBox::SkyBox()
    : m_pCamera(nullptr)
{
    auto pMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::SkyDome);
    AttachMesh(*pMesh);

    //光遮断.
    this->SetLightEnable(false);

    D3DXVECTOR3 Scale = { 5000.0f, 5000.0f, 5000.0f };
    SetScale(Scale);
}

SkyBox::~SkyBox()
{
}

void SkyBox::Update()
{
    // 4. カメラに追従させる（シングルトンの参照渡し . を使用）
    // これにより、どれだけ動いても球体の中心にいられます
    D3DXVECTOR3 camPos = m_pCamera->GetPosition();
    SetPosition(camPos);

    // 親クラスの更新（行列計算）
    StaticMeshObject::Update();
}

void SkyBox::Draw()
{
    auto dx11 = DirectX11::GetInstance();
    auto context = dx11->GetContext();

    dx11->SetDepth(DEPTH_MODE::SKYBOX);

    StaticMeshObject::Draw();

    dx11->SetDepth(DEPTH_MODE::ON);
}

void SkyBox::SetCamera(Camera* pCamera)
{
    m_pCamera = pCamera;
}
