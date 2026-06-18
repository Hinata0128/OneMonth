#pragma once
#include "GameObject//02_StaticMeshObject//StaticMeshObject.h"

#include "Collision/BoundingBox/BoundingBox.h"

class StaticMeshManager;

/***************************************************
*	壁クラス.
**/

class Wall final
	: public StaticMeshObject
{
public:
	Wall();
	~Wall();

	void Update() override;
	void Draw() override;

	std::shared_ptr<BoundingBox> GetBoundingBox() const { return m_pBoundingBox; }

	//表示・非表示を切り替える関数.
	void SetVisible(bool visible) { m_isVisible = visible; }
	bool IsVisible() const { return m_isVisible; }
private:
	std::shared_ptr<BoundingBox> m_pBoundingBox;
	//デフォルトは表示,
	bool m_isVisible = true;
};