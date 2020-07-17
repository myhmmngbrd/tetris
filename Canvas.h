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
	// Ŀ�� ������ ����
	void cursorview(char);
	// Ŀ�� �̵�
	void gotoxy(int, int);
	Log log;
public:
	Canvas(int, int, int, int);
	~Canvas();
	//������ ��Ÿ�Ϸ� �ڽ� �׸���, ĵ���� ������ ����� �ʵ��� ����
	void DrawBox(int, int, int, int);
	void push(Dots);
	void PushString(int, int, std::string);
	void print();
};

