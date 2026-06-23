#pragma once
#include "GameState.h"

class Cheater
{
public:
	void Update(GameState& state);
	bool IsActive() const { return m_isActive; }

private:
	void CheckActivateInput();
	void CheckCheatInput(GameState& state);

	bool m_isActive = false;
};