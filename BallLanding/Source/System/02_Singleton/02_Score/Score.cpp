#include "Score.h"

#include "GameObject/03_UIObject/01_UIScoreNumber/UIScoreNumber.h"

Score::Score()
	: m_CurrentScore	( 0 )
{
	m_pUIScoreNumber = std::make_unique<UIScoreNumber>();
}

Score::~Score()
{
}

void Score::Add(int val)
{
	m_CurrentScore += val;
}

void Score::Set(int val)
{
	m_CurrentScore = val;
}

int Score::GetValue() const
{
	return m_CurrentScore;
}

void Score::Clear()
{
	m_CurrentScore = 0;
}

void Score::SetPosition(const D3DXVECTOR2& pos)
{
	if (m_pUIScoreNumber)
	{
		m_pUIScoreNumber->SetPosition(pos);
	}
}

void Score::Update()
{
	if (m_pUIScoreNumber)
	{
		// 1. 自身の数値を表示クラスに渡す
		m_pUIScoreNumber->SetDisplayValue(m_CurrentScore);

		// 2. 表示クラスの内部更新（スプライトの再生成など）を行う
		m_pUIScoreNumber->Update();
	}
}


void Score::Draw()
{
	if (m_pUIScoreNumber)
	{
		// 描画実行
		m_pUIScoreNumber->Draw();
	}
}