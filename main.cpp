#include "canvas.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define PUPPLE "\x1b[35m"
#define SKYBLUE "\x1b[36m"
#define WHITE "\x1b[37m"
#define RESET "\x1b[0m"

void pushbox(Canvas& canvas, int x, int y, int w, int h) {
	for (int i = 1; i < w-1; i++) {
		canvas.push(x + i, y, "��");
		canvas.push(x + i, y + h - 1, "��");
	}
	for (int j = 1; j < h - 1; j++) {
		canvas.push(x, y + j, "��");
		canvas.push(x + w - 1, y + j, "��");
	}
	canvas.push(x, y, "��");
	canvas.push(x, y + h - 1, "��");
	canvas.push(x + w - 1, y, "��");
	canvas.push(x + w - 1, y + h - 1, "��");
}

int main() {
	std::mutex m;
	int canvas_left = 10,
		canvas_top = 5,
		canvas_width = 30,
		canvas_height = 30;
	Canvas canvas(canvas_left, canvas_top, canvas_width, canvas_height, &m);
	pushbox(canvas, 0, 0, 12, 22);
	canvas.push(2,2,"\x1b[37m��\x1b[0m��");
	canvas.draw();
}