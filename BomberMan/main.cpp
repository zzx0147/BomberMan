#include <Windows.h>

#include <sstream>

#include "resource.h"
#include "SpritesLoader.h"
#include "GameMap.h"
#include "InputClass.h"
#include "Character.h"
#include "TimeClass.h"
#include "BoomManager.h"

using namespace std;

#pragma comment(lib,"Msimg32.lib")

TCHAR szWndAppName[] = TEXT("WndTest");

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HINSTANCE g_hInstance;
GameMap map;
Character character;

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

	BoomManager::Init();

	TimeClass::Init();
	
	InputClass::Initialize();

	SpritesLoader::Initialize(hInstance);

	map.Init();

	HWND hWnd; 
	hWnd = CreateWindow(szWndAppName, szWndAppName,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 736, 759,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	double updateTime = 0.0;

	double fpsShowTime = 0.0;
	int fps = 0;

	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if(PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		{
			TimeClass::Update();

			double deltaTime = TimeClass::GetDeltaTime();
			updateTime += deltaTime;
			fpsShowTime += deltaTime;

			character.Frame(deltaTime);
			BoomManager::Update(deltaTime);

			if (1.0 / 60.0 < updateTime)
			{
				updateTime = 0;

				HDC hdcMain = GetDC(hWnd);

				map.Render();
				character.Render();
				BoomManager::Render();

				SpritesLoader::Render(g_hInstance, hdcMain);

				if (1.0 < fpsShowTime)
				{
					fpsShowTime = 0;

					// FPS Show
					fps = 1.0 / deltaTime;
				}

				wstringstream str;
				str.clear();
				str.str(L"");
				str << "FPS : " << fps << ", " << deltaTime;
				TextOut(hdcMain, 0, 0, str.str().c_str(), str.str().length());

				ReleaseDC(hWnd, hdcMain);
			}
		}
	}

	SpritesLoader::Release();
	DeleteObject(bgBrush);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		case WM_KEYDOWN:
			InputClass::KeyDown((unsigned int)wParam);
			switch (wParam)
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
			}
			return 0;
		case WM_KEYUP:
			InputClass::KeyUp((unsigned int)wParam);
			return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}