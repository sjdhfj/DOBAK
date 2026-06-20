#pragma once
#include <vector>
#include <string>

using std::vector;
using std::string;
using std::wstring;
struct ExcludeRect
{
    int x, y, width, height;
};
struct UIAsciiObjs
{
    vector<string> six;
    vector<string> seven;

    vector<wstring> shopSix;
    vector<wstring> shopSeven;

    vector<string> shopmen;
    vector<string> shophappymen;
    vector<string> shopjuicemen;

    vector<wstring> titleascii;

    int sixOffset = 0;
    int sevenOffset = 0;
};

void AsciiInit(UIAsciiObjs& objs);
void AsciiUpdate(UIAsciiObjs& objs);
void AsciiRender(const UIAsciiObjs& objs, const vector<ExcludeRect>& excludeRects = {});
void DrawTitle(const UIAsciiObjs& objs);
void DrawRowExcluding(const string& fullLine, int y, int width,
    const vector<ExcludeRect>& excludeRects);
string BuildTiledLine(const string& pattern, int offset, int width);