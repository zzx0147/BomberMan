#include "BombManager.h"

#include <cmath>

#include "Bomb.h"
#include "Player.h"
#include "GameMap.h"
#include "InputClass.h"


std::vector<Bomb*> BombManager::_bombs;
std::vector<Bomb*> BombManager::_removeBombs;
int  BombManager::_count = 0;
std::vector<Player*> BombManager::_players;

BombManager::BombManager()
{
}

BombManager::~BombManager()
{
	Release();
}

void BombManager::CreateBomb(const int x, const int y)
{
	int px = round(x / 48.0) * 48.0;
	int py = round(y / 48.0) * 48.0;

	for (auto it = _bombs.begin(); it != _bombs.end(); ++it)
		if ((*it)->GetX() == px && (*it)->GetY() == py) 
			return;

	Bomb* Bomb(new Bomb);
	Bomb->Init(px, py, _count++);

	_bombs.push_back(Bomb);

	InputClass::KeyUp(VK_SPACE);
}

void BombManager::DeleteBomb(const int idx)
{
	if (idx >= _count) return;

	for (auto it = _bombs.begin(); it != _bombs.end(); ++it)
	{
		if ((*it)->GetIdx() == idx)
		{
			--_count;

			GameMap::RemoveBlock((*it)->GetX(), (*it)->GetY());

			_removeBombs.push_back(*it);

			break;
		}
	}
}

void BombManager::AddPlayer(Player & p)
{
	_players.push_back(&p);
}

void BombManager::Init()
{
	_count = 0;
}

void BombManager::Update(double deltaTime)
{
	if (!_bombs.empty())
	{
		for (auto itb = _bombs.begin(); itb != _bombs.end(); ++itb)
		{
			if ((*itb)->GetOnPlayer() && !_players.empty())
			{
				int bx = round((*itb)->GetX() / 48.0) * 48;
				int by = round((*itb)->GetY() / 48.0) * 48;

				for (auto itp = _players.begin(); itp != _players.end(); ++itp)
				{
					int px = round((*itp)->GetX() / 48.0) * 48 + 24;
					int py = round((*itp)->GetY() / 48.0) * 48 + 24;


				}
			}

			(*itb)->Update(deltaTime);
		}
	}

	if (!_removeBombs.empty())
	{
		for (auto itrb = _removeBombs.begin(); itrb != _removeBombs.end(); ++itrb)
		{
			Bomb* removeBomb = *itrb;
			for (auto iter = _bombs.begin(); iter != _bombs.end(); ++iter)
				if ((*iter)->GetIdx() == (*itrb)->GetIdx())
				{
					_bombs.erase(iter);
					break;
				}

			_removeBombs.erase(itrb);

			delete removeBomb;
			if (_removeBombs.empty()) break;
		}
	}
}

void BombManager::Render()
{
	if (_bombs.empty()) return;

	for (auto it = _bombs.begin(); it != _bombs.end(); ++it)
	{
		(*it)->Render();
	}
}

void BombManager::Release()
{
	if (_bombs.empty()) return;

	for (auto itb = _bombs.begin(); itb != _bombs.end(); ++itb)
	{
		Bomb* removeBomb = *itb;

		_bombs.erase(itb);
		--_count;

		delete removeBomb;
	}

	if (_players.empty()) return;

	for (auto itp = _players.begin(); itp != _players.end(); ++itp)
	{
		Player* removePlayer = *itp;

		_players.erase(itp);
		
		delete removePlayer;
	}
}
