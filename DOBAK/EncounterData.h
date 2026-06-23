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
    GoldPercent, //일단 Gold없이 이것만 쓸 예정
    Item,
    Nothing,
    GoShop
};

struct EncounterChoice
{
    string text;
    EncounterChoiceResultType resultType;
    int gold = 0;
    int goldPercent = 0;
    Item item;

    string resultText;
    string percentResultPrefix;
    string percentResultSuffix;
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