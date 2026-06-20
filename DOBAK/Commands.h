#pragma once
#include "ICommand.h"

class AddGoldCommand : public ICommand
{
public:
    AddGoldCommand(int _amount) : amount(_amount) {}
    void Execute(ItemEffectContext& _context) override
    {
        _context.coin += amount;
    }
private:
    int amount;
//ItemEffectContext context{ state.player.item };
//
//for (Item& item : state.player.inventory)
//{
//    if (item.type == ItemType::EQUIP && item.effect)
//        item.effect->Execute(context);
//}
};
class ProbabilityCommand : public ICommand
{
public:
    ProbabilityCommand(int probability) : probability(probability) {}
    void Execute(ItemEffectContext& _context) override
    {
        _context.probability += probability;
    }
private:
    int probability;
};
class MultiplierGoldCommand : public ICommand
{
public:
    MultiplierGoldCommand(int _gold) : gold(_gold) {}
    void Execute(ItemEffectContext& _context) override
    {
        _context.coin = gold;
    }
private:
    int gold;
};
