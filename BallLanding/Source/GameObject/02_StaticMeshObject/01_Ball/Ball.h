#pragma once
#include "GameObject//02_StaticMeshObject//StaticMeshObject.h"

#include "Collision/BoundingSphere/BoundingSphere.h"

class StaticMeshManager;

/*********************************************************
*	ボールクラス
**/

class Ball final
	: public StaticMeshObject
{
public:
	Ball();
	~Ball();

	void Update() override;
	void Draw() override;

	//当たり判定取得用
	std::shared_ptr<BoundingSphere> GetBoundingSphere() 
	{ 
		return m_pSphere; 
	}

	bool IsDead() const { return m_pSphere ? m_pSphere->IsDead() : false; }
private:
	//玉の現在の速度.
	D3DXVECTOR3 m_Velocity;
	//重力の強さ.
	float m_Gravity;

	//当たり判定.
	std::shared_ptr<BoundingSphere> m_pSphere;
};