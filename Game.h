#pragma once
#include <random>
#include <mutex>
#include "Canvas.h"
#include "Block.h"
#include "Board.h"

class Random {
	std::random_device* seed;
	std::mt19937* engine;
	std::uniform_int_distribution<int> distribution;
public:
	Random(int min, int max);
	~Random();
	int get();
};

class Bucket : public Random {
	std::array<int, 7> bucket;
	int index = 0;
	void create();
public:
	Bucket(int min, int max);
	int ref();
	int pop();
};


class Game {
	Bucket type{ 0, 6 };
	Bucket color{ 1, 7 };
	Canvas canvas{ 0,0,20,40 };
	Block block;
	Block illusion;
	Block next;
	Board board;
	Board log;

	std::array<int, 7> bucket;

	int state; // 0: game over, 1: create, 2: down
	condition_variable cv;
	mutex m;
	
	const int& bx = block.getrefX();
	const int& by = block.getrefY();

	const int& ix = illusion.getrefX();
	const int& iy = illusion.getrefY();

	void create();
	void down();
	void left();
	void right();
	void fall();
	void ifall();
	void rotate();

	void view();

public:
	Game();
};

