#include "UIScoreNumber.h"

UIScoreNumber::UIScoreNumber()
    : m_Position(0, 0)
    , m_TargetValue(0)
{
    // スプライト状態設定
    m_SpriteState.Base.w = 520.0f;  // 画像全体幅
    m_SpriteState.Base.h = 96.0f;   // 画像全体高さ
    m_SpriteState.Stride.w = 52.0f;   // 1桁幅
    m_SpriteState.Stride.h = 96.0f;   // 1桁高さ
    m_SpriteState.Disp.w = 52.0f;   // 描画幅
    m_SpriteState.Disp.h = 96.0f;   // 描画高さ
}

UIScoreNumber::~UIScoreNumber()
{
}

void UIScoreNumber::SetDisplayValue(int value)
{
    m_TargetValue = value;
}

void UIScoreNumber::Update()
{
    // シングルトンからではなく、保持している m_TargetValue を使う
    int score = m_TargetValue;

    std::vector<int> digits;
    if (score == 0) digits.push_back(0);
    else
    {
        while (score > 0)
        {
            digits.push_back(score % 10);
            score /= 10;
        }
        std::reverse(digits.begin(), digits.end());
    }

    m_NumberSprites.clear();

    float x = m_Position.x;
    float y = m_Position.y;

    for (int d : digits)
    {
        auto sprite = std::make_unique<Sprite2D>();
        sprite->Init(_T("Data\\Image\\Setting\\Numbers.png"), m_SpriteState);
        sprite->SetPatternNo(d, 0);
        sprite->SetPosition(D3DXVECTOR3(x, y, 0.0f));
        m_NumberSprites.push_back(std::move(sprite));

        x += 52.0f;
    }
}

void UIScoreNumber::Draw()
{
    for (auto& sprite : m_NumberSprites)
    {
        sprite->Render();
    }
}

void UIScoreNumber::SetPosition(const D3DXVECTOR2& pos)
{
    m_Position = pos;
}