#include "WallManager.h"

#include "GameObject/02_StaticMeshObject/02_WallBase/00_Wall/Wall.h"
#include "System/00_Manager/03_CollisionManager/CollisionManager.h"
#include "System/00_Manager/02_ImGuiManager/ImGuiManager.h"

#include <fstream>

using json = nlohmann::json;


WallManager::WallManager()
	: ManagerBase	()
{
}

WallManager::~WallManager()
{
}

void WallManager::Update()
{
#if _DEBUG
    ImGui::Begin(JAPANESE("壁のマネージャ"));

    if (ImGui::Button(JAPANESE("新しい壁を追加"))) { AddWall(); }
    ImGui::SameLine();
    if (ImGui::Button(JAPANESE("一括保存"))) { SaveData("Data\\json\\WallPosition\\WallPosition.json"); }

    ImGui::Separator();

    for (int i = 0; i < m_pWalls.size(); ++i)
    {
        std::string label = "Wall #" + std::to_string(i) + "##" + std::to_string(i);

        if (ImGui::TreeNode(label.c_str()))
        {
            // 位置
            D3DXVECTOR3 pos = m_pWalls[i]->GetPosition();
            if (ImGui::DragFloat3("Position", (float*)&pos, 0.1f)) {
                m_pWalls[i]->SetPosition(pos);
            }

            // スケール
            D3DXVECTOR3 scale = m_pWalls[i]->GetScale();
            if (ImGui::DragFloat3("Scale", (float*)&scale, 0.1f)) {
                m_pWalls[i]->SetScale(scale);
            }

            // 表示フラグ
            bool visible = m_pWalls[i]->IsVisible();
            if (ImGui::Checkbox("Visible", &visible)) {
                m_pWalls[i]->SetVisible(visible);
            }

            if (ImGui::Button(JAPANESE("削除"))) {
                m_pWalls.erase(m_pWalls.begin() + i);
                ImGui::TreePop();
                break;
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();
#endif

    for (auto& wall : m_pWalls) {
        wall->Update();
    }
}

void WallManager::Draw()
{
    for (auto& wall : m_pWalls) {
        wall->Draw();
    }
}

void WallManager::Create()
{
	m_pWalls.clear();
}

void WallManager::Release()
{
}

void WallManager::SaveData(const std::string& FilePath)
{
    json Data;
    for (const auto& wall : m_pWalls)
    {
        D3DXVECTOR3 pos = wall->GetPosition();
        D3DXVECTOR3 scale = wall->GetScale();
        bool visible = wall->IsVisible();

        json wallJson;
        wallJson["Pos"] = { pos.x, pos.y, pos.z };
        wallJson["Scale"] = { scale.x, scale.y, scale.z };
        wallJson["Visible"] = visible;

        Data["Walls"].push_back(wallJson);
    }

    std::ofstream File(FilePath);
    if (File.is_open()) {
        File << std::setw(4) << Data << std::endl;
    }
}

void WallManager::LoadData(const std::string& FilePath)
{
    std::ifstream File(FilePath);
    if (!File.is_open()) return;

    json Data;
    try {
        File >> Data;
        if (!Data.contains("Walls") || !Data["Walls"].is_array()) return;

        m_pWalls.clear();

        for (const auto& wJson : Data["Walls"])
        {
            D3DXVECTOR3 pos(wJson["Pos"][0], wJson["Pos"][1], wJson["Pos"][2]);
            D3DXVECTOR3 scale(wJson["Scale"][0], wJson["Scale"][1], wJson["Scale"][2]);
            bool visible = wJson.value("Visible", true); // キーがない場合はtrue

            AddWall(pos, scale, visible);
        }
    }
    catch (const json::exception& e) {
        OutputDebugStringA(e.what());
    }
}

void WallManager::AddWall(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, bool visible)
{
    // 1. 壁のインスタンスを作成
    auto newWall = std::make_unique<Wall>();

    // 2. パラメータ（位置・大きさ・可視性）を設定
    newWall->SetPosition(pos);
    newWall->SetScale(scale);
    newWall->SetVisible(visible);

    // 3. 重要：一度Updateを呼んで内部のBoundingBoxの座標を確定させる
    // これにより BoundingBox::UpdateWorldAABB が実行される
    newWall->Update();

    // 4. 当たり判定マネージャーに登録
    // これにより、Visibleがfalse（見えない壁）でも当たり判定だけは機能する
    auto pBox = newWall->GetBoundingBox();
    if (pBox)
    {
        CollisionManager::GetInstance()->AddBox(pBox);
    }

    // 5. 管理リストに追加
    m_pWalls.push_back(std::move(newWall));
}
