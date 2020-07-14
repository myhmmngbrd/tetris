#include "Board.h"

int Board::input(Block& block) {
	auto pos = block.getPos();
	int x = pos[0];
	int y = pos[1];
	// 블럭 참조, 보드에 삽입 시 오버플로우 가능성 있음
	for (int i = 0; i < 4; i++) {
		// 천장을 넘어간 블럭 스킵
		if (i + y < 0) continue;
		for (int j = 0; j < 4; j++) {
			// 블럭이
			if (block[i][j]) {
				// 바닥, 왼쪽 벽면, 오른쪽 벽면, 또는 기존 블럭과 닿은 경우
				if (i + y >= 20 || j + x < 0 || j + x >= 10 || board[i + y][j + x]) return 0;
				board[i + y][j + x] = block[i][j];
			}
		}
	}
	//정상 삽입
	return 1;
}

void Board::remove(Block& block) {
	auto pos = block.getPos();
	int x = pos[0];
	int y = pos[1];

	// 블럭 참조, input에서 충돌검사를 했으므로 순서대로 호출한다면 오버플로우 가능성 없음
	for (int i = 0; i < 4; i++) {
		// 범위를 넘어간 줄 스킵
		if (i + y < 0 || i + y >= 20) continue;
		for (int j = 0; j < 4; j++) {
			// 범위를 넘어간 칸 스킵
			if (j + x < 0 || j + x >= 10) continue;
			// 블럭값이 있다면, 삭제
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