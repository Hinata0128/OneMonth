#include "RingManager.h"

#include "GameObject/02_StaticMeshObject/06_Character/00_Player/Player.h"
#include "System/00_Manager/03_CollisionManager/CollisionManager.h"

RingManager::RingManager()
	: ManagerBase()
	, m_pRings()
	, m_pPlayer(nullptr)
{
}

RingManager::~RingManager()
{
}

void RingManager::Update()
{
	if (!m_pPlayer)
	{
		return;
	}
	auto PlayerSphere = m_pPlayer->GetBoundingSphere();
	if (!PlayerSphere)
	{
		return;
	}
	for (auto& ring : m_pRings)
	{
		ring->Update();
	}
}

void RingManager::Draw()
{
	for (auto& ring : m_pRings) 
	{
		ring->Draw();
	}
}

void RingManager::Create()
{
	m_pRings.clear();
	//Å‰‚É¶¬‚³‚¹‚éƒŠƒ“ƒO‚Ì”‚ğİ’è.
	int RingCount = 10;
	for (int i = 0; i < RingCount; ++i)
	{
		auto ring = std::make_unique<Ring>();
		ring->TeleportRandom();
		CollisionManager::GetInstance()->AddRing(ring.get());
		m_pRings.push_back(std::move(ring));
	}
}

void RingManager::Release()
{
}
