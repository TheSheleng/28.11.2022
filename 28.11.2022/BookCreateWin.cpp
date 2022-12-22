#include "BookCreateWin.h"

BOOL BookCreateWin::OnInit(HWND hWnd, HWND Focus, LPARAM lParam)
{
	_h.hDialog = hWnd;

	_h.hName = GetDlgItem(hWnd, IDC_EDIT_NAME);
	_h.hPath = GetDlgItem(hWnd, IDC_EDIT_PATH);

	_h.hStatus = GetDlgItem(hWnd, IDC_COMBO_STATUS);
	_h.hGenre = GetDlgItem(hWnd, IDC_COMBO_GENRE);

	for (const wstring& i : bk::Statuses) SendMessage(_h.hStatus, CB_ADDSTRING, 0, (LPARAM)i.c_str());
	for (const wstring& i : bk::Genres) SendMessage(_h.hGenre, CB_ADDSTRING, 0, (LPARAM)i.c_str());

	_h.hBPath = GetDlgItem(hWnd, IDC_BUTT_PATH);
	_h.hEnter = GetDlgItem(hWnd, IDC_BUTT_ENTER);

	return TRUE;
}

VOID BookCreateWin::OnCommand(HWND hWnd, int id, HWND Ctl, UINT Notify)
{
	switch (id)
	{
	case IDC_BUTT_PATH:
	{
		wstring buff;
		buff.reserve(DIRSTR_LEN);

		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = (LPWSTR)buff.data();
		ofn.nMaxFile = DIRSTR_LEN;
		ofn.lpstrFilter = L"Text file (*.txt)\0*.txt\0";
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn)) SetWindowText(_h.hPath, buff.c_str());
	}

	case IDC_EDIT_NAME:
	case IDC_EDIT_PATH:
	case IDC_COMBO_STATUS:
	case IDC_COMBO_GENRE:
	{
		BOOL EnterEnable = FALSE;

		wstring buff;

		HWNDtoWSTR(&buff, _h.hName);
		if (buff != L"")
		{
			HWNDtoWSTR(&buff, _h.hPath);
			if (buff != L"")
			{
				HWNDtoWSTR(&buff, _h.hStatus);
				if (buff != L"")
				{
					HWNDtoWSTR(&buff, _h.hGenre);
					if (buff != L"")
					{
						EnterEnable = TRUE;
					}
				}
			}
		}

		EnableWindow(_h.hEnter, EnterEnable);
	} break;

	case IDC_BUTT_ENTER:
	{
		GetWindowText(_h.hName, book.name, bk::SZ_FILENAME);
		GetWindowText(_h.hPath, book.path, DIRSTR_LEN);

		book.name[bk::SZ_FILENAME] = '\0';
		book.path[DIRSTR_LEN] = '\0';

		book.status = SendMessage(_h.hStatus, CB_GETCURSEL, 0, 0);
		book.genre = SendMessage(_h.hGenre, CB_GETCURSEL, 0, 0);

		EndDialog(hWnd, TRUE);
	} break;
	}
}

VOID BookCreateWin::OnClose(HWND hWnd)
{
	EndDialog(hWnd, FALSE);
}

BookCreateWin& BookCreateWin::RUN()
{
	static BookCreateWin inst;
	return inst;
}

BOOL BookCreateWin::DlgProc(HWND hWnd, UINT mess, WPARAM wParam, LPARAM lParam)
{
	switch (mess)
	{
		HANDLE_MSG(hWnd, WM_INITDIALOG, RUN().OnInit);
		HANDLE_MSG(hWnd, WM_COMMAND, RUN().OnCommand);
		HANDLE_MSG(hWnd, WM_CLOSE, RUN().OnClose);
	default: return FALSE;
	}
}

BOOL BookCreateWin::WinStartup(HWND hPar)
{
	return DialogBox(
		NULL,
		MAKEINTRESOURCE(BookCreateWin::DialogID),
		hPar,
		(DLGPROC)BookCreateWin::DlgProc
	);
}

const bk::Book& BookCreateWin::GetBook() const
{
	return book;
}

const BookCreateWin::Handls& BookCreateWin::GetHWNDS() const
{
	return _h;
}
