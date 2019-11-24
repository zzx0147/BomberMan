#pragma once
#include <Windows.h>

class GameMap
{
private:
	static int _map[15][15];

	static const int _width = 15;
	static const int _height = 15;
public:
	static void Init();
	static void Update();
	static void Render();

	static const bool IsMovePoint(const int x, const int y);
};

