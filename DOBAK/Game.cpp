#include "Game.h"
#include "SlotMachineScene.h"
#include "SoundManager.h"
#include "Scene.h"
#include "TitleScene.h"
#include "InfoScene.h"
#include "SceneManager.h"
#include "ShopScene.h"
#include "Commands.h"
#include "ShopItems.h"
#include "NextDayScene.h"
#include "QuotaScene.h"
#include "EndingScene.h"
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
	ShopItemInit(state);
	state.player.inventory = {};
	SceneManager::GetInst()->RegisterScene("TitleScene", std::make_unique<TitleScene>());
	SceneManager::GetInst()->RegisterScene("InfoScene", std::make_unique<InfoScene>());
	SceneManager::GetInst()->RegisterScene("ShopScene", std::make_unique<ShopScene>());
	SceneManager::GetInst()->RegisterScene("InGameScene", std::make_unique<InGameScene>());
	SceneManager::GetInst()->RegisterScene("NextDayScene", std::make_unique<NextDayScene>());
	SceneManager::GetInst()->RegisterScene("QuotaScene", std::make_unique<QuotaScene>());
	SceneManager::GetInst()->RegisterScene("EndingScene", std::make_unique<EndingScene>());
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