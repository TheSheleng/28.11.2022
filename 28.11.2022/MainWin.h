#pragma once
#include "Header.h"

#include "Book.h"
#include "InputString.h"
#include "BookCreateWin.h"
#include <fstream>
#include <list>
#include <experimental/filesystem>

namespace fs = experimental::filesystem;

class MainWin
{
	MainWin() = default;
	MainWin(const MainWin&) = delete;
	MainWin& operator=(const MainWin) = delete;

	const wstring DirName = L"Libs";
	const wstring Exten = L".shelf";
	static const wstring GetLibDir();
	list<wstring> OpenShekf;

	HMENU hMenu;

	struct Handls
	{
		HWND hDialog,
			hLibList, hBookList,
			hEdSearchLib,
			hStBooks;
	} _h;

	//Функции событий ("MainWin.cpp")
	BOOL OnInit(HWND, HWND, LPARAM);
	VOID OnCommand(HWND, int, HWND, UINT);
	VOID OnClose(HWND);

	//Функции списка библ. ("LibFuncs.cpp")
	void RefindLib(wstring);
	void CreateLib();
	void DeleteLib();
	void SearchLib();
	void RefreshLib();
	void SelChangeLib();

	//Функции списка книг ("BookFuncs.cpp")
	void CreateBook();
	void DeleteBook();
	void SelChangeBook();

	//Прочие функции ("MainWin.cpp")
	void CreateDir();
	void ListItemToWSTR(wstring&, HWND, int);

	static const int DialogID = IDD_DIALOG_MAIN;
	static BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

public:
	static MainWin& RUN();
	BOOL WinStartup(HINSTANCE);
	const Handls& GetHWNDS() const;
};
