#pragma once
#include "GameObject/02_StaticMeshObject/StaticMeshObject.h"
#include "GameObject//02_StaticMeshObject//05_ShotBase//ShotBase.h"

#include "GameObject/02_StaticMeshObject/06_Character/01_Enemy/Enemy.h"

class StaticMeshManager;

/******************************************************************
*	ショットクラス.
**/

class PlayerShot final
	: public ShotBase
{
public:
	PlayerShot();
	~PlayerShot() override;

	void Update() override;
	void Draw() override;

	//弾の初期化.
	void Init();
	void Reload(const D3DXVECTOR3& pos, const D3DXVECTOR3& direction, float speed)  override {};

	// ShotBase で純粋仮想にした Reload を実装
	void Reload(
		const D3DXVECTOR3& pos,
		Enemy* target,
		float speed,
		const D3DXVECTOR3& forwardDir);

	BoundingSphere* GetSphere() const;

	void OnHit();
private:
	std::unique_ptr<BoundingSphere> m_BSphere;

	Enemy* m_Target;
	float m_HomingPower;   // 追尾の強さ

};