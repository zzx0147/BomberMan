#include "Bomb.h"

#include <cmath>

#include "SpritesLoader.h"
#include "BombManager.h"
#include "Player.h"
#include "GameMap.h"


Bomb::Bomb()
{
}


Bomb::~Bomb()
{
}

void Bomb::Burst()
{
	BombManager::DeleteBomb(_idxNum);

	return;
}

void Bomb::Init(const int x, const int y, const int idxNum)
{
	_x = x;
	_y = y;
	_idxNum = idxNum;
	_aniSpeed = 0;
	_delTime = 0;
	_isActive = true;
	_isOnPlayer = true;
}

void Bomb::Update(double deltaTime)
{
	if (!_isActive) return;

	static int sign = 1;
	_aniSpeed = _aniSpeed + deltaTime * 3 * sign;
	if (_aniSpeed > 2 || _aniSpeed < 0) sign *= -1;

	_delTime += deltaTime;
	if (_delTime > 3.0)
	{
		Burst();
	}
	
	if (!_isOnPlayer) return;

	if (!(round(Player::GetX() / 48) * 48 == _x && round(Player::GetY() / 48) * 48 == _y))
	{
		_isOnPlayer = false;

		GameMap::SetBlock(_x, _y);
	}
}

void Bomb::Render()
{
	if (!_isActive) return;

	SpritesLoader::DrawSprite(_x, _y, 0 + _aniSpeed, 3);
}

void Bomb::Release()
{
}

const int Bomb::GetIdx() const
{
	return _idxNum;
}

const double Bomb::GetX() const
{
	return _x;
}

const double Bomb::GetY() const
{
	return _y;
}
