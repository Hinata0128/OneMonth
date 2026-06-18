#pragma once
#include "GameObject//02_StaticMeshObject//StaticMeshObject.h"

#include "Collision//BoundingSphere//BoundingSphere.h"

class StaticMeshManager;

/*****************************************************************************
*	ボールを入れる用のメッシュクラス.
*	ToDo : 左右移動をさせてボールが落ちてくるところに、動かす.
**/

class PointCan final
	: public StaticMeshObject
{
public:
	PointCan();
	~PointCan() override;

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
};
