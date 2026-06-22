#pragma once
#include <vector>

struct ItemEffectContext
{
    int   coin = 0;
    int   probability = 0;
    float multiplier = 1.0f;
    int   consolationGold = 0;
    int   spinSpeedBonus = 0;
    int   comboBonusPerPattern = 0;
};

class ICommand
{
public:
    virtual ~ICommand() = default;
    virtual void Execute(ItemEffectContext& _context) = 0;
};