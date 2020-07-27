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
				if (i + y > 19) return 1; // 바닥에 충돌
				else if (j + x < 0 || j + x > 9) return 2; // 벽에 충돌
				else if (board[i + y][j + x] > 7) return 3; // 다른 블럭에 충돌
			}
		}
	}
	return 0; // 충돌 없음
}
int Board::inputBlock(Block& block, int x, int y) {
	for (int i = 0; i < 4; i++) {
		if (i + y < 0 || i + y > 19) continue; // 천장이나 바닥에 닿으면 스킵
		for (int j = 0; j < 4; j++) {
			if (j + x < 0 || j + x > 9) continue; // 벽에 닿으면 스킵
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
			if (block[i][j] > 0 && board[i + y][j + x] < 8) board[i + y][j + x] = 0;
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

void Board::clear(int index) {
	for (int i = index; i > 0; i--) {
		board[i] = board[i - 1];
	}
	board[0] = { 0, };
}
void Board::checkClear() {
	int hole;
	for (int i = 0; i < 20; i++) {
		hole = 0;
		for (auto col : board[i]) {
			if (col < 1) {
				hole = 1;
				break;
			}
		}
		if (!hole) clear(i);
	}
}

int Board::down(Block& block, const int& x, const int& y) {
	int error = checkCollision(block, x, y + 1);
	if (error) {
		hardenBlock(block, x, y);
		checkClear();
		return 0;
	} else {
		removeBlock(block, x, y);
		block.down();
		inputBlock(block, x, y);
		return 1;
	}
}
int Board::left(Block& block, const int& x, const int& y) {
	int error = checkCollision(block, x - 1, y);
	if (error) {
;		return 0;
	} else {
		removeBlock(block, x, y);
		block.left();
		inputBlock(block, x, y);
		return 1;
	}
}
int Board::right(Block& block, const int& x, const int& y) {
	int error = checkCollision(block, x + 1, y);
	if (error) {
		return 0;
	} else {
		removeBlock(block, x, y);
		block.right();
		inputBlock(block, x, y);
		return 1;
	}
}
int Board::fall(Block& block, const int& x, const int& y) {
	block.down();
	int error = checkCollision(block, x, y);
	if (error) {
		block.up();
		error = checkCollision(block, x, y);
		if (error) {
			return 0;
		} else {
			inputBlock(block, x, y);
			return 1;
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