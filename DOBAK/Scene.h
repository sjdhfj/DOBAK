#pragma once
#include "GameState.h"
class AbstractScene
{
public:
	virtual void Init(GameState& state) abstract;
	virtual void Update(GameState& state) abstract;
	virtual void Render(const GameState& state) abstract;
	virtual void Release() {}
};