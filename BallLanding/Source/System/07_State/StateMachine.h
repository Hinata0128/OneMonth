#pragma once
#include <memory>
//ステートのベースヘッダー.
#include "StateBase.h"

/************************************************************************
*	ステートマシンヘッダー
*	ここでは、ステートで使用するものの関数の処理を書く.
**/

template<typename FSM_Owner>
class StateMachine
{
public:
	//コンストラクタ.
	StateMachine(FSM_Owner* pOwner)
		: m_pOwner(pOwner)
		, m_pCurrentState(nullptr)
	{}
	//デストラクタ.
	~StateMachine() = default;

	//ステートの変更をする関数.
	void ChangeState(std::shared_ptr<StateBase<FSM_Owner>> pNewState)
	{
		//現在のステートに「終了(Exit)」を告げる.
		if (m_pCurrentState != nullptr)
		{
			m_pCurrentState->Exit();
		}
		//新しいステートに入れ替える.
		//古い方は自動的にメモリ解放される.
		m_pCurrentState = pNewState;
		//新しいステートに「開始(Enter)」を告げる.
		if (m_pCurrentState != nullptr)
		{
			m_pCurrentState->Enter();
		}
	}

	//更新関数.
	void Update()
	{
		if (m_pCurrentState != nullptr)
		{
			m_pCurrentState->Update();
		}
	}
	//描画関数.
	void Draw()
	{
		if (m_pCurrentState != nullptr)
		{
			m_pCurrentState->Draw();
		}
	}

private:
	FSM_Owner* m_pOwner;
	std::shared_ptr<StateBase<FSM_Owner>> m_pCurrentState;
};