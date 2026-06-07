#include "InfoScene.h"
#include "Console.h"
void InfoScene::Init(GameState& state)
{

}
void InfoScene::Update(GameState& state)
{
	if (GetKeyDown(VK_ESCAPE))
		SceneManager::GetInst()->ChangeScene("TitleScene", state);
}

void InfoScene::Render(const GameState& state)
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