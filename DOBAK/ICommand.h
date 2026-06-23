#pragma once
#include <vector>
struct PatternBonus
{
    int   targetWidth;
    int   targetHeight;
    float multiplier = 1.0f;
    int   flatBonus = 0;
};
struct ItemEffectContext
{
    int   coin = 0;
    float multiplier = 1.0f;
    int   consolationGold = 0;
    int   spinSpeedBonus = 0;
    int   comboBonusPerPattern = 0;
    int   sizeBonus = 0;
    std::vector<PatternBonus> patternBonuses;
};

class ICommand
{
public:
    virtual ~ICommand() = default;
    virtual void Execute(ItemEffectContext& _context) = 0;
};