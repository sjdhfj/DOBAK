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
class PatternBonusCommand : public ICommand
{
public:
    PatternBonusCommand(int w, int h, float mult = 1.0f, int flat = 0)
        : _w(w), _h(h), _mult(mult), _flat(flat) {
    }
    void Execute(ItemEffectContext& _context) override
    {
        _context.patternBonuses.push_back({ _w, _h, _mult, _flat });
    }
private:
    int   _w, _h;
    float _mult;
    int   _flat;
};

class PatternSizeBonusCommand : public ICommand
{
public:
    PatternSizeBonusCommand(int _bonusPerCell) : bonusPerCell(_bonusPerCell) {}
    void Execute(ItemEffectContext& _context) override
    {
        _context.sizeBonus += bonusPerCell;
    }
private:
    int bonusPerCell;
};