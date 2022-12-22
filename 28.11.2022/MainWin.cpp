#include "MainWin.h"

const wstring MainWin::GetLibDir()
{
	static wstring dir;

	if (!fs::is_directory(dir))
	{
		dir.resize(DIRSTR_LEN);
		GetModuleFileName(NULL, (wchar_t*)dir.c_str(), dir.size());
		
		size_t i = wcslen(dir.c_str());
		while (dir.at(i) != L'\\') dir.erase(i--);

		dir += RUN().DirName;
		//if (!fs::is_directory(dir)) RUN().CreateDir();
	}

	return dir;
}

BOOL MainWin::OnInit(HWND hWnd, HWND Focus, LPARAM lPara)
{
	_h.hDialog = hWnd;

	hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));
	SetMenu(hWnd, hMenu);

	_h.hLibList = GetDlgItem(hWnd, IDC_LIST1);
	_h.hBookList = GetDlgItem(hWnd, IDC_LIST2);

	_h.hStBooks = GetDlgItem(hWnd, IDC_STATIC_BOOKS);

	_h.hEdSearchLib = GetDlgItem(hWnd, IDC_EDIT_SEARCH_LIB);

	//CreateDir();

	const int NumTab = 2;
	int Tabs[NumTab] = { 120, 20 };
	SendMessage(_h.hBookList, LB_SETTABSTOPS, NumTab, (LPARAM)Tabs);

	RefindLib(L"");

	return TRUE;
}

VOID MainWin::OnCommand(HWND hWnd, int id, HWND Ctl, UINT Notify)
{
	switch (id)
	{
	//Библиотеки
	case ID_LIBRARY_CREATE: CreateLib(); break;
	case ID_LIBRARY_DELETE: DeleteLib(); break;
	case IDC_LIST1:
		switch (Notify)
		{
		case LBN_SELCHANGE: SelChangeLib(); break;
		}
	break;
	case IDC_EDIT_SEARCH_LIB:
		switch (Notify)
		{
		case EN_UPDATE: SearchLib(); break;
		}
	break;
	case IDC_BUTT_REFRESH: RefreshLib(); break;

	//Книги
	case ID_BOOK_CREATE: CreateBook(); break;
	case ID_BOOK_DELETE: DeleteBook(); break;
	case IDC_LIST2:
		switch (Notify)
		{
		case LBN_SELCHANGE:
			EnableMenuItem(hMenu, ID_BOOK_DELETE, MF_BYCOMMAND | MF_ENABLED);
			break;
		case LBN_DBLCLK: SelChangeBook(); break;
		}
	break;

	//Прочее
	case ID_EXIT: EndDialog(hWnd, NULL); break;
	}
}

VOID MainWin::OnClose(HWND hWnd)
{
	EndDialog(hWnd, NULL);
}

void MainWin::CreateDir()
{
	if (!fs::is_directory(GetLibDir()))
	{
		if (IDYES == MessageBox(
			_h.hDialog,
			L"Library folder not found. Create?", 
			L"Error", 
			MB_YESNO | MB_ICONERROR
		)) fs::create_directory(GetLibDir());
		else EndDialog(_h.hDialog, FALSE);
	}
}

void MainWin::ListItemToWSTR(wstring& wstr, HWND hWnd, int index)
{
	wstr.resize(SendMessage(hWnd, LB_GETTEXTLEN, (WPARAM)index, 0));
	SendMessage(hWnd, LB_GETTEXT, (WPARAM)index, (LPARAM)wstr.data());
}

MainWin& MainWin::RUN()
{
	static MainWin inst;
	return inst;
}

BOOL MainWin::WinStartup(HINSTANCE Inst)
{
	return DialogBox(
		Inst,
		MAKEINTRESOURCE(MainWin::DialogID),
		NULL,
		(DLGPROC)MainWin::DlgProc
	);
}

BOOL MainWin::DlgProc(HWND hWnd, UINT mess, WPARAM wParam, LPARAM lParam)
{
	switch (mess)
	{
		HANDLE_MSG(hWnd, WM_INITDIALOG, RUN().OnInit);
		HANDLE_MSG(hWnd, WM_COMMAND, RUN().OnCommand);
		HANDLE_MSG(hWnd, WM_CLOSE, RUN().OnClose);
	default: return FALSE;
	}
}

const MainWin::Handls& MainWin::GetHWNDS() const
{
	return _h;
}