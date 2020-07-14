#include "Canvas.h"
#include <windows.h>


Canvas::Canvas(int x, int y, int w, int h) : left(x), top(y), width(w), height(h) {
	cursorview(0);
}

Canvas::~Canvas() {
	gotoxy(0, left + height);
}

void Canvas::cursorview(char show) {
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void Canvas::gotoxy(int x, int y) {
	COORD Pos;
	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void Canvas::DrawBox(int x, int y, int w, int h) {
	/*
	*좌상: ┌
	* 좌하 : └
	* 우상 : ┐
	* 우하 : ┘
	* 가로 : ─
	* 세로 : │
	* 박스 : □ ■
	*/
}