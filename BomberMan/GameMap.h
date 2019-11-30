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

	static void SetBlock(const double x, const double y, const int blockNum = 2);
	static void RemoveBlock(const double x, const double y);

	static const bool IsMovePoint(const double px, const double py);
	static const bool IsBomb(const int px, const int py);
};

