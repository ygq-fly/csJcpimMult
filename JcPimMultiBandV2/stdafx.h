// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef _SAN_STDAFX_H_
#define _SAN_STDAFX_H_

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
#include <exception>
#include <bitset>
#include <fstream>
#include <list>
#include <map>
#include <regex>
#include <time.h>
#include <io.h>

#include <Winsock2.h>
#include <mswsock.h>

#include "3rdParty/visa.h"
#include "3rdParty/rsnrpz.h"

#ifdef _MSC_VER
#include <codecvt>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#pragma comment(lib, "Version.lib")
#pragma comment(lib, "dbghelp.lib")
#pragma warning(disable:4996)
#endif

#ifdef __GNUC__
#include <pthread.h>
#endif

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define OUT
#define g_swprintf swprintf_s
#define g_sprintf  sprintf_s
#define g_snprintf snprintf_s

#define DISALLOW_EVIL_CONSTRUCTORS(TypeName)    \
TypeName(const TypeName&);                      \
void operator=(const TypeName&)

// TODO: reference additional headers your program requires here
#endif