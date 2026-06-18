#pragma once
#include "GameObject/00_SkinMeshObject/00_SkinMeshCharacter/SkinMeshCharacter.h"

#include <fstream>
#include <iomanip>

#include <nlohmann/json.hpp>

class SkinMeshManager;
class Camera;

class AstralPlayer final
	: public SkinMeshCharacter
{
public:
	//コンストラクタ.
	AstralPlayer();
	//デストラクタ.
	~AstralPlayer() override;
	//動作関数.
	void Update() override;
	//描画関数.
	void Draw() override;
	//初期化関数.
	void Init() override;
	//死んだかどうか.
	bool GetDead() const override;

	//カメラをPlayerにつける関数.
	void SetCamera_Player_Back(Camera* pCamera)
	{
		m_pCamera = pCamera;
	}
	//カメラにつける用の関数.
	float GetAngleY() const { return m_AngleY; }

	float m_AngleY;

	//当たり判定の位置を体と頭につける.
	std::shared_ptr<BoundingSphere> GetSphereBody() const { return m_pSphereBody; }
	std::shared_ptr<BoundingSphere> GetSphereHead() const { return m_pSphereHead; }
	//当たり判定のポジション設定.
	void UpdateCollisionPosition();

	bool IsAttacking() const {
		return m_ShortAttackList == enShort::AttackPreparation ||
			m_ShortAttackList == enShort::Attack;
	}
	std::shared_ptr<BoundingSphere> GetAttackSphere() const { return m_pSphereShort; }

	std::vector<std::shared_ptr<BoundingSphere>> GetSpheres() const {
		return { m_pSphereBody, m_pSphereHead };
	}

	void SetHasHit(bool hit) { m_HasHit = hit; }
	bool GetHasHit() const { return m_HasHit; }

	//Jsonのファイルを読み込み.
	void AstralPlayer_ParamLoadData(const std::string& FilePath);
	//Jsonのファイルを保存.
	void AstralPlayer_ParamSaveData(const std::string& FilePath);

private:
	//Playerのアニメーションの列挙.
	enum class enPlayerAnim : byte
	{
		Idol = 0,			//待機.
		Moving,				//移動への入り.
		Move,				//移動中.
		MoveToIdol,			//移動中から待機へ.

		Mid,				//中距離攻撃.

		Blade_1,			//近距離1段階.
		BladeToIdol,		//近距離1段階から待機へ.
		Blade_1_2_3,		//近距離1-2-3.

		Hacking,			//ハッキングポーズ.

		Hit,				//被弾.
		HitToIdol,			//被弾から待機.
		HitDownToIdol,		//被弾-ダウン-待機.

		ex_ba1,				//バックアタック.
		ex_ba1success,		//プレイヤーのqte失敗.
		ex_ba1failure,		//プレイヤーのqte成功.
		ex_cc1,				//最初のカウンター.
		ex_cc1re,			//プレイヤーが15番につなぐ(待機).
		ex_cc2,				//カウンター成功.
		ex_cc2re,			//敵に対してカウンターできた場合.

		ex_rejectio1,		//粒子法スタンバイ.
		ex_rejectio2stop,	//発射前のポーズで停止.
		ex_rejectio3shoot,	//発射モーション.
		ex_rejectio4finish,	//20のアニメーションからつなぐ.
		ex_rejectio5,		//粒子法回避.
		ex_rejectio6rushin,	//敵のラッシュ攻撃の初段を受けた時の被弾モーション.
		ex_rejectio7roop1,	//敵のラッシュ攻撃の次段を受けた時の被弾モーション.
		ex_rejectio7roop2,	//敵のラッシュ攻撃の次を受けた時のモーション,25,26のモーション.
		ex_rejectio8finish,	//roop1からつながる.

		a,					//武器構えない.
		b,					//武器構える.
		c					//最終待機入る.
	};
private:
	//Playerの近距離攻撃の列挙.
	enum class enShort : byte
	{
		//何もしない.
		None = 0,
		//ボタンが押されたとき.
		//アニメーションの準備.
		AttackPreparation,
		//攻撃中.
		Attack,
		//攻撃終了・待機アニメーションへ戻る.
		AttackEndIdol
	};

	//Playerの回避の構造体.
	struct BlinkParam
	{
		//ブリンクの速度.
		float BlinkSpeed = 1.0f;
		//ブリンクのクールタイム.
		float BlinkCool = 1.0f;
		//ブリンクの持続時間.
		float duration = 0.3f;
	} m_BlinkParam;

	//回避の状態用.
	//残り持続時間.
	float BlinkTimer;
	//残りクールタイム.
	float BlinkCoolDown;
	//現在ブリンク中か.
	bool m_IsBlinking;

private:
	//アニメーションの名前リスト.
	enPlayerAnim m_AnimList;
	//Playerの攻撃.
	enShort m_ShortAttackList;
	//Playerの攻撃用の関数.
	void PlayerShortAttack();

	//WASDの移動の関数.
	void HandleMovement();

	Camera* m_pCamera;
	float   m_MoveSpeed;   //移動速度.
	float   m_RotSpeed;    //旋回速度.

	std::shared_ptr<BoundingSphere> m_pSphereBody;	//胴体.
	std::shared_ptr<BoundingSphere> m_pSphereHead;	//頭部.
	std::shared_ptr<BoundingSphere> m_pSphereShort;	//近距離攻撃.

	bool m_HasHit = false;

};