#pragma once
#include "System/07_State/StateBase.h"

class AstralPlayer;

// StateBaseをpublic継承することで、派生クラスからUpcast（変換）が可能になる
class AstralPlayerStateBase : public StateBase<AstralPlayer>
{
public:
    AstralPlayerStateBase(AstralPlayer* owner) : StateBase<AstralPlayer>(owner) {}
    virtual ~AstralPlayerStateBase() override {}

    virtual void Draw() override {}
};