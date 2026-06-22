#include "SlotMachineScene.h"
#include "Console.h"
#include "AsciiArt.h"
#include <vector>
#include "Pattern.h"
#include "SceneManager.h"
#include "SoundManager.h"
constexpr int SIXSEVENPOS = 30;
constexpr int SpinsPerDay = 5;
constexpr int BaseSpinMs = 1500;
constexpr int MinSpinMs = 300;

SlotMachineState slotState;
ULONGLONG lastSlotUpdateTime;
ULONGLONG lastSlotStartTime;
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

constexpr int InvX = 1;
constexpr int InvY = 3;
constexpr int CellW = 7;       
constexpr int ArtH = 3;    
constexpr int CellH = ArtH + 2; 
constexpr int InvCols = 3;      
constexpr int InvRows = 4;    
constexpr int InvMax = InvCols * InvRows;

int baseReward = 0;

static void OnSpinComplete(GameState& state)
{
    state.dailySpinCount++;
    if (state.dailySpinCount >= SpinsPerDay)
    {
        state.dailySpinCount = 0;
        state.requestNextDay = true;
    }
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

    DrawProbabilityUI(state);
}

void InGameScene::Update(GameState& state)
{
    if (GetKeyDown('B'))
        SceneManager::GetInst()->ChangeScene("ShopScene", state);
    if (GetKeyDown('G'))
        SceneManager::GetInst()->ChangeScene("QuotaScene", state);

    if (GetKeyDown(VK_SPACE) && slotState == SlotMachineState::Idle)
    {
        baseReward = 0;
        curItemEffects = CollectItemEffects(state);

        slotState = SlotMachineState::Rolling;
        lastSlotStartTime = state.curTime;
        lastSlotUpdateTime = state.curTime;
    }

    if (slotState == SlotMachineState::Rolling)
    {

        if (state.curTime - lastSlotUpdateTime >= 20)
        {
            SOUND->Play("Rolling");
            lastSlotUpdateTime = state.curTime;
            for (int i = 0; i < height; ++i)
                for (int j = 0; j < width; ++j)
                    slotArr[i][j] = rand() % 1 + 1;
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

    if (slotState == SlotMachineState::Blinking)
    {
        if (state.curTime - lastPatternBlinkTime >= 50)
        {
            if (patternBlinkCount == 1 && isPatternBlink)
            {   
                ShakeConsoleWindow(5, 100, 1);
                SOUND->Play("Pop");
            }

            lastPatternBlinkTime = state.curTime;
            isPatternBlink = !isPatternBlink;
            if (isPatternBlink)
                patternBlinkCount++;

            if (patternBlinkCount > 3)
            {
                MatchedPattern& match =
                    matchedPatterns[curPatternIndex];

                Pattern& pattern =
                    GamePatterns[match.patternIndex];

                ExecutePatternEvent(pattern, state);

                // 특수 이벤트 상태로 넘어갔다면 여기서 중단
                if (slotState == SlotMachineState::SixSeven)
                {
                    isPatternBlink = false;
                    return;
                }

                curPatternIndex++;
                patternBlinkCount = 0;
                isPatternBlink = false;

                if (curPatternIndex >= (int)matchedPatterns.size())
                {
                    state.player.gold += baseReward;
                    OnSpinComplete(state);
                    slotState = SlotMachineState::Idle;
                }
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
    DrawSixSeven();
    DrawPlusGold();
    DrawProbabilityUI(state);
}

void InGameScene::DrawPlusGold()
{
    int startX = slotX - 4;
    int startY = slotY - 5;

    // 이전 출력 잔상 제거
    SetColor();
    for (int i = 0; i < 3; ++i)
    {
        GotoXY(startX, startY + i);
        cout << "                         ";
    }

    if (slotState != SlotMachineState::Blinking)
        return;

    if (curPatternIndex >= matchedPatterns.size())
        return;

    int reward = baseReward;

    SetColor(Color::LIGHT_YELLOW);

    GotoXY(startX, startY);
    wcout << "총 획득량: +" << reward << "G";
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

void InGameScene::DrawProbabilityUI(const GameState& state)
{
    COORD res = GetConsoleResolution();

    constexpr int panelWidth = 11;
    constexpr int symbolCount = 7;

    int panelX = res.X - panelWidth;
    int panelY = 0;

    SetColor(Color::LIGHT_YELLOW);

    GotoXY(panelX, panelY);
    cout << "+=========+";

    GotoXY(panelX, panelY + 1);
    cout << "| CHANCE  |";

    SetColor(Color::WHITE);

    double probability = 100.0 / symbolCount;

    for (int number = 1; number <= symbolCount; ++number)
    {
        GotoXY(panelX, panelY + 1 + number);

        cout << "|"
            << number << ": "
            << std::fixed << std::setprecision(2)
            << probability << "%|";
    }

    SetColor(Color::LIGHT_YELLOW);

    GotoXY(panelX, panelY + symbolCount + 2);
    cout << "+=========+";

    SetColor();
    cout << std::defaultfloat;
}
void InGameScene::DrawSlotMachine()
{
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
    int total = 0;
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
                            p
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

void InGameScene::ExecutePatternEvent(const Pattern& pattern,GameState& state)
{
    switch (pattern.eventType)
    {
    case PatternEventType::Gold:
    {
        int patternReward = pattern.reward;

        patternReward += curItemEffects.coin;
        patternReward +=
            curItemEffects.comboBonusPerPattern * curPatternIndex;

        patternReward =
            (int)(patternReward * curItemEffects.multiplier);

        baseReward += patternReward;
        break;
    }

    case PatternEventType::SixSeven:
    {
        sixSevenCount = 0;
        isSixSeven = true;
        lastSixSevenMoveTime = state.curTime;

        ShakeConsoleWindow(20, 1250, 1);
        SOUND->Play("67");

        slotState = SlotMachineState::SixSeven;
        break;
    }
    }
}

bool InGameScene::IsPatternMatched(
    int startY,
    int startX,
    const Pattern& pattern)
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