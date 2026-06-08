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
    int _gold = 500; //플레이어
    std::vector<Item> _shopItems;
    std::vector<Item> _inventory; //플레이어 

    std::vector<Item>& CurList();
    int CurPrice(const Item& item) const;
    bool _isSmiling = false;
    ULONGLONG _smileTimer = 0;
};