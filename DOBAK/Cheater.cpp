#include "Cheater.h"
#include "Console.h"

constexpr ULONGLONG CHEAT_GOLD_AMOUNT = 100000;

void Cheater::Update(GameState& state)
{
	CheckActivateInput();

	if (m_isActive)
		CheckCheatInput(state);
}

void Cheater::CheckActivateInput()
{
	bool comboHeld = GetKey(VK_CONTROL) && GetKey(VK_MENU) && GetKey(VK_SHIFT);
	if (comboHeld && GetKeyDown('C'))
		m_isActive = !m_isActive; 
}

void Cheater::CheckCheatInput(GameState& state)
{
	if (GetKeyDown('M'))
		state.player.gold += CHEAT_GOLD_AMOUNT;
}