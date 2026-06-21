#pragma once
#include "GameObject//02_StaticMeshObject//01_Character//Character.h"
#include "..//..//..//..//Collision/BoundingSphere/BoundingSphere.h"

class StaticMeshManager;
class Camera;
class PlayerShotManager;
class CollisionManager;


class Player
	: public Character
{
public:
	Player();
	~Player() override;
	//動作関数.
	void Update() override;
	//描画関数.
	void Draw() override;
	//初期化関数.
	void Init() override;
	//死亡しているかのGet関数.
	bool GetDead() const override;

	//WASDの移動の関数.
	void HandleMovement();

	//カメラをPlayerにつける関数.
	void SetCamera_Player_Back(Camera* pCamera)
	{
		m_pCamera = pCamera;
	}	
	//カメラにつける用の関数.
	float GetAngleY() const { return m_AngleY; }
	
	//プレイヤーの位置を敵に教えるための関数.
	const D3DXVECTOR3 GetPlayerPos() const { return m_Position; }



private:
	Camera* m_pCamera;
	float   m_MoveSpeed;   //移動速度.
	float   m_RotSpeed;    //旋回速度.
	float	m_AngleY;

	std::shared_ptr<BoundingSphere> m_pCollider;

};
