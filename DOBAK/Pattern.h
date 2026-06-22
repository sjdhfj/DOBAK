#pragma once

#include <string>
#include <vector>

enum class PatternType
{
	Same,
	Fixed
};

enum class PatternEventType
{
	Gold,
	SixSeven
};

struct Pattern
{
	std::string patternName;

	int width;
	int height;

	PatternType type;
	std::vector<int> values;

	PatternEventType eventType;
	int reward;
};

struct MatchedPattern
{
	int startY;
	int startX;
	int width;
	int height;
	int reward;

	int patternIndex;
};

extern Pattern GamePatterns[];
extern int GamePatternCount;