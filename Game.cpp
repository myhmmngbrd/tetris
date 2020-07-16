#include "Game.h"
#include <windows.h>
#include <iostream>

#define GAMEOVER 0
#define START 1
#define RUN 2

Random::Random(int min, int max) : distribution(min, max) {
	seed = new std::random_device;
	engine = new std::mt19937((*seed)());
}

Random::~Random() {
	delete seed;
	delete engine;
}

int Random::get() { return distribution(*engine); }

void Game::create() {
	int type = rd.get();
	int color = rd.get() + 1;
	block.init(type, color);
	block.create();
	block.down();
	illusion.init(type, 8);
	illusion.create();
	illusion.down();
	int collision = board.input(block, x, y);
	if (!collision) {
		// �Է� ������ ����� ��ĭ ���� �ٽ� �Է�
		state = RUN;
		board.remove(block, x, y);
		block.up();
		board.input(block, x, y);
		system("cls");
		board.print();
		board.remove(block, x, y);
	} else if (collision) {
		// �Է� ���н� ���ӿ���. ����� �ٽ� �Է�. ���п��δ� ���� Ȯ������ ����
		state = GAMEOVER;
		block.up();
		board.input(block, x, y);
		system("cls");
		board.print();
	}
}

void Game::down() {
	block.down(2); // 2�ܰ� �϶�
	int collision = board.input(block, x, y);
	if (!collision) {
		// �Է� ������ ����� ��ĭ ���� �ٽ� �Է�
		board.remove(block, x, y);
		block.up();
		board.input(block, x, y);
		system("cls");
		board.print();
		board.remove(block, x, y);
	}
	else if (collision) {
		// �Է� ���н� ��ĭ ���� ���Է� / ������ ���� �� �� ȣ��
		block.up();
		board.input(block, x, y);
		system("cls");
		board.print();
		state = START;
	}
}


void Game::left() {
	block.left();
	int collision = board.input(block, x, y);
	if (!collision) {
		system("cls");
		board.print();
		board.remove(block, x, y);
	}
}

void Game::right() {
	block.right();
	int collision = board.input(block, x, y);
	if (!collision) {
		system("cls");
		board.print();
		board.remove(block, x, y);
	}
}

void Game::fall() {
	int collision;
	block.fall();
	while ((collision = board.input(block, x, y))) {
		block.up();
	}
	system("cls");
	board.print();
	state = START;
}

void Game::rotate() {
	block.rotate();
	block.create();
	int collision = board.input(block, x, y);
	if (!collision) {
		system("cls");
		board.print();
		board.remove(block, x, y);
	} else if (collision) {
		block.rotateback();
		block.create();
	}
}



Game::Game() {
	canvas.DrawBox(0, 0, 12, 22);

	create();

	Sleep(1000);
	down();

	Sleep(1000);
	down();

	Sleep(1000);
	left();


	Sleep(1000);
	left();

	Sleep(1000);
	right();

	Sleep(1000);
	right();

	Sleep(1000);
	down();

	Sleep(1000);
	down();

	Sleep(1000);
	rotate();
	
	Sleep(1000);
	rotate();

	Sleep(1000);
	rotate();

	Sleep(1000);
	fall();
}