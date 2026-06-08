#pragma once
#include <string>
#include "Enums.h"
using std::string;

struct Item
{
	int id;
	string name;
	string description;
	int price;
	ItemType type;
};