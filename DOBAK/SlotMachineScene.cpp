#include "SlotMachineScene.h"
#include "Console.h"
#include "AsciiArt.h"

//int main()
//{
//	Init();
//}

//슬롯머신 관련
SlotMachineState slotState;
ULONGLONG lastSlotUpdateTime;
ULONGLONG lastSlotStartTime;
ULONGLONG lastSlotBlinkTime;
int blinkCount = 0;
bool isBlink = false;
int width = 5, height = 3;
int** slotArr = new int* [height];
int slotX = 0, slotY = 0;

int coin = 0;
AsciiObjs asciiArts;

int titleX;
int titleY;

//67 관련
int bonusY = 2;
ULONGLONG lastSixSevenMoveTime;
int sixSevenCount = 0;
bool isSixSeven = false;

int sixWidthBoundary;
int sixHeightBoundary;
int sevenWidthBoundary;
int sevenHeightBoundary;


void InGameScene::Init(GameState& state)
{
	slotState = SlotMachineState::Idle;
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

	lastSixSevenMoveTime = state.curTime;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			slotArr[i][j] = 0;
		}
	}

	//식스세븐 바운더리 설정
	titleX - 27
}
void InGameScene::Update(GameState& state)
{
	if (GetKeyDown(VK_SPACE) && slotState == SlotMachineState::Idle)
	{
		slotState = SlotMachineState::Rolling;
		lastSlotStartTime = state.curTime;
		lastSlotUpdateTime = state.curTime;
		//RandomSlot(state);
	}

	//슬롯머신 상태에 따라 업데이트해줄꺼
	if (slotState == SlotMachineState::Rolling)
	{
		if (state.curTime - lastSlotUpdateTime >= 10)
		{
			lastSlotUpdateTime = state.curTime;

			for (int i = 0; i < height; ++i)
			{
				for (int j = 0; j < width; ++j)
				{
					slotArr[i][j] = rand() % 7 + 1;
				}
			}
		}

		if (state.curTime - lastSlotStartTime >= 1500)
		{
			slotState = SlotMachineState::Blinking;
			lastSlotBlinkTime = state.curTime;
			blinkCount = 0;
			isBlink = false;
		}
	}
	if (slotState == SlotMachineState::Blinking)
	{
		if (state.curTime - lastSlotBlinkTime >= 100 && !isBlink)
		{
			lastSlotBlinkTime = state.curTime;
			isBlink = true;
			blinkCount++;

			if (blinkCount >= 5)
			{
				sixSevenCount = 0;
				isSixSeven = true;
				ShakeConsoleWindow(20, 1250,0.01f);
				slotState = SlotMachineState::SixSeven;
			}
		}
		else if (isBlink)
		{
			if (state.curTime - lastSlotBlinkTime >= 100)
			{
				lastSlotBlinkTime = state.curTime;
				isBlink = false;
			}
		}
	}
	if (slotState == SlotMachineState::SixSeven)
	{
		if (sixSevenCount > 10)
		{
			isSixSeven = false;
			slotState = SlotMachineState::Idle;
		}
	}

	if (isSixSeven)
	{
		if (state.curTime - lastSixSevenMoveTime >= 100)
		{
			lastSixSevenMoveTime = state.curTime;
			bonusY *= -1;
			sixSevenCount++;
		}
	}

	UpdateShakeConsoleWindow();
}
void InGameScene::Render(const GameState& state)
{
	GotoXY(0, 0);

	DrawUI();
	DrawSlotMachine();
	DrawSlotNumbers();
	DrawSixSeven();
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

			if ((slotArr[i][j] == 7 || slotArr[i][j] == 6)
				&& slotState == SlotMachineState::Blinking
				&& isBlink)
				SetColor(Color::WHITE, Color::YELLOW);
			else if (slotArr[i][j] == 1
				&& slotState == SlotMachineState::Blinking
				&& isBlink)
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
	if (!isSixSeven) return;

	SetUniCodeMode();

	for (int i = 0; i < asciiArts.six.size(); ++i)
	{
		SetColor(Color::BLUE);
		GotoXY(titleX - 27, titleY + i + bonusY);
		wcout << asciiArts.six[i];
	}

	for (int i = 0; i < asciiArts.seven.size(); ++i)
	{
		SetColor(Color::RED);
		GotoXY(titleX + 25, titleY + i - bonusY + 1);
		wcout << asciiArts.seven[i];
	}
	SetColor();

	SetDefaultMode();
}
