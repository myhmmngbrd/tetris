#include "game.h"


//block
void Block::init() {
	x = 3;
	y = -2;
	type = 0;
	rotation = 0;
	color = 1;
}
int Block::getX() { return x; }
int Block::getY() { return y; }
const int& Block::getRefX() { return x; }
const int& Block::getRefY() { return y; }
void Block::up() { y--; }
void Block::down() { y++; }
void Block::left() { x--; }
void Block::right() { x++; }
void Block::rotate() { 
	if (rotation == blocks[type].size()) {
		rotation = 0;
	} else {
		rotation++;
	}
}
void Block::counterRotate() {
	if (rotation == 0) {
		rotation = blocks[type].size() - 1;
	} else {
		rotation--;
	}
}
void Block::createBlock() {
	int number, hexa, binary;
	number = blocks[type][rotation];
	for (int i = 0; i < 4; i++) {
		hexa = number % 16;
		for (int j = 0; j < 4; j++) {
			binary = hexa % 2;
			block[i][j] = binary * color;
			hexa /= 2;
		}
		number /= 16;
	}
}

const std::array<int, 4>& Block::operator[] (int index) {
	return block[index];
}

void Block::print() {
	for (auto i : block) {
		for (auto j : i) {
			printf("%d", j);
		}
		printf("\n");
	}
}

//board
int Board::checkCollision(Block& block, int x, int y) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[i][j]) {
				if (i + y > 19) return 1; // �ٴڿ� �浹
				else if (j + x < 0 || j + x > 9) return 2; // ���� �浹
				else if (board[i + y][j + x] > 7) return 3; // �ٸ� ���� �浹
			}
		}
	}
	return 0; // �浹 ����
}
int Board::inputBlock(Block& block, int x, int y) {
	for (int i = 0; i < 4; i++) {
		if (i + y < 0 || i + y > 19) continue; // õ���̳� �ٴڿ� ������ ��ŵ
		for (int j = 0; j < 4; j++) {
			if (j + x < 0 || j + x > 9) continue; // ���� ������ ��ŵ
			if (block[i][j] && board[i + y][j + x] <= 0) {
				board[i + y][j + x] = block[i][j];
			}
		}
	}
	return 1;
}
void Board::removeBlock(Block& block, int x, int y) {
	for (int i = 0; i < 4; i++) {
		if (i + y < 0 || i + y > 19) continue;
		for (int j = 0; j < 4; j++) {
			if (j + x < 0 || j + x > 9) continue;
			if (block[i][j] > 0) board[i + y][j + x] = 0;
			else if (block[i][j] < 0 && board[i + y][j + x] < 0)
				board[i + y][j + x] = 0;
		}
	}
}
void Board::hardenBlock(Block& block, int x, int y) {
	for (int i = 0; i < 4; i++) {
		if (i + y < 0 || i + y > 19) continue;
		for (int j = 0; j < 4; j++) {
			if (j + x < 0 || j + x > 19) continue;
			if (block[i][j]) board[i + y][j + x] += 7;
		}
	}
}


std::array<int, 10>& Board::operator[] (int index) {
	return board[index];
}

void Board::print() {
	for (auto i : board) {
		for (auto j : i) {
			printf("%d", j);
		}
		printf("\n");
	}
}