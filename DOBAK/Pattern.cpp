#include "Pattern.h"

Pattern GamePatterns[] =
{
	{ "2x1패턴", 2, 1, PatternType::Same, {},PatternEventType::Gold,1 },
	{ "1x2패턴", 1, 2, PatternType::Same, {},PatternEventType::Gold, 1},
	{ "6-7패턴", 2, 1, PatternType::Fixed, { 6, 7 },PatternEventType::SixSeven, 0 },
	{ "3x1패턴", 3, 1, PatternType::Same, {},PatternEventType::Gold, 10 },
	{ "1x3패턴", 1, 3, PatternType::Same, {},PatternEventType::Gold, 10},
	{ "6endl7", 1, 2, PatternType::Fixed, { 6, 7 },PatternEventType::SixSeven, 0 },
	{ "4x1패턴", 4, 1, PatternType::Same, {},PatternEventType::Gold, 30},
	{ "4x2패턴", 4, 2, PatternType::Same, {},PatternEventType::Gold, 350},
	{ "5x1패턴", 5, 1, PatternType::Same, {},PatternEventType::Gold, 100},
	{ "2x2패턴", 2, 2, PatternType::Same, {},PatternEventType::Gold, 50},
	{ "3x2패턴", 3, 2, PatternType::Same, {},PatternEventType::Gold, 200},
	{ "2x3패턴", 2, 3, PatternType::Same, {},PatternEventType::Gold, 200},
	{ "3x3패턴", 3, 3, PatternType::Same, {},PatternEventType::Gold, 500},
	{ "4x3패턴", 4, 3, PatternType::Same, {},PatternEventType::Gold, 2000},
	{ "5x3패턴", 5, 3, PatternType::Same, {},PatternEventType::Gold, 6767},
};


int GamePatternCount = sizeof(GamePatterns) / sizeof(GamePatterns[0]);