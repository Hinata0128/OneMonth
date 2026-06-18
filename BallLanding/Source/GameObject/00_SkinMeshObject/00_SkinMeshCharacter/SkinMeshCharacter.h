#pragma once
#include "GameObject/00_SkinMeshObject/SkinMeshObject.h"
#include "Collision/BoundingSphere/BoundingSphere.h"

/**************************************************************
*	スキンメッシュクラス.
**/

class SkinMeshCharacter
	: public SkinMeshObject
{
public:
	SkinMeshCharacter();
	virtual ~SkinMeshCharacter() override {};

	virtual void Update() override;
	virtual void Draw() override;

	virtual void Init() {};
public:
	virtual bool GetDead() const;
protected:
	//死亡.
	bool m_Dead;

};