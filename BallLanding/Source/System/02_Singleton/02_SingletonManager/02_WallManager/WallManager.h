#pragma once
#include "System//00_Manager//ManagerBase.h"
#include <nlohmann/json.hpp>

class Wall;

/*******************************************************************
*	壁マネージャクラス.
**/

class WallManager
	: public ManagerBase
{
public:

	static WallManager* GetInstance()
	{
		static WallManager instance;
		return &instance;
	}

	~WallManager() override;
	
	void Update() override;
	void Draw() override;
	void Create() override;
	void Release() override;

	void SaveData(const std::string& FilePath);
	void LoadData(const std::string& FilePath);

	void AddWall(const D3DXVECTOR3& pos = { 0.0f, 0.0f, 0.0f },
		const D3DXVECTOR3& scale = { 1.0f, 1.0f, 1.0f },
		bool visible = true);

	const std::vector<std::unique_ptr<Wall>>& GetWalls() const { return m_pWalls; }

private:
	WallManager();
	//コピー禁止.
	WallManager(const WallManager& rhs)  = delete;
	WallManager& operator = (const WallManager& rhs) = delete;
private:
	std::vector<std::unique_ptr<Wall>> m_pWalls;
};