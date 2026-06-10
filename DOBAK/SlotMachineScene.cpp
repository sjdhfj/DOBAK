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

int titleX;
int titleY;

int bonusY = 0;

void InGameScene::Init(GameState& state)
{
	AsciiInit(asciiArts);
	srand((unsigned int)time(nullptr));

	COORD res = GetConsoleResolution();

	titleX = (res.X - 25) / 2;
	titleY = res.Y / 4;

	SetConsoleFont(L"NSimSun", { 20,25 }, FW_BOLD);

	for (int i = 0; i < height; ++i)
	{
		slotArr[i] = new int[width];
	}

	slotX = titleX + 7;
	slotY = titleY + 2;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			slotArr[i][j] = 0;
		}
	}
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
	system("cls");

	DrawUI();
	DrawSlotMachine();
	DrawSlotNumbers();
	DrawSixSeven();
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

void InGameScene::DrawUI()
{
	GotoXY(0, 0);
	SetColor();
	cout << "Coin:" << coin;
}

void InGameScene::DrawSlotMachine()
{
	SetColor();

	for (int i = 0; i < asciiArts.slotMachine.size(); ++i)
	{
		GotoXY(titleX, titleY + i);
		cout << asciiArts.slotMachine[i];
	}
}

void InGameScene::DrawSlotNumbers()
{
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			GotoXY(slotX + j * 2, slotY + i);

			if (slotArr[i][j] == 7 || slotArr[i][j] == 6)
				SetColor(Color::WHITE, Color::YELLOW);
			else if (slotArr[i][j] == 1)
				SetColor(Color::WHITE, Color::RED);
			else
				SetColor();

			cout << slotArr[i][j];
			SetColor();
			cout << " ";
		}
	}
}

void InGameScene::DrawSixSeven()
{
	SetUniCodeMode();

	for (int i = 0; i < asciiArts.six.size(); ++i)
	{
		GotoXY(titleX - 27, titleY + i);
		wcout << asciiArts.six[i];
	}

	for (int i = 0; i < asciiArts.seven.size(); ++i)
	{
		GotoXY(titleX + 25, titleY + i);
		wcout << asciiArts.seven[i];
	}

	SetDefaultMode();
}
