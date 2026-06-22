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
    MultiplierGoldCommand(float _mult) : mult(_mult) {}
    void Execute(ItemEffectContext& _context) override
    {
        _context.multiplier *= mult;
    }
private:
    float mult;
};  
class CompositeCommand : public ICommand
{
public:
    CompositeCommand(std::initializer_list<ICommand*> _commands) : commands(_commands) {}
    void Execute(ItemEffectContext& _context) override
    {
        for (ICommand* cmd : commands)
            cmd->Execute(_context);
    }
private:
    std::vector<ICommand*> commands;
};
class ConsolationGoldCommand : public ICommand
{
public:
    ConsolationGoldCommand(int _amount) : amount(_amount) {}
    void Execute(ItemEffectContext& _context) override { _context.consolationGold += amount; }
private:
    int amount;
};

class SpinSpeedCommand : public ICommand
{
public:
    SpinSpeedCommand(int _ms) : ms(_ms) {}
    void Execute(ItemEffectContext& _context) override { _context.spinSpeedBonus += ms; }
private:
    int ms;
};

class ComboBonusCommand : public ICommand
{
public:
    ComboBonusCommand(int _amount) : amount(_amount) {}
    void Execute(ItemEffectContext& _context) override { _context.comboBonusPerPattern += amount; }
private:
    int amount;
};