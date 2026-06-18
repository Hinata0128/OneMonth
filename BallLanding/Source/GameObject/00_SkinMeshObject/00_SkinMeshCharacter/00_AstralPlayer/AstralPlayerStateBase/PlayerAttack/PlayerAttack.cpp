#include "PlayerAttack.h"
#include "../PlayerIdol/PlayerIdol.h"
#include "../../AstralPlayer.h"

PlayerAttack::PlayerAttack(AstralPlayer* owner)
    : AstralPlayerStateBase(owner)
{
}

PlayerAttack::~PlayerAttack()
{
}

void PlayerAttack::Enter()
{
    // 1. アニメーション再生
    m_pOwner->m_pMesh->ChangeAnim(AstralPlayer::enPlayerAnim::Blade_1_2_3, m_pOwner->m_pAnimCtrl);

    // ★追加：攻撃フラグを立てる
    // friend設定されているので、privateな m_ShortAttackList にアクセスできます
    m_pOwner->m_ShortAttackList = AstralPlayer::enShort::Attack;

    // 2. ヒットフラグなどをリセット
    m_pOwner->SetHasHit(false);
}

void PlayerAttack::Update()
{
    float deltaTime = Timer::GetInstance().DeltaTime();

    // アニメーション更新
    m_pOwner->m_pMesh->SetAnimTime(m_pOwner->m_pMesh->GetAnimTime() + deltaTime);

    // 終了判定
    if (m_pOwner->m_pMesh->IsAnimEnd(6))
    {
        m_pOwner->ChangeState(std::make_shared<PlayerIdol>(m_pOwner));
    }
}

void PlayerAttack::Draw()
{
#ifdef _DEBUG
    if (m_pOwner->GetAttackSphere())
    {
        m_pOwner->GetAttackSphere()->Draw();
    }
#endif
}

void PlayerAttack::Exit()
{
    // ★追加：攻撃が終わるのでフラグを戻す
    // これにより、当たり判定が画面外（y=-100など）へ戻ります
    m_pOwner->m_ShortAttackList = AstralPlayer::enShort::None;
}