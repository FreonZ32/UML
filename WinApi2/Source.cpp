#include<Windows.h>
#include"resource.h"

CONST CHAR Invite[] = "Enter login pls";

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
	case WM_INITDIALOG:
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);
		SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_SETTEXT, 0, (LPARAM)Invite);
		//SendMessage(GetDlgItem(hwnd,IDC_EDIT1), WM_SETFOCUS, NULL, 0);
		//HWND hBtn = CreateWindowEx();
	}
	break;
	case WM_COMMAND: 
	{
		switch (LOWORD(wParam))
		{
		case IDC_CHECK1: 
			{
				CONST int SIZE = 256;  CHAR buffer[SIZE]{};
				HWND hEdit = GetDlgItem(hwnd, IDC_EDIT1);
				SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)buffer);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 0, (LPARAM)buffer);
			}break;
		case IDOK: MessageBox(NULL, "Была нажата кнопка ОК", "Info", MB_ICONINFORMATION); break;
		case IDCANCEL: EndDialog(hwnd, 0); break;
		case IDC_EDIT1:
			CONST INT SIZE = 256;
			CHAR buffer[SIZE]{};
			SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_GETTEXT, SIZE, (LPARAM)buffer);
			switch (HIWORD(wParam))
			{
			case EN_SETFOCUS:
				{
					if (strcmp(buffer, Invite) == 0)
					SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_SETTEXT, 0, (LPARAM)""); 
				}break;
			case EN_KILLFOCUS:
				{
					if (strcmp(buffer, "") == 0)SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_SETTEXT, 0, (LPARAM)Invite);
				}break;
			}
		}
	} break;
	case WM_CLOSE:
		EndDialog(hwnd, 0); break;
	}
	return FALSE;
}