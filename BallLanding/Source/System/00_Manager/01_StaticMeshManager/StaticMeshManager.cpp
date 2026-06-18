#include "StaticMeshManager.h"
//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning(disable:4005)

StaticMeshManager::StaticMeshManager()
{
}

StaticMeshManager::~StaticMeshManager()
{
	Release();
}

void StaticMeshManager::Create()
{
	//スタティックメッシュ配列を初期化.
	for (int i = 0; i < static_cast<int>(CMeshList::Max); ++i)
	{
		m_pMesh[i] = std::make_unique<StaticMesh>();
	}

	//読み込みリスト構造体の定義.
	struct MeshList
	{
		int listNo;//CMeshList列挙型を設定.
		const TCHAR path[256];//ファイルパス.
	};

	//読み込み対象リストの設定.
	MeshList MList[] =
	{
		{ static_cast<int>(CMeshList::Ball),			_T("Data\\Mesh\\Static\\00_Bullet\\Ball.x") },
		{ static_cast<int>(CMeshList::Ground),			_T("Data\\Mesh\\Static\\04_Ground\\ground.x") },
		{ static_cast<int>(CMeshList::SphereCollision),	_T("Data\\Mesh\\Static\\02_Collision\\Sphere.x") },
		{ static_cast<int>(CMeshList::EnemyBullet),		_T("Data\\Mesh\\Static\\00_Bullet\\Ball.x") },
		{ static_cast<int>(CMeshList::BoxCollision),	_T("Data\\Mesh\\Static\\02_Collision\\Box.x")},
		{ static_cast<int>(CMeshList::Portal),			_T("Data\\Mesh\\Static\\07_Portal\\Portal.x")},
		{ static_cast<int>(CMeshList::SkyBox),			_T("Data\\Mesh\\Static\\09_SkyBox\\1.x")},
		{ static_cast<int>(CMeshList::PortalFrame),		_T("Data\\Mesh\\Static\\08_PortalFrame\\PortalFrame.x")},
		{ static_cast<int>(CMeshList::Wall),			_T("Data\\Mesh\\Static\\10_Wall\\Wall.x")},
		{ static_cast<int>(CMeshList::Needle),			_T("Data\\Mesh\\Static\\05_Needle\\Needle.x")},
		{ static_cast<int>(CMeshList::PointCan),		_T("Data\\Mesh\\Static\\06_PointCan\\PointCan.x")},
		{ static_cast<int>(CMeshList::Enemy),			_T("Data\\Mesh\\Static\\01_Character\\Enemy\\Enemy.x")},
		{ static_cast<int>(CMeshList::Fighter),			_T("Data\\Mesh\\Static\\03_Fighter\\Fighter.x")},
		{ static_cast<int>(CMeshList::Ring),			_T("Data\\Mesh\\Static\\11_Ring\\Ring.x")},
		{ static_cast<int>(CMeshList::SkyDome),			_T("Data\\Mesh\\Static\\12_SkyDome\\2.x")},
		{ static_cast<int>(CMeshList::SkyDome_No3),		_T("Data\\Mesh\\Static\\12_SkyDome\\3.x")},
		{ static_cast<int>(CMeshList::Rock_OneHundred),	_T("Data\\Mesh\\Static\\13_Rock\\Rock_OneHundred.x")},
		{ static_cast<int>(CMeshList::Rock_Fifty),		_T("Data\\Mesh\\Static\\13_Rock\\Rock_Fifty.x")},
		{ static_cast<int>(CMeshList::Rock_Ten),		_T("Data\\Mesh\\Static\\13_Rock\\Rock_Ten.x")},
		{ static_cast<int>(CMeshList::SkyDome_No4),		_T("Data\\Mesh\\Static\\12_SkyDome\\4.x")},

	};
		//配列の要素数を取得.
	const int listMax = static_cast<int>(std::size(MList));

	//読み込み処理.
	for (int i = 0; i < listMax; ++i)
	{
		int idx = MList[i].listNo;
		if (FAILED(m_pMesh[idx]->Init(MList[i].path)))
		{
			//読み込み失敗時は安全に終了.
			return;
		}
	}
}

void StaticMeshManager::Release()
{
	//unique_ptrなので自動で解放されるが、明示的にリセットしておく.
	for (int i = 0; i < static_cast<int>(CMeshList::Max); ++i)
	{
		m_pMesh[i].reset();
	}
}

void StaticMeshManager::Init()
{
}
