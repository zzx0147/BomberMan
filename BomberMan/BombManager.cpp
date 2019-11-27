#include "BombManager.h"

#include <cmath>

#include "Bomb.h"
#include "GameMap.h"

std::list<Bomb*> BombManager::_bombs;//[_maxCount];
std::list<Bomb*> BombManager::_removeBombs;
int  BombManager::_count = 0;

BombManager::BombManager()
{
}

BombManager::~BombManager()
{
	Release();
}

void BombManager::CreateBomb(const int x, const int y)
{
	if (GameMap::IsMovePoint(x, y))
	{
		Bomb* Bomb(new Bomb);
		Bomb->Init(round(x / 48) * 48, round(y / 48) * 48, _count++);

		_bombs.push_back(Bomb);
	}
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

void BombManager::Init()
{
	_count = 0;
}

void BombManager::Update(double deltaTime)
{
	if (_bombs.empty()) return;
	std::list<Bomb*>::iterator it;
	for (it = _bombs.begin(); it != _bombs.end(); ++it)
	{
		(*it)->Update(deltaTime);
	}

	if (_removeBombs.empty()) return;

	for (it = _removeBombs.begin(); it != _removeBombs.end(); ++it)
	{
		Bomb* removeBomb = *it;
		for (auto iter = _bombs.begin(); iter != _bombs.end(); ++iter)
			if ((*iter)->GetIdx() == (*it)->GetIdx())
			{
				_bombs.erase(iter);
				break;
			}

		_removeBombs.erase(it);

		delete removeBomb;
		if (_removeBombs.empty()) break;
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

	for (auto it = _bombs.begin(); it != _bombs.end(); ++it)
	{
		Bomb* removeBomb = *it;

		_bombs.erase(it);
		--_count;

		delete removeBomb;
	}
}
