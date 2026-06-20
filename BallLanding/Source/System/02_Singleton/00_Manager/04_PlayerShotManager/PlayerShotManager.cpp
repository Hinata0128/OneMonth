#include "PlayerShotManager.h"
#include "GameObject\02_StaticMeshObject\ShotBase\PlayerShot\PlayerShot.h"

//コンストラクタ.
PlayerShotManager::PlayerShotManager()
{
	m_upPlayerShotList.clear();
}

//デストラクタ.
PlayerShotManager::~PlayerShotManager()
{
}

//構築関数.
void PlayerShotManager::Create()
{
}

//動作関数.
void PlayerShotManager::Update()
{
	//先に寿命が切れている・当たって死亡した弾を削除する.
	for (auto it = m_upPlayerShotList.begin(); it != m_upPlayerShotList.end();)
	{
		//新しく作成したIsActive()でチェックする.
		if ((*it) == nullptr || (*it)->IsActive() == false)
		{
			it = m_upPlayerShotList.erase(it);
		}
		else
		{
			++it;
		}
	}

	//生き残っている弾だけを移動「Update」させる.
	for (auto& PlayerShotList : m_upPlayerShotList)
	{
		if (PlayerShotList != nullptr)
		{
			PlayerShotList->Update();
		}
	}
}

//描画関数.
void PlayerShotManager::Draw()
{
	//範囲for文にして弾の描画を管理していく.
	for (auto& PlayerShotList : m_upPlayerShotList)
	{
		//弾の表示のNULLチェック.
		if (PlayerShotList != nullptr)
		{
			//メッシュ表示させる.
			PlayerShotList->Draw();
		}
	}
}

//解放関数.
void PlayerShotManager::Release()
{
}

//弾の制御関数.
void PlayerShotManager::Launch(const D3DXVECTOR3& Pos, const D3DXVECTOR3& Vel, float Radius, float Life)
{
	//ここでPlayerShotクラスをインスタンス生成する.
	auto PlayerShotList = std::make_unique<PlayerShot>();

	//PlayerShotクラスで作成したLaunchクラスを呼ぶ.
	PlayerShotList->Launch(Pos, Vel, Radius, Life);

	//弾を複製する.
	m_upPlayerShotList.push_back(std::move(PlayerShotList));
}

void PlayerShotManager::KillShotByCollider(const std::shared_ptr<BoundingSphere>& sphere)
{
	for (auto& shot : m_upPlayerShotList)
	{
		if (shot && shot->GetCollider() == sphere)
		{
			shot->ShotKill(); //弾を死亡状態にする.
			break;
		}
	}
}
