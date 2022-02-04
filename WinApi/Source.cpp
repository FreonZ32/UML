#include<Windows.h>
#include"resource.h"

//#define MESSAGEBOX

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wPARAM, LPARAM lPAram);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPprevInst, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef MESSAGEBOX
	MessageBox(NULL, "Вас взломали, плати!", "Еррор", MB_YESNOCANCEL | MB_HELP | MB_ICONINFORMATION | MB_SYSTEMMODAL);
#endif // MESSAGEBOX

	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, DlgProc, 0);

return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lPAram)
{
	switch (uMsg)
	{
	case WM_INITDIALOG: break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:MessageBox(NULL, "Была нажата кнопка ОК", "Info", MB_ICONINFORMATION); break;
		case IDCANCEL: EndDialog(hwnd, 0); break;
		}
	}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}