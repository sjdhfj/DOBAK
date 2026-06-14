#pragma once
#include<iostream>
using namespace std;

void InitPattern();

struct Pattern
{
	string patternName;
	int width;
	int height;
	int reward;
};

extern Pattern GamePatterns[];
extern int GamePatternCount; //extern 공부하기

