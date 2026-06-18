#pragma once

#include "System/00_Manager/ManagerBase.h"
//使用するかはまだ未定.
#include <nlohmann/json.hpp>

class AstroEvolution_Rock;

/***********************************************************
*	AstroEvolution_Rock用のマネージャークラス.
*	岩の生成を複製させる.
**/

class AstroEvolution_Rock_Manager final
	: public ManagerBase
{
public:
	
	static AstroEvolution_Rock_Manager* GetInstance()
	{
		static AstroEvolution_Rock_Manager instance;
		return &instance;
	}

	~AstroEvolution_Rock_Manager() override;

	void Update() override;
	void Draw() override;
	void Create() override;
	void Release() override;

	void SaveData_Astro_Rock(const std::string& FilePath);
	void LoadData_Astro_Rock(const std::string& FilePath, int round);
	//ラウンドごとの岩が消滅したか確認する関数.
	bool IsAllDestroyed() const;

	// 現在のラウンドを取得
	int GetCurrentRound() const { return m_CurrentRound; }
	// 次のラウンドへ進める
	void NextRound() { m_CurrentRound++; }
	// ラウンドをリセット（ゲームオーバー時やタイトルに戻る時用）
	void ResetRound() { m_CurrentRound = 1; }

private:
	AstroEvolution_Rock_Manager();
	//コピー禁止.
	AstroEvolution_Rock_Manager(const AstroEvolution_Rock_Manager& rhs) = delete;
	AstroEvolution_Rock_Manager& operator = (const AstroEvolution_Rock_Manager& rhs) = delete;
private:
	//岩の生成位置の設定用構造体.
	struct SpwnConfig
	{
		//生成数.
		int Count = 30;
		//XZの生成範囲.
		float RangeXZ[2] = { -70.0f,70.0f };
		//Yの生成範囲.
		float RangeY[2] = { 0.0f, 20.0f };
	} m_Config;
private:
	std::vector<std::unique_ptr<AstroEvolution_Rock>> m_pAstro_Rock;
	int m_CurrentRound = 1;
};