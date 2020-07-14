#include "Board.h"

int Board::input(Block& block) {
	auto pos = block.getPos();
	int x = pos[0];
	int y = pos[1];
	// �� ����, ���忡 ���� �� �����÷ο� ���ɼ� ����
	for (int i = 0; i < 4; i++) {
		// õ���� �Ѿ �� ��ŵ
		if (i + y < 0) continue;
		for (int j = 0; j < 4; j++) {
			// ����
			if (block[i][j]) {
				// �ٴ�, ���� ����, ������ ����, �Ǵ� ���� ���� ���� ���
				if (i + y >= 20 || j + x < 0 || j + x >= 10 || board[i + y][j + x]) return 0;
				board[i + y][j + x] = block[i][j];
			}
		}
	}
	//���� ����
	return 1;
}

void Board::remove(Block& block) {
	auto pos = block.getPos();
	int x = pos[0];
	int y = pos[1];

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