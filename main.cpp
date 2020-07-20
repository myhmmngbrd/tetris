#include "canvas.h"

int main() {
	std::mutex m;
	Canvas canvas(0, 0, 10, 20, &m);

	Sleep(1000);
	canvas.push(5, 5, "12");
	canvas.draw();
}