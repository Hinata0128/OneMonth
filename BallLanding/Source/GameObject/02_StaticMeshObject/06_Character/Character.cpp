#include "Character.h"

Character::Character()
	: StaticMeshObject()
	, m_HitPoint(0.0f)
	, m_Dead(false)
{
}

Character::~Character()
{
}

void Character::Update()
{
	StaticMeshObject::Update();
}

void Character::Draw()
{
	StaticMeshObject::Draw();
}

void Character::Init()
{
	//何も書かない.
}

bool Character::GetDead() const
{
	return m_Dead;
}
