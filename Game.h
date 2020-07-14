#pragma once
#include <random>
#include "Canvas.h"

class Random {
	std::random_device* seed;
	std::mt19937* engine;
	std::uniform_int_distribution<int> distribution;
public:
	Random(int min, int max);
	~Random();
	int get();
};

class Game {
	Random rd{ 0, 6 };
	Canvas canvas{ 0,0,50,40 };
};

