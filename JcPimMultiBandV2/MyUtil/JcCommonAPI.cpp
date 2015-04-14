//
// Created by san on 15/3/30.
//

#include "JcCommonAPI.h"

#define MAX_LOG_FILE_SIZE 0x4000000

int Util::getMyPath(wchar_t *w_path, uint16_t max, const wchar_t* module_name) {
    if(NULL != w_path) {
		HMODULE hm = NULL;
		hm = GetModuleHandle(module_name);
		//GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR)&SAN::getMyPath, &hm);
        wchar_t wPath[1024] = { 0 };
        ::GetModuleFileNameW(hm, wPath, 1024);
        std::wstring wsPath(wPath);
        if(-1 != wsPath.find(L'\\'))
            wsPath = wsPath.substr(0, wsPath.rfind(L'\\'));
        else {
            //printf("error: %S", wsPath.c_str());
            return -1;
        }

        uint32_t wsSize = wsPath.size();
        if(wsSize <= max) {
            wmemcpy(w_path, wsPath.c_str(), wsSize);
            return wsSize;
        }
        else
            return 0;
    }
    else
        return -1;
}

int Util::getMyParentPath(wchar_t *w_path, uint16_t max) {
    if(NULL != w_path) {
        HMODULE hm = GetModuleHandle(NULL);
        wchar_t wPath[1024] = { 0 };
        ::GetModuleFileNameW(hm, wPath, 1024);
        std::wstring wsPath(wPath);
        wsPath = wsPath.substr(0, wsPath.rfind(L'\\'));
        wsPath = wsPath.substr(0, wsPath.rfind(L'\\'));
        uint32_t wsSize = wsPath.size();
        if(wsSize <= max) {
            wmemcpy(w_path, wsPath.c_str(), wsSize);
            return wsSize;
        }
        else
            return 0;
    }
    else
        return -1;
}

int Util::isFileExist(const wchar_t *w_path) {
    //00: exist, 02: read,  04: write, 06: read and write
    return  _waccess(w_path, 0);
}

int Util::getFileSize(const wchar_t *w_path) {
    struct _stat f_info;
    int s = _wstat(w_path, &f_info);
    if(-1 == s)
        return -1;
    else
        return f_info.st_size;
}

int Util::saveBitmapToFile(HBITMAP hbitmap, const wchar_t *wFileName) {
    // to do!
    return 0;
}

int Util::setSleep(uint32_t millisecond) {
#ifdef _WIN32
    Sleep(millisecond);
#else
    usleep(millisecond);
#endif
    return 0;
}

uint64_t Util::get_tick_count() {
#ifdef _WIN32
    LARGE_INTEGER liCounter;
    LARGE_INTEGER liCurrent;

    if (!QueryPerformanceFrequency(&liCounter))
        return GetTickCount();

    QueryPerformanceCounter(&liCurrent);
    return (uint64_t)(liCurrent.QuadPart * 1000 / liCounter.QuadPart);
#else
	struct timeval tval;
	uint64_t ret_tick;

	gettimeofday(&tval, NULL);

	ret_tick = tval.tv_sec * 1000L + tval.tv_usec / 1000L;
	return ret_tick;
#endif
}

double Util::getIniDouble(const wchar_t* AppName, const wchar_t* KeyName, double DefaultVal, const wchar_t* FilePath) {
	wchar_t w_value[10] = { 0 };
	GetPrivateProfileStringW(AppName, KeyName, std::to_wstring(DefaultVal).c_str(), w_value, 10, FilePath);
	double val = _wtof(w_value);
	return val;
}

void Util::logged(const wchar_t* fmt, ...) {
	wchar_t wInfo[256] = { 0 };
	va_list ap;
	va_start(ap, fmt);
	vswprintf_s(wInfo, fmt, ap);
	va_end(ap);
	MessageBoxW(NULL, wInfo, L"Tips", MB_TOPMOST);
}

void Util::logging(const char *fmt, ...) {
    static int file_no = 1;
    static FILE* pFile = NULL;
    if(NULL == pFile) {
        char log_name[64];
        uint32_t  pid = 637;
#ifdef _WIN32
        _snprintf_s(log_name, 64, ".log_%d_%d", pid, file_no);
		fopen_s(&pFile, log_name, "a");
#else
		snprintf(log_name, 64, ".log_%d_%d", pid, file_no);
		pFile = fopen(log_name, "a");
#endif
        if(TRUE != SetFileAttributesA(log_name, FILE_ATTRIBUTE_HIDDEN)) {
            //return;
        }
        if (NULL == pFile)
            return;
    }

    va_list ap;
    va_start(ap, fmt);
    vfprintf(pFile, fmt, ap);
    va_end(ap);
    fflush(pFile);

    if (ftell(pFile) > MAX_LOG_FILE_SIZE) {
        fclose(pFile);
        pFile = NULL;
        file_no++;
    }
}
