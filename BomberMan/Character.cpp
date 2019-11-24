#include "Character.h"
#include "SpritesLoader.h"
#include "InputClass.h"

Character::Character():x(0),y(0)
{
	
}


Character::~Character()
{
}

void Character::Frame(HWND hWnd)
{
	bool haveToRender = false;

	if (InputClass::IsKeyDown(VK_LEFT))
	{
		--x;
		haveToRender = true;
	}
	else if (InputClass::IsKeyDown(VK_RIGHT))
	{
		++x;
		haveToRender = true;
	}
	else if (InputClass::IsKeyDown(VK_UP))
	{
		--y;
		haveToRender = true;
	}
	else if (InputClass::IsKeyDown(VK_DOWN))
	{
		++y;
		haveToRender = true;
	}

	if (haveToRender)
	{
		RECT r;
		r.top = y - 1;
		r.bottom = y + 49;
		r.left = x - 1;
		r.right = x + 49;
		InvalidateRect(hWnd, &r, true);
	}
}

void Character::Render(HINSTANCE hInstance,HDC hdc)
{
	SpritesLoader::DrawSprite(hdc,hInstance,x, y, 3, 0);
}

RECT Character::GetCharacterRect()
{
	RECT r;
	r.top = y;
	r.left = x;
	r.right = y + 48;
	r.bottom = y + 48;

	return RECT();
}
