#pragma once
#include"Console.h"
#include"Pattern.h"
#include "Scene.h"

enum class SlotMachineState
{
	Idle, Rolling, Blinking, SixSeven
};

class InGameScene : public AbstractScene
{
	void Init(GameState& state);
	void Update(GameState& state);
	void Render(const GameState& state);
	void DrawUI(const GameState& state);
	void DrawSlotMachine();
	void DrawSlotNumbers();
	bool IsCurrentPatternCell(int y, int x);
	void DrawSixSeven();
	void ClearSixSeven();
	int CheckPatternReward(const Pattern& pattern);
	bool IsSameInArea(int startY, int startX, int patternWidth, int patternHeight);
	void FindMatchedPatterns();
	int CalculateReward();
};
