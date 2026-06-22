#include "SlotMachineScene.h"
#include "Console.h"
#include "AsciiArt.h"
#include <vector>
#include "Pattern.h"
#include "SceneManager.h"

constexpr int SIXSEVENPOS = 30;

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
int sixSevenMoveValue = 3;
ULONGLONG lastSixSevenMoveTime;
int sixSevenCount = 0;
bool isSixSeven = false;

vector<vector<int>> sixBoundary(45, vector<int>(17));
vector<vector<int>> sevenBoundary(45, vector<int>(17));

//패턴 블링크 관련
vector<MatchedPattern> matchedPatterns;

int curPatternIndex = 0;
int patternBlinkCount = 0;
bool isPatternBlink = false;
ULONGLONG lastPatternBlinkTime = 0;

//시스템
int currentDay = 1;


void InGameScene::Init(GameState& state)
{
	SetConsoleSize(WIDTH, HEIGHT);
	slotState = SlotMachineState::Idle;
	AsciiInit(asciiArts);
	srand((unsigned int)time(nullptr));

	COORD res = GetConsoleResolution();

	titleX = (res.X - 25) / 2;
	titleY = res.Y / 4;

	//SetConsoleFont(L"NSimSun", { 20,25 }, FW_BOLD);

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

	DrawProbabilityUI(state);
	DrawInventoryUI(state);
}
void InGameScene::Update(GameState& state)
{
	if (GetKeyDown('B'))
	{
		SceneManager::GetInst()->ChangeScene("ShopScene", state);
	}
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
					slotArr[i][j] = rand() % 3 + 1;
				}
			}
		}

		if (state.curTime - lastSlotStartTime >= 1500)
		{
			FindMatchedPatterns();

			if (matchedPatterns.empty())
				      
			{
				slotState = SlotMachineState::Idle;
			}
			else
			{
				curPatternIndex = 0;
				patternBlinkCount = 0;
				isPatternBlink = false;
				lastPatternBlinkTime = state.curTime;

				slotState = SlotMachineState::Blinking;
			}
		}
	}
	if (slotState == SlotMachineState::Blinking)
	{
		if (state.curTime - lastPatternBlinkTime >= 80)
		{
			if (patternBlinkCount == 1)
				ShakeConsoleWindow(5, 100, 1);
			lastPatternBlinkTime = state.curTime;
			isPatternBlink = !isPatternBlink;

			if (isPatternBlink)
			{
				patternBlinkCount++;
			}

			if (patternBlinkCount > 3)
			{
				state.player.gold += matchedPatterns[curPatternIndex].reward;

				curPatternIndex++;
				patternBlinkCount = 0;
				isPatternBlink = false;

				if (curPatternIndex >= matchedPatterns.size())
				{
					sixSevenCount = 0;
					isSixSeven = true;
					lastSixSevenMoveTime = state.curTime;
					ShakeConsoleWindow(20, 1250, 1);
					slotState = SlotMachineState::SixSeven;
				}
			}
		}
	}
	if (slotState == SlotMachineState::SixSeven)
	{
		if (sixSevenCount > 10)
		{
			isSixSeven = false;
			ClearSixSeven();
			slotState = SlotMachineState::Idle;
		}
	}

	if (isSixSeven)
	{

		if (state.curTime - lastSixSevenMoveTime >= 100)
		{
			ClearSixSeven();

			lastSixSevenMoveTime = state.curTime;
				
			sixSevenMoveValue *= -1;

			sixSevenCount++;

		}
	}

	UpdateShakeConsoleWindow();
}
void InGameScene::Render(const GameState& state)
{
	GotoXY(0, 0);

	DrawUI(state);
	DrawSlotMachine();
	DrawSlotNumbers();
	DrawSixSeven();
	DrawPlusGold();
}

void InGameScene::DrawUI(const GameState& state)
{
	GotoXY(0, 0);
	SetColor();
	cout << "Coin:" << state.player.gold;
}


void InGameScene::DrawPlusGold()
{
	int startX = slotX - 4;
	int startY = slotY - 5;

	// 이전 출력 잔상 제거
	SetColor();
	for (int i = 0; i < 3; ++i)
	{
		GotoXY(startX, startY + i);
		cout << "                         ";
	}

	if (slotState != SlotMachineState::Blinking)
		return;

	if (curPatternIndex >= matchedPatterns.size())
		return;

	int reward = matchedPatterns[curPatternIndex].reward;

	SetColor(Color::LIGHT_YELLOW);

	GotoXY(startX, startY);
	cout << "+" << reward << "      _____";

	GotoXY(startX, startY + 1);
	cout << "          /  $  \\";

	GotoXY(startX, startY + 2);
	cout << "          \\_____/";

	SetColor();
}

void InGameScene::DrawProbabilityUI(const GameState& state)
{
	COORD res = GetConsoleResolution();
	int panelStartPos = res.X - asciiArts.probabilityPanel[0].length();;
	GotoXY(panelStartPos, 0);
	wcout << asciiArts.probabilityPanel[0];
	for (int i = 0; i < res.Y - 1; ++i)
	{
		GotoXY(panelStartPos, 1 + i);
		wcout << asciiArts.probabilityPanel[1];
	}
	GotoXY(panelStartPos, res.Y-1);
	wcout << asciiArts.probabilityPanel[2];
}

void InGameScene::DrawInventoryUI(GameState& state)
{
	COORD res = GetConsoleResolution();
	int startX = res.X - ((asciiArts.probabilityPanel[0].length() / 2) + (asciiArts.probabilityPanel[0].length() / 3));

	int startY = 0;

	for (int i = 0; i < state.player.inventory.size(); ++i)
	{
		for (int j = 0; j < state.player.inventory[i].art.size(); ++j)
		{
			startY++;
			GotoXY(startX, startY);
			cout << state.player.inventory[i].art[j];
		}
	}
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
			if (IsCurrentPatternCell(i, j))
			{
				SetColor(Color::WHITE, Color::YELLOW);
			}
			else
				SetColor();

			cout << slotArr[i][j];
			SetColor();
			cout << ' ';
		}
	}
}
void InGameScene::DrawSixSeven()
{
	if (!isSixSeven) return;
	COORD res = GetConsoleResolution();
	SetUniCodeMode();

	SetColor(Color::BLUE);
	for (int i = 0; i < asciiArts.six.size(); ++i)
	{
		GotoXY(SIXSEVENPOS, 6 + i + sixSevenMoveValue);
		wcout << asciiArts.six[i];
	}

	SetColor(Color::RED);
	for (int i = 0; i < asciiArts.seven.size(); ++i)
	{
		GotoXY(res.X - SIXSEVENPOS - asciiArts.seven[0].length(), 6 + i - sixSevenMoveValue);
		wcout << asciiArts.seven[i];
	}

	SetColor();
	SetDefaultMode();
}

void InGameScene::ClearSixSeven()
{
	SetUniCodeMode();
	COORD res = GetConsoleResolution();
	 
	SetColor();

	for (int i = 0; i < asciiArts.six.size() + 2; ++i)
	{
		GotoXY(SIXSEVENPOS, 5 + i + sixSevenMoveValue);
		wcout << L"                         ";
	}

	for (int i = 0; i < asciiArts.seven.size() + 2; ++i)
	{
		GotoXY(res.X - SIXSEVENPOS - asciiArts.seven[0].length(), 5 + i - sixSevenMoveValue);
		wcout << L"                       ";
	}

	SetDefaultMode();
}

//패턴 관련 메서드들
bool InGameScene::IsCurrentPatternCell(int y, int x)
{
	if (slotState != SlotMachineState::Blinking)
		return false;

	if (!isPatternBlink)
		return false;

	if (curPatternIndex >= matchedPatterns.size())
		return false;

	MatchedPattern& match = matchedPatterns[curPatternIndex];

	return y >= match.startY &&
		y < match.startY + match.height &&
		x >= match.startX &&
		x < match.startX + match.width;
}

int InGameScene::CheckPatternReward(const Pattern& pattern)
{
	int totalReward = 0;

	for (int y = 0; y <= height - pattern.height; ++y)
	{
		for (int x = 0; x <= width - pattern.width; ++x)
		{
			if (IsSameInArea(y, x, pattern.width, pattern.height))
			{
				totalReward += pattern.reward;
			}
		}
	}

	return totalReward;
}
void InGameScene::FindMatchedPatterns()
{
	matchedPatterns.clear();

	for (int p = 0; p < GamePatternCount; ++p)
	{
		Pattern& pattern = GamePatterns[p];

		for (int y = 0; y <= height - pattern.height; ++y)
		{
			for (int x = 0; x <= width - pattern.width; ++x)
			{
				if (IsSameInArea(y, x, pattern.width, pattern.height))
				{
					matchedPatterns.push_back(
					{
						y,
						x,
						pattern.width,
						pattern.height,
						pattern.reward
					});
				}
			}
		}
	}
}
bool InGameScene::IsSameInArea(int startY, int startX, int patternWidth, int patternHeight)
{
	int firstValue = slotArr[startY][startX];

	for (int y = 0; y < patternHeight; ++y)
	{
		for (int x = 0; x < patternWidth; ++x)
		{
			if (slotArr[startY + y][startX + x] != firstValue)
				return false;
		}
	}

	return true;
}