#pragma once
#include "Enums.h"
#define NOMINMAX
constexpr int WIDTH = 160;
constexpr int HEIGHT = 45;
#include <Windows.h>
#include <vector>
#include "Item.h"
struct GameState
{
	ShopTab curShopTab = ShopTab::BUY;
	bool  isRunning = true;
	ULONGLONG curTime;
	std::vector<Item> shopItems = {};
};
