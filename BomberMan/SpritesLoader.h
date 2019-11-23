#pragma once
#include <Windows.h>
#include "resource.h"
class SpritesLoader
{
public:
	static void DrawSprite(HDC hdc,HINSTANCE hInstance,int x,int y,int idxX,int idxY)
	{
		HDC MemDC;
		HBITMAP MyBitmap, OldBitmap;

		MemDC = CreateCompatibleDC(hdc);
		MyBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_SPRITES));
		OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);
		TransparentBlt(hdc, x, y, 48, 48, MemDC, idxX * 16, idxY * 16, 16, 16, RGB(56, 135, 0));
		//StretchBlt(hdc, 0, 0, 224, 383, MemDC, 0, 0, 224, 383, SRCCOPY);
		SelectObject(MemDC, OldBitmap);
		DeleteObject(MyBitmap);
		DeleteDC(MemDC);
	}
};

