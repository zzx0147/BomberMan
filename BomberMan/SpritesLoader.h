#pragma once
#include <Windows.h>
#include "resource.h"
class SpritesLoader
{
public:
	static void Initialize(HINSTANCE hInstance);
	static void DrawSprite(HDC hdc, HINSTANCE hInstance, int x, int y, int idxX, int idxY);
	static void Release();

private:
	static HBITMAP Sprites;
};



