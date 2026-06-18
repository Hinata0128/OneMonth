#pragma once
#include "GameObject/02_StaticMeshObject/05_ShotBase/00_PlayerShot/PlayerShot.h"
#include <vector>

#include "GameObject/02_StaticMeshObject/06_Character/01_Enemy/Enemy.h"

class Player;
class Portal;

/**********************************************
*プレイヤー弾マネージャークラス.
*弾の生成・更新・描画・破棄を管理する.
**********************************************/
class PlayerShotManager final
{
public:
	~PlayerShotManager();

	static PlayerShotManager* GetInstance();	//インスタンス取得.

	void Update();	//弾の更新処理.
	void Draw(const D3DXMATRIX& view, const D3DXMATRIX& proj);	//弾の描画処理.
	void Init();

	void AddPlayerShot(const D3DXVECTOR3& Pos, Enemy* target, const D3DXVECTOR3& forwardDir);	//プレイヤーの弾を追加.
	void ReMovePlayerShot(size_t index);	//指定インデックスの弾を削除.

	//Playerが死んだときにGameMain内にPlayerの弾が残るのを一斉に削除する.
	void ClearPlayerShot();

	const std::vector<std::unique_ptr<PlayerShot>>& GetPlayerShot()const;	//弾リストを取得.
	PlayerShot* GetPlayerShot(size_t No);	//指定番号の弾を取得.
	size_t GetPlayerShotCount()const;	//弾の総数を取得.

	std::vector<PlayerShot*> GetShots();

	void AddHomingShot(const D3DXVECTOR3& Pos, Enemy* target, const D3DXVECTOR3& forwardDir);

private:
	PlayerShotManager();
	PlayerShotManager(const PlayerShotManager& rhs) = delete;
	PlayerShotManager& operator=(const PlayerShotManager& rhs) = delete;

private:
	std::vector<std::unique_ptr<PlayerShot>> m_PlayerShot;	//弾リスト.
};