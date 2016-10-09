//
// Created by san on 15/3/30.
//

#include "JcCommonAPI.h"

//64M
#define MAX_LOG_FILE_SIZE 0x4000000
const char* charlist = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

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

int Util::getMyPath(char* path, uint16_t max, const char* module_name) {
	int ret = -1;
	if (NULL != path) {
		HMODULE hm = NULL;
		hm = GetModuleHandleA(module_name);
		//GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR)&SAN::getMyPath, &hm);
		char cPath[1024] = { 0 };
		::GetModuleFileNameA(hm, cPath, 1024);
		std::string sPath(cPath);
		if (std::string::npos != sPath.find(L'\\')) {
			sPath = sPath.substr(0, sPath.rfind(L'\\'));
			ret = sPath.size();
			ret = ret <= max ? ret : max;
			memcpy(path, sPath.c_str(), ret);
		}
	}
	return ret;
}

int Util::isFileExist(const wchar_t *w_path) {
    //00: exist, 02: read,  04: write, 06: read and write
    return  _waccess(w_path, 0);
}

int Util::isFileExist(const char* path) {
	return  _access(path, 0);
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
std::string Util::getIniRow(const char* AppName, const char* Key, const char* DefaultValue, const char* FileName) {
	char strBuff[512] = { 0 };
	GetPrivateProfileStringA(AppName, Key, DefaultValue, strBuff, 512, FileName);
	return std::string(strBuff);
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
	char cInfo[2048] = { 0 };
	va_list ap;
	va_start(ap, fmt);
    vsprintf_s(cInfo, fmt, ap);
	va_end(ap);
	MessageBoxA(GetForegroundWindow(), cInfo, "Tips", MB_TOPMOST);
}

void Util::logging(const char *fmt, ...) {
    static int file_no = 1;
    static FILE* pFile = NULL;
    if(NULL == pFile) {
		char log_name[64] = { 0 };
		sprintf_s(log_name, "LOG\\log_%d.txt", file_no);
		errno_t err = fopen_s(&pFile, log_name, "a");
        if (err != 0)
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

int Util::replaceInString(std::string& str, const std::string& str_from, const std::string& str_to) {
	size_t pos = 0;
	while ((pos = str.find(str_from, pos)) != std::string::npos) {
		if (str_to != "")
			str.replace(pos, str_from.length(), str_to);
		else
			str.erase(pos, pos + str_from.length());
		pos += str_to.length();
	}
	return 0;
}

int Util::replaceInArrary(char* str, int count, const char* from, const char* to) {
	std::vector<std::string> vecTemp;
	int len = 0;
	while (len <= count) {
		std::string temp(str+len);
		len += (temp.length() + 1);
		replaceInString(temp, from, to);
		vecTemp.push_back(temp);
	}
	memset(str, 0x00, count);
	len = 0;
	for (int i = 0; i < vecTemp.size(); i++) {
		memcpy(str + len, vecTemp[i].c_str(), vecTemp[i].length());
		int n = vecTemp[i].length();
		len += (vecTemp[i].length() + 1);
	}
	return len-1;
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

std::vector<std::string> Util::split(const std::string str, const char sc) {
	std::istringstream iss(str);
	std::vector<std::string> ret;
	std::string stemp = "";
	while (std::getline(iss, stemp, sc)) {
		ret.push_back(stemp);
	}
	return ret;
}

void Util::showcldebug(const char* fmt, ...) {
	static HANDLE hcl = NULL;
	if (hcl == NULL) {
		AllocConsole();
		hcl = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	if (std::string(fmt)== "") {
		COORD coordScreen = { 0, 0 };    /* here's where we'll home the
										 cursor */
		BOOL bSuccess;
		DWORD cCharsWritten;
		CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
		DWORD dwConSize;                 /* number of character cells in
										 the current buffer */
		bSuccess = GetConsoleScreenBufferInfo(hcl, &csbi);
		dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
		bSuccess = FillConsoleOutputCharacter(hcl, (TCHAR) ' ',
			dwConSize, coordScreen, &cCharsWritten);
		bSuccess = GetConsoleScreenBufferInfo(hcl, &csbi);
		bSuccess = FillConsoleOutputAttribute(hcl, csbi.wAttributes,
			dwConSize, coordScreen, &cCharsWritten);
		bSuccess = SetConsoleCursorPosition(hcl, coordScreen);
	}
	else {
		char cInfo[1024] = { 0 };
		va_list ap;
		va_start(ap, fmt);
		vsprintf_s(cInfo, fmt, ap);
		va_end(ap);

		DWORD dwWritten = 0;
		WriteConsoleA(hcl, cInfo, strlen(cInfo), &dwWritten, NULL);
		//WriteConsoleA(hcl, "\n", strlen("\n"), &dwWritten, NULL);
	}

	//FreeConsole();
}

int Util::decode64(const char* input, unsigned char* output)
{
	int i, j;
	unsigned char k;
	unsigned char temp[4];
	for (i = 0, j = 0; input[i] != '\0'; i += 4)
	{
		memset(temp, 0xFF, sizeof(temp));
		for (k = 0; k < 64; k++)
		{
			if (charlist[k] == input[i])
				temp[0] = k;
		}
		for (k = 0; k < 64; k++)
		{
			if (charlist[k] == input[i + 1])
				temp[1] = k;
		}
		for (k = 0; k < 64; k++)
		{
			if (charlist[k] == input[i + 2])
				temp[2] = k;
		}
		for (k = 0; k < 64; k++)
		{
			if (charlist[k] == input[i + 3])
				temp[3] = k;
		}

		output[j++] = ((unsigned char)(((unsigned char)(temp[0] << 2)) & 0xFC)) |
			((unsigned char)((unsigned char)(temp[1] >> 4) & 0x03));
		if (input[i + 2] == '=')
			break;

		output[j++] = ((unsigned char)(((unsigned char)(temp[1] << 4)) & 0xF0)) |
			((unsigned char)((unsigned char)(temp[2] >> 2) & 0x0F));
		if (input[i + 3] == '=')
			break;

		output[j++] = ((unsigned char)(((unsigned char)(temp[2] << 6)) & 0xF0)) |
			((unsigned char)(temp[3] & 0x3F));
	}
	return j;
}

int Util::timecmp(const char* cTime1, const char cTime2) {
	return 0;
}

int Util::encode64(const unsigned char* input, char* output, int input_length){
	int i, j;
	unsigned char current;

	for (i = 0, j = 0; i < input_length; i += 3)
	{
		current = (input[i] >> 2);
		current &= (unsigned char)0x3F;
		output[j++] = charlist[(int)current];

		current = ((unsigned char)(input[i] << 4)) & ((unsigned char)0x30);
		if (i + 1 >= input_length)
		{
			output[j++] = charlist[(int)current];
			output[j++] = '=';
			output[j++] = '=';
			break;
		}
		current |= ((unsigned char)(input[i + 1] >> 4)) & ((unsigned char)0x0F);
		output[j++] = charlist[(int)current];

		current = ((unsigned char)(input[i + 1] << 2)) & ((unsigned char)0x3C);
		if (i + 2 >= input_length)
		{
			output[j++] = charlist[(int)current];
			output[j++] = '=';
			break;
		}
		current |= ((unsigned char)(input[i + 2] >> 6)) & ((unsigned char)0x03);
		output[j++] = charlist[(int)current];

		current = ((unsigned char)input[i + 2]) & ((unsigned char)0x3F);
		output[j++] = charlist[(int)current];
	}
	output[j] = '\0';
	return j;

}
