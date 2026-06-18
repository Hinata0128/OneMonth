#pragma once

#include "..//UIObject.h"

//前方宣言.
class Sprite2D;
class Enemy;

class EnemyHealthBar final
	: public UIObject
{
public:
	//コンストラクタ.
	EnemyHealthBar();
	//デストラクタ.
	~EnemyHealthBar() override;
	//動作関数.
	void Update() override;
	//描画関数.
	void Draw() override;
	//構築関数.
	void Create() override;

	//どの敵のHPを参照するか.
	void SetTarget(Enemy* pEnemy);

	//3D座標を2Dスクリーン座標に変換する.
	D3DXVECTOR3 WorldToScreen(const D3DXVECTOR3 WorldPos);
private:
	//表示対象の敵.
	Enemy* m_pTargetEnemy;
	//バーの最大スケール.
	float m_MaxBarScale;

	//各パーツ用のUIオブジェクト.
	std::shared_ptr<UIObject> m_upBase;    //枠.
	std::shared_ptr<UIObject> m_upBack;    //背景.
	std::shared_ptr<UIObject> m_upDamage;  //ダメージ(赤).
	std::shared_ptr<UIObject> m_upGauge;   //現在HP(緑).
	
	//各パーツ用のSprite.
	std::shared_ptr<Sprite2D> m_spBaseSprite;
	std::shared_ptr<Sprite2D> m_spBackSprite;
	std::shared_ptr<Sprite2D> m_spDamageSprite;
	std::shared_ptr<Sprite2D> m_spGaugeSprite;
};
