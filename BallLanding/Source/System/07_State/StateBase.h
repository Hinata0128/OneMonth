#pragma once
#include <iostream>
#include <string>
#include <memory>

/***************************************************************************************
*	全てのステートの親クラス.
*	例: Baseクラスとして使っていく.
*	テンプレートトして作成して行きます。
*	これを使って、PlayerState系統で使える用に継承をさせる目的のクラスになっています.
*	作成者: 西村 日向.
**/

template<typename FSM_Owner>
class StateBase
{
public:
	//コンストラクタ.
	//ステートの持ち主をセットする.
	//この場合templateに書いているFSM_Ownerを引数に書く.
	StateBase(FSM_Owner* owner) : m_pOwner(owner) {}
	virtual ~StateBase() {}

	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Exit() = 0;
	
	//ステートを今変えても大丈夫かを確認するbool.
	//デフォルトではtrue.
	virtual bool CanChangeState() { return true; }

protected:
	//持ち主を保存する変数.
	FSM_Owner* m_pOwner;
};