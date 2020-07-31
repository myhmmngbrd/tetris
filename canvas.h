#pragma once

#include<windows.h>
#include<iostream>
#include<array>
#include<vector>
#include<deque>
#include<thread>
#include<string>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<chrono>

struct Dot {
	int x, y;
	std::string value;
};

class Canvas {
	// 캔버스의 크기 정보
	int canvas_left, canvas_top, canvas_width, canvas_height;
	std::deque<Dot> dots;
	std::deque<std::deque<Dot>> log;
	// 뷰 쓰레드
	std::thread t;
	std::condition_variable cv;
	// 마우스 제거함수
	void cursorview(char);
	// gotoxy 함수
	void gotoxy(int, int);
public:
	Canvas(int, int, int, int, std::mutex* m);
	~Canvas();
	void push(int, int, std::string);
	void draw(std::mutex *m);
};

