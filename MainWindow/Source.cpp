#include<Windows.h>
#include<stdio.h>

CONST CHAR g_szCLASS_NAME[] = "MyWindowClass";
CONST CHAR g_szTitle[] = "My first window";

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstanse, HINSTANCE hPrevList, LPSTR lpCmdLine, INT nCmdShow)
{
	//1)����������� ������ ����
	WNDCLASSEX wc;	//wc-Window class
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;	//��������� �� ��������� ����
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstanse;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 4);
	wc.lpszClassName = g_szCLASS_NAME;
	wc.lpszMenuName = NULL;
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Class registration failed!", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	//2) �������� �����
	int screen_width = GetSystemMetrics(SM_CXSCREEN);
	int screen_height = GetSystemMetrics(SM_CYSCREEN);
	int window_width = screen_width*0.5;
	int window_height = screen_height*0.5;
	int window_start_x = screen_width / 4;
	int window_start_y = screen_height / 4;

	HWND hwnd = CreateWindowEx
	(
		WS_EX_CLIENTEDGE,	//exStyle
		g_szCLASS_NAME,		//��� ������ ����
		g_szTitle,		//��������� ����
		WS_OVERLAPPEDWINDOW,//����� ���� ������ �������� ��� �������� ���� ���������
		window_start_x, window_start_y,	//������� ���� �� ������
		window_width,window_height,	//������ ����
		NULL,	//������������ ����
		NULL,	//���� �����������
		hInstanse, //*.exe ���� ����
		NULL
	);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);	//������ ����� ����������� ����
	UpdateWindow(hwnd);		//���������� ���� (���������� ��������� WM_PAINT)

	//3)��������� ���� ���������:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		break;
	case WM_COMMAND:
		break;
	case WM_SIZE:
	case WM_MOVE:
	{
		RECT rect;
		GetWindowRect(hwnd, &rect);
		CONST INT SIZE = 256;
		CHAR buffer[SIZE]{};
		SendMessage(hwnd, WM_GETTEXT, SIZE, (LPARAM)buffer);
		sprintf_s
		(
			buffer,"%s Position: %dx%d, Size: %dx%d",
			g_szTitle,
			rect.left, rect.top, 
			rect.right-rect.left, rect.bottom-rect.top
		);
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)buffer);
	}break;
	case WM_CLOSE:
		switch (MessageBox(hwnd, "�� ������������� ������ ������� ����?", "Info", MB_YESNO|MB_ICONQUESTION))
		{
		case IDYES:
			//MessageBox(hwnd, "��� ������, �����", "Info", MB_OK | MB_ICONINFORMATION);
			DestroyWindow(hwnd); break;
		case IDNO: MessageBox(hwnd, "Ok", "Info", MB_OK | MB_ICONINFORMATION); break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}