#include "Console.h"

#undef max;
void SetUniCodeMode()
{
	(void)_setmode(_fileno(stdout), _O_U16TEXT);
}

void SetDefaultMode()
{
	(void)_setmode(_fileno(stdout), _O_TEXT);
}
void RenderChar(char ch, int delayTime, bool isSkip)
{
	cout << ch;
	"#";
	// !?.,

	// @#$%^&*
	// string의 find랑 비슷한데 더 빨라요
	if (strchr("@#$%^&*", ch))
	{
		Sleep(200);
		cout << "\b \b";
	}
	if (isSkip)
		return;

	int calcDelay = delayTime + (rand() % 11 - 5);
	switch (ch)
	{
	case '?':
	case '!':
	case '.':
		calcDelay += 400; //0.4초
		break;
	case ',':
		calcDelay += 200; //0.2초
		break;
	case '\n':
		calcDelay += 500; //0.5초
		break;
	case ' ':
		calcDelay += 50; //0.05초
		break;
	default:
		break;
	}

	//if (text[i] == '\n')
	//	Sleep(delayTime + 500); //약 0.5초
	//else
	//	Sleep(delayTime);
	Sleep(std::max(0, calcDelay));
}
void SetConsoleGameTitle(const wstring& title)
{
	SetConsoleTitle(title.c_str());
}
void SetConsoleSize(int width, int height)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	//뷰포트
	//int arr[10] = {1,12,3,4,5,,6,,7,8}
	SMALL_RECT rect = { 0,0,1,1 };
	SetConsoleWindowInfo(handle, true, &rect);
	// 버퍼
	COORD size = { (short)width, (short)height };
	SetConsoleScreenBufferSize(handle, size);

	rect.Right = width - 1;
	rect.Bottom = height - 1;
	SetConsoleWindowInfo(handle, true, &rect);

	//Sleep(100);
	//가운데 설정
	HWND hWnd = GetConsoleWindow();

	//RECT windowRect;
	//GetWindowRect(hWnd, &windowRect);
	//int windowWidth = windowRect.right - windowRect.left;
	//int windowHeight = windowRect.bottom - windowRect.top;

	CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };
	GetCurrentConsoleFontEx(handle, false, &fontInfo);
	int windowWidth = fontInfo.dwFontSize.X * width;
	int windowHeight = fontInfo.dwFontSize.Y * height;
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int posX = (screenWidth - windowWidth) / 2;
	int posY = (screenHeight - windowHeight) / 2;
	SetWindowPos(hWnd, nullptr, posX, posY, 0, 0, SWP_NOSIZE | SWP_FRAMECHANGED);
}
void SetConsoleFullScreen()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };
	GetCurrentConsoleFontEx(handle, false, &fontInfo);

	SetConsoleDisplayMode(handle,
		CONSOLE_FULLSCREEN_MODE, 0);

	int width = GetSystemMetrics(SM_CXSCREEN) / fontInfo.dwFontSize.X;
	int height = GetSystemMetrics(SM_CYSCREEN) / fontInfo.dwFontSize.Y;
	SetConsoleSize(width, height);
}

void SetConsoleSettings(int width, int height, bool isFullScreen, const wstring& title)
{
	//타이틀 설정
	//system("");
	SetConsoleTitle(title.c_str());

	HWND hWnd = GetConsoleWindow();
	//창 설정
	if (isFullScreen)
	{
		ShowWindow(hWnd, SW_MAXIMIZE);
	}
	else
	{
		MoveWindow(hWnd, 0, 0, width, height, true);
	}
}
void RenderDialogue(const string& text, int delayTime)
{
	bool isSkip = false;
	for (int i = 0; i < text.size(); ++i)
	{
		// ? ! . ,
		if (!isSkip && _kbhit())
			(void)_getch(); //버퍼 비우기용
		RenderChar(text[i], delayTime, isSkip);
	}
}
void SetConsoleWindowStyle(bool showTitleBar)
{
	HWND hWnd = GetConsoleWindow();
	LONG style = GetWindowLong(hWnd, GWL_STYLE);
	// AND NOT=> NAND
	style &= ~WS_SIZEBOX & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX & ~WS_SYSMENU;

	if (!showTitleBar)
		style &= ~WS_CAPTION;

	SetWindowLong(hWnd, GWL_STYLE, style);

	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0
		, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);
}

static bool g_isShaking = false;
static int g_shakeIntensity = 0;
static float g_shakeDuration = 0;
static float g_shakeInterval = 0;

static ULONGLONG g_shakeStartTime = 0;
static ULONGLONG g_lastShakeTime = 0;

static int g_originX = 0;
static int g_originY = 0;

void ShakeConsoleWindow(int intensity, int duration, int interval)
{

	HWND hWnd = GetConsoleWindow();
	RECT windowRect;
	GetWindowRect(hWnd, &windowRect);

	g_originX = windowRect.left;
	g_originY = windowRect.top;

	g_shakeIntensity = intensity;
	g_shakeDuration = duration;
	g_shakeInterval = interval;

	g_shakeStartTime = GetTickCount64();
	g_lastShakeTime = g_shakeStartTime;
	g_isShaking = true;

}

void UpdateShakeConsoleWindow()
{
	if (!g_isShaking)
		return;

	ULONGLONG curTime = GetTickCount64();
	HWND hWnd = GetConsoleWindow();

	if (curTime - g_shakeStartTime >= (ULONGLONG)g_shakeDuration)
	{
		SetWindowPos(hWnd, nullptr, g_originX, g_originY, 0, 0, SWP_NOSIZE);
		g_isShaking = false;
		return;
	}

	if (curTime - g_lastShakeTime >= (ULONGLONG)g_shakeInterval)
	{
		g_lastShakeTime = curTime;

		int offsetX = rand() % (g_shakeIntensity * 2 + 1) - g_shakeIntensity;
		int offsetY = rand() % (g_shakeIntensity * 2 + 1) - g_shakeIntensity;

		SetWindowPos(hWnd, nullptr,
			g_originX + offsetX,
			g_originY + offsetY,
			0, 0, SWP_NOSIZE);
	}
}

static bool g_isShrinking = false;
static bool g_shrinkFinished = false;
static float g_shrinkDuration = 0;
static float g_shrinkMinScale = 0.05f;
static ULONGLONG g_shrinkStartTime = 0;

static int g_shrinkOriginX = 0;
static int g_shrinkOriginY = 0;
static int g_shrinkOriginWidth = 0;
static int g_shrinkOriginHeight = 0;

void StartConsoleShrink(int durationMs, float minScale)
{
	HWND hWnd = GetConsoleWindow();
	RECT windowRect;
	GetWindowRect(hWnd, &windowRect);

	g_shrinkOriginX = windowRect.left;
	g_shrinkOriginY = windowRect.top;
	g_shrinkOriginWidth = windowRect.right - windowRect.left;
	g_shrinkOriginHeight = windowRect.bottom - windowRect.top;

	g_shrinkDuration = (float)durationMs;
	g_shrinkMinScale = minScale;
	g_shrinkStartTime = GetTickCount64();
	g_isShrinking = true;
	g_shrinkFinished = false;
}

void UpdateConsoleShrink()
{
	if (!g_isShrinking)
		return;

	ULONGLONG curTime = GetTickCount64();
	float elapsed = (float)(curTime - g_shrinkStartTime);
	float t = elapsed / g_shrinkDuration;

	if (t >= 1.0f)
	{
		t = 1.0f;
		g_isShrinking = false;
		g_shrinkFinished = true;
	}

	float easedT = t * t;
	float scale = 1.0f - (1.0f - g_shrinkMinScale) * easedT;

	int newWidth = (int)(g_shrinkOriginWidth * scale);
	int newHeight = (int)(g_shrinkOriginHeight * scale);

	int centerX = g_shrinkOriginX + g_shrinkOriginWidth / 2;
	int centerY = g_shrinkOriginY + g_shrinkOriginHeight / 2;
	int newX = centerX - newWidth / 2;
	int newY = centerY - newHeight / 2;

	HWND hWnd = GetConsoleWindow();
	SetWindowPos(hWnd, nullptr, newX, newY, newWidth, newHeight, SWP_NOZORDER);
}

bool IsConsoleShrinkFinished()
{
	return g_shrinkFinished;
}

void RestoreConsoleWindowSize()
{
	HWND hWnd = GetConsoleWindow();
	SetWindowPos(hWnd, nullptr,
		g_shrinkOriginX, g_shrinkOriginY,
		g_shrinkOriginWidth, g_shrinkOriginHeight,
		SWP_NOZORDER);
	g_shrinkFinished = false;
}

void GotoXY(int x, int y)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD curPos = { (short)x,(short)y };
	SetConsoleCursorPosition(handle, curPos);
}

BOOL IsGotoXY(int x, int y)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD curPos = { (short)x,(short)y };
	return SetConsoleCursorPosition(handle, curPos);
}

void SetCursorVisible(bool visible, DWORD size)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO curInfo;
	curInfo.bVisible = visible; // on, off
	curInfo.dwSize = size; //1 ~ 100
	SetConsoleCursorInfo(handle, &curInfo);
}

COORD GetConsoleResolution()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	short width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	short height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	return { width, height };
}

void SetColor(Color textColor, Color bgColor)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	int text = (int)textColor;
	int bg = (int)bgColor;
	SetConsoleTextAttribute(handle, (bg << 4) | text);
}

void DrawBar(int x, int y, const string& label,
	int value, int maxValue, int barWidth,
	const string& fillChar, const string& emptyChar)
{
	Color color;
	if (value > maxValue * 0.6f)
		color = Color::LIGHT_GREEN;
	else if (value > maxValue * 0.3f)
		color = Color::LIGHT_YELLOW;
	else
		color = Color::LIGHT_RED;
	//그릴 지점에 가서
	GotoXY(x, y);
	//색깔을 원래대로 해가지고
	SetColor();
	cout << label; // ex. hp
	SetColor(color);
	// bar 출력
	// barWidth; // 10칸

	//10칸 value = 60 / maxValue = 100
	int filledCount = barWidth * value / maxValue;
	for (int i = 0; i < barWidth; ++i)
		cout << ((i < filledCount) ? fillChar : emptyChar);

	SetColor();
	// 10 / 100
	int digits = std::to_string(maxValue).length();
	cout << " " << std::setw(digits) << value << "/"
		<< std::setw(digits) << maxValue;
}

void DrawLine(char ch, int width)
{
	//setfill(): 한번하면 계속
	cout << std::setfill(ch) << std::setw(width) << "" << std::setfill(' ');
}

bool GetKey(int vKey)
{
	return GetAsyncKeyState(vKey) & 0x8000;
}

constexpr int KEY_COUNT = 256;
static bool prevDown[KEY_COUNT] = {};
static bool curDown[KEY_COUNT] = {};
bool GetKeyDown(int vKey)
{
	return curDown[vKey] && !prevDown[vKey];
}

void UpdateInput()
{
	for (int i = 0; i < 256; ++i)
	{
		prevDown[i] = curDown[i];
		curDown[i] = GetAsyncKeyState(i) & 0x8000;
	}
	/*bool cur = GetAsyncKeyState(vKey) & 0x8000;
	bool down = cur && !prevDown[vKey];
	prevDown[vKey] = cur;*/
}

void SetConsoleMouseInputDisable()
{
	HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(handle, &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE;
	mode &= ~ENABLE_INSERT_MODE;

	mode |= ENABLE_MOUSE_INPUT;

	mode |= ENABLE_EXTENDED_FLAGS;
	SetConsoleMode(handle, mode);
}

POINT GetMouseCellPosition()
{
	POINT pt;
	GetCursorPos(&pt);
	HWND hWnd = GetConsoleWindow();
	ScreenToClient(hWnd, &pt);

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };
	GetCurrentConsoleFontEx(handle, false, &fontInfo);

	POINT cellPos = { pt.x / fontInfo.dwFontSize.X,
						pt.y / fontInfo.dwFontSize.Y };
	return cellPos;
}

void FrameSync(int fps)
{
	static ULONGLONG prevTick = GetTickCount64();
	ULONGLONG curTick = GetTickCount64();
	ULONGLONG targetTick = 1000 / fps;
	ULONGLONG elapsed = curTick - prevTick;
	if (elapsed < targetTick)
		Sleep(targetTick - elapsed);
	prevTick = GetTickCount64();
}
int GetCursorX()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return csbi.dwCursorPosition.X;
}

void SetConsoleFont(const wstring& fontName, COORD size, UINT weight)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX info = {};
	info.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	info.dwFontSize = size; // 폭, 높이
	info.FontWeight = weight; // FW_ ~ 
	// 폰트 이름 복사
	wcscpy_s(info.FaceName, fontName.c_str());
	SetCurrentConsoleFontEx(handle, false, &info);
}