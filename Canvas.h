#pragma once
class Canvas
{
	const int left, top, width, height;
	// Ŀ�� ������ ����
	void cursorview(char show);
	// Ŀ�� �̵�
	void gotoxy(int x, int y);
public:
	Canvas(int x, int y, int w, int h);
	~Canvas();
	//������ ��Ÿ�Ϸ� �ڽ� �׸���, ĵ���� ������ ����� �ʵ��� ����
	void DrawBox(int x, int y, int w, int h);
};

