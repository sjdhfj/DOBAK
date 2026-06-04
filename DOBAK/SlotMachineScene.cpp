#include "SlotMachineScene.h"
#include "Console.h"

//int main()
//{
//	Init();
//}

void Init()
{
	int x = 10, y = 10;

	GotoXY(x, y);
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			cout << "1";
		}
		GotoXY(x, y + i + 1);
	}
}