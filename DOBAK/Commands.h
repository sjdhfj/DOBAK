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
//ItemEffectContext context{ state.player.gold };
//
//for (Item& item : state.player.inventory)
//{
//    if (item.type == ItemType::EQUIP && item.effect)
//        item.effect->Execute(context);
//}
};