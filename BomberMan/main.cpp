#include <Windows.h>

TCHAR szWndAppName[] = TEXT("WndTest");

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR    lpCmdLine,
	int       nCmdShow)
{
	WNDCLASS wc = { 0 };
	wc.style = 0;			   	
	wc.lpfnWndProc = WndProc;		
	wc.cbClsExtra = 0;                
	wc.cbWndExtra = 0;               
	wc.hInstance = hInstance; 			
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);				
	wc.hCursor = LoadCursor(0, IDC_ARROW);			        
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); 
	wc.lpszMenuName = NULL;								    
	wc.lpszClassName = szWndAppName;				        

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

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rt = { 50, 50, 400, 400 };
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}