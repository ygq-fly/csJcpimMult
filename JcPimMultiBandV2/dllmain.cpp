// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "JcApi.h"

#include <dbghelp.h>

long __stdcall callback(_EXCEPTION_POINTERS* ex) {
	HANDLE hFile = ::CreateFile(_T("C:\\OneDrive\\Sync\\JcPimMultiBandV2\\Debug\\dumpfile.dmp"),
		GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		MINIDUMP_EXCEPTION_INFORMATION einfo;
		einfo.ThreadId = ::GetCurrentThreadId();
		einfo.ExceptionPointers = ex;
		einfo.ClientPointers = FALSE;

		::MiniDumpWriteDump(::GetCurrentProcess(),
			::GetCurrentProcessId(),
			hFile, MiniDumpNormal, &einfo, NULL, NULL);
		::CloseHandle(hFile);
	}

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved)
{
	SetUnhandledExceptionFilter(callback);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		HwSetExit();
		break;
	}
	return TRUE;
}