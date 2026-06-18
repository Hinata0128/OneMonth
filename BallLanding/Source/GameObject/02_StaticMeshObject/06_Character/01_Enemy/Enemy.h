#pragma once
#include "GameObject//02_StaticMeshObject//06_Character//Character.h"
#include "Collision/BoundingSphere/BoundingSphere.h"

#include "GameObject/03_UIObject/02_EnemyHealthBar/EnemyHealthBar.h"

class StaticMeshManager;
class Player;

/*********************************************************************************************
*	Enemyクラス.
**/

class Enemy final
	: public Character
{
public:
	//敵に動きを追加させる.
	//行動は一体一体ランダムで動作をさせる.
	enum class MoveType : byte
	{
		None,		//静止.
		Horizontal,	//左右.
		Vertical,	//上下.
		Random,		//自由移動.
		
		Max,		//ランダムの範囲指定.
	};
public:
	//コンストラクタ.
	Enemy();
	//デストラクタ.
	~Enemy() override;
	//動作関数.
	void Update() override;
	//敵の移動をまとめている動作関数.
	void UpdateMovement(float deltaTime);
	//描画関数.
	void Draw() override;
	//初期化関数.
	void Init() override;
	//死亡しているかのGet関数.
	bool GetDead() const override;
	//敵から発射される弾の関数.
	void AutoShot();
	
	BoundingSphere& GetBoundingSphere();
	//当たっているかの関数.
	void OnHit(int damege);
	//敵が死んでいるか.
	bool IsDead() const;

	// 画面内に映っているか判定する関数
	bool IsVisibleOnScreen() const;
public:
	void SetTargetPos(const D3DXVECTOR3& pos)
	{
		m_pPlayerPos = pos;
	}
	const D3DXVECTOR3& GetPlayerPos() const { return m_pPlayerPos; }

	void SetPlayer(Player* p) { m_pPlayer = p; }
	BoundingSphere* GetSphere() const { return m_BSphere.get(); }

	//現在の体力を取得.
	int GetHP() const { return m_HP; }
	//最大体力取得.
	int GetMaxHP() const { return m_MaxHP; }
private:

	std::unique_ptr<EnemyHealthBar> m_upHPBar;

	D3DXVECTOR3 m_pPlayerPos;

	Player* m_pPlayer;

	//残りのクールタイム 
	float m_ShotCoolDown;
	//次に打つまで.
	float m_CoolTime;

	std::unique_ptr<BoundingSphere> m_BSphere;
	//死亡判定.
	bool m_IsDead;

	//体力設定.
	//現在の体力.
	int m_HP;
	//最大体力.
	int m_MaxHP;
	//移動タイプ.
	MoveType m_MoveType;
	float m_MoveSpeed;
	D3DXVECTOR3 m_BasePos;  // 生成時の中心位置
	float m_MoveAngle;      // サイン波計算用の角度
	float m_MoveRange;      // 移動する幅
};
