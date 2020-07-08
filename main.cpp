#include <iostream>
#include <windows.h>
#include <vector>
#include <array>
#include <string>
#include <thread>
#include <conio.h>
#include <random>
#include <future>
#include <functional>
#include <mutex>

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

using std::array;
using std::vector;
using std::string;



array< vector<int>, 7 > blocks = {
	vector<int> { 0x00f0, 0x2222 }, // I
	vector<int> { 0x0270, 0x0232, 0x0072, 0x0262 }, // T
	vector<int> { 0x0660 }, // O
	vector<int> { 0x0360, 0x0231 }, // S
	vector<int> { 0x0c60, 0x04c8 }, // Z
	vector<int> { 0x0470, 0x0322, 0x0071, 0x0226 }, // J
	vector<int> { 0x02e0, 0x0446, 0x00e8, 0x0c44 } // L
};

struct Dot {
	int x, y;
	string value;
	string color;
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
		gotoxy((canvas_left + x) * 2, canvas_top);
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
		for (int i = 1; i < canvas_height-1; i++) {
			gotoxy(canvas_left*2, canvas_top + i);
			printf("│");
			gotoxy((canvas_left + canvas_width - 1) * 2, canvas_top + i);
			printf("│");
		}
		for (int i = 1; i < canvas_width-1; i++) {
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
			gotoxy((canvas_left + dot.x) * 2, canvas_top + dot.y);
		}
	}
};


namespace Mino {
	class Block {
	protected:
		vector<int> type;
		int index = 0;
		int x = 3, y = -2;
		array < array<int, 4>, 4> block = { 0, };
		array < array<int, 4>, 4> environment = { 0, };
		int count() {
			return index == type.size() ?  (index=0)++ : index++;
		}
	public:
		Block(vector<int> type) : type(type) { }
		void createBlock() {
			int shape = type[count()];
			for (int i = 0; i < 4; i++) {
				int hexa = shape % 16;
				for (int j = 0; j < 4; j++) {
					int binary = hexa % 2;
					block[3-i][3-j] = binary;
					hexa /= 2;
				}
				shape /= 16;
			}
		}
		void print() {
			std::cout << std::hex << type[0] << std::endl;
			for (auto row : block) {
				for (auto element : row) {
					printf("%d", element);
				}
				printf("\n");
			}
		}
		vector<int> collisioncheck(array< array<int, 10>, 20> board) {
			for (int i = 0; i < 4;  i++) {
				for (int j = 0; j < 4; j++) {
					
				}
			}
		}
	};

	class Imino : public Block {
	public:
		Imino() : Block({ 0x00f0, 0x2222 }) { }
	};

	class Tmino : public Block {
	public:
		Tmino() : Block({ 0x0270, 0x0232, 0x0072, 0x0262 }) { }
	};

	class Omino : public Block {
	public:
		Omino() : Block({ 0x0660 }) { }
	};

	class Smino : public Block {
	public:
		Smino() : Block({ 0x0360, 0x0231 }) { }
	};

	class Zmino : public Block {
	public:
		Zmino() : Block({ 0x0c60, 0x04c8 }) { }
	};

	class Jmino : public Block {
	public:
		Jmino() : Block({ 0x0470, 0x0322, 0x0071, 0x0226 }) { }
	};

	class Lmino : public Block {
	public:
		Lmino() : Block({ 0x02e0, 0x0446, 0x00e8, 0x0c44 }) { }
	};
}

class Board {
	array< array<int, 10>, 20> board = { 0, };
	array< array<int, 10>, 20> before = { 0, };
public:
	void backup() { // 현재 상태를 저장하여, 나중에 출력시 다른 부분 찾는데 사용
		before = board;
	}
	vector<int>& compare() {
		static vector<int> defer;
		for (unsigned int i = 0; i < board.size(); i++) {
			for (unsigned int j = 0; j < board[0].size(); j++) {
				if (board[i][j] != before[i][j]) defer.push_back(i * board.size() + j);
			}
		}
		return defer;
	}
	array<int, 10>& operator[] (int index) {
		return board[index];
	}
};


/*

class Block {
	array< array<int, 4>, 4 > box = { 0, };
	array< array<int, 4>, 4 > before = { 0, };
	array<int, 2> log;
	int x = 0, y = 0; // box[0][1] = {x:1, y:0}
public:
	void backup() {
		log[0] = x;
		log[1] = y;
	}
	void cancel() {
		x = log[0];
		y = log[1];
	}
	void print() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				printf("%d ", box[i][j]);
			}
			printf("\n");
		}
	}
	array<int, 4>& operator[] (int index) {
		return box[index];
	}


	void init(int  type) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				box[i][j] = 0;
			}
		}
		switch (type)
		{
		case I_BLOCK:
			box[2][0] = 1;
			box[2][1] = 1;
			box[2][2] = 1;
			box[2][3] = 1;
			break;
		case O_BLOCK:
			box[1][1] = 1;
			box[1][2] = 1;
			box[2][1] = 1;
			box[2][2] = 1;
			break;
		case L_BLOCK:
			box[1][1] = 1;
			box[2][1] = 1;
			box[2][2] = 1;
			box[2][3] = 1;
			break;
		case J_BLOCK:
			box[1][2] = 1;
			box[2][2] = 1;
			box[2][1] = 1;
			box[2][0] = 1;
			break;
		case S_BLOCK:
			box[2][0] = 1;
			box[2][1] = 1;
			box[1][1] = 1;
			box[1][2] = 1;
			break;
		case Z_BLOCK:
			box[2][3] = 1;
			box[2][2] = 1;
			box[1][2] = 1;
			box[1][1] = 1;
			break;
		case T_BLOCK:
			box[1][1] = 1;
			box[2][0] = 1;
			box[2][1] = 1;
			box[2][2] = 1;
			break;
		default:
			break;
		}
		x = 3;
		y = -2;
	}
	void down() {
		y++;
	}
	void left() {
		x--;
	}
	void right() {
		x++;
	}
	void rotate() {
		int tmp[4][4];
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				before[i][j] = box[i][j];
				tmp[i][j] = box[j][3 - i];
			}
		}
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				box[i][j] = tmp[i][j];
			}
		}
	}
	void rollback() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				box[i][j] = before[i][j];
			}
		}
	}
	int hor() {
		return x;
	}
	int ver() {
		return y;
	}
};

class Board {
	array<array<int, 10>, 20> box = { 0, };
public:
	array<array<int, 10>, 20> log = { 0, };
	void print() {
		system("cls");
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 10; j++) {
				printf("%d ", box[i][j]);
			}
			printf("\n");
		}
	}
	array<int, 10>& operator[] (int index) {
		return box[index];
	}
	void backup() {
		log = box;
	}
	void RemoveBlock() {
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 10; j++) {
				if (box[i][j] == 1) box[i][j] = 0;
			}
		}
	}
	void RemoveRow(int index) {
		for (int i = index; i > 0; i--) {
			box[i] = box[i - 1];
		}
	}
	void CheckFull () {
		int hole = 1;
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 10; j++) {
				if (!box[i][j]) hole = 1;
			}
			if (hole) hole = 0;
			else RemoveRow(i);
		}
	}
	void solid() {
		CheckFull();
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 10; j++) {
				if (box[i][j]) box[i][j] = 2;
			}
		}
	}
};

class Game {
	Block block;
	Board board;
	std::mutex m;
	void message(string m) {
		gotoxy(30, 10);
		std::cout << m << std::endl;
	}
	void cursorview(char show)//커서숨기기
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
	void drawbox(int x, int y, int w, int h) {
		
		* 너비와 높이는 박스가 아닌 컨텐츠기준
		* 좌상: ┌
		* 좌하: └
		* 우상: ┐
		* 우하: ┘
		* 가로: ─
		* 세로: │
		* 박스: □ ■
		
		gotoxy(x * 2, y);
		printf("┌");
		gotoxy(x * 2 + w * 2 + 2, y);
		printf("┐");
		gotoxy(x * 2, y + h + 1);
		printf("└");
		gotoxy(x * 2 + w * 2 + 2, y + h + 1);
		printf("┘");
		for (int i = 2; i <= w * 2; i = i + 2) {
			gotoxy(x * 2 + i, y);
			printf("─");
			gotoxy(x * 2 + i, y + h + 1);
			printf("─");
		}
		for (int i = 1; i <= h; i++) {
			gotoxy(x * 2, y + i);
			printf("│");
			gotoxy(x * 2 + w * 2 + 2, y + i);
			printf("│");
		}
	}
	void Input() { // 보드에 블럭 집어넣기
		board.RemoveBlock();
		int x = block.hor();
		int y = block.ver();
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (block[i][j] && (y + i >= 0) && (x + j >= 0)) {
					board[y + i][x + j] = 1;
				}
			}
		}
	}
	int Create(int type) {
		block.backup();
		block.init(type);
		std::vector<int> crash = CrashCheck();

		if (crash.size()) { //충돌하면
			return 0; //게임종료
		}
		else {
			board.backup();
			Input();
			return 1;
		}
	}
	int down() {
		block.backup();
		board.backup();
		block.down();
		std::vector<int> crash = CrashCheck();
		if (!crash.size()) { // 충돌 없으면
			Input();
			return 1;
		}
		else { // 있으면
			block.cancel();
			board.solid();
			return 0;
		}
	}
	void left() {
		block.backup();
		block.left();
		std::vector<int> crash = CrashCheck();
		if (!crash.size()) {
			board.backup();
			Input();
		} else block.cancel();
	}
	void right() {
		block.backup();
		block.right();
		std::vector<int> crash = CrashCheck();
		if (!crash.size()) {
			board.backup();
			Input();
		} else block.cancel();
	}
	void fall() {
		block.backup();
		block.down();
		std::vector<int> crash = CrashCheck();
		if (!crash.size()) {
			return fall();
		} else {
			board.backup();
			block.cancel();
			Input();
			board.solid();
			return;
		}
	}
	void rotate() {
		block.rotate();
		std::vector<int> crash = CrashCheck();
		if (!crash.size()) {
			board.backup();
			Input();
		}
		else block.rollback();
	}
	std::vector<int> CrashCheck() {
		int x = block.hor();
		int y = block.ver();
		std::vector<int> result;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (block[i][j] && ((i + y >= 20) || (j + x < 0) || (j + x >= 10))) {
					result = { -1 };
					break;
				}
				if (block[i][j] && (i + y >= 0) && (j + x >= 0) && (board[i + y][j + x] == 2)) result.push_back(4 * i + j);
			}
		}
		return result;
	}
	void Update() {
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 10; j++) {
				if (board[i][j] != board.log[i][j]) {
					gotoxy(j * 2 + 2, i + 1);
					if (board[i][j] == 1) {
						printf("■");
					}
					else if (board[i][j] == 2) {
						printf("□");
					}
					else if (!board[i][j]) {
						printf("  ");

					}
				}
			}
		}
	}
	void KeyDown() {
		while (1) {
			// 75 left
			// 77 right
			// 80 down
			// 32 space
			// 72 up
			int key = _getch();
			if (key) {
				m.lock();
				std::vector<int> crash;
				if (key == 75) {
					left();
				} else if (key == 77) {
					right();
				} else if (key == 80) {
					down();
				} else if (key == 32) {
					fall();
				} else if (key == 72) {
					rotate();
				}
				Update();
				m.unlock();
			}
		}
	}
public:
	void start() {
		std::random_device rd;

		std::mt19937 gen(rd());

		std::uniform_int_distribution<int> dis(0, 6);

		cursorview(0);
		drawbox(0, 0, 10, 20);
		std::thread t([this]() { KeyDown(); });
		int gameover, landing;
		do {
			Sleep(200);
			m.lock();
			gameover = Create(dis(gen));
			gotoxy(30, 25);
			std::cout << gameover;
			Update();
			m.unlock();
			do {
				Sleep(100);
				m.lock();
				landing = down();
				Update();
				m.unlock();
			} while (landing);
		} while (gameover);
		t.join();
	}
};

*/
int main() {
	//Game tetris;
	//tetris.start();
	Canvas board(0,0,20,20);
	board.DrawBorder();
	board.DrawBox(0,0,10,10);
	board.DrawDot(0, 0, "\b", "\x1b[31m");
}