#include "EndingScene.h"
#include "Console.h"
#include "SceneManager.h"
#include <algorithm>
#include "UIAsciiObjs.h"
#include "SoundManager.h"
static UIAsciiObjs objs;
constexpr int SWAP_INTERVAL_MS = 1000;
constexpr int RIGHT_MARGIN = 4;
constexpr size_t MAX_PARTICLES = 400;
constexpr int LOSE_SHRINK_DURATION_MS = 900; 
constexpr int LOSE_BURST_TOTAL = 3;          
constexpr int LOSE_BURST_INTERVAL_MS = 500;  
int endingTitleWidth = 0;
int endingTitleHeight = 0;
int gameoverasciiHeight = 0;
int gameoverasciiWidth = 0;
void DrawEndingTitle(const UIAsciiObjs& objs);
void DrawGameOverEndingTitle(const UIAsciiObjs& objs);
void DrawPressEnterPrompt();
int GetMaxLineWidth(const std::vector<std::string>& art)
{
	int maxWidth = 0;
	for (const auto& line : art)
		maxWidth = std::max(maxWidth, (int)line.size());
	return maxWidth;
}
void EndingScene::Init(GameState& state)
{
	AsciiInit(objs);
	endingTitleHeight = objs.titleascii.size();
	gameoverasciiHeight = objs.gameoverascii.size();

	if (state.isWinEnding)
	{
		m_showHappy = false;
		m_lastSwapTime = GetTickCount64();
		m_fireworks.Clear();
		m_nextFireworkTime = GetTickCount64();
		SOUND->PlayBGM("Sound/g.mp3");
	}
	else
	{
		m_gameOverEffect.Clear();
		m_loseBurstCount = 0;
		m_nextBurstTime = 0;
		m_loseSequenceFinished = false;
		StartConsoleShrink(LOSE_SHRINK_DURATION_MS);
		SOUND->PlayBGM("Sound/k.mp3");
	}
}
void EndingScene::Update(GameState& state)
{
	UpdateShakeConsoleWindow();
	WinUIUpdate(state);
	LoseUIUpdate(state);
}
void EndingScene::WinUIUpdate(GameState& state)
{
	if (!state.isWinEnding)
		return;
	if (state.curTime - m_lastSwapTime >= SWAP_INTERVAL_MS)
	{
		m_showHappy = !m_showHappy;
		m_lastSwapTime = state.curTime;
		SOUND->Play("Yeah");
	}
	if (state.curTime >= m_nextFireworkTime && m_fireworks.GetParticleCount() < MAX_PARTICLES)
	{
		int launchX = WIDTH / 6 + rand() % (WIDTH * 2 / 3);
		int apexY = HEIGHT / 6 + rand() % (HEIGHT / 3);
		m_fireworks.Launch(launchX, apexY);
		m_nextFireworkTime = state.curTime + 400 + rand() % 700;
	}
	m_fireworks.Update();
}
void EndingScene::LoseUIUpdate(GameState& state)
{
	if (state.isWinEnding)
		return;

	UpdateConsoleShrink();

	if (IsConsoleShrinkFinished())
	{
		if (m_loseBurstCount == 0 && m_nextBurstTime == 0)
		{
			RestoreConsoleWindowSize();
			m_nextBurstTime = state.curTime;
		}

		if (m_loseBurstCount < LOSE_BURST_TOTAL && state.curTime >= m_nextBurstTime)
		{
			COORD res = GetConsoleResolution();
			int sectionWidth = res.X / LOSE_BURST_TOTAL;
			int cx = sectionWidth * m_loseBurstCount + sectionWidth / 2;
			cx += -10 + rand() % 21;
			int cy = res.Y / 3 + (rand() % 7 - 3);

			m_gameOverEffect.Trigger(cx, cy);
			SOUND->Play("Pop");

			++m_loseBurstCount;
			m_nextBurstTime = state.curTime + LOSE_BURST_INTERVAL_MS;
		}

		if (m_loseBurstCount >= LOSE_BURST_TOTAL)
			m_loseSequenceFinished = true;

		m_gameOverEffect.Update();
	}

	if (m_loseSequenceFinished && GetKeyDown(VK_RETURN))
	{
		SceneManager::GetInst()->ChangeScene("TitleScene", state);
		return;
	}
}
void EndingScene::Render(const GameState& state)
{
	WinUIRender(state);
	LoseUIRender(state);
}
void EndingScene::WinUIRender(const GameState& state)
{
	if (!state.isWinEnding)
		return;

	m_fireworks.Erase();
	const std::vector<std::string>& curArt = m_showHappy ? objs.shophappymen : objs.shopmen;
	int artWidth = std::max(GetMaxLineWidth(objs.shopmen), GetMaxLineWidth(objs.shophappymen));
	int startX = WIDTH - artWidth - RIGHT_MARGIN;
	int startY = HEIGHT / 2 - (int)curArt.size() / 2;
	if (startX < 0) startX = 0;
	if (startY < 0) startY = 0;
	for (int i = 0; i < curArt.size(); ++i)
	{
		GotoXY(startX, startY + (int)i);
		cout << curArt[i];
	}

	m_fireworks.Draw();
	DrawEndingTitle(objs);
}
void EndingScene::LoseUIRender(const GameState& state)
{
	if (state.isWinEnding)
		return;
	m_gameOverEffect.Erase();
	int artWidth = GetMaxLineWidth(objs.shopangrymen);
	int startX = WIDTH - artWidth - RIGHT_MARGIN;
	int startY = HEIGHT / 2 - objs.shopangrymen.size() / 2;
	if (startX < 0) startX = 0;
	if (startY < 0) startY = 0;
	for (int i = 0; i < objs.shopangrymen.size(); ++i)
	{
		GotoXY(startX, startY + (int)i);
		cout << objs.shopangrymen[i];
	}
	m_gameOverEffect.Draw();
	DrawGameOverEndingTitle(objs);

	if (m_loseSequenceFinished)
		DrawPressEnterPrompt();
}
void DrawEndingTitle(const UIAsciiObjs& objs)
{
	COORD res = GetConsoleResolution();
	int titleY = res.Y / 3;
	SetUniCodeMode();
	for (int i = 0; i < endingTitleHeight; ++i)
	{
		GotoXY(res.X / 14, titleY + i);
		wcout << objs.endingascii[i];
	}
	SetDefaultMode();
}
void DrawGameOverEndingTitle(const UIAsciiObjs& objs)
{
	COORD res = GetConsoleResolution();
	int titleY = res.Y / 3;
	SetUniCodeMode();
	for (int i = 0; i < endingTitleHeight; ++i)
	{
		GotoXY(res.X / 14, titleY + i);
		wcout << objs.gameoverascii[i];
	}
	SetDefaultMode();
}
void DrawPressEnterPrompt()
{
	COORD res = GetConsoleResolution();
	const string msg = "Press ENTER to continue";
	int x = res.X / 2 - (int)msg.size() / 2;
	int y = res.Y - 2; // È­¸é ¸Ç ¹Ø
	if (x < 0) x = 0;

	GotoXY(x, y);
	SetColor(Color::LIGHT_YELLOW);
	cout << msg;
	SetColor();
}