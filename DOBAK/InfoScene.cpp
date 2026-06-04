#include "InfoScene.h"
#include "Console.h"
void InitInfo(GameState& state)
{

}
void UpdateInfo(GameState& state)
{
	if (GetKeyDown(VK_ESCAPE))
		state.curScene = Scene::TITLE;

	if (GetKeyDown(VK_LEFT))
	{
		state.curInfoMenu = (InfoMenu)std::max((int)InfoMenu::GAME, (int)state.curInfoMenu - 1);
	}
	if (GetKeyDown(VK_RIGHT))
	{
		state.curInfoMenu = (InfoMenu)std::min((int)InfoMenu::SOUND, (int)state.curInfoMenu + 1);
	}
	
}

void RenderInfo(const GameState& state)
{
	COORD res = GetConsoleResolution();
	int cx = res.X / 2;
	int cy = res.Y / 3;

	const string infoLabels[] =
	{
		"[ 조작 방법 ]",
		"SPACE  : 룰렛 돌리기",
		"ESC 로 돌아가기"
	};
	for (int i = 0; i < 3; ++i)
	{
		GotoXY(cx - 6, cy + i);
		if (i == 0)
			SetColor(Color::LIGHT_YELLOW);
		else if (i == 2)
			SetColor(Color::LIGHT_GRAY);
		else
			SetColor();
		cout << infoLabels[i];
	}
	cout << std::endl;
}