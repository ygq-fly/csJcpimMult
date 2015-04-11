// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include <stdio.h>
#include <tchar.h>
#include <stdint.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

#include <thread>
#include <memory>
#include <locale>
#include <codecvt>

#include "3rdParty/visa.h"
#include "3rdParty/rsnrpz.h"
#include "3rdParty\/sqlite3.h"

#define OUT

#pragma comment(lib, "Version.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS


// TODO: reference additional headers your program requires here
