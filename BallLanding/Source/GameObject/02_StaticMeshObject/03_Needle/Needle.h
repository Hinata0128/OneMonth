#pragma once
#include "GameObject//02_StaticMeshObject//StaticMeshObject.h"

#include "Collision/BoundingSphere/BoundingSphere.h"

class StaticMeshManager;

/**************************************************************************************
*	針クラス.
*	ToDo : 針の表示・当たり判定を設定するクラス.
**/

class Needle final
	: public StaticMeshObject
{
public:
	Needle();
	~Needle() override;

	void Update() override;
	void Draw() override;

	//当たり判定取得用
	std::shared_ptr<BoundingSphere> GetBoundingSphere()
	{
		return m_pSphere;
	}

private:
	//当たり判定.
	std::shared_ptr<BoundingSphere> m_pSphere;
	D3DXVECTOR3 m_DegRota;
};