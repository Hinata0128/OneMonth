#include "ScoreManager.h"
#include <fstream>
#include <iomanip>

using json = nlohmann::json;

ScoreManager::ScoreManager()
{
}

ScoreManager::~ScoreManager()
{
}

std::shared_ptr<Score> ScoreManager::GetScore(const std::string& key)
{
	// 既に同じ名前のスコアがあるか探す
	auto it = m_pScores.find(key);
	if (it != m_pScores.end())
	{
		return it->second;
	}

	// なければ新しく作って保存する
	auto newScore = std::make_shared<Score>();
	m_pScores[key] = newScore;
	return newScore;
}

void ScoreManager::RemoveScore(const std::string& key)
{
	m_pScores.erase(key);
}

void ScoreManager::Clear()
{
	m_pScores.clear();
}

void ScoreManager::SaveToFail(const std::string& FileName)
{
	json OutJson;
	//現在管理しているすべてのスコアをjsonオブジェクトに格納.
	for (auto const& [key, score] : m_pScores)
	{
		// キーの中に "_High" という文字列が含まれている場合のみ
		if (key.find("_High") != std::string::npos)
		{
			OutJson[key] = score->GetValue();
		}
	}
	//ファイル書き出し.
	std::ofstream file(FileName);
	if (file.is_open())
	{
		file << std::setw(4) << OutJson << std::endl;
		file.close();
	}
}

void ScoreManager::LoadFromFile(const std::string& FileName)
{
	std::ifstream file(FileName);
	if (!file.is_open())
	{
		return;
	}

	json InJson;
	file >> InJson;

	try {
		json InJson;
		file >> InJson;

		// ★修正：読み込む時もハイスコア系だけを対象にする
		for (auto it = InJson.begin(); it != InJson.end(); it++)
		{
			if (it.key().find("_High") != std::string::npos)
			{
				auto score = GetScore(it.key());
				score->Set(it.value().get<int>());
			}
		}
	}
	catch (...) {
		// パースエラー対策
	}
}
