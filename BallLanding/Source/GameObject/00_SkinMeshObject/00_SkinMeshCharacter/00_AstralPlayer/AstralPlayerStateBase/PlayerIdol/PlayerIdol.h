#pragma once
#include "..//AstralPlayerStateBase.h"

/**
 * @brief プレイヤーの待機状態クラス
 */
class PlayerIdol : public AstralPlayerStateBase
{
public:
    PlayerIdol(AstralPlayer* owner);
    virtual ~PlayerIdol() override;

    void Enter()  override;
    void Update() override;
    void Draw()   override;
    void Exit()   override;
};