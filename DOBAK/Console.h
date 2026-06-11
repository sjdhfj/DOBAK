#pragma once
#include <io.h>
#include <fcntl.h>
#include <iostream>
#define NOMINMAX
#include <Windows.h>
#include <conio.h>
#include <iomanip>
#include <string> 
#pragma comment(lib, "winmm.lib")
using std::cout;
using std::wcout;
using std::endl;
using std::string;
using std::wstring;

void SetUniCodeMode();
void SetDefaultMode();

void RenderDialogue(const string& text, int delayTime);
void RenderChar(char ch, int delayTime, bool isSkip);

void SetConsoleGameTitle(const wstring& title);
void SetConsoleSize(int width, int height);
void SetConsoleFullScreen();

void SetConsoleWindowStyle(bool showTitleBar);

void ShakeConsoleWindow(int intensity, int duration, int interval);
void UpdateShakeConsoleWindow();

void GotoXY(int x, int y);
BOOL IsGotoXY(int x, int y);

void SetCursorVisible(bool visible, DWORD size = 1);
COORD GetConsoleResolution();

enum class Color
{
    BLACK, BLUE, GREEN, SKYBLUE, RED,
    VIOLET, YELLOW, LIGHT_GRAY, GRAY, LIGHT_BLUE,
    LIGHT_GREEN, CYAN, LIGHT_RED, LIGHT_VIOLET,
    LIGHT_YELLOW, WHITE, END
};
void SetColor(Color textColor = Color::WHITE, Color bgColor = Color::BLACK);
void DrawBar(int x, int y, const string& label,
    int value, int maxValue, int barWidth,
    const string& fillChar="■", const string& emptyChar="□");
void DrawLine(char ch, int width);
//키 입력
bool GetKey(int vKey);
bool GetKeyDown(int vKey);
void UpdateInput();
void SetConsoleMouseInputDisable();

POINT GetMouseCellPosition();
void FrameSync(int fps);
int GetCursorX();

void SetConsoleFont(const wstring& fontName, COORD size, UINT weight = FW_NORMAL);