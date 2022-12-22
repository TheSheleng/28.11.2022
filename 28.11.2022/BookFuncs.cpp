#include "MainWin.h"
#include <sstream>

void MainWin::CreateBook()
{
	if (BookCreateWin::RUN().WinStartup(_h.hDialog))
	{
		bk::Book book = BookCreateWin::RUN().GetBook();

		wstring LibName;
		ListItemToWSTR(LibName, _h.hLibList, SendMessage(_h.hLibList, LB_GETCURSEL, 0, 0));
		
		ofstream file(GetLibDir() + L'\\' + LibName + Exten, ios::app, ios::binary);
		if (file.is_open())
		{
			file.write((char*)&book, sizeof(bk::Book));

			wstring fullstr;

			static size_t TAB_LEN = 4;

			fullstr = book.name;
			fullstr += L"\t" + bk::Statuses[book.status];
			fullstr += L"\t" + bk::Genres[book.genre];

			OpenShekf.push_back(book.path);

			int index = SendMessage(_h.hBookList, LB_ADDSTRING, 0, (LPARAM)fullstr.c_str());

			EnableMenuItem(hMenu, ID_BOOK_DELETE, MF_BYCOMMAND | MF_ENABLED); 
			SendMessage(_h.hBookList, LB_SETITEMDATA, index,(WPARAM)(OpenShekf.size() - 1));
			SendMessage(_h.hBookList, LB_SETCURSEL, index, 0);

			file.close();
		}
		else
		{
			MessageBox(
				_h.hDialog,
				L"Open file error",
				L"Error",
				MB_OK | MB_ICONERROR
			);
		}
	}
	SelChangeLib();
}

void MainWin::DeleteBook()
{
	wstring LibName;
	ListItemToWSTR(LibName, _h.hLibList, SendMessage(_h.hLibList, LB_GETCURSEL, 0, 0));
	ofstream file(GetLibDir() + L'\\' + LibName + Exten, ios::binary);

	if (file.is_open())
	{
		int dell = SendMessage(_h.hBookList, LB_GETCURSEL, 0, 0);

		bk::Book books;

		wstring buff;
		wstringstream wss;
		for (int i = 0; i < SendMessage(_h.hBookList, LB_GETCOUNT, 0, 0); ++i)
		{
			if (i == dell) continue;

			ListItemToWSTR(buff, _h.hBookList, i);
			wss.clear();
			wss << buff;

			getline(wss, buff, L'\t');
			wcscpy_s(books.name, bk::SZ_FILENAME, buff.c_str());

			getline(wss, buff, L'\t');
			for (int i = 0; i < bk::szStatuses; ++i)
			{
				if (buff == bk::Statuses[i])
				{
					books.status = i;
					break;
				}
			}

			getline(wss, buff, L'\t');
			for (int i = 0; i < bk::szGenres; ++i)
			{
				if (buff == bk::Genres[i])
				{
					books.genre = i;
					break;
				}
			}

			auto iter = OpenShekf.begin();
			for (int y = 0; y < i; ++y) 
				++iter;
			wcscpy_s(books.path, DIRSTR_LEN, iter->c_str());

			file.write((char*)&books, sizeof(bk::Book));
		}

		file.close();

		SendMessage(_h.hBookList, LB_DELETESTRING, dell, 0);
		EnableMenuItem(hMenu, ID_BOOK_DELETE, MF_BYCOMMAND | MF_DISABLED);
	}

	else
	{
		MessageBox(
			_h.hDialog,
			L"Delete book error",
			L"Error",
			MB_OK | MB_ICONERROR
		);
	}
}

void MainWin::SelChangeBook()
{
	int index = SendMessage(_h.hBookList, LB_GETCURSEL, 0, 0);

	if (index != -1)
	{
		size_t data = SendMessage(_h.hBookList, LB_GETITEMDATA, index, 0);

		auto y = OpenShekf.begin();
		for (int i = 0; i < data; ++i) ++y;

		if (ShellExecute(NULL, L"open", y->c_str(), NULL, NULL, SW_SHOW) <= (HINSTANCE)32)
		{
			MessageBox(
				_h.hDialog,
				L"Open book error",
				L"Error",
				MB_OK | MB_ICONERROR
			);
		}
	}
}
