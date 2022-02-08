#include<Windows.h>

INT WINAPI WinMain(HINSTANCE Hinstance, HINSTANCE HprevInst, CALPSTR ilCndLine, int nCmdShow)
{
	MessageBox(NULL, "Hello", "Info", MB_OK|MB_ICONASTERISK);
	return 0;
}