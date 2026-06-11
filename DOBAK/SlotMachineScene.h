#pragma once
#include"Console.h"
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
	void RandomSlot(const GameState& state);
	void DrawUI();
	void DrawSlotMachine();
	void DrawSlotNumbers();
	void DrawSixSeven();
};