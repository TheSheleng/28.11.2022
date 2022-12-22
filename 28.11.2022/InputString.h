#pragma once
#include "Header.h"

class InputWin
{
	InputWin() = default;
	InputWin(const InputWin&) = delete;
	InputWin& operator=(const InputWin&) = delete;

	HHOOK hEnterKeyHook;
	static LRESULT CALLBACK EnterKeyProc(int, WPARAM, LPARAM);

	struct Handls
	{
		HWND hDialog, hEnter;
	} _h;

	BOOL OnInit(HWND, HWND, LPARAM);
	VOID OnCommand(HWND, int, HWND, UINT);
	VOID OnClose(HWND);

	static const int DialogID = IDD_DIALOG_INPUTSTR;
	static BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

	wstring title;
	wstring str;

public:
	static InputWin& RUN();
	BOOL WinStartup(HWND, wstring);
	const wstring& GetString() const;
	const Handls& GetHWNDS() const;
};