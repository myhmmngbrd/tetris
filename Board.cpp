#include "Board.h"

int Board::inputcheck(Block& block, int x, int y) {
	for (int i = 0; i < 4; i++) {
		if (i + y < 0) continue;
		for (int j = 0; j < 4; j++) {
			if (block[i][j]) {
				// ���� ���� ����
				if (i + y >= 20) return 1; 
				// ���� ���鿡 ����
				else if (j + x < 0 || j + x >= 10) return 2;
				// ���� ���� ����
				else if (board[i + y][j + x] && board[i + y][j + x] < 8) return 3; 
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
			//���� ����, ������ ����, �Ǵ� ���� ���� ���� ���
			if (j + x < 0 || j + x >= 10 || board[i + y][j + x]) continue;
			if (block[i][j]) board[i + y][j + x] = block[i][j];
		}
	}
	//���� ����
	return 0;
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
			if (block[i][j]) board[i + y][j + x] = 0;
		}
	}
}

void Board::print() {
	for (auto row : board) {
		for (auto e : row) {
			printf("%d", e);
		}
		printf("\n");
	}
}