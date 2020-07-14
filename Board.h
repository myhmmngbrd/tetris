#pragma once
#include <array>
#include "Block.h"

class Board {
	std::array<std::array<int, 10>, 20> board = { 0, };
public:
	// 보드에 블럭 삽입
	int input(Block& block);
	// 삽입 시 이전 상태의 자신의 블럭을 구분할 방법이 없으므로 view 함수 호출 이후 자신의 블럭은 지워줘야 함
	// 바닥에 닿은 경우, 이를 스킵하고 블럭 리셋
	void remove(Block& block);

	void print();
};

