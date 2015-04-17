//
// Created by san on 15/3/30.
//

#ifndef _SAN_JCCOMMONAPI_H
#define _SAN_JCCOMMONAPI_H

#include "../stdafx.h"

#define Jclogging(fmt, ...) Util::logging("<%s>|<%d>|<%s>," fmt "\r\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

namespace Util {
    int getMyPath(OUT wchar_t* w_path, uint16_t max, const wchar_t* module_name);
    int getMyParentPath(OUT wchar_t* w_path, uint16_t max);
    int isFileExist(const wchar_t* w_path);
    int getFileSize(const wchar_t* w_path);
    int saveBitmapToFile(HBITMAP hbitmap, const wchar_t* wFileName);
    int setSleep(uint32_t millisecond);
	std::string ToString(double val);
    uint64_t get_tick_count();
	double getIniDouble(const wchar_t* AppName, const wchar_t* KeyName, double DefaultVal, const wchar_t* FilePath);
	void getNowTime(OUT std::string& strTime);
	void logged(const wchar_t* fmt, ...);
    void logging(const char* fmt, ...);
	void strTrim(std::string& str);
	bool strFind(const std::string& str, const char* str_find);
}

#endif //_SAN_JCCOMMONAPI_H
