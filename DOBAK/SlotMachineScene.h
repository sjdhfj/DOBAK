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
	bool IsPatternMatched(int startY, int startX, const Pattern& pattern);
	void DrawPlusGold();
	void DrawPatternValuePanel(const GameState& state);
	void DrawInventoryUI(GameState& state);
	void DrawSlotMachine();
	void DrawSlotNumbers();
	void DrawInventory(const GameState& state);
	bool IsCurrentPatternCell(int y, int x);
	void DrawSixSeven();
	void ClearSixSeven();
	int CheckPatternReward(const Pattern& pattern);
	bool IsSameInArea(int startY, int startX, int patternWidth, int patternHeight);
	void FindMatchedPatterns();
	void ExecutePatternEvent(const Pattern& pattern, GameState& state);
	ItemEffectContext CollectItemEffects(GameState& state);
	void DrawEffectNotice();
	void DrawDayInfo(const GameState& state);
	string _lastEffectStr = "";
	Color  _lastEffectColor = Color::WHITE;
};