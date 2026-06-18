#pragma once

#include "../UIObject.h" // 基底クラス
#include "Sprite2D/Sprite2D.h"

#include <vector>
#include <memory>

class UIScoreNumber : public UIObject
{
public:
    UIScoreNumber();
    ~UIScoreNumber() override;

    // 数値を更新するための関数を追加
    void SetDisplayValue(int value);

    void Update() override;
    void Draw() override;

    void SetPosition(const D3DXVECTOR2& pos);

private:
    D3DXVECTOR2 m_Position;
    int m_TargetValue;
    int m_LastScore = -1;

    std::vector<std::unique_ptr<Sprite2D>> m_NumberSprites;
    Sprite2D::SPRITE_STATE m_SpriteState;
};