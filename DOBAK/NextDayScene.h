#pragma once
#include "Scene.h"
#include "Console.h"
class NextDayScene : public AbstractScene
{
public:
	// AbstractScene을(를) 통해 상속됨
	void Init(GameState& state) override;
	void Update(GameState& state) override;
	void Render(const GameState& state) override;
private:
	void PlayOpenTransition(const GameState& state, unsigned long delayMs);
	void PlayCloseTransition(const GameState& state, unsigned long delayMs);
private:
	int _goldDiff = 0;
};