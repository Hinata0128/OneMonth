#include "SkinMeshCharacter.h"

SkinMeshCharacter::SkinMeshCharacter()
	: SkinMeshObject()
	, m_Dead(false)
{
}

void SkinMeshCharacter::Update()
{
	SkinMeshObject::Update();
}

void SkinMeshCharacter::Draw()
{
	SkinMeshObject::Draw();
}

bool SkinMeshCharacter::GetDead() const
{
	return m_Dead;
}