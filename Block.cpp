#include "Block.h"


int Block::getX() { return x; }
int Block::getY() { return y; }
const int& Block::getrefX() { return x; }
const int& Block::getrefY() { return y; }


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

int Block::up(int add) {
	return y -= add;
}
int Block::down(int add) {
	return y += add;
}
int Block::left(int add) {
	return x -= add;
}
int Block::right(int add) {
	return x += add;
}

int Block::fall() {
	return y = 18;
}

int Block::rotate() {
	if (++rotation == blocks[type].size()) {
		rotation = 0;
	}
	return rotation;
}

int Block::rotateback() {
	if (--rotation < 0) {
		rotation = blocks[type].size() - 1;
	}
	return rotation;
}

void Block::print() {
	for (auto row : block) {
		for (auto e : row) {
			printf("%d", e);
		}
		printf("\n");
	}
}