#include "AstroEvolution_WallManager.h"

#include "GameObject/02_StaticMeshObject/02_WallBase/01_AstroEvolution_Wall/AstroEvolution_Wall.h"
#include "System/00_Manager/03_CollisionManager/CollisionManager.h"
#include "System/00_Manager/02_ImGuiManager/ImGuiManager.h"
#include <fstream>

using json = nlohmann::json;

AstroEvolution_WallManager::AstroEvolution_WallManager()
	: ManagerBase()
{
}

AstroEvolution_WallManager::~AstroEvolution_WallManager()
{
}

void AstroEvolution_WallManager::Update()
{
#if _DEBUG
    ImGui::Begin(JAPANESE("壁のマネージャ"));

    if (ImGui::Button(JAPANESE("新しい壁を追加"))) { AddWall(); }
    ImGui::SameLine();
    if (ImGui::Button(JAPANESE("一括保存"))) { SaveData("Data\\json\\AstroEvolution_WallPosition\\AstroEvolution_WallPosition.json"); }

    ImGui::Separator();

    for (int i = 0; i < m_pAstroWall.size(); ++i)
    {
        std::string label = "Wall #" + std::to_string(i) + "##" + std::to_string(i);

        if (ImGui::TreeNode(label.c_str()))
        {
            // 位置
            D3DXVECTOR3 pos = m_pAstroWall[i]->GetPosition();
            if (ImGui::DragFloat3("Position", (float*)&pos, 0.1f)) {
                m_pAstroWall[i]->SetPosition(pos);
            }

            // スケール
            D3DXVECTOR3 scale = m_pAstroWall[i]->GetScale();
            if (ImGui::DragFloat3("Scale", (float*)&scale, 0.1f)) {
                m_pAstroWall[i]->SetScale(scale);
            }

            // 表示フラグ
            bool visible = m_pAstroWall[i]->IsVisible();
            if (ImGui::Checkbox("Visible", &visible)) {
                m_pAstroWall[i]->SetVisible(visible);
            }

            if (ImGui::Button(JAPANESE("削除"))) {
                m_pAstroWall.erase(m_pAstroWall.begin() + i);
                ImGui::TreePop();
                break;
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();
#endif
    for (auto& AstroWall : m_pAstroWall)
    {
		AstroWall->Update();
    }
}

void AstroEvolution_WallManager::Draw()
{
    for (auto& AstroWall : m_pAstroWall)
    {
		AstroWall->Draw();  
    }
}

void AstroEvolution_WallManager::Create()
{
	m_pAstroWall.clear();
}

void AstroEvolution_WallManager::Release()
{
}

void AstroEvolution_WallManager::SaveData(const std::string& FilePath)
{
    json Data;
    for (const auto& AstroWall : m_pAstroWall)
    {
        D3DXVECTOR3 Pos = AstroWall->GetPosition();
        D3DXVECTOR3 Scale = AstroWall->GetScale();
        bool Visible = AstroWall->IsVisible();

        json AstroWallJson;
        AstroWallJson["Pos"] = { Pos.x,Pos.y,Pos.z };
        AstroWallJson["Scale"] = { Scale.x,Scale.y,Scale.z };
        AstroWallJson["Visible"] = Visible;

        Data["AstroWall"].push_back(AstroWallJson);
    }

    std::ofstream File(FilePath);
    if (File.is_open())
    {
        File << std::setw(4) << Data << std::endl;
    }
}

void AstroEvolution_WallManager::LoadData(const std::string& FilePath)
{
    std::ifstream File(FilePath);
    if (!File.is_open())
    {
        return;
    }

    json Data;
    try
    {
        File >> Data;
        if (!Data.contains("AstroWall") || !Data["AstroWall"].is_array()) return;

        m_pAstroWall.clear();

        for (const auto& wJson : Data["AstroWall"])
        {
            D3DXVECTOR3 pos(wJson["Pos"][0], wJson["Pos"][1], wJson["Pos"][2]);
            D3DXVECTOR3 scale(wJson["Scale"][0], wJson["Scale"][1], wJson["Scale"][2]);
            bool visible = wJson.value("Visible", true); // キーがない場合はtrue

            AddWall(pos, scale, visible);
        }
    }
    catch (const json::exception& e) 
    {
        OutputDebugStringA(e.what());
    }
}

void AstroEvolution_WallManager::AddWall(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, bool visible)
{
    auto NewAstroWall = std::make_unique<AstroEvolution_Wall>();

    NewAstroWall->SetPosition(pos);
    NewAstroWall->SetScale(scale);
    NewAstroWall->SetVisible(visible);

    NewAstroWall->Update();

    auto pBox = NewAstroWall->GetBoundingBox();
    if (pBox)
    {
        CollisionManager::GetInstance()->AddBox(pBox);
    }

    m_pAstroWall.push_back(std::move(NewAstroWall));

}
