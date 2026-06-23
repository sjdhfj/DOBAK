#include "EndingScene.h"
#include "Console.h"
#include <algorithm>
#include "UIAsciiObjs.h"
#include "SoundManager.h"
static UIAsciiObjs objs;
constexpr int SWAP_INTERVAL_MS = 1000;
constexpr int RIGHT_MARGIN = 4;
constexpr size_t MAX_PARTICLES = 400;
constexpr int LOSE_SHRINK_DURATION_MS = 900; // 콘솔창이 작아지는 데 걸리는 시간
int endingTitleWidth = 0;
int endingTitleHeight = 0;
int gameoverasciiHeight = 0;
int gameoverasciiWidth = 0;
void DrawEndingTitle(const UIAsciiObjs& objs);
void DrawGameOverEndingTitle(const UIAsciiObjs& objs);
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
		m_loseBurstTriggered = false;
		StartConsoleShrink(LOSE_SHRINK_DURATION_MS); // 부드럽게 작아지기 시작
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

	if (IsConsoleShrinkFinished() && !m_loseBurstTriggered)
	{
		RestoreConsoleWindowSize();

		COORD res = GetConsoleResolution();
		int cx = res.X / 2;
		int cy = res.Y / 3;
		m_gameOverEffect.Trigger(cx, cy);
		SOUND->Play("Pop");

		m_loseBurstTriggered = true;
	}

	if (m_loseBurstTriggered)
		m_gameOverEffect.Update();
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