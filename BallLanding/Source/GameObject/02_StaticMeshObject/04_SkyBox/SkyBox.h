#pragma once
#include "GameObject//02_StaticMeshObject//StaticMeshObject.h"

class StaticMeshManager;
class Camera;

class SkyBox final
	: public StaticMeshObject
{
public:
	SkyBox();
	~SkyBox() override;

	void Update() override;
	void Draw() override;

	void SetCamera(Camera* pCamera);

private:
	Camera* m_pCamera;
};