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
	int type = 0; // block[type]
	int rotation = 0; // block[type][rotation]
	int x = 3, y = -2; // x=0/y=0 : block[0][0], board[0][0] 
	int color; // 0~6
	array<int, 3> movelog = { 0, };
	array< array<int, 4>, 4> block = { 0, };
	array< array<int, 4>, 4> oldblock = { 0, };
	void posbackup(int x, int y, int r) {
		movelog[0] = x;
		movelog[1] = y;
		movelog[2] = r;
	}
	void blockbackup() {
		oldblock = block;
	}
	void create() {
		blockbackup();
		int number, hex, binary;
		number = block[type][rotation];
		for (int i = 0; i < 4; i++) {
			hex = number % 16;
			for (int j = 0; j < 4; j++) {
				binary = hex % 2;
				block[3 - i][3 - j] = binary;
				hex /= 2;
			}
			number /= 16;
		}
	}
public:
	Block() {
		init();
	}
	void init() {
		x = 3;
		y = -2;
		rotation = 0;
		type = 0; //랜덤함수
		color = 0; // 랜덤함수
	}
	void back() {
		x = movelog[0];
		y = movelog[1];
		if (rotation != movelog[2]) { // rotate 했다면
			block = oldblock;
			rotation = movelog[2];
		}
	}
	void down() { posbackup(x, y++, rotation); }
	void left() { posbackup(x--, y, rotation); }
	void right() { posbackup(x++, y, rotation); }
	void rotate() { 
		posbackup(x, y, rotation);
		if (++rotation == blocks[type].size()) rotation = 0;
		create();
	}
	int getblock() { return blocks[type][rotation]; }
	int getx() { return x; }
	int gety() { return y; }
	int getoldx() { return movelog[0]; }
	int getoldy() { return movelog[1]; }
	int getcolor() { return color; }
};

class Board {
	/*
		0 : 빈칸
		1 ~ 7 : 색상 블럭
	*/
	array< array<int, 10>, 20> board = { 0, };
	array< array<int, 10>, 20> oldboard = { 0, };
public:
	void backup() {
		oldboard = board;
	}
	void deleterow(int row) {
		for (int i = row; i > 0; i--) {
			board[i] = board[i - 1];
		}
		board[0] = { 0, };
	}
	vector<int> compare() {
		vector<int> collisions;
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 10; j++) {
				if (board[i][j] != oldboard[i][j]) {
					collisions.push_back(i * 10 + j);
				}
			}
		}
		return collisions;
	}
	array<int, 10>& operator[] (int index) {
		return board[index];
	}
};

class Game {
	Block block;
	Board board;
	Canvas canvas = Canvas(0, 0, 30, 30);
	array< array<int, 4>, 4> mino = { 0, };
	array< array<int, 4>, 4> oldmino = { 0, };
public:
	Game() {
		block.init();
		canvas.DrawBorder();
		canvas.DrawBox(1, 1, 12, 22);
		CreateBlock();
		view();
		Sleep(500);
		LowerBlock();
		view();
	}
	~Game () {
		//delete canvas;
	}
	void print() {
		for (auto row : mino) {
			for (auto e : row) {
				printf("%d", e);
			}
			printf("\n");
		}
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 10; j++) {
				printf("%d", board[i][j]);
			}
			printf("\n");
		}

	}

	void blocktoarray() {
		
	}
	void CreateBlock() {
		board.backup();
		block.init();
		blocktoarray();
		int crash = crashcheck();
		std::cout << crash << std::endl;
		if (crash != 1) {
			inputblock();
		}
	}
	void ShiftLeftBlock() {
		board.backup();
	}
	void ShiftRightBlock() {

	}
	void LowerBlock() {
		board.backup();
		block.down();
		int crash = crashcheck();
		if (!crash) {
			inputblock();
		}
	}
	void DropBlock() {

	}
	int crashcheck() {
		// 벽에 부딪히는 경우 : block의 인덱스가 board 범위 초과
		// 블럭에 부딪히는 경우 : block의 인덱스와 board의 1 이상의 인덱스 충돌
			// 본인 블럭에 부딪히는 경우 제외
				// 이동 시, oldx, oldy
				// 회전시, oldmino
		int x, y, oldi, oldi;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				x = j + block.getx();
				oldj = x - block.getoldx();

				y = i + block.gety();
				if (mino[i][j]) {
					if (x < 0 || x >= 10 || y < 0 || y >= 20) {
						return -1;
					} else if (board[y][x]) {
						return 1;
					}
				}
			}
		}
		return 0;
	}
	void removeblock() {
		int x, y;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				x = j + block.getx();
				y = i + block.gety();
				if (mino[i][j] && x >= 0 && x < 10 && y >= 0 && y < 20) {
					board[y][x] = 0;
				}
			}
		}
	}
	void inputblock() {
		int x, y;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				x = j + block.getx();
				y = i + block.gety(); 
				if (mino[i][j] && x >= 0 && x < 10 && y >= 0 && y < 20) {
					board[y][x] = mino[i][j];
				}
			}
		}
	}
	void view() {
		vector<Dot> dots;
		Dot dot;
		vector<int> elements = board.compare();
		int value;
		array<string, 7> colors = {
			ANSI_RESET,
			ANSI_RED,
			ANSI_GREEN,
			ANSI_YELLOW,
			ANSI_BLUE,
			ANSI_MAGENTA,
			ANSI_CYAN,
		};
		for (auto index : elements) {
			value = board[index / 10][index % 10];
			dot.x = index % 10 + 2;
			dot.y = index / 10 + 2;
			if (value) {
				dot.color = colors[value];
				dot.value = "■";
			}
			dots.push_back(dot);
		}
		canvas.DrawDots(dots);
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

	Game tetris;

}