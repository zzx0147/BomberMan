#pragma once
#include <iostream>
#include <vector>

class Bomb;
class Player;

class BombManager
{
private:
	static int _count;
	static std::vector<Bomb*> _bombs;
	static std::vector<Bomb*> _removeBombs;

	static std::vector<Player*> _players;
public:
	BombManager();
	~BombManager();

	static void CreateBomb(const int x, const int y);
	static void DeleteBomb(const int idx);

	static void AddPlayer(Player& p);

	static void Init();
	static void Update(double deltaTime);
	static void Render();
	static void Release();
};

