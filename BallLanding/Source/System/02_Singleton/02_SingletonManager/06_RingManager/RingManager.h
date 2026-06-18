#pragma once
#include "System/00_Manager/ManagerBase.h"

#include "GameObject/02_StaticMeshObject/07_Ring/Ring.h"
class Player;

class RingManager final
	: public ManagerBase
{
public:
	//インスタンス取得.
	static RingManager* GetInstance()
	{
		//唯一のインスタンスを作成する.
		static RingManager s_Instance;
		return &s_Instance;
	}

	~RingManager() override;

	void Update() override;
	void Draw() override;
	void Create() override;
	void Release() override;

	void SetPlayer(Player* pPlayer)
	{
		m_pPlayer = pPlayer;
	}
private:
	RingManager();
	//コピーを禁止している.
	RingManager(const RingManager& rhs) = delete;
	RingManager& operator = (const RingManager& rhs) = delete;
private:
	std::vector<std::unique_ptr<Ring>> m_pRings;
	Player* m_pPlayer;
};