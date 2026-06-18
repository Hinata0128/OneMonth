#pragma once
#include "GameObject//02_StaticMeshObject//06_Character//Character.h"
#include "Collision/BoundingSphere/BoundingSphere.h"

class StaticMeshManager;
class PShotManager;
class Camera;
class Enemy;

class Player final
	: public Character
{
public:
	Player();
	~Player() override;

	void Update() override;
	void Draw() override;

	void Init() override;

	bool GetDead() const override;

	//回転．
	float GetAngleY() const { return m_angleY; }
	void SetAngleY(float angle) { m_angleY = angle; }

	float GetPitch() const { return m_pitch; }

	// --- forward取得（飛行機用） ---
	D3DXVECTOR3 GetForward() const;

	//当たり判定取得用
	std::shared_ptr<BoundingSphere> GetBoundingSphere()
	{
		return m_pSphere;
	}

	void OnHit(int damage);

	void SetCamera(Camera* camera);
	Enemy* FindAimTarget(float cosThreshold);

	D3DXVECTOR3 GetStraightForward() const;



private:
	float m_angleY;
	float m_tiltZ;
	std::shared_ptr<BoundingSphere> m_pSphere;
	Camera* m_pCamera;

	bool m_Dead;

	//プレイヤーの弾の発射間隔.
	//発射後の経過時間.
	float m_ShotTimer;
	//発射感覚.
	float m_ShotInteval;


	// --- 回転 ---
	float m_pitch;    // ピッチ
	float m_roll;

};