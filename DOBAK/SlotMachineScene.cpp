#include "SlotMachineScene.h"
#include "Console.h"

//int main()
//{
//	Init();
//}

int width = 5, height = 3;
int** slotArr = new int* [height];

void Init()
{
	srand((unsigned int)time(nullptr));

	COORD res = GetConsoleResolution();
	int x = res.X / 2 - 4;
	int y = res.Y / 3 * 2;
	const wstring ascii[] =
	{
		L"   +-------------+   ",
		L"   |.-----------.|   ",
		L"   ||           ||   ",
		L"   ||           ||   ",
		L"   ||           ||   ",
		L"   |+-----------+|   ",
		L"   +-..-------..-+   ",
		L"   .-------------.   ",
		L"  / /===========\\ \\  ",
		L" / /=============\\ \\ ",
		L"/___________________\\",
		L"\\___________________/ ",


	};
	int titleX = (res.X - 50) / 2;
	int titleY = res.Y / 4;
	SetUniCodeMode();
	for (int i = 0; i < 12; ++i)
	{
		GotoXY(titleX, titleY + i);
		wcout << ascii[i];
	}
	SetDefaultMode();

	SetConsoleFont(L"NSimSun", { 20,25 }, FW_BOLD);
	for (int i = 0; i < height; ++i)
	{
		slotArr[i] = new int[width];
	}

	RandomSlot(titleX + 6,titleY + 2);

	while (true)
	{
		Render();
	}
}

void Render()
{
	//system("cls");
	GotoXY(0, 0);
}

void RandomSlot(int x, int y)
{
	for (int w = 0; w < 150; ++w)
	{
		GotoXY(x, y);
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				slotArr[i][j] = rand() % 7 + 1;
				cout << slotArr[i][j] << " ";
			}
			GotoXY(x, y + i + 1);
		}
		Sleep(10);
	}

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (slotArr[i][j] == 7)
			{
				GotoXY(x + j - 1, y + i);
				SetColor(Color::WHITE, Color::YELLOW);
				cout << 7;
			}
			else if (slotArr[i][j] == 6)
			{
				GotoXY(x + j - 1, y + i);
				SetColor(Color::WHITE, Color::YELLOW);
				cout << 6;
			}
		}
	}
}
