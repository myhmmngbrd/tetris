#include "Board.h"


// ���� ���� �����ϴ� ��� �����÷ο� ����

int Board::inputcheck(Block& block, int x, int y) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[i][j]) {
				// ���� ���� ����
				if (i + y >= 20) return 1; 
				// ���� ���鿡 ����
				else if (j + x < 0 || j + x >= 10) return 2;
				// ���� ���� ���� ����
				else if (i + y >= 0 && board[i + y][j + x] > 7) return 3;
			}
		}
	}
	return 0;
}

int Board::input(Block& block, int x, int y) {
	//�浹 üũ
	int collision = inputcheck(block, x, y);
	if (collision) return collision;
	for (int i = 0; i < 4; i++) {
		// õ���� �Ѿ �� ��ŵ
		if (i + y < 0 || i + y >= 20) continue;
		for (int j = 0; j < 4; j++) {
			// ������� ���� ������ �ڸ��� ����°�찡 �����Ƿ� ��ġ�� ��찡 ����
			if (block[i][j] && !board[i + y][j + x]) board[i + y][j + x] = block[i][j];
		}
	}
	//���� ����
	return 0;
}

void Board::forcedinput(Block& block, int x, int y) {
	for (int i = 0; i < 4; i++) {
		if (i + y < 0 || i + y >= 20) continue;
		for (int j = 0; j < 4; j++) {
			if (block[i][j]) {
				if (board[i + y][j + x] < 0) continue;
				board[i + y][j + x] = block[i][j];
			}
		}
	}
}

void Board::remove(Block& block, int x, int y) {
	// �� ����, input���� �浹�˻縦 �����Ƿ� ������� ȣ���Ѵٸ� �����÷ο� ���ɼ� ����
	for (int i = 0; i < 4; i++) {
		// ������ �Ѿ �� ��ŵ
		if (i + y < 0 || i + y >= 20) continue;
		for (int j = 0; j < 4; j++) {
			// ������ �Ѿ ĭ ��ŵ
			if (j + x < 0 || j + x >= 10) continue;
			// ������ �ִٸ�, ����
			if (block[i][j]) {
				if (block[i][j] == -1 && board[i + y][j + x] > 0) continue;
				board[i + y][j + x] = 0;
			}
		}
	}
}


void Board::solid(Block& block, int x, int y) {
	for (int i = 0; i < 4; i++) {
		if (i + y < 0 || i + y >= 20) continue;
		for (int j = 0; j < 4; j++) {
			if (j + x < 0 || j + x >= 10) continue;
			if (block[i][j]) board[i + y][j + x] += 7;
		}
	}
}

void Board::clearcheck() {
	int hole;
	for (int i = 0; i < 20; i++) {
		hole = 0;
		for (auto e : board[i]) {
			if (e <= 0) hole = 1;
		}
		if (!hole) clear(i);
	}
}

void Board::clear(int index) {
	for (int i = index; i > 0; i--) {
		board[i] = board[i - 1];
	}
	board[0] = { 0, };
}


std::array<int, 10>& Board::operator[] (int index) {
	return board[index];
}

