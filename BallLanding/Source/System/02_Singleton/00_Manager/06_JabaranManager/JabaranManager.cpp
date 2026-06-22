#include "JabaranManager.h"
#include "..//..//..//..//GameObject//02_StaticMeshObject//01_Character//Jabaran//Jabaran.h"
#include "..//02_ImGuiManager//ImGuiManager.h"
#include <fstream>
//JSONを使用するためのインクルード.
#include <nlohmann//json.hpp>

using json = nlohmann::json;
const std::string JSON_PATH = "Data//JsonFileNameBox//JabaranSpown.json";

//コンストラクタ.
JabaranManager::JabaranManager()
	: ManagerBase	()
{
	Init();
}

//デストラクタ.
JabaranManager::~JabaranManager()
{
}

//構築関数.
void JabaranManager::Create()
{
}

//動作関数.
void JabaranManager::Update()
{
	for (auto& JabaranManagerList : m_upJabaranList)
	{
		if (JabaranManagerList)
		{
			JabaranManagerList->Update();
		}
	}

	m_upJabaranList.erase(
		std::remove_if(m_upJabaranList.begin(), m_upJabaranList.end(),
			[](const std::unique_ptr<Jabaran>& pEnemy) 
			{
				return pEnemy && pEnemy->GetDead();
			}),
		m_upJabaranList.end()
	);
}

//描画関数.
void JabaranManager::Draw()
{
	for (auto& JabaranManagerList : m_upJabaranList)
	{
		if (JabaranManagerList)
		{
			JabaranManagerList->Draw();
		}
	}

	ImGui::Begin("Enemy Spawn Tool");

	//敵の新規スポーン.
	if (ImGui::CollapsingHeader("Spawn Enemy", ImGuiTreeNodeFlags_DefaultOpen))
	{
		//敵の最初のスポーン位置.
		static D3DXVECTOR3 SpawnPos = { 0.0f, 0.0f, 10.0f };
		ImGui::DragFloat3("Spawn Position", (float*)&SpawnPos, 0.1f);

		if (ImGui::Button("Spawn Jabaran"))
		{
			auto JabaranList = std::make_unique<Jabaran>();
			JabaranList->SetPosition(SpawnPos);
			m_upJabaranList.push_back(std::move(JabaranList));
		}
	}

	ImGui::Separator();

	if (ImGui::CollapsingHeader("Active Enemies", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Total Enemies: %d", m_upJabaranList.size());

		//削除対象のインデックスを保持する変数.
		int BeleteIndex = -1;

		for (size_t i = 0; i < m_upJabaranList.size(); ++i)
		{
			if (!m_upJabaranList[i]) continue;

			//ImGuiのIDが衝突しないように一意のラベルを作成.
			std::string Label = "Jabaran [" + std::to_string(i) + "]";
			D3DXVECTOR3 Pos = m_upJabaranList[i]->GetPosition();

			//座標変更スライダー.
			if (ImGui::DragFloat3(Label.c_str(), (float*)&Pos, 0.05f))
			{
				m_upJabaranList[i]->SetPosition(Pos);
			}

			//同じ行に削除ボタンを配置するためにSameLineを使う.
			ImGui::SameLine();
			std::string BtnLabel = "Delete##" + std::to_string(i);

			if (ImGui::Button(BtnLabel.c_str()))
			{
				BeleteIndex = static_cast<int>(i);
			}
		}

		//ループの直後に安全に削除を実行する.
		if (BeleteIndex != -1)
		{
			//配列から要素を削除.
			m_upJabaranList.erase(m_upJabaranList.begin() + BeleteIndex);
		}

		ImGui::Spacing();

		//確定した配置をJabaranSpown.jsonへ上書き保存する.
		if (ImGui::Button("Save Layout to JSON"))
		{
			json OutData;
			for (const auto& JabaranList : m_upJabaranList)
			{
				if (!JabaranList) continue;
				D3DXVECTOR3 P = JabaranList->GetPosition();

				json JabaranJSON;
				JabaranJSON["Type"] = "Jabaran";
				JabaranJSON["PosX"] = P.x;
				JabaranJSON["PosY"] = P.y;
				JabaranJSON["PosZ"] = P.z;

				OutData["Enemies"].push_back(JabaranJSON);
			}

			// 指定されたパスへ保存
			std::ofstream File(JSON_PATH);
			if (File.is_open())
			{
				File << OutData.dump(4);
				OutputDebugStringA("Successfully saved layout to JabaranSpown.json!\n");
			}
			else
			{
				OutputDebugStringA("Failed to save JSON file!\n");
			}
		}
	}

	ImGui::End();
}

//解放関数.
void JabaranManager::Release()
{
	m_upJabaranList.clear();
}

//初期化関数.
void JabaranManager::Init()
{
	Release();
	LoadJabaranFromSpownPosName(JSON_PATH);
}

//JSONファイルから敵を読み込んで配置する関数.
void JabaranManager::LoadJabaranFromSpownPosName(const std::string& FilePath)
{
	//現在管理している敵を一度リセット.
	Release();
	//ファイルストリームを開く.
	std::ifstream File(FilePath);
	if (!File.is_open())
	{
		//読み込み失敗時のログ.
		OutputDebugStringA(("Failed To Open JSON File: " + FilePath + "\n").c_str());
		return;
	}
	try
	{
		//JSONバース.
		json Data;
		File >> Data;
		//Jabaranという配列が存在するかチェック.
		if (Data.contains("Enemies") && Data["Enemies"].is_array())
		{
			for (const auto& JabaranList : Data["Enemies"])
			{
				//各敵の座標データを取得
				float x = JabaranList["PosX"].get<float>();
				float y = JabaranList["PosY"].get<float>();
				float z = JabaranList["PosZ"].get<float>();

				//敵のインスタンスを生成.
				auto pEnemy = std::make_unique<Jabaran>();

				//JSONから取得した初期座標を設定.
				pEnemy->SetPosition(D3DXVECTOR3(x, y, z));

				//マネージャーのリストに登録.
				m_upJabaranList.push_back(std::move(pEnemy));
			}
		}
	}
	catch (const json::parse_error& e)
	{
		//JSONの文法エラーなどが発生した場合の対策.
		OutputDebugStringA(("JSON Parse Error: " + std::string(e.what()) + "\n").c_str());
	}
}
