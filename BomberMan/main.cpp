#include <Windows.h>

TCHAR szWndAppName[] = TEXT("WndTest");

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// 0. ������ ���� �Լ� 
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR    lpCmdLine,
	int       nCmdShow)
{

	// 1. ������ Ŭ���� ���� 

	WNDCLASS wc = { 0 };
	wc.style = 0;			   					 // ������ Ŭ������ ��Ÿ�� ����
	wc.lpfnWndProc = WndProc;						     // �޽��� ó�� �Լ��� ������ ���ν��� ������	
	wc.cbClsExtra = 0;                                   // ������ Ŭ���� ������ ���� ���� �ʱⰪ 0  
	wc.cbWndExtra = 0;                                   // ������ ������ ���� ����, �ʱⰪ 0  
	wc.hInstance = hInstance; 				             // �ش� ������ Ŭ������ ����� �������α׷��� �ν��Ͻ�
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);				  // �����찡 �ּ�ȭ �ɶ� ������ ������ 	
	wc.hCursor = LoadCursor(0, IDC_CROSS);			          // �ش� ������ Ŭ������ ������� �������� ���콺 Ŀ�� �ڵ�, IDC_ARROW, IDC_CROSS, IDC_IBEAM, IDC_NO, IDC_WAIT
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  // ������ �۾� ������ ĥ�� ��� �귯�� �ڵ� (���)	
	wc.lpszMenuName = NULL;								      // ����� �޴��� �����Ѵ�. 	
	wc.lpszClassName = szWndAppName;				          // ����ϰ��� �ϴ� ������ Ŭ���� �̸� 

	if (!RegisterClass(&wc))
		return -1;


	// 2. ������ ���� 
	HWND   hWnd;   //
	hWnd = CreateWindow(szWndAppName, szWndAppName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);



	// 3. ������ ȭ�� ��� 
	ShowWindow(hWnd, nCmdShow);

	//MessageBox(NULL, TEXT("Hello"), TEXT("Hello"), MB_OK);


	// 4. �޽��� ���� 
	MSG Message;

	while (GetMessage(&Message, 0, 0, 0)) //�޽��� �������� �޽��� ����(WM_QUIT�̸� FALSE ��ȯ)
	{
		TranslateMessage(&Message);       //WM_KEYDOWN�̰� Ű�� ���� Ű�� �� WM_CHAR �߻�
		DispatchMessage(&Message);        //�ݹ� ���ν����� ������ �� �ְ� ����ġ ��Ŵ
	}

	return 0;
}

// 0.1. ������ ���ν��� 
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