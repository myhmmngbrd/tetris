#include "Board.h"

int Board::inputcheck(Block& block, int x, int y) {
	for (int i = 0; i < 4; i++) {
		if (i + y < 0) continue;
		for (int j = 0; j < 4; j++) {
			if (block[i][j]) {
				// 블럭이 땅에 닿음
				if (i + y >= 20) return 1; 
				// 블럭이 벽면에 닿음
				else if (j + x < 0 || j + x >= 10) return 2;
				// 블럭이 블럭에 닿음
				else if (board[i + y][j + x] && board[i + y][j + x] < 8) return 3; 
			}
		}
	}
	return 0;
}

int Board::input(Block& block, int x, int y) {
	//충돌 체크
	int collision = inputcheck(block, x, y);
	if (collision) return collision;
	for (int i = 0; i < 4; i++) {
		// 천장을 넘어간 블럭 스킵
		if (i + y < 0 || i + y >= 20) continue;
		for (int j = 0; j < 4; j++) {
			//왼쪽 벽면, 오른쪽 벽면, 또는 기존 블럭과 닿은 경우
			if (j + x < 0 || j + x >= 10 || board[i + y][j + x]) continue;
			if (block[i][j]) board[i + y][j + x] = block[i][j];
		}
	}
	//정상 삽입
	return 0;
}

void Board::remove(Block& block, int x, int y) {
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