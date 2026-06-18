#pragma once
#include<vector>
#include<string>
#include<algorithm>
using std::vector;
using std::string;
using std::wstring;

struct AsciiObjs
{
	vector<string> slotMachine;
	vector<wstring> six;
	vector<wstring> seven;
	vector<wstring> probabilityPanel;
};

void AsciiInit(AsciiObjs& objs);
void AsciiUpdate(AsciiObjs& objs);
void AsciiRender(const AsciiObjs& objs);
