#pragma once

#include "SkinMesh//SkinMesh.h"
#include "System//02_Singleton//00_Manager//ManagerBase.h"

/*****************************************************************************
*	スキンメッシュマネージャークラス
*	最終的には、アセットクラスを作成してそこにスタティックとスキンメッシュを統合させる.
*	今はシングルトンにしておきます.
**/

class SkinMeshManager
	: public ManagerBase<SkinMeshManager>
{
public:
	//基底クラスの GetInstance()にだけ生成を許可する.
	friend class ManagerBase<SkinMeshManager>;
	// スキンモデルの種類の列挙型.
	enum class SkinList
	{
		Player,		// プレイヤー.
		Enemy,		// 仮の敵です.
		zako,

		Max
	};

public:
	SkinMeshManager();
	~SkinMeshManager() override;

	// ManagerBaseの関数をオーバーライド
	void Create() override;
	void Release() override;

	SkinMesh* GetSkinMeshInstance(SkinList skinNo);

private:
	std::unique_ptr<SkinMesh> m_pSkin[static_cast<int>(SkinList::Max)];
};
