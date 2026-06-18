#pragma once
#include "GameObject/02_StaticMeshObject/StaticMeshObject.h"
#include "Collision/BoundingSphere/BoundingSphere.h"

class StaticMeshManager;
class Player;

class Ring final
	: public StaticMeshObject
{
public:
	Ring();
	~Ring() override;

	void Update() override;
	void Draw() override;
	// ランダムな位置に再配置する関数
	void TeleportRandom();

	// 当たり判定取得
	std::shared_ptr<BoundingSphere> GetBoundingSphere() { return m_pSphere; }

private:
	std::shared_ptr<BoundingSphere> m_pSphere;
};