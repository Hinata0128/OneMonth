#pragma once
#include "System//00_Manager//ManagerBase.h"	//シングルトンで作成しているけど継承はマネージャのベースクラスです.
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

class Needle;

/****************************************************************
*	針クラスのマネージャクラス.
*	ToDo : 針の複製をしている.
*		   シングルトンで作成することによりどこでも呼べる.
**/

class NeedleManager final
	: public ManagerBase
{
public:
	//シングルトンインスタンスを作成.
	static NeedleManager* GetInstance()
	{
		static NeedleManager instance;
		return &instance;
	}

	~NeedleManager() override;

	void Update() override;
	void Draw() override;
	void Create() override;
	void Release() override;

	//json保存と読み込み.
	void SaveData(const std::string& FilePath);
	void LoadData(const std::string& FilePath);
	//針の追加.
	void AddNeedle();

private:
	NeedleManager();
	//コピー禁止.
	NeedleManager(const NeedleManager& rhs) = delete;
	NeedleManager& operator = (const NeedleManager& rhs) = delete;

private:
	std::vector<std::unique_ptr<Needle>> m_Needles;
};