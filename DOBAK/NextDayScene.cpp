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
            // gold/dailyQuota가 둘 다 long long이므로 그냥 빼기만 하면 정상적으로 음수/양수 판단 가능.
            // 그래도 혹시 모를 음수 결과는 0으로 클램프(이월액이 음수가 되는 걸 방지).
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
            state.carryOverQuota = 0; // 달성하면 이월 해소
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
                // 이전엔 state.dailyQuota * pow(1.5, week-1) 로 계산해서
                // 이미 누적된 dailyQuota에 거듭제곱을 또 곱하는 이중 누적 버그가 있었음.
                // 이제는 항상 "기준 골드(base=10) x 주차"로부터 새로 계산해서 누적 폭주를 방지.
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