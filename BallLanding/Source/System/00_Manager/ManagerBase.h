#pragma once

/******************************************************************
*	マネージャーベースクラス.
*	管理するオブジェクトの型を自由に指定できます.
**/

template <typename T>
class ManagerBase
{
public:
	static T* GetInstance()
	{
		static T s_Instance;
		return &s_Instance;
	}
	virtual ~ManagerBase() {}

	// 共通インターフェース
	virtual void Create() {}
	virtual void Update() {}
	virtual void Draw() {}
	virtual void Release() {}

protected:
	// 外部からのインスタンス化を禁止
	ManagerBase() = default;

	// コピー・代入を禁止（シングルトンの保証）
	ManagerBase(const ManagerBase&) = delete;
	ManagerBase& operator=(const ManagerBase&) = delete;
	ManagerBase(ManagerBase&&) = delete;
	ManagerBase& operator=(ManagerBase&&) = delete;
};
