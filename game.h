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
		rotation, 
		type, // 0 ~ 6 
		color; // 1 ~ 7
	std::array<std::array<int, 4>, 4> block;
public:
	int getX();
	int getY();
	void up();
	void down();
	void left();
	void right();
	void rotate();
	void counterrotate()
};


class Board {
public:
	int 
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