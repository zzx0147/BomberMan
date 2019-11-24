#include "SpritesLoader.h"
HBITMAP SpritesLoader::_Sprites;
RenderInfo* SpritesLoader::_RenderTargets;
int SpritesLoader::_num = 0;

void SpritesLoader::Initialize(HINSTANCE hInstance)
{
	_Sprites = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_SPRITES));
	_RenderTargets = new RenderInfo[500];
}

//void SpritesLoader::DrawSprite(HDC hdc, HINSTANCE hInstance, int x, int y, int idxX, int idxY)
void SpritesLoader::DrawSprite(int x, int y, int idxX, int idxY)
{
	_RenderTargets[_num++] = { x,y,idxX,idxY };
	//HDC MemDC;
	//HBITMAP OldBitmap;

	//MemDC = CreateCompatibleDC(hdc);

	//OldBitmap = (HBITMAP)SelectObject(MemDC, _Sprites);
	//TransparentBlt(hdc, x, y, 48, 48, MemDC, idxX * 16, idxY * 16, 16, 16, RGB(56, 135, 0));
	////StretchBlt(hdc, x, y, 48, 48, MemDC, idxX * 16, idxY * 16, 16, 16, SRCCOPY);
	//SelectObject(MemDC, OldBitmap);
	//DeleteDC(MemDC);
}

void SpritesLoader::Release()
{
	DeleteObject(_Sprites);
}

void SpritesLoader::Render(HINSTANCE hInstance, HDC hdc)
{
	const static RECT rt = { 0,0,736,759 };

	HDC MemDC, backDC;
	MemDC = CreateCompatibleDC(hdc);
	backDC = CreateCompatibleDC(hdc);

	HBITMAP backBit, oldBackBit;
	backBit = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
	oldBackBit = (HBITMAP)SelectObject(backDC, backBit);
	HBRUSH myBrush = CreateSolidBrush(RGB(56, 135, 0));
	FillRect(backDC, &rt, myBrush);

	SelectObject(MemDC, _Sprites);
	// Render
	for (int i = 0; i < _num; ++i)
	{
		TransparentBlt(backDC, _RenderTargets[i].x, _RenderTargets[i].y, 48, 48, MemDC,
			_RenderTargets[i].idxX * 16, _RenderTargets[i].idxY * 16, 16, 16, RGB(56, 135, 0));
	}

	BitBlt(hdc, 0, 0, rt.right, rt.bottom, backDC, 0, 0, SRCCOPY);

	DeleteObject(SelectObject(backDC, oldBackBit));
	DeleteObject(myBrush);
	DeleteDC(backDC);
	DeleteDC(MemDC);
	_num = 0;
}
