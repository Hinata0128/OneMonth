#pragma once
#include "GameObject//02_StaticMeshObject//05_ShotBase//ShotBase.h"

class StaticMeshManager;

/**********************************************************************
*	“G’Êí’eƒNƒ‰ƒX.
**/

class EnemyShot final
	: public ShotBase
{
public:
	EnemyShot();
	~EnemyShot() override;

	void Update() override;
	void Draw() override;

	//’e‚Ì‰Šú‰».
	void Init() override;

	void Reload(const D3DXVECTOR3& pos, const D3DXVECTOR3& direction, float speed) override;

	BoundingSphere* GetSphere() const
	{
		return m_pSphere.get();
	}
private:
	std::shared_ptr<BoundingSphere> m_pSphere;
};