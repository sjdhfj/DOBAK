#include "Console.h"
#include <iostream>
#include "GameState.h"
#include "Game.h"
#include <string>
int main()
{
	GameState state;
	Init(state);
	SetConsoleSize(20, 40);
	while (state.isRunning)
	{
		state.curTime = GetTickCount64();
		Update(state);
		Render(state);
		FrameSync(60);
	}
	
}