#pragma once
#include "..//ManagerBase.h"
#include "..//..//..//..//Collision/BoundingSphere/BoundingSphere.h"

/****************************************************
*	プレイヤーの弾をまとめて管理するマネージャークラス.
**/

class PlayerShot;

class PlayerShotManager final
	: public ManagerBase<PlayerShotManager>
{
public:
	//コンストラクタ.
	PlayerShotManager();
	//デストラクタ.
	~PlayerShotManager();
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

	size_t GetShotCount() const { return m_upPlayerShotList.size(); }

	void KillShotByCollider(const std::shared_ptr<BoundingSphere>& sphere);
	
private:
	//プレイヤーの弾をvectorで複製する.
	std::vector<std::unique_ptr<PlayerShot>> m_upPlayerShotList;
};