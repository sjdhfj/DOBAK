#pragma once
#include<vector>
#include<string>
#include<algorithm>
using std::vector;
using std::string;
struct UIAsciiObjs
{
	vector<string> title;

};
void AsciiInit(UIAsciiObjs& objs);
void AsciiUpdate(UIAsciiObjs& objs);
void AsciiRender(const UIAsciiObjs& objs);