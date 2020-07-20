#include "canvas.h"


//private
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


//public
//积己磊
Canvas::Canvas(int x, int y, int w, int h, std::mutex* m) : 
	canvas_left(x), 
	canvas_top(y), 
	canvas_width(w), 
	canvas_height(h), 
	t([this](std::mutex* m) {
		while (1) {
			std::unique_lock<std::mutex> lk(*m);
			this->cv.wait(lk, [this]() {return this->log.size(); });
			lk.unlock();
			if (this->log.front().size()) {
				std::deque<Dot>& dots = this->log.front();
				for (Dot& dot : dots) {
					gotoxy(dot.x, dot.y);
					printf("%s", dot.value.c_str());
				}
			}
		}
	}, m) {
	cursorview(0); //目辑 昏力
}
//家戈磊
Canvas::~Canvas() {
	gotoxy(0, canvas_top + canvas_height + 1);
}

void Canvas::push(int x, int y, std::string value) {
	dots.push_back({ x, y, value });
}

void Canvas::draw() {
	log.push_back(std::move(dots));
	cv.notify_one();
}