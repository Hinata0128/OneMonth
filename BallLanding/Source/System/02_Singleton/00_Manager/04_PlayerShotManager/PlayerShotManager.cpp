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
	//範囲for文にして弾の動作を管理していく.
	for (auto& PlayerShotList : m_upPlayerShotList)
	{
		//弾がNULLではないとき.
		if (PlayerShotList != nullptr)
		{
			PlayerShotList->Update();
		}
	}
	//弾の寿命が来たら消去(非表示)にする.
	for (auto it = m_upPlayerShotList.begin(); it != m_upPlayerShotList.end();)
	{
		//もし、その番号の弾の寿命がきていたら.
		if ((*it)->Active() == false)
		{
			it = m_upPlayerShotList.erase(it);
		}
		//まだ寿命じゃないとき.
		else
		{
			++it;
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
