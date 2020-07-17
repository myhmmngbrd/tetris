#pragma once
#include <array>
#include "Block.h"

class Board {
	std::array<std::array<int, 10>, 20> board = { 0, };
public:
	// ���忡 �� ����
	int inputcheck(Block&, int, int);
	int input(Block&, int, int);
	void forcedinput(Block&, int, int);
	// ���� �� ���� ������ �ڽ��� ���� ������ ����� �����Ƿ� view �Լ� ȣ�� ���� �ڽ��� ���� ������� ��
	// �ٴڿ� ���� ���, �̸� ��ŵ�ϰ� �� ����
	void remove(Block&, int, int);
	void solid(Block&, int, int);

	std::array<int, 10>& operator[](int);

	void clearcheck();
	void clear(int);
};

