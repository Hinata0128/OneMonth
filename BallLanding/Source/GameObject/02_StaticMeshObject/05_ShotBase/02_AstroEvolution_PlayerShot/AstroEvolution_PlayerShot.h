#pragma once
#include "GameObject/02_StaticMeshObject/05_ShotBase/ShotBase.h"
#include "Collision/BoundingSphere/BoundingSphere.h"
#include "Effect/Effect.h" // エフェクト管理クラス

class AstroEvolution_PlayerShot final 
    : public ShotBase
{
public:
    AstroEvolution_PlayerShot();
    ~AstroEvolution_PlayerShot() override;

    void Update() override;
    void Draw() override;
    void Init() override;

    // 発射：ここでエフェクトを再生開始する
    void Reload(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir, float speed);

    // 当たった時の処理
    void OnHit();

    bool GetIsActive() const { return m_IsActive; }
    std::shared_ptr<BoundingSphere> GetSphere() const { return m_BSphere; }

private:
    std::shared_ptr<BoundingSphere> m_BSphere;
    int m_EffectHandle = -1; // 再生中のエフェクト識別子
    float m_LifeTime;
};