#include <iostream>
#include <windows.h>
#include <vector>

#define WIDTH 10
#define HEIGHT 20

class Game {
	Board board;
 public:
	Game() {
		_COORD coord;
		coord.X = 200;
		coord.Y = 50;

		_SMALL_RECT Rect;
		Rect.Top = 0;
		Rect.Left = 0;
		Rect.Right = 200 - 1;
		Rect.Bottom = 50 - 1;

		HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleScreenBufferSize(Handle, coord);
		SetConsoleWindowInfo(Handle, TRUE, &Rect);
	}
	
	void gotoxy(int x, int y) {
		COORD Pos;
		Pos.X = x;
		Pos.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
	}
	void drawbox(int x, int y, int w, int h) {
		/*
		* 너비와 높이는 박스가 아닌 컨텐츠기준
		* 좌상: ┌
		* 좌하: └
		* 우상: ┐
		* 우하: ┘
		* 가로: ─
		* 세로: │
		* 박스: □ ■
		*/
		gotoxy(x*2, y);
		printf("┌");
		gotoxy(x*2 + w * 2 + 2, y);
		printf("┐");
		gotoxy(x*2, y + h + 1);
		printf("└");
		gotoxy(x*2 + w * 2 + 2, y + h + 1);
		printf("┘");
		for (int i = 2; i <= w * 2; i = i + 2) {
			gotoxy(x*2 + i, y);
			printf("─");
			gotoxy(x*2 + i, y + h + 1);
			printf("─");
		}
		for (int i = 1; i <= h; i++) {
			gotoxy(x*2, y + i);
			printf("│");
			gotoxy(x*2 + w * 2 + 2, y + i);
			printf("│");
		}
	}
	void start() {
		drawbox(0, 0, 10, 20);
		drawbox(12, 0, 4, 2);
	}
	void update() {

	}
};

class Board {
	int box[20][10] = { 0, };
public:
	void fall() {

	}

	void rotate() {

	}

	void shift(int index) {
		if (index >= 20) exit(1);
		for (int i = index; index >= 0; i--) {
			for (int j = 0; j < 10; j++) {
				box[i][j] = box[i - 1][j];
			}
		}
	}
	void print() {
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < WIDTH; j++) {
				printf("%d ", box[i][j]);
			}
			printf("\n");
		}
	}
	int& operator[] (int index) {
		return box[index];
	}
};

class Block {
	char type;
	int x, y, r;
	std::vector<int> box;
public:
	void create(char type) {
		switch (type)
		{
		case 'I':
			box = { 0, -2, 0, -1, 0, 0, 0, -1 };
			break;
		case 'O':
			box = { 0, 0, 0, 1, 1, 1, 1, 0 };
		case 'L':
			
			break;
		case 'J':
			break;
		case 'Z':
			break;
		case 'S':
			break;
		case 'T':
			break;
		default:
			break;
		}
	}
};




void cursorview(char show)//커서숨기기
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

int main() {
	Game Tetris;
	Tetris.start();

	return 0;
}