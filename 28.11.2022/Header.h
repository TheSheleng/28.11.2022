#pragma once

#include <Windows.h>
#include <WindowsX.h>
#include <string>
#include "resource.h"

using namespace std;

void HWNDtoWSTR(wstring*, HWND);

#define DIRSTR_LEN 256