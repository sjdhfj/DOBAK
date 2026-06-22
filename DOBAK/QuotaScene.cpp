#include "QuotaScene.h"

// ── 패널 상수 ────────────────────────────────────────────────────
constexpr int PanelW = 38;

static void PanelLine(int x, int y, const string& content = "")
{
    GotoXY(x, y);
    SetColor(Color::GRAY);
    cout << "|";
    SetColor();
    // content를 PanelW-2 칸에 맞춰 출력
    int pad = PanelW - 2 - (int)content.size();
    cout << content;
    if (pad > 0) cout << string(pad, ' ');
    SetColor(Color::GRAY);
    cout << "|";
}

static void PanelDivider(int x, int y)
{
    GotoXY(x, y);
    SetColor(Color::GRAY);
    cout << "+" << string(PanelW - 2, '-') << "+";
}

static string Centered(const string& s, int width)
{
    int total = width - (int)s.size();
    int left = total / 2;
    int right = total - left;
    return string(left, ' ') + s + string(right, ' ');
}

// ────────────────────────────────────────────────────────────────

void QuotaScene::Init(GameState& state)
{
    state.quotaMet = (state.player.gold >= state.dailyQuota);
    PlayOpenTransition(state, 1);
}

void QuotaScene::Update(GameState& state)
{
    state.quotaMet = (state.player.gold >= state.dailyQuota);

    bool canSubmit = !state.quotaSubmitted;

    if (canSubmit && (GetKeyDown(VK_RETURN) || GetKeyDown(VK_SPACE)))
    {
        state.quotaSubmitted = true;
        PlaySubmitAnimation(state);
        PlayCloseTransition(state, 1);
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
    int py = res.Y / 2 - 9;

    DrawPanel(state);

    // ── 안내 문구 ────────────────────────────────────────────────
    int hintY = py + 17;
    GotoXY(px, hintY);
    SetColor(Color::GRAY);
    cout << string(PanelW, ' ');

    GotoXY(px, hintY);
    if (state.quotaSubmitted)
    {
        SetColor(Color::LIGHT_GRAY);
        cout << Centered("이미 오늘 제출했습니다.", PanelW);
    }
    else if (state.quotaFromDayEnd)
    {
        SetColor(Color::LIGHT_YELLOW);
        cout << Centered("[ENTER] 제출하고 다음날로", PanelW);
    }
    else
    {
        SetColor(Color::LIGHT_GRAY);
        cout << Centered("[ENTER] 제출   [ESC] 돌아가기", PanelW);
    }
    SetColor();
}

void QuotaScene::DrawPanel(const GameState& state) const
{
    COORD res = GetConsoleResolution();
    int px = res.X / 2 - PanelW / 2;
    int py = res.Y / 2 - 9;
    int row = py;

    bool met = (state.player.gold >= state.dailyQuota);
    int  remain = state.dailyQuota - state.player.gold;
    int  daysUntilIncrease = state.quotaIncreaseDay
        - (state.day % state.quotaIncreaseDay);

    // ── 상단 ────────────────────────────────────────────────────
    GotoXY(px, row++);
    SetColor(Color::GRAY);
    cout << "+" << string(PanelW - 2, '=') << "+";

    // 타이틀
    GotoXY(px, row++);
    SetColor(Color::GRAY); cout << "|";
    SetColor(Color::LIGHT_YELLOW);
    cout << Centered("[ 오늘의 할당량 제출 ]", PanelW - 2);
    SetColor(Color::GRAY); cout << "|";

    PanelDivider(px, row++);

    // 날짜 + 할당량 상승 예고
    {
        string dayStr = " " + std::to_string(state.day) + "일째";
        string riseStr = "다음 상승까지 " + std::to_string(daysUntilIncrease) + "일 ";
        // 두 문자열을 한 줄에
        int gap = PanelW - 2 - (int)dayStr.size() - (int)riseStr.size();
        GotoXY(px, row++);
        SetColor(Color::GRAY);    cout << "|";
        SetColor(Color::WHITE);   cout << dayStr;
        cout << string(std::max(0, gap), ' ');
        SetColor(Color::CYAN);    cout << riseStr;
        SetColor(Color::GRAY);    cout << "|";
    }

    PanelDivider(px, row++);

    // ── 할당량 바 ───────────────────────────────────────────────
    {
        constexpr int BarW = 18;
        int barVal = std::min(state.player.gold, state.dailyQuota);
        int filled = BarW * barVal / std::max((ULONGLONG)1, state.dailyQuota);
        Color barColor = met ? Color::LIGHT_GREEN
            : (barVal > state.dailyQuota * 0.5f)
            ? Color::LIGHT_YELLOW : Color::LIGHT_RED;

        // 바 그리기
        GotoXY(px, row++);
        SetColor(Color::GRAY); cout << "|";
        SetColor();            cout << " 할당량 ";
        SetColor(barColor);
        for (int i = 0; i < BarW; ++i)
            cout << (i < filled ? "■" : "□");
        SetColor();
        string ratio = " " + std::to_string(barVal)
            + "/" + std::to_string(state.dailyQuota) + "G";
        cout << ratio;
        int pad = PanelW - 2 - 8 - BarW - (int)ratio.size();
        if (pad > 0) cout << string(pad, ' ');
        SetColor(Color::GRAY); cout << "|";
    }

    PanelLine(px, row++);

    // 현재/목표/부족
    {
        string cur = " 현재 골드  : " + std::to_string(state.player.gold) + "G";
        string goal = " 목표 골드  : " + std::to_string(state.dailyQuota) + "G";

        GotoXY(px, row++);
        SetColor(Color::GRAY);         cout << "|";
        SetColor(Color::LIGHT_YELLOW); cout << cur;
        cout << string(PanelW - 2 - (int)cur.size(), ' ');
        SetColor(Color::GRAY);         cout << "|";

        GotoXY(px, row++);
        SetColor(Color::GRAY); cout << "|";
        SetColor(Color::CYAN); cout << goal;
        cout << string(PanelW - 2 - (int)goal.size(), ' ');
        SetColor(Color::GRAY); cout << "|";

        if (!met)
        {
            string lack = " 부족       : " + std::to_string(remain) + "G";
            GotoXY(px, row++);
            SetColor(Color::GRAY);     cout << "|";
            SetColor(Color::LIGHT_RED); cout << lack;
            cout << string(PanelW - 2 - (int)lack.size(), ' ');
            SetColor(Color::GRAY);     cout << "|";
        }
        else
        {
            PanelLine(px, row++);
        }
    }

    PanelDivider(px, row++);

    // ── 달성 배너 ───────────────────────────────────────────────
    GotoXY(px, row++);
    SetColor(Color::GRAY); cout << "|";
    if (met)
    {
        SetColor(Color::LIGHT_GREEN);
        cout << Centered("★  할당량 달성!  ★", PanelW - 2);
    }
    else
    {
        SetColor(Color::LIGHT_RED);
        cout << Centered("✗  할당량 미달성  ✗", PanelW - 2);
    }
    SetColor(Color::GRAY); cout << "|";

    // ── 하단 ────────────────────────────────────────────────────
    GotoXY(px, row++);
    SetColor(Color::GRAY);
    cout << "+" << string(PanelW - 2, '=') << "+";

    SetColor();
}

void QuotaScene::PlaySubmitAnimation(const GameState& state)
{
    COORD res = GetConsoleResolution();
    int cx = res.X / 2;
    int cy = res.Y / 2 + 7;   // 패널 하단 바로 밑
    bool met = (state.player.gold >= state.dailyQuota);

    // "제출 중..." 점 애니메이션
    for (int i = 0; i <= 8; ++i)
    {
        GotoXY(cx - 7, cy);
        SetColor(Color::LIGHT_YELLOW);
        string dots(i % 4, '.');
        cout << "제출 중" << dots << "        ";
        Sleep(150);
    }

    // 결과 플래시
    for (int i = 0; i < 6; ++i)
    {
        GotoXY(cx - 12, cy);
        if (i % 2 == 0)
            SetColor(met ? Color::LIGHT_GREEN : Color::LIGHT_RED,
                met ? Color::GREEN : Color::RED);
        else
            SetColor();

        if (met) cout << "  ★★  할당량 달성! 제출 완료!  ★★  ";
        else     cout << "    ✗  할당량 미달 — 제출 완료    ✗  ";
        Sleep(250);
    }

    GotoXY(cx - 12, cy);
    SetColor();
    cout << string(36, ' ');
    Sleep(200);
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
        SetColor(c, c);
        GotoXY(0, y);
        for (int x = 0; x < W; ++x) cout << " ";
    }
    for (int step = 0; step < half; ++step)
    {
        GotoXY(0, half - 1 - step); for (int x = 0; x < W; ++x) cout << " ";
        GotoXY(0, half + step);     for (int x = 0; x < W; ++x) cout << " ";
        Sleep(delayMs);
    }
    SetColor();
    Render(state);
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