#pragma once
#include "GameObject//02_StaticMeshObject//StaticMeshObject.h"
#include "Collision//BoundingSphere//BoundingSphere.h"

/***************************************************************
*	キャラクタークラス.
*	ToDo : このクラスを親としてプレイヤー等の作成をしていく.
**/

class Character
	: public StaticMeshObject
{
public:
	Character();
	virtual ~Character() override;

	virtual void Update() override;
	virtual void Draw() override;

	virtual void Init();
public:
	//死んだかどうかのゲット関数.
	virtual bool GetDead() const;

	float GetSpeed() const { return m_Speed; }
	void SetSpeed(float speed) { m_Speed = speed; }

	float GetCharacterHP() const { return m_CaracterHp; }
	void SetCharacterHP(float Hp) { m_CaracterHp = Hp; }

protected:
	//当たり判定を中心天にオフセット.
	D3DXVECTOR3 m_HitCenterOffset;
	//体力用.
	float		m_HitPoint;
	//死亡.
	bool m_Dead;
	//スピード.
	float m_Speed;
	//キャラクターの体力.
	float m_CaracterHp;

};