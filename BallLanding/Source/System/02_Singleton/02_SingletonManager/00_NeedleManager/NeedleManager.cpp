#include "NeedleManager.h"

#include "GameObject//02_StaticMeshObject//03_Needle//Needle.h"
#include "System//00_Manager//02_ImGuiManager//ImGuiManager.h"

#include <fstream>

using json = nlohmann::json;

NeedleManager::NeedleManager()
	: ManagerBase	()
{
}

NeedleManager::~NeedleManager()
{
	Release();
}

void NeedleManager::Update()
{
#if _DEBUG
	ImGui::Begin(JAPANESE("針のマネージャ"));

	if (ImGui::Button(JAPANESE("新しい針を追加"))) { AddNeedle(); }
	ImGui::SameLine();
	if (ImGui::Button(JAPANESE("一括保存"))) { SaveData("Data\\json\\Needle\\Needel.json"); }

	ImGui::Separator();

	// 針のリストを表示
	for (int i = 0; i < m_Needles.size(); ++i)
	{
		// 各針に固有のIDを持たせる (##i は表示されない識別子)
		std::string label = "Needle #" + std::to_string(i) + "##" + std::to_string(i);

		if (ImGui::TreeNode(label.c_str()))
		{
			// --- 位置の編集 ---
			D3DXVECTOR3 pos = m_Needles[i]->GetPosition();
			if (ImGui::DragFloat3("Position", (float*)&pos, 0.1f)) {
				m_Needles[i]->SetPosition(pos);
			}

			// --- 回転の編集 (マネージャ側で度数管理) ---
			// Needleクラスから現在の回転(radian)を取得して度数に変換して表示も可能ですが、
			// 簡易的にそのまま操作する場合はこちら
			D3DXVECTOR3 rot = m_Needles[i]->GetRotation();
			if (ImGui::DragFloat3("Rotation", (float*)&rot, 0.01f)) {
				m_Needles[i]->SetRotation(rot);
			}

			// --- 削除機能 ---
			if (ImGui::Button(JAPANESE("削除"))) {
				m_Needles.erase(m_Needles.begin() + i);
				ImGui::TreePop();
				break; // 要素数が変わるのでループを抜ける
			}

			ImGui::TreePop();
		}
	}

	ImGui::End();
#endif

	// 実際の座標更新などのロジック実行
	for (auto& needle : m_Needles) {
		needle->Update();
	}
}

void NeedleManager::Draw()
{
	for (auto& Needle : m_Needles)
	{
		Needle->Draw();
	}
}

void NeedleManager::Create()
{
}

void NeedleManager::Release()
{
	m_Needles.clear();
}

void NeedleManager::SaveData(const std::string& FilePath)
{
	json Data;
	//すべての針を配列に格納.
	for (const auto& Needle : m_Needles)
	{
		//位置.
		D3DXVECTOR3 Position	= Needle->GetPosition();
		//回転.
		D3DXVECTOR3 Rotation	= Needle->GetRotation();

		json NeedleJson;
		NeedleJson["Pos"] = { Position.x, Position.y, Position.z };
		NeedleJson["Rot"] = { Rotation.x, Rotation.y, Rotation.z };

		Data["Needles"].push_back(NeedleJson);
	}

	//ファイル書き出し.
	std::ofstream File(FilePath);
	if (File.is_open())
	{
		File << std::setw(4) << Data << std::endl;
	}
}

void NeedleManager::LoadData(const std::string& FilePath)
{
	std::ifstream File(FilePath);
	if (!File.is_open())
	{
		return;
	}

	json Data;

	try {
		File >> Data;

		// "Needles" というキーがない、または配列じゃない場合は中断
		if (!Data.contains("Needles") || !Data["Needles"].is_array()) return;

		m_Needles.clear();

		for (const auto& NeedleJson : Data["Needles"])
		{
			// Pos と Rot が存在し、かつ配列(サイズ3)であることを確認
			if (NeedleJson.contains("Pos") && NeedleJson["Pos"].is_array() &&
				NeedleJson.contains("Rot") && NeedleJson["Rot"].is_array())
			{
				auto NewNeedle = std::make_unique<Needle>();

				D3DXVECTOR3 Pos(NeedleJson["Pos"][0], NeedleJson["Pos"][1], NeedleJson["Pos"][2]);
				D3DXVECTOR3 Rot(NeedleJson["Rot"][0], NeedleJson["Rot"][1], NeedleJson["Rot"][2]);

				NewNeedle->SetPosition(Pos);
				NewNeedle->SetRotation(Rot);

				m_Needles.push_back(std::move(NewNeedle));
			}
		}
	}
	catch (const json::exception& e) {
		// JSONの型がどうしても合わない時のエラー内容をデバッグ出力
		OutputDebugStringA(e.what());
	}
}

//針の追加.
void NeedleManager::AddNeedle()
{
	auto NewNeedle = std::make_unique<Needle>();

	// 針の数に応じて X 座標を 2.0 ずつ右にずらして生成する例
	float offsetX = (float)m_Needles.size() * 2.0f;
	NewNeedle->SetPosition(D3DXVECTOR3(offsetX, 5.0f, 0.0f));

	NewNeedle->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_Needles.push_back(std::move(NewNeedle));
}
