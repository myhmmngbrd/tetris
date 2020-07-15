#include "Game.h"
#include <windows.h>

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
	block.init(rd.get(), rd.get() + 1);
	block.create();
	block.down();
	int result = board.input(block);
	if (result) {
		state = RUN;
		board.remove(block);
		block.up();
		board.input(block);
		system("cls");
		board.print();
		board.remove(block);
	} else if (!result) {
		state = GAMEOVER;
		block.up();
		board.input(block);
		system("cls");
		board.print();
	}
}

void Game::down() {
	block.down(2); // 2�ܰ� �϶�
	int result = board.input(block);
	if (result) {
		// �Է� ������ ����� ��ĭ ���� �ٽ� �Է�
		board.remove(block);
		block.up();
		board.input(block);
		system("cls");
		board.print();
		board.remove(block);
	}
	else if (!result) {
		// �Է� ���н� ��ĭ ���� ���Է� / ������ ���� �� �� ȣ��
		block.up();
		board.input(block);
		system("cls");
		board.print();
		state = START;
	}
}


void Game::left() {
	block.left();
	int result = board.input(block);
	if (result) {
		system("cls");
		board.print();
		board.remove(block);
	}
}

void Game::right() {
	block.right();
	int result = board.input(block);
	if (result) {
		system("cls");
		board.print();
		board.remove(block);
	}
}

void Game::rotate() {
	block.rotate();
	block.create();
	int result = board.input(block);
	if (result) {
		system("cls");
		board.print();
		board.remove(block);
	} else if (!result) {
		block.rotateback();
		block.create();
	}
}



Game::Game() {
	canvas.DrawBox(0, 0, 12, 22);

	create();

	Sleep(300);
	system("cls");
	block.down();
	board.input(block);
	board.print();
	board.remove(block);

	Sleep(300);
	system("cls");
	block.left();
	board.input(block);
	board.print();
	board.remove(block);


	Sleep(300);
	system("cls");
	block.right();
	board.input(block);
	board.print();
	board.remove(block);

	Sleep(300);
	system("cls");
	block.down();
	board.input(block);
	board.print();
	board.remove(block);

	Sleep(300);
	system("cls");
	block.down();
	board.input(block);
	board.print();
	board.remove(block);

	Sleep(300);
	system("cls");
	block.rotate();
	block.create();
	board.input(block);
	board.print();
	board.remove(block);
}