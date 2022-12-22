#include "Header.h"

void HWNDtoWSTR(wstring* wstr, HWND hWnd)
{
	wstr->resize(GetWindowTextLength(hWnd));
	GetWindowText(hWnd, (LPWSTR)wstr->data(), wstr->size() + 1);
}