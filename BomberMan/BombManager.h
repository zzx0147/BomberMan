#pragma once
#include <iostream>
#include <list>

class Bomb;

class BombManager
{
private:
	static int _count;
	static std::list<Bomb*> _bombs;
	static std::list<Bomb*> _removeBombs;
public:
	BombManager();
	~BombManager();

	static void CreateBomb(const int x, const int y);
	static void DeleteBomb(const int idx);

	static void Init();
	static void Update(double deltaTime);
	static void Render();
	static void Release();
};

