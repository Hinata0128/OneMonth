#pragma once

#include "System/00_Manager/ManagerBase.h"
#include <nlohmann/json.hpp>

class AstroEvolution_Wall;

/**********************************************************
*	AstroEvolution用の壁のマネージャークラス.
**/

class AstroEvolution_WallManager
	: public ManagerBase
{
public:

	static AstroEvolution_WallManager* GetInstance()
	{
		static AstroEvolution_WallManager instance;
		return &instance;
	}

	~AstroEvolution_WallManager() override;

	void Update() override;
	void Draw() override;
	void Create() override;
	void Release() override;

	void SaveData(const std::string& FilePath);
	void LoadData(const std::string& FilePath);

	void AddWall(const D3DXVECTOR3& pos = { 0.0f, 0.0f, 0.0f },
		const D3DXVECTOR3& scale = { 1.0f, 1.0f, 1.0f },
		bool visible = true);


private:
	AstroEvolution_WallManager();
	//コピー禁止.
	AstroEvolution_WallManager(const AstroEvolution_WallManager& rhs) = delete;
	AstroEvolution_WallManager& operator = (const AstroEvolution_WallManager& rhs) = delete;

private:
	std::vector<std::unique_ptr<AstroEvolution_Wall>> m_pAstroWall;

};
