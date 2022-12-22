#pragma once

#include "Header.h"
#include <string>

using namespace std;

namespace bk
{
	const wstring const Statuses[] =
	{
		L"Will read",
		L"Read",
		L"Been read",
	};
	const size_t szStatuses = sizeof(Statuses) / sizeof(wstring);

	const wstring const Genres[] =
	{
		L"Fantasy",
		L"Fiction",
		L"Romance",
		L"Scientific",
		L"Horrer",
		L"Comedy",
		L"Historical",
	};
	const size_t szGenres = sizeof(Genres) / sizeof(wstring);

	const size_t SZ_FILENAME = 16;
	struct Book
	{
		wchar_t name[SZ_FILENAME + 1];
		unsigned status;
		unsigned genre;
		wchar_t path[DIRSTR_LEN + 1];
	};
}
