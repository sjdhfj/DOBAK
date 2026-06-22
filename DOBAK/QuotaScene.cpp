#include "QuotaScene.h"
#include "SoundManager.h"
void QuotaScene::Init(GameState& state)
{
    SOUND->PlayBGM("Sound/c.mp3");
    state.quotaMet = (state.player.gold >= state.dailyQuota);
    PlayOpenTransition(state, 1);
}

void QuotaScene::Update(GameState& state)
{
    state.quotaMet = (state.player.gold >= state.dailyQuota);

    if (GetKeyDown(VK_ESCAPE))
    {
        PlayCloseTransition(state, 1);
        SceneManager::GetInst()->ChangeScene("InGameScene", state);
    }
}

void QuotaScene::Render(const GameState& state)
{
    COORD res = GetConsoleResolution();
    int cx = res.X / 2;
    int cy = res.Y / 2;

    GotoXY(cx - 8, cy - 6);
    SetColor(Color::LIGHT_YELLOW);
    cout << "=== 오늘의 할당량 ===";

    GotoXY(cx - 8, cy - 4);
    SetColor(Color::LIGHT_GRAY);
    cout << state.day << "일째";

    GotoXY(cx - 15, cy - 3);
    SetColor(Color::GRAY);
    DrawLine('-', 30);

    int barVal = std::min(state.player.gold, state.dailyQuota);
    DrawBar(cx - 15, cy - 1,
        "할당량  ",
        barVal, state.dailyQuota,
        20, "■", "□");

    GotoXY(cx - 15, cy + 1);
    SetColor();
    cout << "현재 골드 : ";
    SetColor(Color::LIGHT_YELLOW);
    cout << state.player.gold << "G";

    GotoXY(cx - 15, cy + 2);
    SetColor();
    cout << "목표 골드 : ";
    SetColor(Color::CYAN);
    cout << state.dailyQuota << "G";

    GotoXY(cx - 15, cy + 4);
    if (state.quotaMet)
    {
        SetColor(Color::LIGHT_GREEN);
        cout << "★ 할당량 달성! ★              ";
    }
    else
    {
        int remain = state.dailyQuota - state.player.gold;
        SetColor(Color::LIGHT_RED);
        cout << "부족: " << remain << "G 더 필요합니다.     ";
    }

    GotoXY(cx - 15, cy + 6);
    SetColor(Color::GRAY);
    DrawLine('-', 30);

    GotoXY(cx - 10, cy + 8);
    SetColor(Color::LIGHT_GRAY);
    cout << "[ESC] 게임으로 돌아가기";

    SetColor();
}

void QuotaScene::PlayOpenTransition(const GameState& state, unsigned long delayMs)
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
        int cy = (y < half) ? y : (H - 1 - y);
        Color c = (Color)std::min((int)(((float)cy / half) * colorCount), colorCount - 1);
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

void QuotaScene::PlayCloseTransition(const GameState& state, unsigned long delayMs)
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
        GotoXY(0, step - 1);   for (int x = 0; x < W; ++x) cout << " ";
        GotoXY(0, H - step);   for (int x = 0; x < W; ++x) cout << " ";
        Sleep(delayMs);
    }
    SetColor();
}