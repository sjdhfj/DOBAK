#pragma once
#include <string>
#include "Enums.h"
#include "ICommand.h"
using std::string;
using std::vector;

struct Item
{
	int id;
	string name;
	string description;
	int price;
	ItemType type;
	ICommand* effect = nullptr;
	vector<string> art;
};

//void RenderInventoryArt(const vector<Item>& inventory, int x, int startY)
//{
//    int y = startY;
//    for (const auto& item : inventory)
//    {
//        for (const auto& line : item.art)
//        {
//            GotoXY(x, y);
//            cout << std::setw(1) << std::left << line;
//            ++y;
//        }
//        ++y; 
//    }
//}