#include "Game.h"
#include "SlotMachineScene.h"
#include "SoundManager.h"
#include "TitleScene.h"
#include "InfoScene.h"

void Init(GameState& state)
{
	SOUND->Init();
	srand((unsigned int)time(nullptr));
	SetConsoleSize(WIDTH, HEIGHT);
	SetCursorVisible(false);
	state = GameState{};
	Init();
}

void Update(GameState& state)
{
	SOUND->Update();
	bool sceneChanged =
		state.curScene != state.prevScene;

	state.prevScene = state.curScene;
	UpdateInput();
	switch (state.curScene)
	{
	case Scene::TITLE:
		if (sceneChanged)
			InitTitle(state);
		UpdateTitle(state);
		break;
	case Scene::INFO:
		UpdateInfo(state);
		break;
	case Scene::INGAME:
		break;
	case Scene::SHOP:
		break;
	case Scene::GAMEOVER:
		break;
	default:
		break;
	}
}

void Render(const GameState& state)
{
	if (state.prevScene != state.curScene)
		system("cls");

	GotoXY(0, 0);
	switch (state.curScene)
	{
	case Scene::TITLE:
		RenderTitle(state);
		break;
	case Scene::INFO:
		RenderInfo(state);
		break;
	case Scene::INGAME:
		break;
	case Scene::SHOP:
		break;
	case Scene::GAMEOVER:
		break;
	default:
		break;
	}
}
