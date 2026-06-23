#include "QuotaScene.h"
#include <cmath>
#include "SoundManager.h"

constexpr int PanelW = 44;
constexpr int BarW = 16;
constexpr int TotalWeeks = 67;
constexpr int MaxQuotaFails = 3;
constexpr int DaysPerWeek = 3;

static int StrDispW(const string& s)
{
    int w = 0;
    for (int i = 0; i < (int)s.size(); )
    {
        unsigned char c = (unsigned char)s[i];
        if (c >= 0x81 && c <= 0xFE && i + 1 < (int)s.size())
        {
            w += 2; i += 2;
        }
        else
        {
            w += 1; i += 1;
        }
    }
    return w;
}

static string Centered(const string& s, int width)
{
    int dw = StrDispW(s);
    int total = width - dw;
    if (total <= 0) return s;
    int l = total / 2, r = total - l;
    return string(l, ' ') + s + string(r, ' ');
}

static void PanelDivider(int x, int y, char fill = '-')
{
    GotoXY(x, y);
    SetColor(Color::GRAY);
    cout << "+" << string(PanelW - 2, fill) << "+";
}

static void PanelLine(int x, int y, const string& content = "",
    Color c = Color::WHITE)
{
    GotoXY(x, y);
    SetColor(Color::GRAY); cout << "|";
    SetColor(c);
    int dw = StrDispW(content);
    int pad = PanelW - 2 - dw;
    cout << content;
    if (pad > 0) cout << string(pad, ' ');
    SetColor(Color::GRAY); cout << "|";
}

static void PanelBarLine(int x, int y, long long barVal, long long maxVal, Color barColor)
{
    int filled = (maxVal > 0) ? (int)std::min<long long>(BarW, BarW * barVal / maxVal) : 0;
    string ratio = " " + std::to_string(barVal) + "/" + std::to_string(maxVal) + "G";

    GotoXY(x, y);
    SetColor(Color::GRAY); cout << "|";
    SetColor();            cout << " 할당량[";
    SetColor(barColor);
    for (int i = 0; i < BarW; ++i) cout << (i < filled ? '#' : '.');
    SetColor();            cout << "]" << ratio;
    int used = 8 + BarW + 1 + (int)ratio.size();
    int pad = PanelW - 2 - used;
    if (pad > 0) cout << string(pad, ' ');
    SetColor(Color::GRAY); cout << "|";
}

static string BuildHeartStr(int remain, int max)
{
    string s = " ";
    for (int i = 0; i < max; ++i)
        s += (i < remain) ? "♥ " : "♡ ";
    return s;
}
static long long CalcNextQuota(const GameState& state)
{
    bool met = (state.player.gold >= state.dailyQuota);

    long long nextBase = CalcQuotaForWeek(state.week + 1, 10);

    long long shortfall = met ? 0 : std::max<long long>(0, state.dailyQuota - state.player.gold);
    long long carry = met ? 0 : state.carryOverQuota + shortfall;

    return nextBase + carry;
}

void QuotaScene::Init(GameState& state)
{
    SOUND->PlayBGM("Sound/e.mp3");
    state.quotaMet = (state.player.gold >= state.dailyQuota);
    PlayOpenTransition(state, .5f);
}

void QuotaScene::Update(GameState& state)
{
    state.quotaMet = (state.player.gold >= (int)state.dailyQuota);

    if (!state.quotaSubmitted && (GetKeyDown(VK_RETURN) ))
    {
        state.quotaSubmitted = true;
        PlaySubmitAnimation(state);
        PlayCloseTransition(state, .7f);

        if (state.quotaFromDayEnd && !state.quotaMet)
        {
            state.isWinEnding = false;
            state.requestEndGame = true;
            return;
        }

        state.requestNextDay = true;
        return;
    }

    if (!state.quotaFromDayEnd && GetKeyDown(VK_ESCAPE))
    {
        PlayCloseTransition(state, 1);
        SceneManager::GetInst()->ChangeScene("InGameScene", state);
    }
}

void QuotaScene::Render(const GameState& state)
{
    COORD res = GetConsoleResolution();
    int px = res.X / 2 - PanelW / 2;
    int py = res.Y / 2 - 11;
    DrawPanel(state, px, py);

    int hintY = py + 20;
    GotoXY(px, hintY);
    cout << string(PanelW, ' ');
    GotoXY(px, hintY);

    if (state.quotaSubmitted)
    {
        SetColor(Color::LIGHT_GRAY);
        cout << Centered("이미 제출했습니다.", PanelW);
    }
    else if (state.quotaFromDayEnd)
    {
        SetColor(Color::LIGHT_YELLOW);
        cout << Centered("[ENTER] 제출하고 다음날로", PanelW);
    }
    else
    {
        SetColor(Color::LIGHT_GRAY);
        cout << Centered("[ENTER] 제출 후 다음날   [ESC] 돌아가기", PanelW);
    }
    SetColor();
}

void QuotaScene::DrawPanel(const GameState& state, int px, int py) const
{
    bool met = (state.player.gold >= state.dailyQuota);
    long long barVal = std::min(state.player.gold, state.dailyQuota);
    long long remain = std::max<long long>(0, state.dailyQuota - state.player.gold);
    int  livesLeft = MaxQuotaFails - state.quotaFailCount;
    int  dayInWeek = ((state.day - 1) % DaysPerWeek) + 1;
    bool isWeekEnd = (state.day % DaysPerWeek == 0);

    Color barColor = met ? Color::LIGHT_GREEN
        : (barVal > state.dailyQuota / 2)
        ? Color::LIGHT_YELLOW : Color::LIGHT_RED;

    long long nextQ = CalcNextQuota(state);

    int row = py;

    PanelDivider(px, row++, '=');

    GotoXY(px, row++);
    SetColor(Color::GRAY); cout << "|";
    SetColor(Color::LIGHT_YELLOW);
    cout << Centered("[ 할당량 제출 ]", PanelW - 2);
    SetColor(Color::GRAY); cout << "|";

    PanelDivider(px, row++);

    {
        string weekStr = " " + std::to_string(state.week) + "주차  "
            + std::to_string(dayInWeek) + "/" + std::to_string(DaysPerWeek) + "일";
        string progStr = std::to_string(state.week) + "/" + std::to_string(TotalWeeks) + "주 ";
        int gap = PanelW - 2 - StrDispW(weekStr) - StrDispW(progStr);
        GotoXY(px, row++);
        SetColor(Color::GRAY);  cout << "|";
        SetColor(Color::WHITE); cout << weekStr << string(std::max(0, gap), ' ');
        SetColor(Color::CYAN);  cout << progStr;
        SetColor(Color::GRAY);  cout << "|";
    }

    {
        string heartStr = BuildHeartStr(livesLeft, MaxQuotaFails);
        string label = " 남은 기회: ";
        GotoXY(px, row++);
        SetColor(Color::GRAY);  cout << "|";
        SetColor(Color::WHITE); cout << label;
        SetColor(livesLeft > 1 ? Color::LIGHT_RED : Color::LIGHT_YELLOW);
        cout << heartStr;
        int pad = PanelW - 2 - StrDispW(label) - StrDispW(heartStr);
        if (pad > 0) cout << string(pad, ' ');
        SetColor(Color::GRAY);  cout << "|";
    }

    PanelDivider(px, row++);

    PanelBarLine(px, row++, barVal, state.dailyQuota, barColor);
    PanelLine(px, row++);

    PanelLine(px, row++,
        " 현재 골드  : " + std::to_string(state.player.gold) + "G",
        Color::LIGHT_YELLOW);
    PanelLine(px, row++,
        " 목표 골드  : " + std::to_string(state.dailyQuota) + "G",
        Color::CYAN);

    if (state.carryOverQuota > 0)
        PanelLine(px, row++,
            " 이월 포함  : +" + std::to_string(state.carryOverQuota) + "G",
            Color::LIGHT_VIOLET);
    else
        PanelLine(px, row++);

    if (!met)
        PanelLine(px, row++,
            " 부족       : " + std::to_string(remain) + "G",
            Color::LIGHT_RED);
    else
        PanelLine(px, row++);

    PanelDivider(px, row++);

    GotoXY(px, row++);
    SetColor(Color::GRAY); cout << "|";
    if (met)
    {
        SetColor(Color::LIGHT_GREEN);
        cout << Centered("할당량 달성!", PanelW - 2);
    }
    else
    {
        SetColor(Color::LIGHT_RED);
        cout << Centered("미달 - 기회 " + std::to_string(livesLeft) + "회 남음", PanelW - 2);
    }
    SetColor(Color::GRAY); cout << "|";

    PanelDivider(px, row++);

    {
        string nextLabel = " 다음 할당량: ";
        string nextVal = std::to_string(nextQ) + "G";
        if (!met && state.carryOverQuota + std::max<long long>(0, state.dailyQuota - state.player.gold) > 0)
            nextVal += "  (이월 포함)";
        GotoXY(px, row++);
        SetColor(Color::GRAY);       cout << "|";
        SetColor(Color::LIGHT_GRAY); cout << nextLabel;
        SetColor(met ? Color::CYAN : Color::LIGHT_RED);
        cout << nextVal;
        int pad = PanelW - 2 - StrDispW(nextLabel) - StrDispW(nextVal);
        if (pad > 0) cout << string(pad, ' ');
        SetColor(Color::GRAY);       cout << "|";
    }

    PanelDivider(px, row++, '=');
}

void QuotaScene::PlaySubmitAnimation(const GameState& state)
{
    COORD res = GetConsoleResolution();
    int cx = res.X / 2;
    int cy = res.Y / 2 + 10;
    bool met = state.quotaMet;

    for (int i = 0; i <= 8; ++i)
    {
        GotoXY(cx - 7, cy);
        SetColor(Color::LIGHT_YELLOW);
        cout << "제출 중" << string(i % 4, '.') << "        ";
        Sleep(150);
    }
    for (int i = 0; i < 6; ++i)
    {
        GotoXY(cx - 14, cy);
        SetColor((i % 2 == 0)
            ? (met ? Color::LIGHT_GREEN : Color::LIGHT_RED)
            : Color::WHITE);
        cout << (met
            ? "할당량 달성! 제출 완료!"
            : "할당량 미달 - 이월 처리됩니다");
        Sleep(250);
    }
    GotoXY(cx - 14, cy);
    SetColor(); cout << string(44, ' ');
}

void QuotaScene::PlayOpenTransition(const GameState& state, unsigned long delayMs)
{
    COORD res = GetConsoleResolution();
    int W = res.X, H = res.Y, half = H / 2;
    int colorCount = (int)Color::END;

    for (int col = 0; col < W; ++col)
    {
        Color c = (Color)std::min((int)(((float)col / W) * colorCount), colorCount - 1);
        SetColor(c, c);
        for (int y = 0; y < H; ++y) { GotoXY(col, y); cout << " "; }
        Sleep(delayMs);
    }
    SetColor();
    for (int y = 0; y < H; ++y)
    {
        int cy = (y < half) ? y : (H - 1 - y);
        Color c = (Color)std::min((int)(((float)cy / half) * colorCount), colorCount - 1);
        SetColor(c, c); GotoXY(0, y);
        for (int x = 0; x < W; ++x) cout << " ";
    }
    for (int step = 0; step < half; ++step)
    {
        GotoXY(0, half - 1 - step); for (int x = 0; x < W; ++x) cout << " ";
        GotoXY(0, half + step);     for (int x = 0; x < W; ++x) cout << " ";
        Sleep(delayMs);
    }
    SetColor(); Render(state);
}

void QuotaScene::PlayCloseTransition(const GameState& state, unsigned long delayMs)
{
    COORD res = GetConsoleResolution();
    int W = res.X, H = res.Y, half = H / 2;
    int colorCount = (int)Color::END;
    for (int step = 1; step <= half; ++step)
    {
        Color c = (Color)std::min((int)(((float)step / half) * colorCount), colorCount - 1);
        SetColor(c, c);
        GotoXY(0, step - 1); for (int x = 0; x < W; ++x) cout << " ";
        GotoXY(0, H - step); for (int x = 0; x < W; ++x) cout << " ";
        Sleep(delayMs);
    }
    SetColor();
}