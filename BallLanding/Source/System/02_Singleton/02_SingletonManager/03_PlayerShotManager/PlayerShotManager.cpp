#include "PlayerShotManager.h"
#include "GameObject/02_StaticMeshObject/06_Character/00_Player/Player.h"

#include "Sound/SoundManager.h"

PlayerShotManager::PlayerShotManager()
{
}

PlayerShotManager::~PlayerShotManager()
{
}

PlayerShotManager* PlayerShotManager::GetInstance()
{
	static PlayerShotManager s_Instance;
	return &s_Instance;
}

void PlayerShotManager::Update()
{
	for (auto it = m_PlayerShot.begin(); it != m_PlayerShot.end();)
	{
		(*it)->Update();	

		if (!(*it)->IsActive())	
		{
			it = m_PlayerShot.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void PlayerShotManager::Draw(const D3DXMATRIX& view, const D3DXMATRIX& proj)
{
	for (auto& shot : m_PlayerShot)
	{
		shot->Draw();	

#ifdef _DEBUG
		// ƒfƒoƒbƒOŽž‚Ì‚ÝA’e‚Ì“–‚½‚è”»’è‚ð•\Ž¦
		if (shot->IsActive() && shot->GetSphere())
		{
			// 1. CollisionManager‚É“o˜^‚µ‚ÄˆêŠ‡•`‰æ‚³‚¹‚éê‡
			// CollisionManager::GetInstance()->AddSphere(shot->GetSphere());

			// 2. ‚à‚µ BoundingSphere Ž©‘Ì‚É Draw ‚ª‚ ‚é‚È‚ç’¼ÚŒÄ‚Ô
			shot->GetSphere()->Draw();
		}
#endif
	}
}

void PlayerShotManager::Init()
{
	m_PlayerShot.clear();
}

void PlayerShotManager::AddPlayerShot(const D3DXVECTOR3& Pos, Enemy* target, const D3DXVECTOR3& forwardDir)
{
	constexpr float Speed = 1.0f;	//’e‘¬.

	auto r_playerShot = std::make_unique<PlayerShot>();	//V‹K’e¶¬.
	r_playerShot->Reload(Pos, target, Speed, forwardDir);	//ˆÊ’uE•ûŒüE‘¬“x‚ðÝ’è.
	r_playerShot->SetDisplay(true);	//•`‰æON.


	m_PlayerShot.push_back(std::move(r_playerShot));	//ƒŠƒXƒg‚É’Ç‰Á.

	SoundManager::GetInstance()->PlaySE(SoundManager::SE_Shot);
}

void PlayerShotManager::ReMovePlayerShot(size_t index)
{
	if (index < m_PlayerShot.size())
	{
		m_PlayerShot.erase(m_PlayerShot.begin() + index);	//Žw’è’e‚ðíœ.
	}
}

void PlayerShotManager::ClearPlayerShot()
{
	m_PlayerShot.clear();
}

const std::vector<std::unique_ptr<PlayerShot>>& PlayerShotManager::GetPlayerShot()const
{
	return m_PlayerShot;	//’eƒŠƒXƒg‚ð•Ô‚·.
}

PlayerShot* PlayerShotManager::GetPlayerShot(size_t No)
{
	if (No < m_PlayerShot.size())
	{
		return m_PlayerShot[No].get();	//Žw’è’e‚ðŽæ“¾.
	}
	return nullptr;	//”ÍˆÍŠO.
}

size_t PlayerShotManager::GetPlayerShotCount()const
{
	return m_PlayerShot.size();	//’e‚Ì‘”‚ð•Ô‚·.
}

std::vector<PlayerShot*> PlayerShotManager::GetShots()
{
	std::vector<PlayerShot*> rawPointers;
	for (const auto& shot : m_PlayerShot)
	{
		if (shot)
		{
			rawPointers.push_back(shot.get());
		}
	}
	return rawPointers;
}

void PlayerShotManager::AddHomingShot(
	const D3DXVECTOR3& pos,
	Enemy* target,
	const D3DXVECTOR3& forwardDir)
{
	auto shot = std::make_unique<PlayerShot>();

	float speed = 0.5f;

	shot->Reload(pos, target, speed, forwardDir);

	m_PlayerShot.emplace_back(std::move(shot));
}