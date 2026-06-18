#pragma once

#include "..//UIObject.h"
#include "GameObject/02_StaticMeshObject/06_Character/04_AstroEvoultion_Boss/AstroEvoultion_Boss.h"

class Sprite2D;

/************************************************************
*	ボスのHPBar表示クラス.
**/

class AstroBoss_HPBar final
	: public UIObject
{
public:
	AstroBoss_HPBar();
	~AstroBoss_HPBar() override;

	void Update() override;
	void Draw() override;
	void Create() override;

	//参照するボスをセットする関数.
	void SetTargetBoss(AstroEvoultion_Boss* AstroBoss)
	{
		m_pTargetBoss = AstroBoss;
	}

	AstroEvoultion_Boss* m_pTargetBoss;
	float m_maxBarScaleX;

private:
	//各パーツ用のSprite
	std::shared_ptr<Sprite2D> m_spBaseSprite;   // HPBase (枠)
	std::shared_ptr<Sprite2D> m_spBackSprite;   // GaugeBack (背景)
	std::shared_ptr<Sprite2D> m_spDamageSprite; // HPDamage (赤)
	std::shared_ptr<Sprite2D> m_spGaugeSprite;  // HPGauge (緑)

	//各パーツ用のUIオブジェクト
	std::shared_ptr<UIObject> m_upBase;
	std::shared_ptr<UIObject> m_upBack;
	std::shared_ptr<UIObject> m_upDamage;
	std::shared_ptr<UIObject> m_upGauge;

};

