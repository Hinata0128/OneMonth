#pragma once
#include "..//ShotBase.h"

/**********************************************************
*	プレイヤーの攻撃.
*	基底クラスを親クラスとしてこれらの
**/

class StaticMeshManager;

class PlayerShot
	: public ShotBase
{
public:
	//コンストラクタ.
	PlayerShot();
	//デストラクタ.
	~PlayerShot() override;

	//動作関数.
	void Update() override;
	//描画関数.
	void Draw() override;
	//弾の発射関数(初期化).
	void Launch(const D3DXVECTOR3& Pos,
				const D3DXVECTOR3& Vel,
				float Radius,
				float Life) override;

private:

};