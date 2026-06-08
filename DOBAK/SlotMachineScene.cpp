#include "SlotMachineScene.h"
#include "Console.h"
#include "AsciiArt.h"

//int main()
//{
//	Init();
//}

int width = 5, height = 3;
int** slotArr = new int* [height];
int slotX = 0, slotY = 0;

int coin = 0;
AsciiObjs asciiArts;

void InGameScene::Init(GameState& state)
{
	AsciiInit(asciiArts);
	srand((unsigned int)time(nullptr));

	COORD res = GetConsoleResolution();
	int x = res.X / 2 - 4;
	int y = res.Y / 3 * 2;


	int titleX = (res.X -60) / 2;
	int titleY = res.Y / 4;
	SetUniCodeMode();
	for (int i = 0; i < 12; ++i)
	{
		GotoXY(titleX, titleY + i);
		cout << asciiArts.slotMachine[i];
	}
	SetDefaultMode();

	SetConsoleFont(L"NSimSun", { 20,25 }, FW_BOLD);
	for (int i = 0; i < height; ++i)
	{
		slotArr[i] = new int[width];
	}

	slotX = titleX + 6, slotY = titleY + 2;
	
	GotoXY(slotX, slotY);
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			slotArr[i][j] = 0;
			cout << slotArr[i][j] << " ";
		}
		GotoXY(slotX, slotY + i + 1);
	}

	SixSeven();
}
void InGameScene::Update(GameState& state)
{
	if (GetKeyDown(VK_SPACE))
	{
		RandomSlot();
	}
}
void InGameScene::Render(const GameState& state)
{
	GotoXY(0, 0);
	std::setw(30);
	cout << "                    ";
	GotoXY(0, 0);
	cout << "Coin:" << coin;
}

void InGameScene::RandomSlot()
{
	for (int w = 0; w < 150; ++w)
	{
		GotoXY(slotX, slotY);
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				slotArr[i][j] = rand() % 7 + 1;
				cout << slotArr[i][j] << " ";
			}
			GotoXY(slotX, slotY + i + 1);
		}
		Sleep(10);
	}
	
	int addCoin = 0;

	for (int w = 0; w < 5; ++w)
	{
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				if (slotArr[i][j] == 7 || slotArr[i][j] == 6)
				{
					GotoXY(slotX + j * 2, slotY + i);
					SetColor();
					cout << slotArr[i][j];
				}
				else if (slotArr[i][j] == 1)
				{
					GotoXY(slotX + j * 2, slotY + i);
					SetColor();
					cout << slotArr[i][j];
				}
			}
		}
		Sleep(100);
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				if (slotArr[i][j] == 7 || slotArr[i][j] == 6)
				{
					GotoXY(slotX + j * 2, slotY + i);
					SetColor(Color::WHITE, Color::YELLOW);
					cout << slotArr[i][j];

					addCoin++;
				}
				else if (slotArr[i][j] == 1)
				{
					GotoXY(slotX + j * 2, slotY + i);
					SetColor(Color::WHITE, Color::RED);
					cout << slotArr[i][j];

					addCoin--;
				}
			}
		}
		Sleep(100);
		coin += addCoin;
		SetColor();
	}
}

void InGameScene::SixSeven()
{
	/*int x = res.X / 2 - 4;
	int y = res.Y / 3 * 2;

	int titleX = (res.X - 60) / 2;
	int titleY = res.Y / 4;*/

	SetUniCodeMode();
	for (int i = 0; i < 27; ++i)
	{
		GotoXY(20, 10 + i);
		wcout << asciiArts.six[i];
	}

	for (int i = 0; i < 22; ++i)
	{
		GotoXY(40, 10 + i);
		wcout << asciiArts.seven[i];
	}
	SetDefaultMode();
}
