#pragma once

#include "..//UIObject.h"

class Sprite2D;
class DirectX11;
class Timer;

/*********************************************************************
*	制限時間クラス.
*	ToDo : 全ラウンド共通のもの.
*		 : タイマークラスに書いているdeltaTimeを取得して実装していく.
**/

class LimitTime
	: public UIObject
{
public:
	LimitTime(float LimitTime);
	~LimitTime() override;

	void Create() override;
	void Update() override;
	void Draw() override;
public:
	bool IsTimeUp() const { return m_RemainTime <= 0.0f; }

	//残り時間を取得する.
	float GetRemainTime() const { return m_RemainTime; }

	//時間を再設定したい場合の関数.
	void SetTime(float LimitTime)
	{
		m_LimitTime		= LimitTime;
		m_RemainTime	= LimitTime;
	}
private:

	//制限時間に必要なメンバ変数.
	//灰色の枠.
	std::shared_ptr<Sprite2D> m_pClockBackSprite;
	//黒色の枠.
	std::shared_ptr<Sprite2D> m_pClockFrameSprite;
	//白色の枠.
	std::shared_ptr<Sprite2D> m_pClockFrontSprite;
	//時計の針.
	std::shared_ptr<Sprite2D> m_pClockMinSprite;

	std::shared_ptr<UIObject> m_upClockBack;
	std::shared_ptr<UIObject> m_upClockFrame;
	std::shared_ptr<UIObject> m_upClockFront;
	std::shared_ptr<UIObject> m_upClockMin;

private:
	//制限時間.
	float m_LimitTime;
	//残りの時間.
	float m_RemainTime;
};