#include <iostream>
#include <windows.h>
#include <vector>
#include <string>

#define WIDTH 10
#define HEIGHT 20

class Block {
	int box[4][4] = { 0, };
	int x=0, y=0;
	char type;
public:
	Block(char type = 0) : type(type) {
		switch (type)
		{
		default:
			box[1][1] = 1;
			box[1][2] = 1;
			box[2][1] = 1;
			box[2][2] = 1;
			break;
		}
	}
	void init() {
		x = 5;
		y = 1;
	}
	void left() {
		x--;
	}
	void right() {
		x++;
	}
	void down() {
		y++;
	}
	int ver() {
		return y;
	}
	int hor() {
		return x;
	}
	int* operator[] (int index) {
		return box[index];
	}
};

class Board {
	int box[20][10] = { 0, };
public:
	int before[20][10] = { 0, };
	int index = 0;
	void backup() {
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 10; j++) {
				before[i][j] = box[i][j];
			}
		}
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
	int* operator[] (int index) {
		return box[index];
	}
};

class Game {
	Board board;
	Block block;
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
		update();
		update();
		update();
		update();
		gotoxy(25, 25);
	}
	void update() {
		Sleep(500);
		board[0][board.index++] = 1;
		if (board.index > 2) board[0][board.index - 2] = 0;
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 10; j++) {
				if (board[i][j] != board.before[i][j]) {
					gotoxy(j * 2 + 2, i + 1);
					board[i][j] ? printf("■") : printf("  ");
				}
			}
		}
		board.backup();
	}
	void CreateBlock() {
		block.init();

	}
	int InputBlock() {
		int collision = CheckCollision();
		if (collision) {
			return 0;
		} else 
	}
	int CheckCollision() {
		int x = block.hor();
		int y = block.ver();
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (!block[i][j]) continue;
				if (board[y - 1 + i][x - 1 + j] > 0) { // 충돌
					return 1;
				}
			}
		}
		return 0;
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