#pragma once
#include "..//ManagerBase.h"
#include "..//..//..//..//Collision/BoundingSphere/BoundingSphere.h"

/***********************************************************
*	敵の弾をまとめて管理するマネージャークラス.
**/

class JabaranShot;

class JabaranShotManager final
	: public ManagerBase<JabaranShotManager>
{
public:
	JabaranShotManager();
	~JabaranShotManager() override;
	//構築関数.
	void Create() override;
	//動作関数.
	void Update() override;
	//描画関数.
	void Draw() override;
	//解放関数.
	void Release() override;
	//弾の状態設定(ShotBaseにある).
	void Launch(const D3DXVECTOR3& Pos, const D3DXVECTOR3& Vel, float Radius, float Life);

	size_t GetShotCount() const { return m_upJabaranShotList.size(); }

	//敵に当たった時に弾の当たり判定を消す関数.
	void KillShotByCollider(const std::shared_ptr<BoundingSphere>& sphere);


private:
	//敵の弾をvectorで複製する.
	std::vector<std::unique_ptr<JabaranShot>> m_upJabaranShotList;

};