#include "Game.h"
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <chrono>

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


Bucket::Bucket(int min, int max) : Random(min, max) {
	create();
}

void Bucket::create() {
	int n, exist;
	for (unsigned int i = 0; i < bucket.size(); i++) {
		do {
			exist = 0;
			n = get();
			for (unsigned int j = 0; j < i; j++) {
				if (bucket[j] == n) {
					exist = 1;
					break;
				}
			}
		} while (exist);
		bucket[i] = n;
	}
}

int Bucket::pop() {
	int n, exist, result;
	if (index == bucket.size()) index = 0;
	result = bucket[index];
	do {
		exist = 0;
		n = get();
		for (unsigned int j = 0; j < index; j++) {
			if (bucket[j] == n) {
				exist = 1;
				break;
			}
		}
	} while (exist);
	bucket[index] = n;
	index++;
	return result;
}

int Bucket::ref() {
	return bucket[index == bucket.size() ? 0 : index];
}

void Game::create() {
	int type = this->type.pop();
	int color = this->color.pop();
	block.init(type, color);
	block.create();


	Dot dot;
	Dots dots;
	next.init(this->type.ref(), this->color.ref());
	next.create();
	for (int i = 1; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
				dot = { 13 + j, i+1, next[i][j] };
				dots.push_back(dot);
		}
	}
	if (dots.size()) canvas.push(dots);

	int collision = board.input(block, bx, by);
	if (!collision) {
		illusion.init(type, -1);
		ifall();
		state = RUN;
		view();
		board.remove(block, bx, by);
	} else if (collision) {;
		view();
		state = GAMEOVER;
	}
}

void Game::down() {
	block.down();
	int collision = board.input(block, bx, by);
	if (!collision) {
		view();
		board.remove(block, bx, by);
	} else if (collision) {
		block.up();
		board.input(block, bx, by);
		board.solid(block, bx, by);
		board.clearcheck();
		view();
		state = START;
	}
}


void Game::left() {
	block.left();


	int collision = board.input(block, bx, by);
	if (!collision) {
		ifall();
		view();
		board.remove(block, bx, by);
	}
	else {
		block.right();
	}
}

void Game::right() {
	block.right();


	int collision = board.input(block, bx, by);
	if (!collision) {
		ifall();
		view();
		board.remove(block, bx, by);
	}
	else {
		block.left();
	}
}

void Game::fall() {
	while (!board.input(block, bx, by)) { // input시 충돌이 없으면
		board.remove(block, bx, by);
		block.down();
	}
	block.up();
	board.input(block, bx, by);
	board.solid(block, bx, by);
	board.clearcheck();
	view();
	state = START;
	cv.notify_one();
}

void Game::rotate() {
	block.rotate();
	block.create();

	int collision = board.input(block, bx, by);
	if (!collision) {
		ifall();
		view();
		board.remove(block, bx, by);
	} else if (collision) {
		block.rotateback();
		block.create();
	}
}

void Game::ifall() {
	board.remove(illusion, ix, iy);
	illusion = block;
	illusion.create();
	while (!board.input(illusion, ix, iy)) {
		board.remove(illusion, ix, iy);
		illusion.down();
	}
	illusion.up();
	board.input(illusion, ix, iy);
}

void Game::view() {
	Dot dot;
	Dots dots;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 10; j++) {
			if (log[i][j] != board[i][j]) {
				dot = { j + 1, i + 1, board[i][j] };
				dots.push_back(dot);
				log[i][j] = board[i][j];
			}
		}
	}
	if (dots.size()) canvas.push(dots);
}



Game::Game() {
	canvas.DrawBox(0, 0, 12, 22);
	canvas.DrawBox(12, 0, 6, 5);

	thread t([this]() {
		int key;
		while (1) {
			key = _getch();
			if (key && this->state == RUN) {
				switch (key) {
				case 75:
					this->left();
					break;
				case 77:
					this->right();
					break;
				case 80:
					this->down();
					break;
				case 32:
					this->fall();
					break;
				case 72:
					this->rotate();
					break;
				default:
					break;
				}
			}
		}
	});


	do {
		create();
		do {
			unique_lock<mutex> lk(m);
			cv.wait_for(lk, chrono::milliseconds(1000) , [this]() { return this->state == START; });
			if (this->state == START) break;
			down();
		} while (state == RUN);
		Sleep(50);
	} while (state == START);

	t.join();

}