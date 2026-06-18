#include "AstroEvolution_Rock_Manager.h"
#include "GameObject/02_StaticMeshObject/06_Character/03_AstroEvolution_Rock/AstroEvolution_Rock.h"

#include "System/00_Manager/02_ImGuiManager/ImGuiManager.h"

#include "System/00_Manager/03_CollisionManager/CollisionManager.h"

#include <random>
#include <fstream>

using json = nlohmann::json;

AstroEvolution_Rock_Manager::AstroEvolution_Rock_Manager()
	: ManagerBase()
{
}

AstroEvolution_Rock_Manager::~AstroEvolution_Rock_Manager()
{
}

void AstroEvolution_Rock_Manager::Update()
{
#if _DEBUG
    ImGui::Begin(JAPANESE("岩のマネージャ"));

    // 生成ルールの設定
    ImGui::Text(JAPANESE("生成ルール設定"));
    ImGui::SliderInt(JAPANESE("個数"), &m_Config.Count, 0, 100);
    ImGui::DragFloat2(JAPANESE("X/Z範囲"), m_Config.RangeXZ, 0.5f, 0.0f, 200.0f);
    ImGui::DragFloat2(JAPANESE("Y範囲"), m_Config.RangeY, 0.5f, -50.0f, 50.0f);

    static int debugRound = 1;
    ImGui::InputInt(JAPANESE("読み込みラウンド"), &debugRound);

    if (ImGui::Button(JAPANESE("この設定で再生成")))
    {
        LoadData_Astro_Rock("Data//json//Astro_Rock//Astro_Rock_Param.json", debugRound);
    }
    ImGui::SameLine();

    if (ImGui::Button(JAPANESE("設定を保存"))) {
        SaveData_Astro_Rock("Data//json//Astro_Rock//Astro_Rock_Param.json");
    }

    ImGui::Separator();

    // 個別の岩を確認したい場合（WallManager方式）
    if (ImGui::TreeNode(JAPANESE("個別の岩リスト"))) {
        for (int i = 0; i < m_pAstro_Rock.size(); ++i) {
            std::string label = "Rock #" + std::to_string(i);
            if (ImGui::TreeNode(label.c_str())) {
                D3DXVECTOR3 pos = m_pAstro_Rock[i]->GetPosition();
                if (ImGui::DragFloat3("Position", (float*)&pos, 0.1f)) {
                    m_pAstro_Rock[i]->SetPosition(pos);
                }
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }

    ImGui::End(); 
#endif
	for (auto& AstroRock : m_pAstro_Rock)
	{
		AstroRock->Update();
	}
}

void AstroEvolution_Rock_Manager::Draw()
{
	for (auto& AstroRock : m_pAstro_Rock)
	{
		AstroRock->Draw();
	}
}

void AstroEvolution_Rock_Manager::Create()
{
	m_pAstro_Rock.clear();
    //開始時は毎回ラウンド1から.
    LoadData_Astro_Rock("Data//json//Astro_Rock//Astro_Rock_Param.json", 1);
}

void AstroEvolution_Rock_Manager::Release()
{
}

void AstroEvolution_Rock_Manager::SaveData_Astro_Rock(const std::string& FilePath)
{
    json Data;
    Data["SpawnConfig"]["Count"] = m_Config.Count;
    Data["SpawnConfig"]["RangeXZ"] = { m_Config.RangeXZ[0], m_Config.RangeXZ[1] };
    Data["SpawnConfig"]["RangeY"] = {m_Config.RangeY[0], m_Config.RangeY[1]};

    std::ofstream File(FilePath);
    if (File.is_open())
    {
        File << std::setw(4) << Data << std::endl;
    }
}

void AstroEvolution_Rock_Manager::LoadData_Astro_Rock(const std::string& FilePath, int round)
{
    std::ifstream File(FilePath);
    if (!File.is_open())
    {
        return;
    }

    json Data;
    File >> Data;

    json roundData;
    bool found = false;
    for (const auto& r : Data["Rounds"]) {
        if (r["Round"] == round) {
            roundData = r;
            found = true;
            break;
        }
    }

    if (!found) {
        // デバッグログなどを出すと原因がわかりやすくなります
        return;
    }

    //jsonから設定を読み込む.
    m_Config.Count = roundData["Count"];
    m_Config.RangeXZ[0] = roundData["RangeXZ"][0];
    m_Config.RangeXZ[1] = roundData["RangeXZ"][1];
    m_Config.RangeY[0] = roundData["RangeY"][0];
    m_Config.RangeY[1] = roundData["RangeY"][1];

    //リストをクリアして再生成.
    m_pAstro_Rock.clear();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> DistXZ(m_Config.RangeXZ[0], m_Config.RangeXZ[1]);
    std::uniform_real_distribution<float> DistY(m_Config.RangeY[0], m_Config.RangeY[1]);

    for (int i = 0; i < m_Config.Count; ++i)
    {
        auto NewRock = std::make_unique<AstroEvolution_Rock>();

        //ランダム座標.
        D3DXVECTOR3 Pos(DistXZ(gen), DistY(gen), DistXZ(gen));
        //指定した範囲に岩がランダムで生成される.
        NewRock->SetPosition(Pos);

        CollisionManager::GetInstance()->AddRock(NewRock.get());
        //岩をリスト変換.
        m_pAstro_Rock.push_back(std::move(NewRock));
    }
}

bool AstroEvolution_Rock_Manager::IsAllDestroyed() const
{
    for (const auto& rock : m_pAstro_Rock) 
    {
        if (!rock->IsDead()) return false;
    }
    return true;
}
