#pragma once

#include "System//02_Singleton//Singleton.h"
#include "Score.h"
#include <map>
#include <string>

#include <nlohmann/json.hpp>

class ScoreManager final
	: public Singleton
{
public:
	~ScoreManager() override;

	static ScoreManager& GetInstance();

	//指定した名前のスコアを取得.
	std::shared_ptr<Score> GetScore(const std::string& key);

	//特定のスコアを削除.
	void RemoveScore(const std::string& key);

	// 全てのスコアをリセット.
	void Clear() override;

	//スコアデータをjsonファイルに書き込む.
	void SaveToFail(const std::string& FileName);
	//jsonファイルからスコアデータを読み込む.
	void LoadFromFile(const std::string& FileName);

	void SetCurrentGameID(const std::string& id) { m_CurrentGameID = id; }
	std::string GetCurrentGameID() const { return m_CurrentGameID; }
private:
	ScoreManager();
	//コピー禁止.
	ScoreManager(const ScoreManager&) = delete;
	ScoreManager& operator=(const ScoreManager&) = delete;
private:
	// スコアの保管庫
	std::map<std::string, std::shared_ptr<Score>> m_pScores;
	std::string m_CurrentGameID;
};