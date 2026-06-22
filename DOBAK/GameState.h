#pragma once
#include "Enums.h"
#define NOMINMAX
constexpr int WIDTH = 160;
constexpr int HEIGHT = 45;
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include <Windows.h>
#include "Item.h"
struct PlayerData
{
	std::vector<Item> inventory;
	ULONGLONG gold = 0;
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
	ULONGLONG dailyQuota = 1000;
	bool quotaMet = false;
	bool quotaSubmitted = false;
	int  quotaIncreaseDay = 3;
	bool quotaFromDayEnd = false;
};
