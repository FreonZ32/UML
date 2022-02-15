#include<Windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevList, LPSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASS);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL,"Не получилось зарегистрировать класс!","Ошибка", MB_OK);
        return NULL; 
    }
}