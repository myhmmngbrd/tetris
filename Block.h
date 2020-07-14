#pragma once
#include <array>
#include <vector>

const std::array<std::vector<int>, 7> blocks = {
	std::vector<int> { 0x00f0, 0x2222 }, // I
	std::vector<int> { 0x0270, 0x0232, 0x0072, 0x0262 }, // T
	std::vector<int> { 0x0660 }, // O
	std::vector<int> { 0x0360, 0x0231 }, // S
	std::vector<int> { 0x0c60, 0x04c8 }, // Z
	std::vector<int> { 0x0470, 0x0322, 0x0071, 0x0226 }, // J
	std::vector<int> { 0x02e0, 0x0446, 0x00e8, 0x0c44 } // L
};

class Block {
	int x, y;
	int type, rotation; // type 0~6, rotation blocks[type]
	int color; // 1~7
	std::array<std::array<int, 4>, 4> block = { 0, };
public:
	std::array<int, 2> getPos();
	void init(int type, int color);
	void create();
	const std::array<int, 4>& operator[] (int index);
	void print();
};

