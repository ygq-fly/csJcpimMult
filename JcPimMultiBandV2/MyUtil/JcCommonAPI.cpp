//
// Created by san on 15/3/30.
//

#include "JcCommonAPI.h"

//64M
#define MAX_LOG_FILE_SIZE 0x4000000

int Util::getMyPath(wchar_t *w_path, uint16_t max, const wchar_t* module_name) {
    int ret = -1;
    if(NULL != w_path) {
		HMODULE hm = NULL;
		hm = GetModuleHandleW(module_name);
		//GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR)&SAN::getMyPath, &hm);
        wchar_t wPath[1024] = { 0 };
        ::GetModuleFileNameW(hm, wPath, 1024);
        std::wstring wsPath(wPath);
        if(std::string::npos != wsPath.find(L'\\')) {
            wsPath = wsPath.substr(0, wsPath.rfind(L'\\'));
            ret = wsPath.size();
            ret = ret <= max ? ret : max;
            wmemcpy(w_path, wsPath.c_str(), ret);
        }
    }
    return ret;
}

int Util::isFileExist(const wchar_t *w_path) {
    //00: exist, 02: read,  04: write, 06: read and write
    return  _waccess(w_path, 0);
}

int Util::getFileSize(const wchar_t *w_path) {
    struct _stat f_info;
    int s = _wstat(w_path, &f_info);
    return s == -1 ? -1 : f_info.st_size;
}

int Util::setSleep(uint32_t millisecond) {
#ifdef _WIN32
	Sleep(millisecond);
#else
    usleep(millisecond);
#endif
    return 0;
}

std::string Util::ToString(double val) {
	auto str = std::to_string(val);
	str.erase(str.find_last_not_of('0') + 1, std::string::npos);
	return str;
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
	return _wtof(w_value);
}

void Util::getNowTime(OUT std::string& strTime) {
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	char cTime[256];
	sprintf_s(cTime, "%04d-%02d-%02d, %02d:%02d:%02d.%03d", sysTime.wYear, sysTime.wMonth, sysTime.wDay,
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
	strTime.assign(cTime);
}

void Util::logged(const wchar_t* fmt, ...) {
	wchar_t wInfo[256] = { 0 };
	va_list ap;
	va_start(ap, fmt);
	vswprintf_s(wInfo, fmt, ap);
	va_end(ap);
	MessageBoxW(GetForegroundWindow(), wInfo, L"Tips", MB_TOPMOST);
}

void Util::logged(const char* fmt, ...) {
	char cInfo[256] = { 0 };
	va_list ap;
	va_start(ap, fmt);
    vsprintf_s(cInfo, fmt, ap);
	va_end(ap);
	MessageBoxA(GetForegroundWindow(), cInfo, "Tips", MB_TOPMOST);
}

void Util::logging(const wchar_t* log_file, const char *fmt, ...) {
    static int file_no = 1;
    static FILE* pFile = NULL;
    if(NULL == pFile) {
		wchar_t log_name[1024];
        //char log_name[1024];
        //_snprintf_s(log_name, 1024, "%s_%d", log_file, file_no);
		//fopen_s(&pFile, log_name, "a");
		//使用wchar_t
		swprintf_s(log_name, L"%s_%d", log_file, file_no);
#ifdef _MSC_VER
		_wfopen_s(&pFile, log_name, L"a");
#else
        pFile = _wfopen(log_name, L"a");
#endif
		//不需要隐身了
		//if(TRUE != SetFileAttributesW(log_name, FILE_ATTRIBUTE_HIDDEN)) {
		//	//return;
		//}

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

void Util::strTrim(std::string& str) {
	str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
}

bool Util::strFind(const std::string& str, const char* str_find) {
	return (str.find(str_find) != std::string::npos);
}

std::wstring Util::utf8_to_wstring(const std::string& str)
{
#if (_MSC_VER >= 1800)
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.from_bytes(str);
#else
	std::locale old_loc = std::locale::global(std::locale(""));
	const char* src_str = str.c_str();
	const size_t buffer_size = str.size() + 1;
	wchar_t dst_wstr[256] = { 0 };
    #ifdef _MSC_VER
	size_t i;
	mbstowcs_s(&i, dst_wstr, 256, src_str, buffer_size);
    #else
	mbstowcs(dst_wstr, src_str, buffer_size);
    #endif
	std::locale::global(old_loc);

	return std::wstring(dst_wstr);
#endif
}

std::string Util::wstring_to_utf8(const std::wstring& str)
{
#if (_MSC_VER >= 1800)
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.to_bytes(str);
#else
	std::locale old_loc = std::locale::global(std::locale(""));
	const wchar_t* src_wstr = str.c_str();
	size_t buffer_size = str.size() * 4 + 1;
	char dst_str[256] = { 0 };
    #ifdef _MSC_VER
	size_t i;
	wcstombs_s(&i, dst_str, 256, src_wstr, buffer_size);
    #else
	wcstombs(dst_str, src_wstr, buffer_size);
    #endif
	std::locale::global(old_loc);

	return std::string(dst_str);
#endif
}

