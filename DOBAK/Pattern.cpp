#include "Pattern.h"

Pattern GamePatterns[] =
{
	{ "3x1", 3, 1, 10 },
	{ "4x1", 4, 1, 30 },
	{ "5x1", 5, 1, 100 },
	{ "2x2", 2, 2, 50 },
	{ "3x3", 3, 3, 200 },
};

int GamePatternCount = sizeof(GamePatterns) / sizeof(GamePatterns[0]);