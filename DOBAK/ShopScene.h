#pragma once
#include "Console.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Item.h"
#include <vector>

constexpr int DescWidth = 35;
constexpr int DescLines = 2;

class ShopScene : public AbstractScene
{
public:
    void Init(GameState& state) override;
    void Update(GameState& state) override;
    void Render(const GameState& state) override;
    void Release() override {}
private:
    void PlayOpenTransition(const GameState& state, unsigned long delaymilisecond);
    void PlayCloseTransition(const GameState& state, unsigned long delayMilisecond);
    void RenderSixSeven(const GameState& state);
    void RenderItemArt(const Item& item);
    void ClampCursorAndScroll(int size);
    std::vector<Item>& CurList(GameState& state);
    int CurPrice(const Item& item) const;
private:
    ShopTab _curTab = ShopTab::BUY;
    std::vector<Item> _shopItems;
    bool _isSmiling = false;
    bool _isSelling = false;
    ULONGLONG _smileTimer = 0;
    ULONGLONG _juiceTimer = 0;
    ULONGLONG _sixSevenTimer = 1000;
    int _cursor = 0;
    int _scrollOffset = 0;
    int _savedCursor[2] = { 0, 0 };
    int _savedScroll[2] = { 0, 0 };
};