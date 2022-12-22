#include "InputString.h"

LRESULT CALLBACK InputWin::EnterKeyProc(int idCode, WPARAM wParam, LPARAM lParam)
{
	//Если поймано событие и клавиша Ентер и фокус на едите вернуть строку
	if (HC_ACTION == idCode && wParam == VK_RETURN && GetFocus() == RUN().GetHWNDS().hEnter)
	{
		HWNDtoWSTR(&RUN().str, RUN().GetHWNDS().hEnter);
		EndDialog(RUN().GetHWNDS().hDialog, TRUE);
	}
	else return CallNextHookEx(RUN().hEnterKeyHook, idCode, wParam, lParam);
}

BOOL InputWin::OnInit(HWND hWnd, HWND Focus, LPARAM lParam)
{
	//Установка заголовка
	SetWindowText(hWnd, InputWin::title.c_str());

	_h.hDialog = hWnd;
	_h.hEnter = GetDlgItem(hWnd, IDC_EDIT_STR);
	hEnterKeyHook = SetWindowsHookEx(WH_KEYBOARD, EnterKeyProc, NULL, GetCurrentThreadId());

	return TRUE;
}

VOID InputWin::OnCommand(HWND hWnd, int id, HWND Ctl, UINT Notify)
{
	switch (id)
	{
	case IDC_BUTT_SEND: 
	{	
		HWNDtoWSTR(&str, _h.hEnter);
		EndDialog(_h.hDialog, TRUE); 
	} break;
	}
}

VOID InputWin::OnClose(HWND hWnd)
{
	EndDialog(hWnd, FALSE);
}

InputWin& InputWin::RUN()
{
	static InputWin inst;
	return inst;
}

BOOL InputWin::WinStartup(HWND hPar, wstring title)
{
	this->title = title;

	return DialogBox(
		NULL,
		MAKEINTRESOURCE(InputWin::DialogID),
		hPar,
		(DLGPROC)InputWin::DlgProc
	);
}

const wstring& InputWin::GetString() const
{
	return str;
}

const InputWin::Handls& InputWin::GetHWNDS() const
{
	return _h;
}

BOOL InputWin::DlgProc(HWND hWnd, UINT mess, WPARAM wParam, LPARAM lParam)
{
	switch (mess)
	{
		HANDLE_MSG(hWnd, WM_INITDIALOG, RUN().OnInit);
		HANDLE_MSG(hWnd, WM_COMMAND, RUN().OnCommand);
		HANDLE_MSG(hWnd, WM_CLOSE, RUN().OnClose);
	default: return FALSE;
	}
}