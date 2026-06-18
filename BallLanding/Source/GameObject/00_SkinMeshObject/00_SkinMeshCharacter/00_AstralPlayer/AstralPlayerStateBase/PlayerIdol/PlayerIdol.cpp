#include "PlayerIdol.h"
#include "../PlayerAttack/PlayerAttack.h"
#include "../../AstralPlayer.h"

PlayerIdol::PlayerIdol(AstralPlayer* owner)
    : AstralPlayerStateBase(owner)
{
}

PlayerIdol::~PlayerIdol()
{
}

void PlayerIdol::Enter()
{
    m_pOwner->m_pMesh->ChangeAnim(AstralPlayer::enPlayerAnim::Idol, m_pOwner->m_pAnimCtrl);
}

void PlayerIdol::Update()
{
    // 右クリックで攻撃状態へ
    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
    {
        m_pOwner->ChangeState(std::make_shared<PlayerAttack>(m_pOwner));
    }
}

void PlayerIdol::Draw()
{
}

void PlayerIdol::Exit()
{
}