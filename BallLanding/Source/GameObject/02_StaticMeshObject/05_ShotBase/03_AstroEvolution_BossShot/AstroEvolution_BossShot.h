#pragma once
#include "GameObject/02_StaticMeshObject/05_ShotBase/ShotBase.h"
#include "Collision/BoundingSphere/BoundingSphere.h"
#include "Effect/Effect.h"

/************************************************************
*	ボスの弾クラス
*	プレイヤーの弾クラスをベースに、敵用パラメータを適用
**/
class AstroEvolution_BossShot final : public ShotBase
{
public:
    AstroEvolution_BossShot();
    ~AstroEvolution_BossShot() override;

    void Update() override;
    void Draw() override;
    void Init() override;

    // 発射処理（位置、方向、速度を指定）
    void Reload(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir, float speed);

    // 当たった時の処理
    void OnHit();

    bool GetIsActive() const { return m_IsActive; }
    std::shared_ptr<BoundingSphere> GetSphere() const { return m_BSphere; }

private:
    std::shared_ptr<BoundingSphere> m_BSphere;
    int   m_EffectHandle = -1;
    float m_LifeTime = 0.0f;
    const float MAX_LIFE_TIME = 240.0f; // プレイヤーより少し長めに生存
};