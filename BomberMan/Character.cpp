#include "Character.h"

#include <cmath>

#include "BoomManager.h"
#include "SpritesLoader.h"
#include "InputClass.h"
#include "GameMap.h"

Character::Character() :x(48), y(48)
{

}

Character::~Character()
{
}

void Character::Frame(double deltaTime)
{
	static double aniResetTime = 0.0;
	double moveSpeed = 3 * 48 * deltaTime;

	aniResetTime += deltaTime;
	if (InputClass::IsKeyDown(VK_LEFT))
	{
		if (GameMap::IsMovePoint(x - moveSpeed, round(y / 48) * 48))
		{
			aniResetTime = 0;
			_dir = eDirection::LEFT;

			x -= moveSpeed;
			y = round(y / 48) * 48;

			_aniSpeed = _aniSpeed > 2 ? 0 : _aniSpeed + (deltaTime * 3);
		}
	}
	else if (InputClass::IsKeyDown(VK_RIGHT))
	{
		if (GameMap::IsMovePoint(x + moveSpeed, round(y / 48) * 48))
		{
			aniResetTime = 0;
			_dir = eDirection::RIGHT;

			x += moveSpeed;
			y = round(y / 48) * 48;

			_aniSpeed = _aniSpeed > 2 ? 0 : _aniSpeed + (deltaTime * 3);
		}
	}

	if (InputClass::IsKeyDown(VK_UP))
	{
		if (GameMap::IsMovePoint(round(x / 48) * 48, y - moveSpeed))
		{
			aniResetTime = 0;
			_dir = eDirection::UP;

			x = round(x / 48) * 48;
			y -= moveSpeed;

			_aniSpeed = _aniSpeed > 2 ? 0 : _aniSpeed + (deltaTime * 3);
		}
	}
	else if (InputClass::IsKeyDown(VK_DOWN))
	{
		if (GameMap::IsMovePoint(round(x / 48) * 48, (y + moveSpeed)))
		{
			aniResetTime = 0;
			_dir = eDirection::DOWN;

			x = round(x / 48) * 48;
			y += moveSpeed;

			_aniSpeed = _aniSpeed > 2 ? 0 : _aniSpeed + (deltaTime * 3);
		}
	}

	if (InputClass::IsKeyDown(VK_SPACE))
	{
		if (GameMap::IsMovePoint(x, y))
		{
		//	BoomManager::CreateBoom(x, y);

		}
	}

	x = x < 0 ? 0 : x > 720 - 48 ? 720 - 48 : x;
	y = y < 0 ? 0 : y > 720 - 48 ? 720 - 48 : y;

	if (.2f < aniResetTime)
	{
		aniResetTime = 0;
		_aniSpeed = 1;
	}

}

void Character::Render()
{
	switch (_dir)
	{
		case LEFT: SpritesLoader::DrawSprite(x, y, 0 + _aniSpeed, 0); break;
		case DOWN: SpritesLoader::DrawSprite(x, y, 3 + _aniSpeed, 0); break;
		case RIGHT: SpritesLoader::DrawSprite(x, y, 0 + _aniSpeed, 1); break;
		case UP: SpritesLoader::DrawSprite(x, y, 3 + _aniSpeed, 1); break;
	}
}

RECT Character::GetCharacterRect()
{
	RECT r;
	r.top = y;
	r.left = x;
	r.right = x + 48;
	r.bottom = y + 48;

	return RECT();
}
