#include "Pattern.h"

Pattern GamePatterns[] =
{
	{ "2x1", 2, 1, PatternType::Same, {},PatternEventType::Gold,1 },
	{ "1x2", 1, 2, PatternType::Same, {},PatternEventType::Gold, 1},
	{ "67", 2, 1, PatternType::Fixed, { 6, 7 },PatternEventType::SixSeven, 0 },
	{ "3x1", 3, 1, PatternType::Same, {},PatternEventType::Gold, 10 },
	{ "1x3", 1, 3, PatternType::Same, {},PatternEventType::Gold, 10},
	{ "6endl7", 1, 2, PatternType::Fixed, { 6, 7 },PatternEventType::SixSeven, 0 },
	{ "4x1", 4, 1, PatternType::Same, {},PatternEventType::Gold, 30},
	{ "4x2", 4, 2, PatternType::Same, {},PatternEventType::Gold, 350},
	{ "5x1", 5, 1, PatternType::Same, {},PatternEventType::Gold, 100},
	{ "2x2", 2, 2, PatternType::Same, {},PatternEventType::Gold, 50},
	{ "3x2", 3, 2, PatternType::Same, {},PatternEventType::Gold, 200},
	{ "2x3", 2, 3, PatternType::Same, {},PatternEventType::Gold, 200},
	{ "3x3", 3, 3, PatternType::Same, {},PatternEventType::Gold, 500},
	{ "4x3", 4, 3, PatternType::Same, {},PatternEventType::Gold, 2000},
	{ "5x3", 5, 3, PatternType::Same, {},PatternEventType::Gold, 6767},
	{ "61", 2, 1, PatternType::Fixed, { 6, 1 },PatternEventType::SixOne, 0 },
};


int GamePatternCount = sizeof(GamePatterns) / sizeof(GamePatterns[0]);