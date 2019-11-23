#include <Windows.h>
#include "resource.h"
#include "SpritesLoader.h"
#include "GameMap.h"

#pragma comment(lib,"Msimg32.lib")

TCHAR szWndAppName[] = TEXT("WndTest");

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HINSTANCE g_hInstance;
GameMap map;

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


	if (!RegisterClass(&wc))
		return -1;

	g_hInstance = hInstance;

	map.Init();

	HWND   hWnd; 
	hWnd = CreateWindow(szWndAppName, szWndAppName,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 736, 759,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);


	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if(PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
		}
	}


	DeleteObject(bgBrush);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;


	switch (message) 
	{
		case WM_KEYDOWN:
			switch (wParam)
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
			}
			return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		map.Render(g_hInstance, hdc);

		EndPaint(hWnd, &ps);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}