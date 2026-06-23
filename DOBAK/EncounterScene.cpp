#include "EncounterScene.h"
#include "SoundManager.h"
#include "EncounterData.h"

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