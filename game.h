#pragma once
#include "canvas.h"

const std::array<std::vector<int>, 7> blocks = {
	std::vector<int> { 0x00f0, 0x2222 }, // I
	std::vector<int> { 0x0e40, 0x4c40, 0x4e00, 0x4640 }, // T
	std::vector<int> { 0x0660 }, // O
	std::vector<int> { 0x0360, 0x4620 }, // S
	std::vector<int> { 0x0c60, 0x2640 }, // Z
	std::vector<int> { 0x0470, 0x0322, 0x0071, 0x0226 }, // J
	std::vector<int> { 0x02e0, 0x0446, 0x00e8, 0x0c44 } // L
};

class Block {
	int x, y, 
		rotation = 0, 
		type = 0, // 0 ~ 6 
		color = 1; // 1 ~ 7
	std::array<std::array<int, 4>, 4> block = { 0, };
public:
	int getX();
	int getY();
	const int& getRefX();
	const int& getRefY();
	void init();
	void up();
	void down();
	void left();
	void right();
	void rotate();
	void counterRotate();
	void createBlock();

	const std::array<int, 4>& operator[] (int);

	void print();
};


class Board {
	std::array<std::array<int, 10>, 20> board = { 0, };
public:
	int checkCollision(Block&, int, int);
	int inputBlock(Block&, int, int);
	void removeBlock(Block&, int, int);
	void hardenBlock(Block&, int, int);
	
	void clear(int);
	void checkClear();

	int down(Block&, const int&, const int&);
	int left(Block&, const int&, const int&);
	int right(Block&, const int&, const int&);
	int fall(Block&, const int&, const int&);
	int rotate(Block&, int, int);

	std::array<int, 10>& operator[] (int);
	
	void print();
};

class Game {
	Canvas canvas;
	int canvas_left,
		canvas_top,
		canvas_width,
		canvas_height;
	Block block;
	Board board;

};