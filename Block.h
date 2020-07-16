#pragma once
#include <array>
#include <vector>

const std::array<std::vector<int>, 7> blocks = {
	std::vector<int> { 0x00f0, 0x2222 }, // I
	std::vector<int> { 0x00e4, 0x04c4, 0x04e0, 0x0464 }, // T
	std::vector<int> { 0x0660 }, // O
	std::vector<int> { 0x0360, 0x4620 }, // S
	std::vector<int> { 0x0c60, 0x2640 }, // Z
	std::vector<int> { 0x0470, 0x0322, 0x0071, 0x0226 }, // J
	std::vector<int> { 0x02e0, 0x0446, 0x00e8, 0x0c44 } // L
};

class Block {
	int x, y;
	int type, rotation; // type 0~6, rotation blocks[type]
	int color; // 1~7
	std::array<std::array<int, 4>, 4> block = { 0, };
public:
	int getX();
	int getY();
	const int& getrefX();
	const int& getrefY();
	void init(int type, int color);
	void create();
	const std::array<int, 4>& operator[] (int index);
	int up(int = 1);
	int down(int = 1);
	int left(int = 1);
	int right(int = 1);
	int fall();
	int rotate();
	int rotateback();
	void print();
};

