#include <Windows.h>

TCHAR szWndAppName[] = TEXT("WndTest");

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// 0. 윈도우 메인 함수 
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR    lpCmdLine,
	int       nCmdShow)
{

	// 1. 윈도우 클래스 정의 

	WNDCLASS wc = { 0 };
	wc.style = 0;			   					 // 윈도우 클래스의 스타일 종류
	wc.lpfnWndProc = WndProc;						     // 메시지 처리 함수인 윈도우 프로시저 포인터	
	wc.cbClsExtra = 0;                                   // 윈도우 클래스 데이터 예약 영역 초기값 0  
	wc.cbWndExtra = 0;                                   // 윈도우 데이터 예약 영역, 초기값 0  
	wc.hInstance = hInstance; 				             // 해당 윈도우 클래스를 등록한 응용프로그램의 인스턴스
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);				  // 윈도우가 최소화 될때 보여줄 아이콘 	
	wc.hCursor = LoadCursor(0, IDC_CROSS);			          // 해당 윈도우 클래스로 만들어진 윈도우의 마우스 커서 핸들, IDC_ARROW, IDC_CROSS, IDC_IBEAM, IDC_NO, IDC_WAIT
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  // 윈도우 작업 영역을 칠할 배경 브러쉬 핸들 (흰색)	
	wc.lpszMenuName = NULL;								      // 사용할 메뉴를 지정한다. 	
	wc.lpszClassName = szWndAppName;				          // 등록하고자 하는 윈도우 클래스 이름 

	if (!RegisterClass(&wc))
		return -1;


	// 2. 윈도우 생성 
	HWND   hWnd;   //
	hWnd = CreateWindow(szWndAppName, szWndAppName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);



	// 3. 윈도우 화면 출력 
	ShowWindow(hWnd, nCmdShow);

	//MessageBox(NULL, TEXT("Hello"), TEXT("Hello"), MB_OK);


	// 4. 메시지 루프 
	MSG Message;

	while (GetMessage(&Message, 0, 0, 0)) //메시지 루프에서 메시지 꺼냄(WM_QUIT이면 FALSE 반환)
	{
		TranslateMessage(&Message);       //WM_KEYDOWN이고 키가 문자 키일 때 WM_CHAR 발생
		DispatchMessage(&Message);        //콜백 프로시저가 수행할 수 있게 디스패치 시킴
	}

	return 0;
}

// 0.1. 윈도우 프로시저 
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