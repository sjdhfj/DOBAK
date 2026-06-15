#include "Game.h"
#include "SlotMachineScene.h"
#include "SoundManager.h"
#include "Scene.h"
#include "TitleScene.h"
#include "InfoScene.h"
#include "SceneManager.h"
#include "ShopScene.h"
#include "Commands.h"

void Init(GameState& state)
{
	SetConsoleWindowStyle(true);
	SetConsoleFont(L"NSimSun", { 8, 16 });
	SOUND->Init();
	srand((unsigned int)time(nullptr));
	SetConsoleSize(WIDTH, HEIGHT);
	SetCursorVisible(false);
	SetConsoleMouseInputDisable();
	state = GameState{};

	state.shopItems = 
	{
		{1, "name", "description", 100, ItemType::CONSUME, new GoldCommand(10),
			{
				" ___ ",
				"($$$)",
				" --- "
			}
		}
	};

	SceneManager::GetInst()->RegisterScene("TitleScene", std::make_unique<TitleScene>());
	SceneManager::GetInst()->RegisterScene("InfoScene", std::make_unique<InfoScene>());
	SceneManager::GetInst()->RegisterScene("ShopScene", std::make_unique<ShopScene>());
	SceneManager::GetInst()->RegisterScene("InGameScene", std::make_unique<InGameScene>());
	SceneManager::GetInst()->ChangeScene("TitleScene", state);
	//Init();
}

void Update(GameState& state)
{
	SOUND->Update();
	UpdateInput();
	SceneManager::GetInst()->Update(state);
}

void Render(const GameState& state)
{
	GotoXY(0, 0);
	SceneManager::GetInst()->Render(state);
}
void Release()
{
	SOUND->Release();
	SceneManager::DestroyInst();
	
}
