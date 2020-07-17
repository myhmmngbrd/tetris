#include "Canvas.h"


Canvas::Canvas(int x, int y, int w, int h) : left(x), top(y), width(w), height(h),
	t([this]() {
		Dots dots;
		while (1) {
			//*
			if (log.size()) {
				dots = this->log.front();
				if (dots.size()) {
					for (Dot dot : dots) {
						gotoxy(dot[0], dot[1]);
						dot[2] > 0 ?
							printf("\x1b[3%dm" "■" "\x1b[0m", dot[2] % 7 + 1) :
							dot[2] == 0 ? printf("  ") : printf("□");
					}
					log.pop_front();
				}
			}
			//*/
			Sleep(33);
		}
	})
{
	cursorview(0);
}

Canvas::~Canvas() {
	t.join();
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
	Pos.X = x * 2;
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
	for (int i = 1; i < w - 1; i++) {
		gotoxy(x + i, y);
		printf("─");
		gotoxy(x + i, y + h - 1);
		printf("─");
	}
	for (int i = 1; i < h - 1; i++) {
		gotoxy(x, y + i);
		printf("│");
		gotoxy(x + w - 1, y + i);
		printf("│");
	}

	gotoxy(x, y);
	printf("┌");
	gotoxy(x, y + h - 1);
	printf("└");
	gotoxy(x + w - 1, y);
	printf("┐");
	gotoxy(x + w - 1, y + h - 1);
	printf("┘");



}

void Canvas::push(Dots dots) {
	log.push_back(dots);
}

void Canvas::PushString(int x, int y, std::string str) {

}

void Canvas::print() {
	for (Dots dots : log) {
		for (Dot dot : dots) {
			printf("x: %d, y: %d, color: %d\n", dot[0], dot[1], dot[2]);
		}
	}
}