#pragma once
#include "System/00_Manager/01_StaticMeshManager/StaticMeshManager.h"

#include "System/00_Manager/02_ImGuiManager/ImGuiManager.h"
#include "Collision/BoundingSphere/BoundingSphere.h"

#include "..//02_AstroEvolution_Player/AstroEvolution_Player.h"

#include "..//Character.h"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/************************************************************
*	AstroEvolutionのボスクラス
*	登場時にイベントシーンを実装したりしていく.
**/

class AstroEvoultion_Boss final
	: public Character
{
public:
	AstroEvoultion_Boss();
	~AstroEvoultion_Boss() override;

	void Update() override;
	void Draw() override;

	void Init() override;

	bool GetDead() const override;

	//Json入出力.
	void AstroEvolution_BossParamLoadData(const std::string& FilePath);
	void AstroEvolution_BossParamSaveData(const std::string& FilePath);

	std::shared_ptr<BoundingSphere> GetBoundingSphere()
	{
		return m_pSphere;
	}

	void SetTargetPlayer(AstroEvolution_Player* pPlayer) {
		m_pTargetPlayer = pPlayer;
	}

	//現在の体力を取得.
	float GetHP() const { return m_HP; }
	//最大体力取得.
	float GetMaxHP() const { return m_MaxHP; }

	void Hit(float damage)
	{
		if (m_InvincibleTimer > 0.0f) return;

		m_HP -= damage;

		m_InvincibleTimer = INVINCIBLE_DURATION;

		if (m_HP <= 0)
		{
			m_HP = 0;
			m_Dead = true;
		}
	}


private:
	std::shared_ptr<BoundingSphere> m_pSphere;
	AstroEvolution_Player* m_pTargetPlayer = nullptr;
private:
	enum class BossState {
		Wait,    // 待機
		Chase,   // 追跡（今の動き）
		Charge,  // 突進（急加速）
		Barrage,
		Cooldown // 攻撃後の硬直
	};

	BossState m_State;
	float m_StateTimer = 0.0f;
	D3DXVECTOR3 m_DashDir; // 突進する方向を記憶

	void ShootBarrage();

	//体力設定.
//現在の体力.
	float m_HP;
	//最大体力.
	float m_MaxHP;

	float m_InvincibleTimer = 0.0f; // 🌟追加：無敵残り時間
	const float INVINCIBLE_DURATION = 0.1f; // 🌟追加：無敵時間（秒）

private:
	bool m_IsActive = false;

public:
	void SetActive(bool active) { m_IsActive = active; }
	bool IsActive() const { return m_IsActive; }
};