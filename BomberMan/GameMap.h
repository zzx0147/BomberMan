#pragma once
#include <Windows.h>

class GameMap
{
private:
	int _map[15][15] = { 0 };

	const int _width = 15;
	const int _height = 15;
public:
	GameMap();
	~GameMap();

	void Init();
	void Render(HINSTANCE hInstance, HDC hdc);
};

