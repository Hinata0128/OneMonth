#pragma once
#include "System/00_Manager/ManagerBase.h"
#include "GameObject/02_StaticMeshObject/05_ShotBase/03_AstroEvolution_BossShot/AstroEvolution_BossShot.h"
#include <algorithm>
#include <vector>
#include <memory>

class AstroEvolution_BossShot_Manager final : public ManagerBase
{
public:
    static AstroEvolution_BossShot_Manager* GetInstance()
    {
        static AstroEvolution_BossShot_Manager instance;
        return &instance;
    }
    ~AstroEvolution_BossShot_Manager();

    void Update() override;
    void Draw() override;
    void Create() override;
    void Release() override;

    // ボスから呼ばれるバラマキ発射関数
    // pos: 発射位置, baseDir: 基準方向, numShots: 弾数, angleStep: 弾の間隔(ラジアン)
    void BossFireWay(const D3DXVECTOR3& pos, const D3DXVECTOR3& baseDir, int numShots, float angleStep);

    const std::vector<std::unique_ptr<AstroEvolution_BossShot>>& GetShots() const {
        return m_pShots;
    }

private:
    AstroEvolution_BossShot_Manager();
    AstroEvolution_BossShot_Manager(const AstroEvolution_BossShot_Manager&) = delete;
    AstroEvolution_BossShot_Manager& operator=(const AstroEvolution_BossShot_Manager&) = delete;

    std::vector<std::unique_ptr<AstroEvolution_BossShot>> m_pShots;
};