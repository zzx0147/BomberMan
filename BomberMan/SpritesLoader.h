#pragma once
#include <Windows.h>
#include "resource.h"
struct RenderInfo
{
	int x;
	int y;
	int idxX;
	int idxY;
};

class SpritesLoader
{
public:
	static void Initialize(HINSTANCE hInstance);
	static void DrawSprite(HDC hdc, HINSTANCE hInstance, int x, int y, int idxX, int idxY);
	static void Release();
	static void Render(HINSTANCE,HDC);


private:
	static HBITMAP _Sprites;
	static RenderInfo* _RenderTargets;
	static int _num;
};



