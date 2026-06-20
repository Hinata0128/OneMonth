#pragma once
#include "StaticMash//StaticMesh.h"
#include "System//02_Singleton//00_Manager//ManagerBase.h"

/**********************************************
*	スタティックメッシュマネージャークラス.
**********************************************/
class StaticMeshManager
	: public ManagerBase<StaticMeshManager> 
{
public:
	//基底クラスの GetInstance()にだけ生成を許可する.
	friend class ManagerBase<StaticMeshManager>;
	//モデルの種類の列挙型.
	enum class CMeshList : byte
	{
		Ground,				//地面.
		SphereCollision,	//球の当たり判定.
		BoxCollision,		//ボックスの当たり判定.

		Fighter,			//プレイヤーのメッシュ.
		Jabaran,			//敵のメッシュ.
		PlayerShot,			//プレイヤーの弾.

		Max,
	};

public:
	StaticMeshManager();
	~StaticMeshManager() override;

	void Create() override;
	void Release() override;
	void Init();

	//スタティックメッシュのインスタンス取得.
	StaticMesh* GetMeshInstance(CMeshList MeshNo)
	{
		return m_pMesh[static_cast<int>(MeshNo)].get();
	};

private:
	std::unique_ptr<StaticMesh> m_pMesh[static_cast<int>(CMeshList::Max)];
};
