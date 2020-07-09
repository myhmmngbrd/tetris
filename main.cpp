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
		if (++rotation = blocks[type].size()) rotation = 0;
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
		auto c = compare();
		for (auto e : c) {
			printf(" %d ", e);
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
	std::random_device rd;

	std::mt19937 gen;

	std::uniform_int_distribution<int> dis;
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
				if (block[i][j] && i + y >= 0) {
					if (i + y >= 20) return 1; // 바닥에 충돌
					else if (j + x >= 10 || j + x < 0) return 2; // 벽면에 충돌
					else if (board[i + y][j + x] && board[i + y][j + x] != 8) { return 3; } // 자신의 이전 상태가 아닌 벽돌과 충돌, 
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
				if (oldblock[i][j] && i + y < 20 && i + y >= 0 && j + x < 20 && j + x >= 0) {
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
				if (oldblock[i][j] && i + y < 20 && i + y >= 0 && j + x < 20 && j + x >= 0) {
					board[i + y][j + x] = oldblock[i][j];
				}
			}
		}
	}

	void createblock(Block& block) {
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
		}
	}

	void lowerblock(Block& block) {
		block.down();
		int collision = compare(block);
		if (!collision) {
			input(block);
			block.backupy();
		} else {
			block.reverty();
		}
	}

	void dropblock(Block& block) {
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
				return;
			} else {
				block.reverty();
				return;
			}
		}
	}

	void moveleftblock(Block& block) {
		block.left();
		int collision = compare(block);
		if (!collision) {
			input(block);
			block.backupx();
			return;
		} else {
			block.revertx();
			return;
		}
	}
	void moverightblock(Block& block) {
		block.right();
		int collision = compare(block);
		if (!collision) {
			input(block);
			block.backupx();
			return;
		}
		else {
			block.revertx();
			return;
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

	void start() {
		Sleep(500);
		createblock(block);
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
		Sleep(500);
		lowerblock(block);
		view();
		//board.print();
		board.backupboard();
		Sleep(500);
		lowerblock(block);
		view();
		//board.print();
		board.backupboard();
		Sleep(500);
		dropblock(block);
		view();
		//board.print();
		board.backupboard();

		Sleep(500);
		createblock(block);
		remove(block);
		illusion.init(block.gettype(), 8);
		illusion.backupblock();
		illusion.backupx();
		illusion.backupy();
		dropblock(illusion);
		reinput(block);
		view();

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
	Game game;
	game.start();
}