#pragma once
#include "Scene.h"
#include "Console.h"
#include "SceneManager.h"
#include "Item.h"
#include <vector>
#include <string>

enum class EncounterViewType
{
    TextBox,
    BigAscii,
    ShopLike
};

enum class EncounterChoiceResultType
{
    Gold,
    Item,
    Nothing,
    GoShop
};

struct EncounterChoice
{
    string text;
    EncounterChoiceResultType resultType;
    int gold = 0;
    Item item;

    string resultText;
};

struct EncounterData
{
    string title;
    vector<string> lines;
    EncounterViewType viewType;
    vector<string> asciiArt;
    vector<EncounterChoice> choices;
};

class EncounterScene : public AbstractScene
{
public:
    void Init(GameState& state) override;
    void Update(GameState& state) override;
    void Render(const GameState& state) override;
    void Release() override {}

private:
    void DrawTextBox();
    void DrawBigAscii();
    void DrawShopLike();
    void DrawChoices();
    void DrawResult();
    void ApplyChoice(const EncounterChoice& choice, GameState& state);
};