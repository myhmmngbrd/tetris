#include "Board.h"


// 보드 값을 참조하는 경우 오버플로우 주의

int Board::inputcheck(Block& block, int x, int y) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[i][j]) {
				// 블럭이 땅에 닿음
				if (i + y >= 20) return 1; 
				// 블럭이 벽면에 닿음
				else if (j + x < 0 || j + x >= 10) return 2;
				// 블럭이 굳은 블럭에 닿음
				else if (i + y >= 0 && board[i + y][j + x] > 7) return 3;
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
			// 투명블럭은 블럭이 생성된 자리에 생기는경우가 있으므로 겹치는 경우가 생김
			if (block[i][j] && !board[i + y][j + x]) board[i + y][j + x] = block[i][j];
		}
	}
	//정상 삽입
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
	// 블럭 참조, input에서 충돌검사를 했으므로 순서대로 호출한다면 오버플로우 가능성 없음
	for (int i = 0; i < 4; i++) {
		// 범위를 넘어간 줄 스킵
		if (i + y < 0 || i + y >= 20) continue;
		for (int j = 0; j < 4; j++) {
			// 범위를 넘어간 칸 스킵
			if (j + x < 0 || j + x >= 10) continue;
			// 블럭값이 있다면, 삭제
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

