#pragma once
#include "Console.h"
#include "Scene.h"
#include"SceneManager.h"
#include"Item.h"
class ShopScene : public AbstractScene
{
public:
	void Init(GameState& state) override;
	void Update(GameState& state) override;
	void Render(const GameState& state) override;
private:
    ShopTab _curTab = ShopTab::BUY;
    int _cursor = 0;
    std::vector<Item> _shopItems;

    std::vector<Item>& CurList(GameState& state);
    int CurPrice(const Item& item) const;
    bool _isSmiling = false;
    bool _isSelling = false;
    ULONGLONG _smileTimer = 0;
    ULONGLONG _juiceTimer = 0;
};