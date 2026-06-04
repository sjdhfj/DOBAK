#pragma once
#include "Enums.h"
#define NOMINMAX
#include <Windows.h>
#include <vector>
struct GameState
{
	Scene prevScene = Scene::NONE;
	Scene curScene = Scene::TITLE;
	Menu  curMenu = Menu::START;
	bool  isRunning = true;
	ULONGLONG curTime;
};
