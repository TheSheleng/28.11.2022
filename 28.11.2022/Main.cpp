#include "Header.h"
#include "MainWin.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE nPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
	return MainWin::RUN().WinStartup(hInst);
}