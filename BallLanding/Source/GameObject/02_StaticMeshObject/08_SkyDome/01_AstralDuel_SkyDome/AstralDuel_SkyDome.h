#pragma once
#include "GameObject//02_StaticMeshObject//StaticMeshObject.h"

class StaticMeshManager;
class Camera;

class AstralDuel_SkyDome final
	: public StaticMeshObject
{
public:
	AstralDuel_SkyDome();
	~AstralDuel_SkyDome() override;

	void Update() override;
	void Draw() override;

	void SetCamera(Camera* pCamera);

private:
	Camera* m_pCamera;
};