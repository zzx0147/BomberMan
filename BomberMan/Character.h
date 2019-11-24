#pragma once
#include <Windows.h>
class Character
{
public:
	Character();
	~Character();

	void Frame(HWND hWnd);
	void Render(HINSTANCE,HDC);
	RECT GetCharacterRect();
private:
	int x;
	int y;
};

