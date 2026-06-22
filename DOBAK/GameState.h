#pragma once
#include "Enums.h"
#define NOMINMAX
constexpr int WIDTH = 160;
constexpr int HEIGHT = 45;
#include <Windows.h>
#include <vector>
#include "Item.h"
struct PlayerData
{
	std::vector<Item> inventory;
	int gold = 0;
};
struct GameState
{
	ShopTab curShopTab = ShopTab::BUY;
	bool  isRunning = true;
	ULONGLONG curTime;
	std::vector<Item> shopItems = {};
	PlayerData player;
	bool requestNextDay = false;
	int day = 1;
	int goldAtDayStart = 0;
	int dailySpinCount = 0;
	int dailyQuota = 1000;
	bool quotaMet = false;
};
