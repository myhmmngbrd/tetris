#include "Block.h"


std::array<int, 2> Block::getPos() { return { x,y }; }

void Block::init(int type, int color) {
	x = 3;
	y = -2;
	rotation = 0;
	this->type = type; //0-6
	this->color = color; //1-7
}

void Block::create() {
	int number, hex, binary;
	//xmax = 0, ymax = 0;
	number = blocks[type][rotation];
	for (int i = 0; i < 4; i++) {
		if (hex = number % 16) {
			//ymax++;
		}
		for (int j = 0; j < 4; j++) {
			if (binary = hex % 2) {
				//xmax++;
			}
			block[3 - i][3 - j] = binary * color;
			if (hex /= 2) {
				//xmin++;
			}
		}
		number /= 16;
	}
}

const std::array<int, 4>& Block::operator[] (int index) {
	return block[index];
}

void Block::up(int add) {
	y -= add;
}

void Block::down(int add) {
	y += add;
}

void Block::left(int add) {
	x -= add;
}

void Block::right(int add) {
	x += add;
}

void Block::rotate() {
	if (++rotation == blocks[type].size()) {
		rotation = 0;
	}
}

void Block::rotateback() {
	if (--rotation < 0) {
		rotation = blocks[type].size() - 1;
	}
}

void Block::print() {
	for (auto row : block) {
		for (auto e : row) {
			printf("%d", e);
		}
		printf("\n");
	}
}