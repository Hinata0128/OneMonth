#include "EnemyShotManager.h"

EnemyShotManager* EnemyShotManager::m_pInstance = nullptr;

EnemyShotManager::EnemyShotManager()
{
}

EnemyShotManager::~EnemyShotManager()
{
}

EnemyShotManager* EnemyShotManager::GetInstance()
{
	static EnemyShotManager s_Instance;
	return &s_Instance;
}

void EnemyShotManager::Update()
{
	for (auto it = m_pBossShot.begin(); it != m_pBossShot.end();)
	{
		(*it)->Update();	//’e‚ÌXVˆ—.

		if (!(*it)->IsActive())	//”ñƒAƒNƒeƒBƒu‚È’e‚Ííœ.
		{
			it = m_pBossShot.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void EnemyShotManager::Draw(const D3DXMATRIX& view, const D3DXMATRIX& proj)
{
	for (std::unique_ptr<EnemyShot>& ENomalShot : m_pBossShot)
	{
		ENomalShot->Draw();	//’e‚Ì•`‰æˆ—.

	}
}

void EnemyShotManager::Init()
{
	m_pBossShot.clear();
}

void EnemyShotManager::AddEnemyNomalShot(const D3DXVECTOR3& Pos, const D3DXVECTOR3& InitDirecton)
{
	constexpr float Speed = 1.0f;	//’e‚Ì‘¬“xİ’è.

	auto ENomalShot = std::make_unique<EnemyShot>();	//’e‚Ì¶¬.

	ENomalShot->Reload(Pos, InitDirecton, Speed);	//’e‚Ì‰Šú‰».

	m_pBossShot.push_back(std::move(ENomalShot));	//ƒŠƒXƒg‚É’Ç‰Á.
}

void EnemyShotManager::Clear()
{
	m_pBossShot.clear();
}

void EnemyShotManager::ReMoveEnemyNomalShot(size_t index)
{
	if (index < m_pBossShot.size())
	{
		m_pBossShot.erase(m_pBossShot.begin() + index);	//w’è’eíœ.
	}
}

const std::vector<std::unique_ptr<EnemyShot>>& EnemyShotManager::GetEnemyNomalShot() const
{
	return m_pBossShot;	//’eƒŠƒXƒg•Ô‹p.
}

EnemyShot* EnemyShotManager::GetEnemyNomalShot(size_t No)
{
	if (No < m_pBossShot.size())
	{
		return m_pBossShot[No].get();	//w’è’e•Ô‹p.
	}
	return nullptr;
}

size_t EnemyShotManager::GetEnemyNomalShotCount() const
{
	return m_pBossShot.size();	//’e”•Ô‹p.
}

std::vector<EnemyShot*> EnemyShotManager::GetShots()
{
	std::vector<EnemyShot*> rawPointers;

	for (const auto& shot : m_pBossShot)
	{
		if (shot)
		{
			rawPointers.push_back(shot.get());
		}
	}
	return rawPointers; // ’l‚Æ‚µ‚Ä•Ô‚·
}

