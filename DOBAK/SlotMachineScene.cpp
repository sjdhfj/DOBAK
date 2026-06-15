#include "SlotMachineScene.h"
#include "Console.h"
#include "AsciiArt.h"
#include <vector>
#include "Pattern.h"
#include "SceneManager.h"

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


void InGameScene::Init(GameState& state)
{


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
					slotArr[i][j] = rand() % 7 + 1;
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
		if (state.curTime - lastPatternBlinkTime >= 50)
		{
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
					ShakeConsoleWindow(10, 1250, 1);
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
}

void InGameScene::DrawUI(const GameState& state)
{
	GotoXY(0, 0);
	SetColor();
	cout << "Coin:" << state.player.gold;
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

void InGameScene::DrawSixSeven()
{
	if (!isSixSeven) return;

	SetUniCodeMode();

	SetColor(Color::BLUE);
	for (int i = 0; i < asciiArts.six.size(); ++i)
	{
		GotoXY(2, 6 + i + sixSevenMoveValue);
		wcout << asciiArts.six[i];
	}

	SetColor(Color::RED);
	for (int i = 0; i < asciiArts.seven.size(); ++i)
	{
		GotoXY(50, 6 + i - sixSevenMoveValue);
		wcout << asciiArts.seven[i];
	}

	SetColor();
	SetDefaultMode();
}

void InGameScene::ClearSixSeven()
{
	SetUniCodeMode();

	 
	SetColor();

	for (int i = 0; i < asciiArts.six.size() + 2; ++i)
	{
		GotoXY(2, 5 + i + sixSevenMoveValue);
		wcout << L"                         ";
	}

	for (int i = 0; i < asciiArts.seven.size() + 2; ++i)
	{
		GotoXY(50, 5 + i - sixSevenMoveValue);
		wcout << L"                       ";
	}

	SetDefaultMode();
}

//패턴 관련 메서드들
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


int InGameScene::CalculateReward()
{
	int totalReward = 0;

	for (int i = 0; i < GamePatternCount; ++i)
	{
		totalReward += CheckPatternReward(GamePatterns[i]);
	}

	return totalReward;
}
