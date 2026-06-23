#include "EncounterScene.h"
#include "SoundManager.h"

vector<EncounterData> encounters =
{
    {
        "꼬롬한 붉은 새.",
        {
            "상점으로 가는 길, 바위 위에 앉아있던 붉은 새가 당신을 노려본다.",
            "붉은 새는 당신 방향으로 고개를 기울이고 있었다.",
            "무슨 말이라도 해야 할 것 같은 분위기다."
        },
        EncounterViewType::BigAscii,
        {
            R"(   ...~.       ..               ... ~--:~         )",
            R"(   ,-:,~.    -,,.,~            ,-~~,---~;,        )",
            R"(  ..,,.,~   ---~~--,           ,-,-~,,~~;;        )",
            R"(  ,-,:,--  ,~:;;:~,:            ~,-~-,~~;!.       )",
            R"( ~~-,~--, .,:;;;;~~:.     .,-~:~~:~~~--~;!~       )",
            R"( ---,:--- .~;;;;;:~~-   .~~~~~--~!;~---~:!:       )",
            R"( -,--:~~~ ~~;:;;;~-~;  ~:~-----~;**!:~~-~;!-      )",
            R"( -,--~-~~.-::-;;:.--!-;~-------~**#$*;:~~:!!~     )",
            R"( -,--~-~~,-;~-:;,,--;*:~---,,--:*$$$$=*;~~;!;-    )",
            R"( --~-~-~~--:-;!---~,;!!:--,,,,--~;=$#$$*!~:!;;-   )",
            R"( --~-~~~---:~;:--~~,;!;::-,,,,----:*=$$$!~:;;;:   )",
            R"( --~-~::---:~:~~::-,~;:~~,,,,,,----~;*==:~~;;;;,  )",
            R"( --~-~~;:,~~~-~;!;~,,~~--,,,,,-------~::~-~;!!*-  )",
            R"( --~-~-;~~;---~:;!~,,,-,-,,,,-------~-----:!**=:  )",
            R"( --~~~~;~;;---~~;:,,,,-----------~~--~~-~;!**=$!  )",
            R"( ~-~::;;~!!~---~;:.,,,----------~,    -;!**$$$=*  )",
            R"( ~-~::::;!!;-,-~:!:,,------------  .   ;=$#$$*!!  )",
            R"( ~-::~-~;!!!-,,-~;--,----------: !##   ,#=;~~;;;. )",
            R"( ~-:~~-~;!*;~,,,,,,----------~~~ *##   .**,  ;;;. )",
            R"( ~--~--~:!*~,-,,,,~:~-------~~:~ .=!   .$$~ .~!;. )",
            R"( ~--~~--~;*- :.---:!:~----~~~~::      .,~:. .:!;. )",
            R"( ~---~~~~;*, ,:.,~*=;~~~~~~~~:::-..  .,~   .,!!!  )",
            R"( ~----~~~;*,  ,-:!=*;~~~~~~~~::::-,,,,~-...,~*!!  )",
            R"(.~--~~~~~;*,.....~=*;~~~~~~~~::::::~-,,,,,,~!*!: .)",
            R"(.~~~~~~~~;;.......!*;~~~~~~~~~~::~-,,,,,-~:;**!-..)",
            R"(.:~~~:::;*,.......-*;~~~~~~:~~~:-,,,,,,,-~;;*!;...)",
            R"(.:~~:!!!!*,,.....,,!!::~::::~::;~-,,,,,,-~:!*!,..,)",
            R"(,~:;!!!**!,,,,,,,,,~*;:::::::~-,:~-------~~!!-,,,,)",
            R"(,,~!!****-,,,,,,,,,-;!;;;;;~-----:::-----~:;-,,,,,)",
            R"(,,,:!!*!-,,--,,,,,,,~;!!!!~~-----;;:::~~~::~,,,,,,)",
            R"(-----------------~~~::;**;::~~~~-~:;;!!!;;:;------)",
            R"(----------~::;;;;;;;;;;!**;;;:::;;;;:;;!**!!::::--)",
            R"(-------~~:;;;;;;;;;;;!;;;!***!!!!!;!!!;;;;;;;;;;:-)"
        },
        {
            { "\"ㅇ..아.아 안녕..?\" 라고 말한다", EncounterChoiceResultType::Nothing, 0, Item{}, "붉은 새는 아무 말 없이 귀만 기울이고 있었다." },
            { "\"겁나 못생겼네 국그릇핑크봉구스밥버겅 ㅋ\" 라고 말한다", EncounterChoiceResultType::Gold, -100, Item{}, "화가 난 붉은 새가 100G를 물고 날아가 버렸다!" },
            { "\"우리 집에서 고양이 보고 갈래? 후훗ㅎ\" 라고 말한다", EncounterChoiceResultType::Gold, 100, Item{}, "붉은 새는 이상하게 만족한 듯 당신에게 100G를 건넸다." }
        }
    },

    {
        "수상한 상자",
        {
            "길가에 잠긴 상자가 놓여 있다.",
            "안쪽에서 달그락거리는 소리가 난다."
        },
        EncounterViewType::BigAscii,
        {
            "   +--------+   ",
            "   |  LOCK  |   ",
            "   +--------+   "
        },
        {
            { "열어본다", EncounterChoiceResultType::Gold, 100 },
            { "무시한다", EncounterChoiceResultType::Nothing }
        }
    }
};

EncounterData curEncounter;
int selectedChoice = 0;

bool isShowingResult = false;
string curResultText;
ULONGLONG resultStartTime = 0;
constexpr ULONGLONG ResultShowMs = 3000;

void EncounterScene::Init(GameState& state)
{
    SOUND->PlayBGM("Sound/l.mp3");
    SetConsoleSize(WIDTH, HEIGHT);

    selectedChoice = 0;
    isShowingResult = false;
    curResultText = "";
    resultStartTime = 0;

    int index = rand() % encounters.size();
    curEncounter = encounters[index];

    if (curEncounter.viewType == EncounterViewType::TextBox)
        DrawTextBox();

    else if (curEncounter.viewType == EncounterViewType::BigAscii)
        DrawBigAscii();

    else if (curEncounter.viewType == EncounterViewType::ShopLike)
        DrawShopLike();

    GotoXY(0, 0);
    SetColor();
    cout << "|";
    SetColor(Color::VIOLET);
    cout << "앗! 깜짝 이벤트 등장!!";
    SetColor();
    cout << "|";

    DrawChoices();
}
void EncounterScene::Update(GameState& state)
{
    if (isShowingResult)
    {
        if (state.curTime - resultStartTime >= ResultShowMs)
        {
            SceneManager::GetInst()->ChangeScene("ShopScene", state);
        }

        return;
    }

    if (GetKeyDown(VK_UP))
        selectedChoice--;

    if (GetKeyDown(VK_DOWN))
        selectedChoice++;

    if (selectedChoice < 0)
        selectedChoice = 0;

    if (selectedChoice >= (int)curEncounter.choices.size())
        selectedChoice = (int)curEncounter.choices.size() - 1;

    if (GetKeyDown(VK_RETURN) || GetKeyDown(VK_SPACE))
    {
        EncounterChoice& choice = curEncounter.choices[selectedChoice];

        ApplyChoice(choice, state);

        curResultText = choice.resultText;
        isShowingResult = true;
        resultStartTime = state.curTime;

        system("cls");
        DrawResult();
    }
}

void EncounterScene::Render(const GameState& state)
{
    if (isShowingResult)
    {
        DrawResult();
        return;
    }

    DrawChoices();
}

void EncounterScene::DrawResult()
{
    COORD res = GetConsoleResolution();

    int w = 64;
    int x = res.X / 2 - w / 2;
    int y = res.Y / 2 - 4;

    SetColor(Color::LIGHT_YELLOW);
    GotoXY(x, y);
    cout << "+" << string(w - 2, '=') << "+";

    GotoXY(x, y + 1);
    cout << "|";
    SetColor(Color::WHITE);
    cout << " 결과";
    SetColor(Color::LIGHT_YELLOW);
    cout << string(w - 7, ' ') << "|";

    GotoXY(x, y + 2);
    cout << "+" << string(w - 2, '-') << "+";

    GotoXY(x + 2, y + 4);
    SetColor(Color::LIGHT_GREEN);
    cout << curResultText;

    GotoXY(x + 2, y + 6);
    SetColor(Color::LIGHT_GRAY);
    cout << "3초 뒤 상점으로 이동합니다...";

    SetColor();
}


void EncounterScene::ApplyChoice(const EncounterChoice& choice, GameState& state)
{
    switch (choice.resultType)
    {
    case EncounterChoiceResultType::Gold:
        state.player.gold += choice.gold;
        break;

    case EncounterChoiceResultType::Item:
        state.player.inventory.push_back(choice.item);
        break;

    case EncounterChoiceResultType::Nothing:
        break;

    case EncounterChoiceResultType::GoShop:
        break;
    }
}

void EncounterScene::DrawTextBox()
{
    COORD res = GetConsoleResolution();
    int x = res.X / 2 - 30;
    int y = res.Y / 2 - 8;
    int w = 60;

    SetColor(Color::LIGHT_YELLOW);
    GotoXY(x, y);
    cout << "+" << string(w - 2, '=') << "+";

    GotoXY(x, y + 1);
    cout << "|";
    SetColor(Color::WHITE);
    cout << curEncounter.title;
    SetColor(Color::LIGHT_YELLOW);
    cout << string(w - 2 - curEncounter.title.size(), ' ') << "|";

    GotoXY(x, y + 2);
    cout << "+" << string(w - 2, '-') << "+";

    SetColor(Color::WHITE);
    for (int i = 0; i < (int)curEncounter.lines.size(); ++i)
    {
        GotoXY(x + 2, y + 4 + i);
        cout << curEncounter.lines[i];
    }

    SetColor();
}

void EncounterScene::DrawBigAscii()
{
    COORD res = GetConsoleResolution();

    int artX = 3;
    int artY = 3;

    SetColor(Color::CYAN);
    for (int i = 0; i < (int)curEncounter.asciiArt.size(); ++i)
    {
        GotoXY(artX, artY + i);
        cout << curEncounter.asciiArt[i] << "   ";
    }

    int textX = 75;
    int textY = 6;

    SetColor(Color::LIGHT_YELLOW);
    GotoXY(textX, textY);
    cout << curEncounter.title << "                    ";

    SetColor(Color::WHITE);
    for (int i = 0; i < (int)curEncounter.lines.size(); ++i)
    {
        GotoXY(textX, textY + 2 + i);
        cout << curEncounter.lines[i] << "                    ";
    }

    SetColor();
}

void EncounterScene::DrawShopLike()
{
    COORD res = GetConsoleResolution();
    int x = res.X / 2 - 35;
    int y = res.Y / 2 - 10;
    int w = 70;

    SetColor(Color::LIGHT_VIOLET);
    GotoXY(x, y);
    cout << "+" << string(w - 2, '=') << "+";

    GotoXY(x, y + 1);
    cout << "|";
    SetColor(Color::LIGHT_YELLOW);

    cout << " " << curEncounter.title;
    SetColor(Color::LIGHT_VIOLET);
    cout << string(w - 3 - curEncounter.title.size(), ' ') << "|";

    GotoXY(x, y + 2);
    cout << "+" << string(w - 2, '=') << "+";

    SetColor(Color::WHITE);
    for (int i = 0; i < (int)curEncounter.lines.size(); ++i)
    {
        GotoXY(x + 3, y + 4 + i);
        cout << curEncounter.lines[i];
    }

    SetColor();
}

void EncounterScene::DrawChoices()
{
    int x = 75;
    int y = 20;
    int clearWidth = 70;

    for (int i = 0; i < (int)curEncounter.choices.size(); ++i)
    {
        GotoXY(x, y + i * 2);
        cout << string(clearWidth, ' ');

        GotoXY(x, y + i * 2);

        if (i == selectedChoice)
        {
            SetColor(Color::LIGHT_YELLOW);
            cout << "> " << curEncounter.choices[i].text << " ";
        }
        else
        {
            SetColor(Color::WHITE);
            cout << "  " << curEncounter.choices[i].text << " ";
        }
    }

    SetColor();
}