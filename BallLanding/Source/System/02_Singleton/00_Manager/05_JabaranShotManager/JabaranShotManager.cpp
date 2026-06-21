#include "JabaranShotManager.h"
#include "..//..//..//..//GameObject//02_StaticMeshObject//ShotBase//JabaranShot//JabaranShot.h"

//コンストラクタ.
JabaranShotManager::JabaranShotManager()
{
	//ここでリストをすべてなくす.
	m_upJabaranShotList.clear();
}

//デストラクタ.
JabaranShotManager::~JabaranShotManager()
{
}

//構築関数.
void JabaranShotManager::Create()
{
}

//動作関数.
void JabaranShotManager::Update()
{
	//イテレータを使用して先に寿命が切れている・当たって志望した弾を削除する.
	for (auto it = m_upJabaranShotList.begin(); it != m_upJabaranShotList.end();)
	{
		if ((*it) == nullptr || (*it)->IsActive() == false)
		{
			it = m_upJabaranShotList.erase(it);
		}
		else
		{
			++it;
		}
	}

	//生き残ている弾だけを移動「Update」させる.
	for (auto& JabaranShptList : m_upJabaranShotList)
	{
		if (JabaranShptList != nullptr)
		{
			JabaranShptList->Update();
		}
	}
}

//描画関数.
void JabaranShotManager::Draw()
{
	//範囲for文にして弾の描画を管理していく.
	for (auto& JabaranShotList : m_upJabaranShotList)
	{
		//弾の表示のNULLチェック.
		if (JabaranShotList != nullptr)
		{
			//敵の弾のメッシュを表示する.
			JabaranShotList->Draw();
		}
	}
}

//解放関数.
void JabaranShotManager::Release()
{
}

//弾の制御関数.
void JabaranShotManager::Launch(const D3DXVECTOR3& Pos, const D3DXVECTOR3& Vel, float Radius, float Life)
{
	//ここでJabaranShotクラスのインスタンス生成をする.
	//この関数が呼ばれる時に敵の攻撃の寿命とかが入る.
	auto JabaranShotList = std::make_unique<JabaranShot>();
	//JabaranShotクラスで作成したLaunch関数を呼ぶ.
	JabaranShotList->Launch(Pos, Vel, Radius, Life);
	//弾の複製をする.
	m_upJabaranShotList.push_back(std::move(JabaranShotList));
}

//敵に当たった時に弾の当たり判定を消す関数.
void JabaranShotManager::KillShotByCollider(const std::shared_ptr<BoundingSphere>& sphere)
{
	for (auto& JabaranShotList : m_upJabaranShotList)
	{
		if (JabaranShotList && JabaranShotList->GetCollider() == sphere)
		{
			//弾を死亡状態にする.
			JabaranShotList->ShotKill();
			break;
		}
	}
}
