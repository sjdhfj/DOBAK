#pragma once
#include<vector>
#include<string>
#include<algorithm>
using std::vector;
using std::string;
using std::wstring;
struct UIAsciiObjs
{
	vector<string> six;
	vector<string> seven;

	vector<wstring> shopSix;
	vector<wstring> shopSeven;
	vector<string> shopmen;
	vector<string> shophappymen;
	vector<string> shopjuicemen;
};
void AsciiInit(UIAsciiObjs& objs);
void AsciiUpdate(UIAsciiObjs& objs);
void AsciiRender(const UIAsciiObjs& objs);