#include "Pattern.h"

Pattern GamePatterns[] =
{
	{ "3x1", 3, 1, 10 },
	{ "1x3", 1, 3, 10 },
	{ "4x1", 4, 1, 30 },
	{ "4x2", 4, 2, 350 },
	{ "5x1", 5, 1, 100 },
	{ "2x2", 2, 2, 50 },
	{ "3x2", 3, 2, 200 },
	{ "2x3", 2, 3, 200 },
	{ "3x3", 3, 3, 500 },
	{ "4x3", 4, 3, 2000 },
	{ "5x3", 5, 3, 6767 },
};


int GamePatternCount = sizeof(GamePatterns) / sizeof(GamePatterns[0]);