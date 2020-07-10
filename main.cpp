#include <iostream>
#include <windows.h>
#include <vector>
#include <array>
#include <string>
#include <thread>
#include <conio.h>
#include <random>
#include <mutex>
#include <future>
#include <chrono>

#define I_BLOCK 0
#define T_BLOCK 1
#define O_BLOCK 2
#define	S_BLOCK 3
#define Z_BLOCK 4
#define J_BLOCK 5
#define L_BLOCK 6

#define ANSI_RED "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN "\x1b[36m"
#define ANSI_RESET "\x1b[0m"

#define STANDBY 0
#define START 1
#define FALLING 2

using std::array;
using std::vector;
using std::string;


class Dot {
public:
	int x, y;
	string value = "  ";
	string color = "";
	bool operator==(Dot& other) {
		return (value == other.value) && (color == other.color);
	}
};

class Canvas {
	/*
	 * 좌상: ┌
	 * 좌하 : └
	 * 우상 : ┐
	 * 우하 : ┘
	 * 가로 : ─
	 * 세로 : │
	 * 박스 : □ ■
	 */
	int canvas_left, canvas_top, canvas_width, canvas_height;
	void cursorview(char show)
	{
		HANDLE hConsole;
		CONSOLE_CURSOR_INFO ConsoleCursor;

		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		ConsoleCursor.bVisible = show;
		ConsoleCursor.dwSize = 1;

		SetConsoleCursorInfo(hConsole, &ConsoleCursor);
	}
	void gotoxy(int x, int y) {
		COORD Pos;
		Pos.X = x;
		Pos.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
	}
	void DrawDot(int x, int y, string value, string color) {
		if (x < 0 || y < 0 || x > canvas_width || y > canvas_height) {
			return;
		}
		gotoxy((canvas_left + x) * 2, canvas_top + y);
		printf((color + value + "\x1b[0m").c_str());
	}
public:
	Canvas(int left, int top, int width, int height) : canvas_left(left), canvas_top(top), canvas_width(width), canvas_height(height) {
		cursorview(0);
	}
	~Canvas() {
		gotoxy(0, canvas_top + canvas_height);
	}
	void DrawBorder() {
		for (int i = 1; i < canvas_height - 1; i++) {
			gotoxy(canvas_left * 2, canvas_top + i);
			printf("│");
			gotoxy((canvas_left + canvas_width - 1) * 2, canvas_top + i);
			printf("│");
		}
		for (int i = 1; i < canvas_width - 1; i++) {
			gotoxy((canvas_left + i) * 2, canvas_top);
			printf("─");
			gotoxy((canvas_left + i) * 2, canvas_top + canvas_height - 1);
			printf("─");
		}
		gotoxy(canvas_left * 2, canvas_top);
		printf("┌");
		gotoxy(canvas_left * 2, canvas_top + canvas_height - 1);
		printf("└");
		gotoxy((canvas_left + canvas_width - 1) * 2, canvas_top);
		printf("┐");
		gotoxy((canvas_left + canvas_width - 1) * 2, canvas_top + canvas_height - 1);
		printf("┘");
	}
	void DrawBox(int left, int top, int width, int height) {
		if ((left < 0) || (top < 0) || (left + width > canvas_width) || (top + height > canvas_height)) {
			// 박스가 캔버스 범위 초과
			return;
		}
		for (int i = 1; i < height - 1; i++) {
			gotoxy((canvas_left + left) * 2, (canvas_top + top) + i);
			printf("│");
			gotoxy((canvas_left + left + width - 1) * 2, (canvas_top + top) + i);
			printf("│");
		}
		for (int i = 1; i < width - 1; i++) {
			gotoxy((canvas_left + left + i) * 2, (canvas_top + top));
			printf("─");
			gotoxy((canvas_left + left + i) * 2, canvas_top + top + height - 1);
			printf("─");
		}
		gotoxy((canvas_left + left) * 2, canvas_top + top);
		printf("┌");
		gotoxy((canvas_left + left) * 2, canvas_top + top + height - 1);
		printf("└");
		gotoxy((canvas_left + left + width - 1) * 2, canvas_top + top);
		printf("┐");
		gotoxy((canvas_left + left + width - 1) * 2, canvas_top + top + height - 1);
		printf("┘");
	}
	void DrawDots(vector<Dot>& dots) {
		for (Dot dot : dots) {
			DrawDot(dot.x, dot.y, dot.value, dot.color);
		}
	}
};


/*
	테트리스 게임
	'보드'와 '블럭'
	보드는 가로 10, 세로 20칸
	블럭은 7종류, 색상값을 가짐.
	블럭은 오른쪽, 왼쪽, 아래쪽으로 각각 한칸씩 이동 가능
	블럭은 각 모양에 맞게 회전 가능
	블럭은 아래쪽으로 이동 시, 보드 바닥이나 다른 블럭에 막힌다면 고정됨
	블럭은 오른쪽, 왼쪽으로 이동시, 보드 벽면이나 다른 블럭에 막힌다면 이동하지 않음
	블럭은 회전 시, 보드 벽면이나 다른 블럭에 막힌다면 그 경우에 따라 반대 방향으로 이동하여 회전 재시도.

			* 너비와 높이는 박스가 아닌 컨텐츠기준
		* 좌상: ┌
		* 좌하: └
		* 우상: ┐
		* 우하: ┘
		* 가로: ─
		* 세로: │
		* 박스: □ ■
*/




class Block {
	array< vector<int>, 7 > blocks = {
	vector<int> { 0x00f0, 0x2222 }, // I
	vector<int> { 0x0270, 0x0232, 0x0072, 0x0262 }, // T
	vector<int> { 0x0660 }, // O
	vector<int> { 0x0360, 0x0231 }, // S
	vector<int> { 0x0c60, 0x04c8 }, // Z
	vector<int> { 0x0470, 0x0322, 0x0071, 0x0226 }, // J
	vector<int> { 0x02e0, 0x0446, 0x00e8, 0x0c44 } // L
	};
	int type, rotation; // blocks[type][rotation]
	int x, y, color;
	array<array<int, 4>, 4> block = { 0, };
	array<array<int, 4>, 4> oldblock = { 0, };
	array<int, 3> log;
public:
	Block() {}
	void init(int type = 0, int color = 1) {
		x = 3;
		y = -2;
		rotation = 0;
		this->type = type;
		this->color = color;
		updateblock();
	}
	void updateblock() {
		backupblock();
		int number, hex, binary;
		number = blocks[type][rotation];
		for (int i = 0; i < 4; i++) {
			hex = number % 16;
			for (int j = 0; j < 4; j++) {
				binary = hex % 2;
				block[3 - i][3 - j] = binary * color;
				hex /= 2;
			}
			number /= 16;
		}
	}
	void backupblock() { oldblock = block; }
	void revertblock() { block = oldblock; }
	void backupx() { log[0] = x; }
	void revertx() { x = log[0]; }
	void backupy() { log[1] = y; }
	void reverty() { y = log[1]; }
	void backuprotation() { log[2] = rotation; }
	void revertrotation() { rotation = log[2]; }

	void up() { y--; }
	void down() { y++; }
	void left() { x--; }
	void right() { x++; }
	void rotate() {
		if (++rotation == blocks[type].size()) rotation = 0;
		updateblock();
	}
	void setx(int x) { this->x = x; }
	void sety(int y) { this->y = y; }
	void clear() {
		block = { 0, };
		oldblock = { 0, };
		x = 0;
		y = 0;
		rotation = 0;
		log = { 0, };
	}

	auto& getoldblock() { return oldblock; }
	auto getoldx() { return log[0]; }
	auto getoldy() { return log[1]; }
	auto getx() { return x; }
	auto gety() { return y; }
	auto getcolor() { return color; }
	auto gettype() { return type; }

	/*
	auto compare() { // 예전 블럭과 현재 블럭의 충돌지점 반환
		vector<int> collisions;
		int oldx = log[0], oldy = log[1];
		if (abs(x - oldx) > 3 || abs(y - oldy) > 3) return collisions;
		int imin, imax, jmin, jmax;
		imin = y <= oldy ? 0 - y + oldy : 0;
		imax = y > oldy ? 4 - y + oldy : 4;
		jmin = x <= oldx ? 0 - x + oldx : 0;
		jmax = x > oldx ? 4 - x + oldx : 4;

		for (int i = imin; i < imax; i++) {
			for (int j = jmin; j < jmax; j++) {
				if (block[i][j] && oldblock[i + y - oldy][abs(j + x - oldx)]) collisions.push_back(i * 4 + j);
			}
		}
		return collisions;
	}
	*/

	auto& operator[] (int index) {
		return block[index];
	}
};

class Board {
	array<array<int, 10>, 20> board = { 0, };
	array<array<int, 10>, 20> oldboard = { 0, };
public:
	void backupboard() {
		oldboard = board;
	}
	auto compare() {
		vector<int> collisions;
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 10; j++) {
				if (board[i][j] != oldboard[i][j]) collisions.push_back(i * 10 + j);
			}
		}
		return collisions;
	}

	void clear(int index) {
		for (int i = index; i > 0; i--) {
			board[i] = board[i - 1];
		}
	}
	void checkclear() {
		int full;
		for (int i = 0; i < 20; i++) {
			full = 1;
			for (int j = 0; j < 10; j++) {
				if (!board[i][j]) full = 0;
			}
			if (full) clear(i);
		}
	}

	void print() {
		system("cls");
		for (auto row : board) {
			for (auto e : row) {
				printf("%d", e);
			}
			printf("\n");
		}
		printf("\n");
		for (auto row : oldboard) {
			for (auto e : row) {
				printf("%d", e);
			}
			printf("\n");
		}
	}

	auto& operator[] (int index) {
		return board[index];
	}
};

class Game {
	Block block;
	Block illusion;
	Board board;
	Canvas canvas = Canvas(0, 0, 50, 30);
	std::mutex m;
	std::condition_variable cv;
	int moveable = 0;
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_int_distribution<int> dis;
	int step = STANDBY;
public:
	Game() {
		gen = std::mt19937(rd());
		dis = std::uniform_int_distribution<int>(0, 6);

		canvas.DrawBorder();
		canvas.DrawBox(1, 1, 12, 22);
	}
	auto compare(Block& block) {
		remove(block);
		int x = block.getx();
		int y = block.gety();
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (block[i][j]) {
					if (i + y >= 20) return 1; // 바닥에 충돌
					else if (j + x >= 10 || j + x < 0) return 2; // 벽면에 충돌
					else if (i + y >= 0 && board[i + y][j + x] && board[i + y][j + x] != 8) { return 3; } // 자신의 이전 상태가 아닌 벽돌과 충돌, 
				}
			}
		}
		//reinput(block);
		return 0; // 충돌 없음
	}
	void input(Block& block) {
		//remove(block);
		int x = block.getx();
		int y = block.gety();
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (block[i][j] && j + x < 10 && j + x >= 0 && i + y < 20 && i + y >= 0) {
					board[i + y][j + x] = block[i][j];
				}
			}
		}
	}
	void remove(Block& block) {
		auto oldblock = block.getoldblock();
		int x = block.getoldx(), y = block.getoldy();
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (oldblock[i][j] && i + y < 20 && i + y >= 0 && j + x < 10 && j + x >= 0) {
					if (oldblock[i][j] == board[i + y][j + x]) {
						board[i + y][j + x] = 0;
					}
				}
			}
		}
	}
	void reinput(Block& block) {
		auto oldblock = block.getoldblock();
		int x = block.getoldx(), y = block.getoldy();
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (oldblock[i][j] && i + y < 20 && i + y >= 0 && j + x < 10 && j + x >= 0) {
					board[i + y][j + x] = oldblock[i][j];
				}
			}
		}
	}

	int createblock(Block& block) {
		block.init(dis(gen), dis(gen) + 1);
		block.backupblock();
		block.backupx();
		block.backupy();
		int collision = compare(block);
		if (collision != 3) {
			input(block);
			block.backupblock();
			block.backupx();
			block.backupy();
			block.backuprotation();
			return 1;
		}
		else {
			return 0;
		}
	}
	int lowerblock(Block& block) {
		block.down();
		int collision = compare(block);
		if (!collision) {
			input(block);
			block.backupy();
			return 1;
		} else {
			block.reverty();
			reinput(block);
			return 0;
		}
	}
	int dropblock(Block& block) {
		block.down();
		int collision = compare(block);
		if (!collision) {
			return dropblock(block);
		} else {
			block.up();
			collision = compare(block);
			if (!collision) {
				input(block);
				block.backupy();
				return 1;
			} else {
				return 0;
			}
		}
	}
	int moveleftblock(Block& block) {
		block.left();
		int collision = compare(block);
		if (!collision) {
			input(block);
			block.backupx();
			return 1;
		} else {
			block.revertx();
			return 0;
		}
	}
	int moverightblock(Block& block) {
		block.right();
		int collision = compare(block);
		if (!collision) {
			input(block);
			block.backupx();
			return 1;
		}
		else {
			block.revertx();
			return 0;
		}
	}
	int rotateblock(Block& block) {
		block.rotate();
		int collision = compare(block);
		if (!collision) {
			input(block);
			block.backupblock();
			block.backuprotation();
			return 1;
		}
		else {
			block.revertblock();
			block.revertrotation();
			return 0;
		}
	}

	void view() {
		vector<string> colors = {
			"",
			"\x1b[0m",
			"\x1b[31m",
			"\x1b[32m",
			"\x1b[33m",
			"\x1b[34m",
			"\x1b[35m",
			"\x1b[36m",
		};
		vector<Dot> dots;
		Dot dot;
		vector<int> changes = board.compare();
		int i, j;
		for (auto index : changes) {
			i = index / 10;
			j = index % 10;
			dot.x = j + 2;
			dot.y = i + 2;
			dot.color = colors[board[i][j] < 8 ? board[i][j] : 1];
			dot.value = board[i][j] == 8 ? "□" :
				board[i][j] == 0 ? "  " : "■";
			dots.push_back(dot);
		}
		canvas.DrawDots(dots);
	}
	int create() {
		int success = createblock(block);
		if (!success) {
			this->step = STANDBY;
			return 0;
		}
		remove(block);
		illusion.init(block.gettype(), 8);
		illusion.backupblock();
		illusion.backupx();
		illusion.backupy();
		dropblock(illusion);
		reinput(block);
		view();
		//board.print();
		board.backupboard();
		this->step = FALLING;
		this->moveable = 1;
		return 1;
	}
	int down() {
		int success = lowerblock(block);
		if (!success) {
			this->step = START;
			this->moveable = 0;
			board.backupboard();
			board.checkclear();
			view();
			return 0;
		}
		view();
		//board.print();
		board.backupboard();
		this->step = FALLING;
		return 1;
	}
	int fall() {
		int success = dropblock(block);
		if (!success) {
			board.backupboard();
			return 0;
		}
		board.checkclear();
		view();
		//board.print();
		board.backupboard();
		return 1;
	}

	int left() {
		int success = moveleftblock(block);
		if (!success) return 0;
		remove(block);
		illusion.sety(block.gety());
		moveleftblock(illusion);
		dropblock(illusion);
		reinput(block);
		view();
		//board.print();
		board.backupboard();
		return 1;
	}

	int right() {
		int success = moverightblock(block);
		if (!success) return 0;
		remove(block);
		illusion.sety(block.gety());
		moverightblock(illusion);
		dropblock(illusion);
		reinput(block);
		view();
		//board.print();
		board.backupboard();
		return 1;
	}

	int rotate() {
		int success = rotateblock(block);
		if (!success) return 0;
		remove(block);
		illusion.sety(block.gety());
		rotateblock(illusion);
		dropblock(illusion);
		reinput(block);
		view();
		//board.print();
		board.backupboard();
		return 1;
	}

	void keyboard() {
		while (1) {
			// 75 left
			// 77 right
			// 80 down
			// 32 space
			// 72 up
			int key = 0;
			key = _getch();
			if (this->moveable && key) {
				m.lock();
				if (key == 75) {
					left();
				}
				else if (key == 77) {
					right();
				}
				else if (key == 80) {
					if (!down()) {
						this->step = START;
						this->moveable = 0;
					}
				}
				else if (key == 32) {
					this->step = START;
					this->moveable = 0;
					fall();
					cv.notify_one();
				}
				else if (key == 72) {
					rotate();
				}
				m.unlock();
			}
		};
	}

	void start() {
		std::thread t([this]() {keyboard(); });
		std::unique_lock<std::mutex> lk(m);
		
		do {
			Sleep(200);
			//m.lock();
			create();
			//m.unlock();
			do {
				if (cv.wait_for(lk, std::chrono::seconds(1), [&]() {return this->step == START; })) break;
				if (!down()) {
					this->step = START;
					this->moveable = 0;
				}
			} while (this->step == FALLING);
		} while (this->step == START);

		t.join();
	}

};


int main() {
	Game game;
	game.start();
}