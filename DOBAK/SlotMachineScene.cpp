#include "SlotMachineScene.h"
#include "Console.h"

void Init();

//int main()
//{
//	Init();
//}

void Init()
{
	GotoXY(30, 30);
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			cout << "1";
		}
		GotoXY(30, 30 + i + 1);
	}
}