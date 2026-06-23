#pragma once
#include <string>
#include <vector>
#include "Item.h"

using std::string;
using std::vector;

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

extern vector<EncounterData> encounters;