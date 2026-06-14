#pragma once
struct ItemEffectContext
{
	int& coin;
};

class ICommand
{
public:
	virtual ~ICommand() = default;
	virtual void Execute(ItemEffectContext& _context) abstract;
};
