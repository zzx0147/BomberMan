#include <Windows.h>
#include "resource.h"

TCHAR szWndAppName[] = TEXT("WndTest");

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HINSTANCE g_hInstance;

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR    lpCmdLine,
	int       nCmdShow)
{
	HBRUSH bgBrush = CreateSolidBrush(RGB(56,135,0));

	WNDCLASS wc = { 0 };
	wc.style = 0;			   	
	wc.lpfnWndProc = WndProc;		
	wc.cbClsExtra = 0;                
	wc.cbWndExtra = 0;               
	wc.hInstance = hInstance; 			
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);				
	wc.hCursor = LoadCursor(0, IDC_ARROW);			        
	wc.hbrBackground = bgBrush;
	wc.lpszMenuName = NULL;								    
	wc.lpszClassName = szWndAppName;				        

	g_hInstance = hInstance;

	if (!RegisterClass(&wc))
		return -1;

	HWND   hWnd; 
	hWnd = CreateWindow(szWndAppName, szWndAppName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	MSG Message;

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);  
		DispatchMessage(&Message); 
	}

	DeleteObject(bgBrush);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, MemDC;
	PAINTSTRUCT ps;
	HBITMAP MyBitmap, OldBitmap;

	switch (message) 
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		MemDC = CreateCompatibleDC(hdc);

		MyBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_SPRITES));
		OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);
		StretchBlt(hdc, 0, 0, 48, 48, MemDC, 64, 0, 16, 16, SRCCOPY);

		SelectObject(MemDC, OldBitmap);
		DeleteObject(MyBitmap);
		DeleteDC(MemDC);

		EndPaint(hWnd, &ps);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}