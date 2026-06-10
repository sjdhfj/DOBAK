#include "TitleScene.h"

static UIAsciiObjs objs;
void TitleScene::Init(GameState& state)
{
	AsciiInit(objs);
}
void TitleScene::Update(GameState& state)
{
	AsciiUpdate(objs);
	// 키입력 화살표 왔다갔다
	if (GetKeyDown(VK_UP))
	{
		_curMenu = (Menu)std::max((int)Menu::START, (int)_curMenu - 1);
	}
	if (GetKeyDown(VK_DOWN))
	{
		_curMenu = (Menu)std::min((int)Menu::QUIT, (int)_curMenu + 1);
	}
	if (GetKeyDown(VK_RETURN))
	{
		switch (_curMenu)
		{
		case Menu::START:
			PlayTransition();
			SceneManager::GetInst()->ChangeScene("ShopScene", state);
			break;
		case Menu::INFO:
			SceneManager::GetInst()->ChangeScene("InfoScene", state);
			break;
		case Menu::QUIT:
			state.isRunning = false;
			break;
		}
	}
}

void TitleScene::Render(const GameState& state)
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
		cout << (i == (int)_curMenu ? "> " : "  ") << labels[i];
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
	cout << (_curMenu == Menu::START ? ">" : " ");
	GotoXY(x - 2, y + 1);
	cout << (_curMenu == Menu::INFO ? ">" : " ");
	GotoXY(x - 2, y + 2);
	cout << (_curMenu == Menu::QUIT ? ">" : " ");*/
}

void TitleScene::PlayTransition()
{
	COORD res = GetConsoleResolution();
	int delayMs = 20;
	int flashCount = 5;
	FlashAnimation(res, flashCount, delayMs);
	CrossAnimation(res, delayMs);
	system("cls");
}

void TitleScene::FlashAnimation(COORD resolution, int count, int delayMs)
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

void TitleScene::CrossAnimation(COORD resolution, int delayMs)
{
	system("cls");

	int totalSteps = 300;
	float prevAngle = 0.0f;
	float pi = 2.0f * 3.1415926535897932f;

	for (int i = 1; i <= totalSteps; ++i)
	{
		float targetAngle = (float)i / totalSteps * pi;
		Circle(prevAngle, targetAngle, "a");
		prevAngle = targetAngle;
		Sleep(0.5f);
	}
}
void TitleScene::Circle(float prevAngle, float targetAngle, const string& fillChar)
{
	COORD res = GetConsoleResolution();
	int width = res.X;
	int height = res.Y;

	float centerX = width / 2.0f;
	float centerY = height / 2.0f;
	float bojung = 2.0f;
	float pibojung = 2.0f * 3.1415926535897932f;
	int colorCnt = (int)Color::END;

	for (int y = 0; y < height; ++y)
	{
		int startX = -1;
		Color color = Color::BLACK;

		for (int x = 0; x < width; ++x)
		{
			float dx = (x - centerX) * bojung; //현제 픽셀에서 중심점까지의 거리
			float dy = y - centerY;
			float angle = atan2f(dx, -dy);
			if (angle < 0) angle += pibojung;

			bool inPrev = (angle <= prevAngle);
			bool inTarget = (angle <= targetAngle);

			if (inTarget && !inPrev) //이번 프레임에 새로 그려야 될거
			{
				float angleRatio = angle / pibojung;
				int colorIndex = (int)(angleRatio * colorCnt);
				if (colorIndex < 0) colorIndex = 0;
				if (colorIndex >= colorCnt) colorIndex = colorCnt - 1;				
				Color c = (Color)colorIndex;

				if (startX < 0) //시작점이 없으면
				{
					startX = x; //현재의 X값을 시작점으로 기억한다.
					color = c;
				}
				else if (c != color) //색이 바뀔때
				{
					GotoXY(startX, y);
					SetColor(color, color);
					for (int rx = startX; rx < x; ++rx) cout << fillChar;
					startX = x;
					color = c;
				}
			}
			else
			{
				if (startX >= 0)
				{
					GotoXY(startX, y);
					SetColor(color, color);
					for (int rx = startX; rx < x; ++rx) cout << fillChar;
					startX = -1;
				}
			}
		}
		if (startX >= 0)
		{
			GotoXY(startX, y);
			SetColor(color, color);
			for (int rx = startX; rx < width; ++rx) cout << fillChar;
		}
	}
	SetColor(Color::WHITE, Color::BLACK);
}