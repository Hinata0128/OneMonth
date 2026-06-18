#pragma once
#include "System/00_Manager/01_StaticMeshManager/StaticMeshManager.h"

#include "System/00_Manager/02_ImGuiManager/ImGuiManager.h"


#include "System/06_Camera/Camera.h"

#include "Collision//BoundingSphere//BoundingSphere.h"

#include "..//Character.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include "System/02_Singleton/02_SingletonManager/09_AstroEvolution_PlayerShot_Manager/AstroEvolution_PlayerShot_Manager.h"

using json = nlohmann::json;

class AstroEvolution_Player final
	: public Character
{
public:
	AstroEvolution_Player();
	~AstroEvolution_Player() override;

	void Update() override;
	void Draw() override;

	void Init() override;

	bool GetDead() const override;

	//Json入出力用.
	void LoadData(const std::string& FilePath);
	void SaveData(const std::string& FilePath);

	//プレイヤーのステータスをもとに戻す.
	void ResetStatus();

	//カメラをPlayerにつける関数.
	void SetCamera_Player_Back(Camera* pCamera)
	{
		m_pCamera = pCamera;
	}

	float GetAngleY() const { return m_AngleY; }

	std::shared_ptr<BoundingSphere> GetBoundingSphere()
	{
		return m_pSphere;
	}

	int GetShotLevel() const { return m_ShotLevel; }
	float GetMoveSpeed() const { return m_MoveSpeed; } 
	float GetRotSpeed() const { return m_RotSpeed; }

	void UpgradeShotLevel() { m_ShotLevel++; }
	void UpgradeMoveSpeed() { m_MoveSpeed += 0.2f; } // 数値は適宜調整
	void UpgradeRotationSpeed() { m_RotSpeed += 0.02f; }
	
	//現在の体力を取得.
	float GetHP() const { return m_HP; }
	//最大体力取得.
	float GetMaxHP() const { return m_MaxHP; }

	void Hit(float damage);

private:
	std::shared_ptr<BoundingSphere> m_pSphere;

	float m_AngleY;
	float m_MoveSpeed;
	float m_RotSpeed;
	//強化パラメーター.
	int m_ShotLevel;
	int m_AttackPower;
	float m_MaxShield;
	float m_CurrentShield;
	
	Camera* m_pCamera;

	float m_ShotTimer = 0.0f;          // 連射制限用タイマー
	const float SHOT_INTERVAL = 0.15f; // 次の弾を撃てるまでの秒数

	//体力設定.
	//現在の体力.
	float m_HP;
	//最大体力.
	float m_MaxHP;

	float m_InvincibleTimer = 0.0f;     // 無敵残り時間
	const float INVINCIBLE_DURATION = 1.0f; // プレイヤーは1秒くらい無敵があると安心
};
