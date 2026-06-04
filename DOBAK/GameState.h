#pragma once
#include "Enums.h"
#define NOMINMAX
constexpr int WIDTH = 160;
constexpr int HEIGHT = 45;
#include <Windows.h>
#include <vector>
struct GameState
{
	Scene prevScene = Scene::NONE;
	Scene curScene = Scene::TITLE;
	Menu  curMenu = Menu::START;
	InfoMenu curInfoMenu = InfoMenu::GAME;
	bool  isRunning = true;
	ULONGLONG curTime;
};
