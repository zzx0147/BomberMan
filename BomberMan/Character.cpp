#include "Character.h"

#include <cmath>
#include <string>

#include "SpritesLoader.h"
#include "InputClass.h"
#include "GameMap.h"

// 
Character::Character() :x(48), y(48)
{

}


Character::~Character()
{
}

void Character::Frame()
{
	double moveSpeed = 5;

	if (InputClass::IsKeyDown(VK_LEFT))
	{
		y = round(y / 48) * 48;
		if (GameMap::IsMovePoint(x - moveSpeed, y))
		{
			x -= moveSpeed;
		}
		//if (GameMap::IsMovePoint(x - 3, y))
		//{
		//	x -= moveSpeed;
		//}
		//else
		//{
		//	y = round((int)y);
		//	if (GameMap::IsMovePoint(x - 3, y))
		//	{
		//		x -= moveSpeed;
		//	}
		//}
	}
	else if (InputClass::IsKeyDown(VK_RIGHT))
	{
		y = round(y / 48) * 48;
		if (GameMap::IsMovePoint(x + moveSpeed, y))
		{
			x += moveSpeed;
		}
	}
	
	if (InputClass::IsKeyDown(VK_UP))
	{
		x = round(x / 48) * 48;
		if (GameMap::IsMovePoint(x, y - moveSpeed))
		{
			y -= moveSpeed;
		}
	}
	else if (InputClass::IsKeyDown(VK_DOWN))
	{
		x = round(x / 48) * 48;
		if (GameMap::IsMovePoint(x, y + moveSpeed))
		{
			y += moveSpeed;
		}
	}

	x = x < 0 ? 0 : x + 48 > 672 ? 672 - 48 : x;
	y = y < 0 ? 0 : y + 48 > 672 ? 672 - 48 : y;
}

void Character::Render()
{
	SpritesLoader::DrawSprite(x, y, 4, 0);
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
