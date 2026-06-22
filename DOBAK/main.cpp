#include "Console.h"
#include <iostream>
#include "GameState.h"
#include "Game.h"
#include "SoundManager.h"
#include <string>
int main()
{
	GameState state;
	Init(state);
	SetConsoleSize(80, 40);
	SOUND->Load("Yeah", "Sound/Yayyy!.mp3");
	SOUND->Load("Drink", "Sound/bloxy-cola.mp3");
	SOUND->Load("Pop", "Sound/SlotMachineSelect.mp3");
	SOUND->PlayBGM("Sound/f.mp3");
	while (state.isRunning)
	{
		state.curTime = GetTickCount64();
		Update(state);
		Render(state);
		FrameSync(60);
	}
	Release();
}