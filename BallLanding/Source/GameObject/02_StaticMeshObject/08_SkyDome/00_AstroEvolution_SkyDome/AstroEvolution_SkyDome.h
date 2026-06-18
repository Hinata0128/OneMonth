#pragma once
#include "GameObject//02_StaticMeshObject//StaticMeshObject.h"

class StaticMeshManager;
class Camera;

class AstroEvolution_SkyDome final
	: public StaticMeshObject
{
public:
	AstroEvolution_SkyDome();
	~AstroEvolution_SkyDome() override;

	void Update() override;
	void Draw() override;

	void SetCamera(Camera* pCamera);

private:
	Camera* m_pCamera;
};