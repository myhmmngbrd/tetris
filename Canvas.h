#pragma once
#include <iostream>
#include <windows.h>
#include <array>
#include <vector>
#include <deque>
#include <string>
#include <thread>

using namespace std;

typedef array<int, 3> Dot;
typedef deque<Dot> Dots;
typedef deque<Dots> Log;


class Canvas
{
	thread t;
	const int left, top, width, height;
	// 커서 깜박임 제거
	void cursorview(char);
	// 커서 이동
	void gotoxy(int, int);
	Log log;
public:
	Canvas(int, int, int, int);
	~Canvas();
	//일정한 스타일로 박스 그리기, 캔버스 범위를 벗어나지 않도록 교정
	void DrawBox(int, int, int, int);
	void push(Dots);
	void PushString(int, int, std::string);
	void print();
};

