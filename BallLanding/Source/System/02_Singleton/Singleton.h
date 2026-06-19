#pragma once

/*************************************************************
*	シングルトンベースクラス.
**/

template <typename T>
class Singleton
{
public:

	static T& GetInstance()
	{
		static T s_Instance;
		return s_Instance;
	}

	virtual ~Singleton() = default;
	//書くクラスで必要に応じてリセットできる関数.
	virtual void Clear() {}
protected:
	Singleton() = default;
private:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;
};