#pragma once
#include "GameObject/02_StaticMeshObject/StaticMeshObject.h"
#include "Collision/BoundingBox/BoundingBox.h"

class StaticMeshManager;

/*********************************************************
*	AstroEvolution用の壁クラス.
**/

class AstroEvolution_Wall final
	: public StaticMeshObject
{
public:
	AstroEvolution_Wall();
	~AstroEvolution_Wall();

	void Update() override;
	void Draw() override;
	
	std::shared_ptr<BoundingBox> GetBoundingBox() const
	{
		return m_pBoundingBox;
	}

	//表示・非表示を切り替える関数.
	void SetVisible(bool visible) { m_isVisible = visible; }
	bool IsVisible() const { return m_isVisible; }

private:
	std::shared_ptr<BoundingBox> m_pBoundingBox;
	//デフォルトは表示,
	bool m_isVisible = true;
};
