#include <Windows.h>

#include <sstream>
#include <crtdbg.h>

#include "resource.h"

#include "InputClass.h"
#include "TimeClass.h"
#include "Player.h"
#include "GameMap.h"
#include "BombManager.h"

#include "SpritesLoader.h"

using namespace std;

#pragma comment(lib,"Msimg32.lib")

TCHAR szWndAppName[] = TEXT("WndTest");

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HINSTANCE g_hInstance;

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR    lpCmdLine,
	int       nCmdShow)
{
	HBRUSH bgBrush = CreateSolidBrush(RGB(56, 135, 0));

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

	InputClass::Initialize();
	TimeClass::Init();

	SpritesLoader::Initialize(hInstance);

	GameMap::Init();
	Player player;
	player.Init();

	BombManager::AddPlayer(player);

	HWND hWnd;
	hWnd = CreateWindow(szWndAppName, szWndAppName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 736, 759,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	double updateTime = 0.0;

	double fpsShowTime = 0.0;
	double fps = 0;
	wstringstream str;

	MSG msg = { 0 };
	while (WM_QUIT != msg.message && !InputClass::IsKeyDown(VK_ESCAPE))
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		{
			TimeClass::Update();

			double deltaTime = TimeClass::GetDeltaTime();
			updateTime += deltaTime;
			fpsShowTime += deltaTime;

			GameMap::Update();
			BombManager::Update(deltaTime);

			player.Update(deltaTime);

			if (1.0 / 60.0 < updateTime)
			{
				updateTime = 0;

				HDC hdcMain = GetDC(hWnd);

				GameMap::Render();
				BombManager::Render();

				player.Render();

				SpritesLoader::Render(g_hInstance, hdcMain);

				// FPS Show
				++fps;
				if (1.0 < fpsShowTime)
				{
					fpsShowTime = 0;

					str.clear();
					str.str(L"");
					str << "FPS : " << fps;

					fps = 0;
				}

				TextOut(hdcMain, 0, 0, str.str().c_str(), str.str().length());

				ReleaseDC(hWnd, hdcMain);
			}
		}
	}

	SpritesLoader::Release();

	BombManager::Release();

	DeleteObject(bgBrush);

	_CrtDumpMemoryLeaks();

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_KEYDOWN: InputClass::KeyDown((unsigned int)wParam); return 0;
		case WM_KEYUP: InputClass::KeyUp((unsigned int)wParam); return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}