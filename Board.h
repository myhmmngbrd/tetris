#pragma once
#include <array>
#include "Block.h"

class Board {
	std::array<std::array<int, 10>, 20> board = { 0, };
public:
	// ���忡 �� ����
	int input(Block& block);
	// ���� �� ���� ������ �ڽ��� ���� ������ ����� �����Ƿ� view �Լ� ȣ�� ���� �ڽ��� ���� ������� ��
	// �ٴڿ� ���� ���, �̸� ��ŵ�ϰ� �� ����
	void remove(Block& block);

	void print();
};

