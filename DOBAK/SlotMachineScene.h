#pragma once
#include"Console.h"
#include "Scene.h"

class InGameScene : public AbstractScene
{
	void Init(GameState& state);
	void Update(GameState& state);
	void Render(const GameState& state);
	void RandomSlot();
	void SixSeven();
};