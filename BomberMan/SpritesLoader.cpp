#include "SpritesLoader.h"
HBITMAP SpritesLoader::Sprites;

void SpritesLoader::Initialize(HINSTANCE hInstance)
{
	Sprites = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_SPRITES));
}

void SpritesLoader::DrawSprite(HDC hdc, HINSTANCE hInstance, int x, int y, int idxX, int idxY)
{
	HDC MemDC;
	HBITMAP OldBitmap;

	MemDC = CreateCompatibleDC(hdc);

	OldBitmap = (HBITMAP)SelectObject(MemDC, Sprites);
	TransparentBlt(hdc, x, y, 48, 48, MemDC, idxX * 16, idxY * 16, 16, 16, RGB(56, 135, 0));
	//StretchBlt(hdc, x, y, 48, 48, MemDC, idxX * 16, idxY * 16, 16, 16, SRCCOPY);
	SelectObject(MemDC, OldBitmap);
	//DeleteObject(Sprites);
	DeleteDC(MemDC);
}

void SpritesLoader::Release()
{
	DeleteObject(Sprites);
}
