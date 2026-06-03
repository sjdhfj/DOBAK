#pragma once
#include<vector>
#include<string>
#include<algorithm>
using std::vector;
using std::string;

struct AsciiObjs
{
	vector<string> slotMachine;
};

void AsciiInit(AsciiObjs& objs);
void AsciiUpdate(AsciiObjs& objs);
void AsciiRender(const AsciiObjs& objs);
