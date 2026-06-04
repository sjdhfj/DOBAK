#pragma once
#include"Console.h"

void Init();

int main()
{
	GotoXY(30, 30);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			cout << "1";
		}
		GotoXY(30, 30 + i +1);
	}
}