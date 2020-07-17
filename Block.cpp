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
	number = blocks[type][rotation];
	for (int i = 0; i < 4; i++) {
		hex = number % 16;
		for (int j = 0; j < 4; j++) {
			binary = hex % 2;
			block[3 - i][3 - j] = binary * color;
			hex /= 2;
		}
		number /= 16;
	}
}

const std::array<int, 4>& Block::operator[] (int index) {
	return block[index];
}

Block& Block::operator=(Block& block) {
	this->x = block.x;
	this->y = block.y;
	this->rotation = block.rotation;
	return *this;
}

void Block::up() { y--; }
void Block::down() { y++; }
void Block::left() { x--; }
void Block::right() { x++; }
void Block::fall() { y = 18; }
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
