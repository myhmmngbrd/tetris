#pragma once
class Canvas
{
	const int left, top, width, height;
	// 커서 깜박임 제거
	void cursorview(char show);
	// 커서 이동
	void gotoxy(int x, int y);
public:
	Canvas(int x, int y, int w, int h);
	~Canvas();
	//일정한 스타일로 박스 그리기, 캔버스 범위를 벗어나지 않도록 교정
	void DrawBox(int x, int y, int w, int h);
};

