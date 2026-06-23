#pragma once
#include "Enums.h"
#define NOMINMAX
constexpr int WIDTH = 160;
constexpr int HEIGHT = 45;
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <Windows.h>
#include "Item.h"
struct PlayerData
{
	std::vector<Item> inventory;
	unsigned long long gold = 0;
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
	long long goldAtDayStart = 0;
	int dailySpinCount = 0;
	unsigned long long dailyQuota = 100;
	bool quotaMet = false;
	bool quotaSubmitted = false;
	int  quotaIncreaseDay = 3;
	bool quotaFromDayEnd = false;
	bool requestQuotaCheck = false;
	bool isWinEnding = false;
	int week = 1;
	int quotaFailCount = 0;
	bool requestEndGame = false;
	long long carryOverQuota = 0;
	unsigned long long baseQuota = dailyQuota;
};
inline long long CalcQuotaForWeek(int week, long long base)
{
	constexpr double p = 1.2;
	constexpr double k = 4.92;

	double growth = std::pow(1.0 + week, p)
		* std::exp(week / k)
		* (1.0 + std::log(1.0 + week));

	double result = (double)base * growth;

	constexpr double LL_MAX = 9223372036854775807.0;
	if (result > LL_MAX) result = LL_MAX;
	if (result < 0) result = 0;

	return (long long)result;
}