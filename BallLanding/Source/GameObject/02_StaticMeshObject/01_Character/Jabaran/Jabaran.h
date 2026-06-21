#pragma once
#include "GameObject//02_StaticMeshObject//01_Character//Character.h"
#include "..//..//..//..//Collision/BoundingSphere/BoundingSphere.h"

class StaticMeshManager;
class CollisionManager;
class JabaranShotManager;

class Jabaran final
	: public Character
{
public:
	Jabaran();
	~Jabaran() override;
	//動作関数.
	void Update() override;
	//描画関数.
	void Draw() override;
	//初期化関数.
	void Init() override;
	//死亡しているかのGet関数.
	bool GetDead() const override;
private:
	std::shared_ptr<BoundingSphere> m_pCollider;
};
