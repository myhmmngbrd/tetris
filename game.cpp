#include "game.h"
/*
*�»�: ��
* ���� : ��
* ��� : ��
* ���� : ��
* ���� : ��
* ���� : ��
* �ڽ� : �� ��
*/

//block
void Block::init(int type, int color) {
	x = 3;
	y = -2;
	this->type = type;
	rotation = 0;
	this->color = color;
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
	if (++rotation == blocks[type].size()) {
		rotation = 0;
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
Block& Block::operator= (Block& other) {
	this->x = other.x;
	this->y = other.y;
	this->rotation = other.x;
	this->type = other.type;
	this->block = other.block;
	return *this;
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
				else if (i + y >= 0 && board[i + y][j + x] > 7) return 3; // �ٸ� ���� �浹
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

int Board::createBlock(Block& block, int type, int color) {
	block.init(type, color);
	block.createBlock();
	int error = checkCollision(block, 3, -2);
	if (error) {
		return 0;
	}
	else {
		inputBlock(block, 3, -2);
		return 1;
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
	removeBlock(block, x, y);
	int error = checkCollision(block, x, y + 1);
	if (error) {
		error = checkCollision(block, x, y);
		if (error) {
			return 0;
		} else {
			inputBlock(block, x, y);
			hardenBlock(block, x, y);
			checkClear();
			return 1;
		}
	} else {
		block.down();
		return fall(block, x, y);
	}
}
int Board::rotate(Block& block, int x, int y) {
	removeBlock(block, x, y);
	block.rotate();
	block.createBlock();
	int error = checkCollision(block, x, y);
	if (error) {
		block.counterRotate();
		block.createBlock();
		return 0;
	}
	else {
		inputBlock(block, x, y);
		return 1;
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

//game
Game::Game(int x, int y, int w, int h) :
	canvas_left(x),
	canvas_top(y),
	canvas_width(w),
	canvas_height(h),
	canvas(x, y, w, h, &m) 
{
	// �� ȭ��
	drawBoarder(0,0,12,22);
	// �׸���
	canvas.draw(&m);
	
	const int& blockX = block.getRefX();
	const int& blockY = block.getRefY();
	board.createBlock(block, 0, 1);
	board.down(block, blockX, blockY);
	board.down(block, blockX, blockY);
	drawBlock();
	old_board = board;
	Sleep(500);
	board.down(block, blockX, blockY);
	drawBlock();
}
void Game::drawBoarder(int boxX, int boxY, int boxW, int boxH) {
	// �𼭸�
	canvas.push(boxX, boxY, "��");
	canvas.push(boxX + boxW - 1, boxY, "��");
	canvas.push(boxX, boxY + boxH -1, "��");
	canvas.push(boxX + boxW - 1, boxY + boxH -1, "��");
	// ���μ�
	for (int i = boxX + 1; i < boxX + boxW - 1; i++) {
		canvas.push(i, boxY, "��");
		canvas.push(i, boxY + boxH - 1, "��");
	}
	// ���μ�
	for (int i = boxY + 1; i < boxY + boxH - 1; i++) {
		canvas.push(boxX, i, "��");
		canvas.push(boxX + boxW - 1, i, "��");
	}
}
void Game::drawBlock() {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 10; j++) {
			if (board[i][j] != old_board[i][j]) {
				canvas.push(j + 1, i + 1, std::to_string(board[i][j]));
			}
		}
	}
	canvas.draw(&m);
}