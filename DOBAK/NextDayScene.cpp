#include "NextDayScene.h"
#include "SceneManager.h"
constexpr int DaysPerWeek = 3;
constexpr int TotalWeeks = 67;
constexpr int MaxQuotaFails = 3;

void NextDayScene::Init(GameState& state)
{
    _goldDiff = state.player.gold - state.goldAtDayStart;
    state.day++;
    state.dailySpinCount = 0;
    state.quotaSubmitted = false;
    state.quotaFromDayEnd = false;
    state.goldAtDayStart = state.player.gold;

    if (state.day % DaysPerWeek == 0)
    {
        state.week++;

        bool met = state.quotaMet;

        if (!met)
        {
            long long shortfall = std::max<long long>(0, state.dailyQuota - state.player.gold);
            state.carryOverQuota += shortfall;
            state.quotaFailCount++;

            if (state.quotaFailCount >= MaxQuotaFails)
            {
                state.isWinEnding = false;
                state.requestEndGame = true;
            }
        }
        else
        {
            state.carryOverQuota = 0;
        }

        if (!state.requestEndGame)
        {
            if (state.week > TotalWeeks)
            {
                state.isWinEnding = true;
                state.requestEndGame = true;
            }
            else
            {
                state.baseQuota = CalcQuotaForWeek(state.week, 10);
                state.dailyQuota = state.baseQuota + state.carryOverQuota;
                state.quotaMet = false;
            }
        }
    }

    PlayOpenTransition(state, 1);
}

void NextDayScene::Update(GameState& state)
{
    if (GetKeyDown(VK_RETURN) || GetKeyDown(VK_SPACE))
    {
        PlayCloseTransition(state, 1);
        SceneManager::GetInst()->ChangeScene("ShopScene", state);
    }
}

void NextDayScene::Render(const GameState& state)
{
    COORD res = GetConsoleResolution();
    int cx = res.X / 2;
    int cy = res.Y / 2;

    string dayStr = std::to_string(state.day) + "일째";
    string dayLabel = "=== " + dayStr + " ===";
    GotoXY(cx - (int)dayLabel.size() / 2, cy - 3);
    SetColor(Color::LIGHT_YELLOW);
    cout << dayLabel;

    GotoXY(cx - 15, cy - 1);
    SetColor(Color::GRAY);
    DrawLine('-', 30);

    GotoXY(cx - 15, cy);
    SetColor();
    cout << "보유 골드  : ";
    SetColor(Color::LIGHT_YELLOW);
    cout << state.player.gold << "G";

    GotoXY(cx - 15, cy + 1);
    SetColor();
    cout << "오늘 수익  : ";
    if (_goldDiff >= 0)
    {
        SetColor(Color::LIGHT_GREEN);
        cout << "+" << _goldDiff << "G";
    }
    else
    {
        SetColor(Color::LIGHT_RED);
        cout << _goldDiff << "G";
    }

    GotoXY(cx - 15, cy + 3);
    SetColor(Color::GRAY);
    DrawLine('-', 30);

    string hint = "[SPACE / ENTER] 상점으로";
    GotoXY(cx - (int)hint.size() / 2, cy + 5);
    SetColor(Color::LIGHT_GRAY);
    cout << hint;

    SetColor();
}
void NextDayScene::PlayOpenTransition(const GameState& state, unsigned long delayMs)
{
    COORD res = GetConsoleResolution();
    int W = res.X;
    int H = res.Y;
    int half = H / 2;
    int colorCount = (int)Color::END;

    for (int col = 0; col < W; ++col)
    {
        Color c = (Color)std::min((int)(((float)col / W) * colorCount), colorCount - 1);
        SetColor(c, c);
        for (int y = 0; y < H; ++y)
        {
            GotoXY(col, y); cout << " ";
        }
        Sleep(delayMs);
    }

    SetColor();
    for (int y = 0; y < H; ++y)
    {
        int cy2 = (y < half) ? y : (H - 1 - y);
        Color c = (Color)std::min((int)(((float)cy2 / half) * colorCount), colorCount - 1);
        SetColor(c, c);
        GotoXY(0, y);
        for (int x = 0; x < W; ++x) cout << " ";
    }

    for (int step = 0; step < half; ++step)
    {
        int topY = half - 1 - step;
        int bottomY = half + step;
        GotoXY(0, topY);    for (int x = 0; x < W; ++x) cout << " ";
        GotoXY(0, bottomY); for (int x = 0; x < W; ++x) cout << " ";
        Sleep(delayMs);
    }
    SetColor();
    Render(state);
}

void NextDayScene::PlayCloseTransition(const GameState& state, unsigned long delayMs)
{
    COORD res = GetConsoleResolution();
    int W = res.X;
    int H = res.Y;
    int half = H / 2;
    int colorCount = (int)Color::END;

    for (int step = 1; step <= half; ++step)
    {
        Color c = (Color)std::min((int)(((float)step / half) * colorCount), colorCount - 1);
        SetColor(c, c);
        GotoXY(0, step - 1);
        for (int x = 0; x < W; ++x) cout << " ";
        GotoXY(0, H - step);
        for (int x = 0; x < W; ++x) cout << " ";
        Sleep(delayMs);
    }
    SetColor();
}