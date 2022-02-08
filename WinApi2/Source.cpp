#include<Windows.h>
#include"resource.h"


//#define MSGBOX
BOOL CALLBACK DLGProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR ilCmdLine, int nCmdShow)
{
#ifdef MSGBOX
	MessageBox(NULL, "Hello", "Info", MB_OK | MB_TOPMOST);
#endif // MSG
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, DLGProc, 0);
	return 0;
}

BOOL CALLBACK DLGProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG: break;
	case WM_COMMAND: 
	{
		switch (LOWORD(wParam))
		{
		case IDOK: MessageBox(NULL, "Была нажата кнопка ОК", "Info", MB_ICONINFORMATION); break;
		case IDCANCEL: EndDialog(hwnd, 0); break;
		default:
			break;
		}
	} break;
	case WM_CLOSE:
		EndDialog(hwnd, 0); break;
	}
	return FALSE;
}