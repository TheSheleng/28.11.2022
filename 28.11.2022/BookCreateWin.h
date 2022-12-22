#pragma once
#include "Header.h"
#include "Book.h"

class BookCreateWin
{
	BookCreateWin() = default;
	BookCreateWin(const BookCreateWin&) = delete;
	BookCreateWin& operator=(const BookCreateWin&) = delete;

	struct Handls
	{
		HWND hDialog;

		//Поля для ввода
		HWND hName, hPath, hStatus, hGenre;

		//Записать
		HWND hBPath, hEnter;
	} _h;

	BOOL OnInit(HWND, HWND, LPARAM);
	VOID OnCommand(HWND, int, HWND, UINT);
	VOID OnClose(HWND);

	static const int DialogID = IDD_DIALOG_BOOK_CREATE;
	static BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

	bk::Book book;

public:
	static BookCreateWin& RUN();
	BOOL WinStartup(HWND);
	const bk::Book& GetBook() const;
	const Handls& GetHWNDS() const;
};