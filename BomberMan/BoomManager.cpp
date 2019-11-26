#include "BoomManager.h"

#include "Boom.h"
#include "GameMap.h"

Boom* BoomManager::_booms;//[_maxCount];
int  BoomManager::_count = 0;

BoomManager::BoomManager()
{
}

BoomManager::~BoomManager()
{
	Release();
}

void BoomManager::CreateBoom(const int x, const int y)
{
	if (_maxCount <= _count) return;

	if (GameMap::IsMovePoint(x, y))
	{
		GameMap::SetBlock(x, y);
		_booms[_count].Init(x, y, _count++);
	}
}

void BoomManager::DeleteBoom(const int idx)
{
	if (_booms[idx].GetIdx() != idx) return;
	if (idx >= _count) return;

	GameMap::RemoveBlock(_booms[idx].GetX(), _booms[idx].GetY());

	for (auto i = _count; i > idx; --i)
	{
		_booms[i - 1] = _booms[i];
	}
	--_count;
}

void BoomManager::Init()
{
	_count = 0;
	_booms = new Boom[_maxCount];
}

void BoomManager::Update(double deltaTime)
{
	for (auto i = 0; i < _count; ++i)
	{
		_booms[i].Update(deltaTime);
	}
}

void BoomManager::Render()
{
	for (auto i = 0; i < _count; ++i)
	{
		_booms[i].Render();
	}
}

void BoomManager::Release()
{
	if (0 != _booms)
	{
		delete[] _booms;
		_booms = 0;
	}
}
