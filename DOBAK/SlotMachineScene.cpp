#include <iomanip>
#include <string>
#include <vector>
#include "SlotMachineScene.h"
#include "Console.h"
#include "AsciiArt.h"
#include <algorithm>
#include "Pattern.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "Cheater.h"

constexpr int SIXSEVENPOS = 30;
constexpr int SpinsPerDay = 5;
constexpr int BaseSpinMs = 1500;
constexpr int MinSpinMs = 300;
constexpr int ResultBlinkIntervalMs = 150;
constexpr int ResultBlinkTimes = 3;

SlotMachineState slotState;
ULONGLONG lastSlotUpdateTime;
ULONGLONG lastSlotStartTime;
ULONGLONG rollingSoundLastTime;
int width = 5, height = 3;
int** slotArr = new int* [height];
int slotX = 0, slotY = 0;

AsciiObjs asciiArts;
int titleX;
int titleY;

int sixSevenMoveValue = 3;
ULONGLONG lastSixSevenMoveTime;
int  sixSevenCount = 0;
bool isSixSeven = false;

vector<MatchedPattern> matchedPatterns;
int  curPatternIndex = 0;
int  patternBlinkCount = 0;
bool isPatternBlink = false;
ULONGLONG lastPatternBlinkTime = 0;

ItemEffectContext curItemEffects{};

static Cheater cheater;

constexpr int InvX = 1;
constexpr int InvY = 3;
constexpr int CellW = 7;
constexpr int ArtH = 3;
constexpr int CellH = ArtH + 2;
constexpr int InvCols = 3;
constexpr int InvRows = 4;
constexpr int InvMax = InvCols * InvRows;
constexpr int DaysPerWeek = 3;

long long baseReward = 0;

bool isResultBlinking = false;
bool isResultBlinkYellow = false;
int resultBlinkCount = 0;
ULONGLONG lastResultBlinkTime = 0;
bool pendingSixSevenEvent = false;

constexpr int MaxPatternBlinkIntervalMs = 60;
constexpr int MinPatternBlinkIntervalMs = 20;
constexpr int PatternBlinkSpeedStepMs = 5;

static void OnSpinComplete(GameState& state)
{
    state.dailySpinCount++;
    if (state.dailySpinCount >= SpinsPerDay)
    {
        state.dailySpinCount = 0;
        bool isWeekEndDay = (state.day % DaysPerWeek == 0); // day++ 전, 끝낸 날 기준
        state.day++;

        if (isWeekEndDay)
        {
            state.quotaFromDayEnd = true;
            state.requestQuotaCheck = true;
        }
        else
        {
            state.requestNextDay = true;
        }
    }
}

static string BuildEffectNotice(const ItemEffectContext& ctx,
    int pw, int ph, long long thisReward)
{
    vector<string> parts;
    char buf[48]{};

    for (const auto& pb : ctx.patternBonuses)
    {
        if (pb.targetWidth == pw && pb.targetHeight == ph)
        {
            if (pb.multiplier != 1.0f)
            {
                snprintf(buf, sizeof(buf), "%dx%d x%.0f배", pw, ph, pb.multiplier);
                parts.push_back(buf);
            }
            if (pb.flatBonus != 0)
            {
                snprintf(buf, sizeof(buf), "%dx%d +%dG", pw, ph, pb.flatBonus);
                parts.push_back(buf);
            }
        }
    }
    if (ctx.multiplier != 1.0f)
    {
        snprintf(buf, sizeof(buf), "배율 x%.2f", ctx.multiplier);
        parts.push_back(buf);
    }
    if (ctx.coin != 0)
    {
        snprintf(buf, sizeof(buf), "고정 %+dG", ctx.coin);
        parts.push_back(buf);
    }
    if (ctx.comboBonusPerPattern != 0)
    {
        snprintf(buf, sizeof(buf), "콤보 +%dG", ctx.comboBonusPerPattern);
        parts.push_back(buf);
    }
    if (ctx.sizeBonus != 0)
    {
        snprintf(buf, sizeof(buf), "칸당 +%dG(%dx%d)", ctx.sizeBonus, pw, ph);
        parts.push_back(buf);
    }

    if (parts.empty()) return "";

    string result = "[효과 발동] ";
    for (int i = 0; i < (int)parts.size(); ++i)
    {
        if (i > 0) result += " / ";
        result += parts[i];
    }
    snprintf(buf, sizeof(buf), "  -> +%lldG", thisReward);
    result += buf;
    return result;
}

void InGameScene::Init(GameState& state)
{
    SOUND->PlayBGM("Sound/a.mp3");
    SetConsoleSize(WIDTH, HEIGHT);
    slotState = SlotMachineState::Idle;
    AsciiInit(asciiArts);
    srand((unsigned int)time(nullptr));

    COORD res = GetConsoleResolution();
    titleX = (res.X - 25) / 2;
    titleY = res.Y / 4;

    for (int i = 0; i < height; ++i)
        slotArr[i] = new int[width];

    slotX = titleX + 7;
    slotY = titleY + 2;

    lastSixSevenMoveTime = state.curTime;

    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            slotArr[i][j] = 0;

    _lastEffectStr = "";
    DrawPatternValuePanel(state);
}

void InGameScene::Update(GameState& state)
{
    cheater.Update(state);
    if (GetKeyDown('G') && slotState == SlotMachineState::Idle && !isSixSeven)
        SceneManager::GetInst()->ChangeScene("QuotaScene", state);
    if (GetKeyDown('B') && slotState == SlotMachineState::Idle && !isSixSeven)
        SceneManager::GetInst()->ChangeScene("ShopScene", state);

    if (GetKeyDown(VK_SPACE) && slotState == SlotMachineState::Idle)
    {
        baseReward = 0;
        pendingSixSevenEvent = false;
        isResultBlinking = false;
        isResultBlinkYellow = false;
        resultBlinkCount = 0;
        _lastEffectStr = "";
        curItemEffects = CollectItemEffects(state);

        slotState = SlotMachineState::Rolling;
        lastSlotStartTime = state.curTime;
        lastSlotUpdateTime = state.curTime;
        rollingSoundLastTime = state.curTime;
    }

    if (slotState == SlotMachineState::Rolling)
    {
        if (state.curTime - lastSlotUpdateTime >= 20)
        {
            if (state.curTime - rollingSoundLastTime >= 50)
            {
                rollingSoundLastTime = state.curTime;
                SOUND->Play("Rolling");
            }

            lastSlotUpdateTime = state.curTime;
            for (int i = 0; i < height; ++i)
                for (int j = 0; j < width; ++j)
                    slotArr[i][j] = rand() % 7 + 1;
        }

        int spinMs = std::max(MinSpinMs, BaseSpinMs - curItemEffects.spinSpeedBonus);

        if (state.curTime - lastSlotStartTime >= (ULONGLONG)spinMs)
        {
            FindMatchedPatterns();

            if (matchedPatterns.empty())
            {
                if (curItemEffects.consolationGold > 0)
                    state.player.gold += curItemEffects.consolationGold;

                OnSpinComplete(state);
                slotState = SlotMachineState::Idle;
            }
            else
            {
                curPatternIndex = 0;
                patternBlinkCount = 0;
                isPatternBlink = false;
                lastPatternBlinkTime = state.curTime;
                slotState = SlotMachineState::Blinking;
            }
        }
    }

    if (slotState == SlotMachineState::Blinking && !isResultBlinking)
    {
        int patternBlinkIntervalMs =
            MaxPatternBlinkIntervalMs - ((int)matchedPatterns.size() - 1) * PatternBlinkSpeedStepMs;

        patternBlinkIntervalMs = std::max(MinPatternBlinkIntervalMs, patternBlinkIntervalMs);

        if (state.curTime - lastPatternBlinkTime >= (ULONGLONG)patternBlinkIntervalMs)
        {
            lastPatternBlinkTime = state.curTime;

            isPatternBlink = !isPatternBlink;

            if (isPatternBlink)
            {
                patternBlinkCount++;

                if (patternBlinkCount == 1)
                {
                    MatchedPattern& match = matchedPatterns[curPatternIndex];

                    if (!match.rewardApplied)
                    {
                        Pattern& pattern = GamePatterns[match.patternIndex];
                        ExecutePatternEvent(pattern, state);
                        match.rewardApplied = true;
                    }

                    ShakeConsoleWindow(5, 100, 1);
                    SOUND->Play("Pop");
                }
            }

            if (patternBlinkCount > 3)
            {
                curPatternIndex++;
                patternBlinkCount = 0;
                isPatternBlink = false;

                if (curPatternIndex >= (int)matchedPatterns.size())
                {
                    isResultBlinking = true;
                    isResultBlinkYellow = false;
                    resultBlinkCount = 0;
                    lastResultBlinkTime = state.curTime;
                }
            }
        }
    }

    if (isResultBlinking &&
        state.curTime - lastResultBlinkTime >= ResultBlinkIntervalMs)
    {
        lastResultBlinkTime = state.curTime;
        isResultBlinkYellow = !isResultBlinkYellow;

        if (isResultBlinkYellow)
        {
            resultBlinkCount++;
            SOUND->Play("SlotEmission");
        }

        if (resultBlinkCount >= ResultBlinkTimes && !isResultBlinkYellow)
        {
            isResultBlinking = false;
            _lastEffectStr = "";
            state.player.gold += std::max<long long>(0, baseReward);
            SOUND->Play("SlotMoney");

            if (pendingSixSevenEvent)
            {
                sixSevenCount = 0;
                isSixSeven = true;
                lastSixSevenMoveTime = state.curTime;
                ShakeConsoleWindow(20, 1250, 1);
                SOUND->Play("67");
                slotState = SlotMachineState::SixSeven;
            }
            else
            {
                OnSpinComplete(state);
                slotState = SlotMachineState::Idle;
            }
        }
    }

    if (slotState == SlotMachineState::SixSeven)
    {
        if (sixSevenCount > 10)
        {
            isSixSeven = false;
            ClearSixSeven();
            OnSpinComplete(state);
            slotState = SlotMachineState::Idle;
        }
    }

    if (isSixSeven && state.curTime - lastSixSevenMoveTime >= 100)
    {
        ClearSixSeven();
        lastSixSevenMoveTime = state.curTime;
        sixSevenMoveValue *= -1;
        sixSevenCount++;
    }

    UpdateShakeConsoleWindow();
}

void InGameScene::Render(const GameState& state)
{
    GotoXY(0, 0);
    DrawUI(state);
    DrawInventory(state);
    DrawSlotMachine();
    DrawSlotNumbers();
    DrawEffectNotice();
    DrawSixSeven();
    DrawPlusGold();
    DrawPatternValuePanel(state);
    DrawDayInfo(state);

    COORD res = GetConsoleResolution();
    int cheatY = res.Y - 1;
    GotoXY(0, cheatY);
    cout << string(30, ' ');
    if (cheater.IsActive())
    {
        SetColor(Color::LIGHT_RED);
        GotoXY(0, cheatY);
        cout << "[Cheater 활성화중]";
        SetColor();
    }
}

void InGameScene::DrawEffectNotice()
{
    int noticeX = titleX;
    int noticeY = titleY + (int)asciiArts.slotMachine.size() + 3;

    GotoXY(noticeX, noticeY);
    cout << string(60, ' ');

    if (_lastEffectStr.empty()) return;

    /*if (slotState == SlotMachineState::Blinking && isPatternBlink)
        SetColor(Color::BLACK, _lastEffectColor);
    else*/
        SetColor(Color::CYAN);

    GotoXY(noticeX, noticeY);
    cout << " " << _lastEffectStr << " ";
    SetColor();
}

void InGameScene::DrawPlusGold()
{
    int startX = slotX - 4;
    int startY = slotY - 5;

    SetColor();
    for (int i = 0; i < 3; ++i)
    {
        GotoXY(startX, startY + i);
        cout << "                                                                                                                              ";
    }

    if (slotState != SlotMachineState::Blinking)
        return;

    if (baseReward <= 0)
        return;

    GotoXY(startX, startY);
    SetColor(Color::WHITE);
    cout << "획득량: +";

    if (isResultBlinking && isResultBlinkYellow)
        SetColor(Color::YELLOW);
    else
        SetColor(Color::WHITE);

    cout << baseReward << "G   ";
    SetColor();
}

void InGameScene::DrawUI(const GameState& state)
{
    GotoXY(0, 0);
    SetColor();
    cout << "Coin: " << state.player.gold << "G   ";

    GotoXY(0, 1);
    SetColor(Color::LIGHT_GRAY);
    cout << "스핀: " << state.dailySpinCount << " / " << SpinsPerDay << "  ";
}

void InGameScene::DrawInventory(const GameState& state)
{
    GotoXY(InvX, InvY);
    SetColor(Color::LIGHT_YELLOW);
    cout << "[ 보유 아이템 ]";

    GotoXY(InvX, InvY + 1);
    SetColor(Color::GRAY);
    DrawLine('-', CellW * InvCols);

    const auto& inv = state.player.inventory;
    int count = std::min((int)inv.size(), InvMax);

    for (int i = 0; i < InvMax; ++i)
    {
        int col = i % InvCols;
        int row = i / InvCols;
        int x = InvX + col * CellW;
        int y = InvY + 2 + row * CellH;

        if (i < count)
        {
            const Item& item = inv[i];

            for (int a = 0; a < ArtH; ++a)
            {
                GotoXY(x, y + a);
                SetColor(Color::CYAN);
                if (a < (int)item.art.size())
                {
                    string line = item.art[a];
                    if ((int)line.size() >= CellW)
                        line = line.substr(0, CellW);
                    cout << std::left << std::setw(CellW) << line;
                }
                else
                {
                    cout << string(CellW, ' ');
                }
            }

            GotoXY(x, y + ArtH);
            SetColor(Color::WHITE);
            const string& name = item.name;
            string clipped;
            int dispW = 0;
            for (int c = 0; c < (int)name.size(); )
            {
                unsigned char ch = (unsigned char)name[c];
                int cw = (ch >= 0x81 && ch <= 0xFE) ? 2 : 1;
                if (dispW + cw > CellW) break;
                clipped += name.substr(c, cw);
                dispW += cw;
                c += cw;
            }
            cout << clipped;
            int remain = CellW - dispW;
            if (remain > 0) cout << string(remain, ' ');

            GotoXY(x, y + ArtH + 1);
            cout << string(CellW, ' ');
        }
        else
        {
            for (int a = 0; a < CellH; ++a)
            {
                GotoXY(x, y + a);
                cout << string(CellW, ' ');
            }
        }
    }

    int bottomY = InvY + 2 + InvRows * CellH;
    GotoXY(InvX, bottomY);
    SetColor(Color::GRAY);
    DrawLine('-', CellW * InvCols);

    GotoXY(InvX, bottomY + 1);
    SetColor(Color::LIGHT_GRAY);
    cout << "총 " << (int)inv.size() << "개  ";
    SetColor();
}

void InGameScene::DrawPatternValuePanel(const GameState& state)
{
    COORD res = GetConsoleResolution();

    constexpr int panelWidth = 22;
    int panelX = res.X - panelWidth;
    int panelY = 0;

    SetColor(Color::LIGHT_YELLOW);
    GotoXY(panelX, panelY);
    cout << "+" << string(panelWidth - 2, '=') << "+";

    GotoXY(panelX, panelY + 1);
    cout << "|  PATTERN VALUE   |";

    GotoXY(panelX, panelY + 2);
    cout << "+" << string(panelWidth - 2, '=') << "+";

    for (int i = 0; i < GamePatternCount; ++i)
    {
        const Pattern& pattern = GamePatterns[i];

        string valueText;
        if (pattern.eventType == PatternEventType::SixSeven) valueText = "x2 EVENT";
        else if (pattern.eventType == PatternEventType::SixOne)   valueText = "RESET";
        else                                                        valueText = std::to_string(pattern.reward) + "G";

        int maxContent = panelWidth - 4;
        string name = pattern.patternName;
        string sep = ":";
        int valW = (int)valueText.size();
        int nameW = maxContent - valW - 1;
        if (nameW < 1) nameW = 1;
        if ((int)name.size() > nameW)
            name = name.substr(0, nameW);

        int gap = maxContent - (int)name.size() - 1 - valW;
        string line = name + string(std::max(0, gap), ' ') + sep + valueText;

        GotoXY(panelX, panelY + 3 + i);
        SetColor(pattern.eventType == PatternEventType::Gold ? Color::WHITE : Color::LIGHT_YELLOW);
        cout << "| " << std::left << std::setw(maxContent) << line << " |";
    }

    SetColor(Color::LIGHT_YELLOW);
    GotoXY(panelX, panelY + 3 + GamePatternCount);
    cout << "+" << string(panelWidth - 2, '=') << "+";
    SetColor();
}

void InGameScene::DrawSlotMachine()
{
    if (isResultBlinking && isResultBlinkYellow)
        SetColor(Color::YELLOW);
    else
        SetColor();
    for (int i = 0; i < (int)asciiArts.slotMachine.size(); ++i)
    {
        GotoXY(titleX, titleY + i);
        cout << asciiArts.slotMachine[i];
    }
}

void InGameScene::DrawSlotNumbers()
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            GotoXY(slotX + j * 2, slotY + i);
            if (IsCurrentPatternCell(i, j))
                SetColor(Color::WHITE, Color::YELLOW);
            else
                SetColor();
            cout << slotArr[i][j];
            SetColor();
            cout << ' ';
        }
    }
}

void InGameScene::DrawSixSeven()
{
    if (!isSixSeven) return;
    COORD res = GetConsoleResolution();
    SetUniCodeMode();

    SetColor(Color::BLUE);
    for (int i = 0; i < (int)asciiArts.six.size(); ++i)
    {
        GotoXY(SIXSEVENPOS, 6 + i + sixSevenMoveValue);
        wcout << asciiArts.six[i];
    }

    SetColor(Color::RED);
    for (int i = 0; i < (int)asciiArts.seven.size(); ++i)
    {
        GotoXY(res.X - SIXSEVENPOS - (int)asciiArts.seven[0].length(),
            6 + i - sixSevenMoveValue);
        wcout << asciiArts.seven[i];
    }

    SetColor();
    SetDefaultMode();
}

void InGameScene::ClearSixSeven()
{
    SetUniCodeMode();
    COORD res = GetConsoleResolution();
    SetColor();
    for (int i = 0; i < (int)asciiArts.six.size() + 2; ++i)
    {
        GotoXY(SIXSEVENPOS, 5 + i + sixSevenMoveValue);
        wcout << L"                         ";
    }
    for (int i = 0; i < (int)asciiArts.seven.size() + 2; ++i)
    {
        GotoXY(res.X - SIXSEVENPOS - (int)asciiArts.seven[0].length(),
            5 + i - sixSevenMoveValue);
        wcout << L"                       ";
    }
    SetDefaultMode();
}

bool InGameScene::IsCurrentPatternCell(int y, int x)
{
    if (slotState != SlotMachineState::Blinking)         return false;
    if (!isPatternBlink)                                  return false;
    if (curPatternIndex >= (int)matchedPatterns.size())   return false;

    MatchedPattern& match = matchedPatterns[curPatternIndex];
    return y >= match.startY && y < match.startY + match.height &&
        x >= match.startX && x < match.startX + match.width;
}

int InGameScene::CheckPatternReward(const Pattern& pattern)
{
    long long total = 0;
    for (int y = 0; y <= height - pattern.height; ++y)
        for (int x = 0; x <= width - pattern.width; ++x)
            if (IsSameInArea(y, x, pattern.width, pattern.height))
                total += pattern.reward;
    return total;
}

void InGameScene::FindMatchedPatterns()
{
    matchedPatterns.clear();
    for (int p = 0; p < GamePatternCount; ++p)
    {
        Pattern& pattern = GamePatterns[p];
        for (int y = 0; y <= height - pattern.height; ++y)
            for (int x = 0; x <= width - pattern.width; ++x)
                if (IsPatternMatched(y, x, pattern))
                {
                    matchedPatterns.push_back(
                        {
                            y,
                            x,
                            pattern.width,
                            pattern.height,
                            pattern.reward,
                            p,
                            false
                        });
                }
    }
}

bool InGameScene::IsSameInArea(int startY, int startX, int pw, int ph)
{
    int first = slotArr[startY][startX];
    for (int y = 0; y < ph; ++y)
        for (int x = 0; x < pw; ++x)
            if (slotArr[startY + y][startX + x] != first)
                return false;
    return true;
}

ItemEffectContext InGameScene::CollectItemEffects(GameState& state)
{
    ItemEffectContext ctx{};
    ctx.multiplier = 1.0f;

    for (Item& item : state.player.inventory)
        if (item.type == ItemType::EQUIP && item.effect)
            item.effect->Execute(ctx);

    return ctx;
}

void InGameScene::ExecutePatternEvent(const Pattern& pattern, GameState& state)
{
    switch (pattern.eventType)
    {
    case PatternEventType::Gold:
    {
        long long patternReward = pattern.reward;
        patternReward += curItemEffects.coin;
        patternReward += (long long)curItemEffects.comboBonusPerPattern * curPatternIndex;

        patternReward += (long long)curItemEffects.sizeBonus * pattern.width * pattern.height;

        float patternMult = 1.0f;
        for (const auto& pb : curItemEffects.patternBonuses)
        {
            if (pb.targetWidth == pattern.width && pb.targetHeight == pattern.height)
            {
                patternMult *= pb.multiplier;
                patternReward += pb.flatBonus;
            }
        }

        float globalMult = curItemEffects.multiplier;
        if (std::isnan(globalMult) || globalMult <= 0.0f) globalMult = 1.0f;
        else if (!std::isfinite(globalMult)) globalMult = 67.0f;
        else globalMult = std::min(globalMult, 67.0f);

        if (std::isnan(patternMult) || patternMult <= 0.0f) patternMult = 1.0f;
        else if (!std::isfinite(patternMult)) patternMult = 67.0f;
        else patternMult = std::min(patternMult, 67.0f);

        patternReward = (long long)(patternReward * patternMult);
        patternReward = (long long)(patternReward * globalMult);
        patternReward = std::max<long long>(pattern.reward, patternReward);

        baseReward += patternReward;

        _lastEffectStr = BuildEffectNotice(curItemEffects, pattern.width, pattern.height, patternReward);
        if (patternReward >= 500) _lastEffectColor = Color::LIGHT_YELLOW;
        else if (patternReward >= 100) _lastEffectColor = Color::LIGHT_GREEN;
        else                           _lastEffectColor = Color::CYAN;
        break;
    }

    case PatternEventType::SixSeven:
    {
        baseReward *= 2;
        pendingSixSevenEvent = true;
        break;
    }
    case PatternEventType::SixOne:
    {
        baseReward = 0;
        break;
    }
    }
}
void InGameScene::DrawDayInfo(const GameState& state)
{
    COORD res = GetConsoleResolution();
    string dayStr = std::to_string(state.day) + "일째";
    int x = res.X - (int)dayStr.size() - 1;
    int y = res.Y - 2;

    GotoXY(x, y);
    cout << string(15, ' ');
    GotoXY(x, y);
    SetColor(Color::LIGHT_GRAY);
    cout << dayStr;
    SetColor();
}
bool InGameScene::IsPatternMatched(int startY, int startX, const Pattern& pattern)
{
    if (pattern.type == PatternType::Same)
    {
        return IsSameInArea(
            startY,
            startX,
            pattern.width,
            pattern.height
        );
    }

    if (pattern.type == PatternType::Fixed)
    {
        for (int y = 0; y < pattern.height; ++y)
        {
            for (int x = 0; x < pattern.width; ++x)
            {
                int valueIndex = y * pattern.width + x;
                int slotValue = slotArr[startY + y][startX + x];

                if (slotValue != pattern.values[valueIndex])
                    return false;
            }
        }

        return true;
    }

    return false;
}