#pragma once
#include "..//AstralPlayerStateBase.h"

/**
 * @brief プレイヤーの攻撃状態クラス
 */
class PlayerAttack : public AstralPlayerStateBase
{
public:
    PlayerAttack(AstralPlayer* owner);
    virtual ~PlayerAttack() override;

    void Enter()  override;
    void Update() override;
    void Draw()   override;
    void Exit()   override;
};