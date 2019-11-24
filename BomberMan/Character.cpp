#include "Character.h"
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
		x -= moveSpeed;
	}
	else if (InputClass::IsKeyDown(VK_RIGHT))
	{
		x += moveSpeed;
	}
	else if (InputClass::IsKeyDown(VK_UP))
	{
		y -= moveSpeed;
	}
	else if (InputClass::IsKeyDown(VK_DOWN))
	{
		y += moveSpeed;
	}

	x = x < 0 ? 0 : x + 48 > 720 ? 720 - 48 : x;
	y = y < 0 ? 0 : y + 48 > 720 ? 720 - 48 : y;
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
