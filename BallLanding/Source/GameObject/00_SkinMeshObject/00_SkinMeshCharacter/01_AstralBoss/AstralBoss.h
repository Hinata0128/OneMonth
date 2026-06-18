#pragma once
#include "GameObject/00_SkinMeshObject/00_SkinMeshCharacter/SkinMeshCharacter.h"

#include "Effect/Effect.h"

class SkinMeshManager;
class Camera;

class AstralBoss final
	: public SkinMeshCharacter
{
public:
	AstralBoss();
	~AstralBoss() override;

	void Update() override;
	void Draw() override;

	void Init() override;

	bool GetDead() const override;

	//カメラをPlayerにつける関数.
	void SetCamera_Player_Back(Camera* pCamera)
	{
		m_pCamera = pCamera;
	}

	float GetAngleY() const { return m_AngleY; }

	float m_AngleY;

	std::shared_ptr<BoundingSphere> GetSphereBody() const { return m_pSphereBody; }
	std::shared_ptr<BoundingSphere> GetSphereHead() const { return m_pSphereHead; }
	void UpdateCollisionPosition();

	void OnHit(float damage) {
		// ここにHPを減らす処理や被弾アニメーションの再生を書く
		if (m_InvincibleTimer > 0.0f) return;

		 m_HP -= damage;
		 m_InvincibleTimer = INVINCIBLE_DURATION;

		 D3DXVECTOR3 effectPos = GetPosition();
		 effectPos.y += 2.0f;

		 Effect::GetInstance()->Play(Effect::Laser01, effectPos);

		 if (m_HP <= 0.0f)
		 {
			 m_HP = 0.0f;
			 m_Dead = true;
		 }
	}

	// 【追加】すべての判定用スフィアをまとめて返す
	std::vector<std::shared_ptr<BoundingSphere>> GetSpheres() const {
		return { m_pSphereBody, m_pSphereHead , m_pSphereBody_2, m_pSphereBody_3 };
	}
private:
	Camera* m_pCamera;
	float   m_MoveSpeed;   // 移動速度
	float   m_RotSpeed;    // 旋回速度

	std::shared_ptr<BoundingSphere> m_pSphereBody; // 胴体
	std::shared_ptr<BoundingSphere> m_pSphereHead; // 頭部・背中
	std::shared_ptr<BoundingSphere> m_pSphereBody_2; // 頭部・背中
	std::shared_ptr<BoundingSphere> m_pSphereBody_3; // 頭部・背中

	float m_HP = 100.0f;
	float m_InvincibleTimer = 0.0f; 
	const float INVINCIBLE_DURATION = 1.0f; 

};