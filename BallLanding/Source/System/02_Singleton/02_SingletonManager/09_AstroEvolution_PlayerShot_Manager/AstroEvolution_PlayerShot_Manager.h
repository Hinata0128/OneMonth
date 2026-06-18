#pragma once
#include "System/00_Manager/ManagerBase.h"
#include "GameObject/02_StaticMeshObject/05_ShotBase/02_AstroEvolution_PlayerShot/AstroEvolution_PlayerShot.h"
#include <algorithm>

class AstroEvolution_PlayerShot_Manager final
	: public ManagerBase
{
public:
	static AstroEvolution_PlayerShot_Manager* GetInstance()
	{
		static AstroEvolution_PlayerShot_Manager instance;
		return &instance;
	}
	~AstroEvolution_PlayerShot_Manager();

	void Update() override;
	void Draw() override;
	void Create() override;
	void Release() override;

	// プレイヤーから呼ばれる発射関数
	// pos: 発射位置, angleY: プレイヤーの向き, shotLevel: 現在の強化段階
	void PlayerFire(const D3DXVECTOR3& pos, float angleY, int shotLevel);

	const std::vector<std::unique_ptr<AstroEvolution_PlayerShot>>& GetShots() const {
		return m_pShots;
	}

private:
	AstroEvolution_PlayerShot_Manager();
	AstroEvolution_PlayerShot_Manager(const AstroEvolution_PlayerShot_Manager&) = delete;
	AstroEvolution_PlayerShot_Manager& operator=(const AstroEvolution_PlayerShot_Manager&) = delete;
private:
	std::vector<std::unique_ptr<AstroEvolution_PlayerShot>> m_pShots;
};