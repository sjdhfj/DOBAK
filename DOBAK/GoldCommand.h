#pragma once
#include "ICommand.h"
class GoldCommand : public ICommand
{
public:
    GoldCommand(int amount) : amount(amount) {}
    void Execute(ItemEffectContext& _context) override
    {
        _context.coin += amount;
    }
private:
    int amount;
};