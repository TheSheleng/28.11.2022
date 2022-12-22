#include "MainWin.h"

void MainWin::RefindLib(wstring Filter)
{
	CreateDir();
	SendMessage(_h.hLibList, LB_RESETCONTENT, 0, 0);
	SendMessage(_h.hBookList, LB_RESETCONTENT, 0, 0);
	EnableMenuItem(hMenu, ID_LIBRARY_DELETE, MF_BYCOMMAND | MF_DISABLED);
	EnableMenuItem(hMenu, ID_BOOK_CREATE, MF_BYCOMMAND | MF_DISABLED);
	EnableMenuItem(hMenu, ID_BOOK_DELETE, MF_BYCOMMAND | MF_DISABLED);

	//Перебор файлов
	for (auto& f : fs::directory_iterator(GetLibDir()))
	{
		//Провека или файл
		if (!fs::is_regular_file(f.status())) continue;

		fs::path name(f.path().filename());

		//Проверка или то расширения 
		if (name.extension().compare(Exten)) continue;

		//Добавление имени в список если соответствует фильтру
		if (-1 != name.replace_extension().wstring().find(Filter))
			SendMessage(
				_h.hLibList,
				LB_ADDSTRING,
				0,
				(LPARAM)name.replace_extension().c_str()
			);
	}

}

void MainWin::CreateLib()
{
	CreateDir();

	if (InputWin::RUN().WinStartup(_h.hDialog, L"Enter lib name"))
	{
		wstring LibName = InputWin::RUN().GetString();

		if (LibName == L"")
		{
			MessageBox(
				_h.hDialog,
				L"Enter a string.",
				L"Hint",
				MB_OK | MB_ICONINFORMATION
			);
		}
		else if (LB_ERR == SendMessage(
			_h.hLibList,
			LB_FINDSTRINGEXACT,
			-1,
			(LPARAM)LibName.c_str()
		))
		{
			//Создание новой полки
			ofstream(GetLibDir() + L'\\' + LibName + Exten).close();

			//Запись в список
			SendMessage(
				_h.hLibList,
				LB_SETCURSEL,
				SendMessage(_h.hLibList, LB_ADDSTRING, 0, (LPARAM)LibName.c_str()),
				0
			);

			SelChangeLib();
			SetWindowText(_h.hStBooks, LibName.c_str());
		}
		else
		{
			MessageBox(
				_h.hDialog,
				L"A library with the same name already exists",
				L"Hint",
				MB_OK | MB_ICONINFORMATION
			);
		}
	}
}

void MainWin::DeleteLib()
{
	CreateDir();

	int index = SendMessage(_h.hLibList, LB_GETCURSEL, 0, 0);

	if (index != -1)
	{
		EnableMenuItem(hMenu, ID_LIBRARY_DELETE, MF_BYCOMMAND | MF_DISABLED);
		EnableMenuItem(hMenu, ID_BOOK_CREATE, MF_BYCOMMAND | MF_DISABLED);
		EnableMenuItem(hMenu, ID_BOOK_DELETE, MF_BYCOMMAND | MF_DISABLED);

		wstring LibName;
		ListItemToWSTR(LibName, _h.hLibList, index);

		if (0 != DeleteFile(wstring(GetLibDir() + L'\\' + LibName + Exten).c_str()))
			SendMessage(_h.hLibList, LB_DELETESTRING, index, 0);

		SendMessage(_h.hBookList, LB_RESETCONTENT, 0, 0);
		SetWindowText(_h.hStBooks, L"");
	}
}

void MainWin::SearchLib()
{
	CreateDir();

	wstring Search;
	HWNDtoWSTR(&Search, _h.hEdSearchLib);
	RefindLib(Search);
}

void MainWin::RefreshLib()
{
	EnableMenuItem(hMenu, ID_LIBRARY_DELETE, MF_BYCOMMAND | MF_DISABLED);
	SetWindowText(_h.hEdSearchLib, L"");
	RefindLib(L"");
}

void MainWin::SelChangeLib()
{
	CreateDir();

	SendMessage(_h.hBookList, LB_RESETCONTENT, 0, 0);
	int index = SendMessage(_h.hLibList, LB_GETCURSEL, 0, 0);

	if (index != -1)
	{
		//Получение имени
		wstring LibName;
		ListItemToWSTR(LibName, _h.hLibList, index);

		SetWindowText(_h.hStBooks, LibName.c_str());

		//Окрытие полки
		ifstream file(GetLibDir() + L'\\' + LibName + Exten, ios::app, ios::binary);
		
		if (file.is_open())
		{
			if (file.peek() != -1)
			{
				bk::Book book;
				wstring fullstr;

				while (file.read((char*)&book, sizeof(bk::Book)))
				{
					static size_t TAB_LEN = 4;

					fullstr = book.name;
					fullstr += L"\t" + bk::Statuses[book.status];
					fullstr += L"\t" + bk::Genres[book.genre];
					
					OpenShekf.push_back(book.path);

					SendMessage(_h.hBookList, LB_SETITEMDATA, 
						SendMessage(_h.hBookList, LB_ADDSTRING, 
							0, (LPARAM)fullstr.c_str()), 
						(WPARAM)(OpenShekf.size() - 1)
					);
				}
			}

			EnableMenuItem(hMenu, ID_LIBRARY_DELETE, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(hMenu, ID_BOOK_CREATE, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(hMenu, ID_BOOK_DELETE, MF_BYCOMMAND | MF_DISABLED);
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
}