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
		// 입력 성공시 지우고 한칸 위에 다시 입력
		state = RUN;
		board.remove(block, x, y);
		block.up();
		board.input(block, x, y);
		system("cls");
		board.print();
		board.remove(block, x, y);
	} else if (collision) {
		// 입력 실패시 게임오버. 지우고 다시 입력. 실패여부는 굳이 확인하지 않음
		state = GAMEOVER;
		block.up();
		board.input(block, x, y);
		system("cls");
		board.print();
	}
}

void Game::down() {
	block.down(2); // 2단계 하락
	int collision = board.input(block, x, y);
	if (!collision) {
		// 입력 성공시 지우고 한칸 위에 다시 입력
		board.remove(block, x, y);
		block.up();
		board.input(block, x, y);
		system("cls");
		board.print();
		board.remove(block, x, y);
	}
	else if (collision) {
		// 입력 실패시 한칸 위에 재입력 / 지우지 말고 새 블럭 호출
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