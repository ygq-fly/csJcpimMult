//
//工具类
// Created by san on 15/3/30.
//
//各种转换，各种系统API（BY SAN）
//

#ifndef _SAN_JCCOMMONAPI_H
#define _SAN_JCCOMMONAPI_H

#include "../stdafx.h"

#define Jclogging(fmt, ...) Util::logging("<%s>|<%d>|<%s>," fmt "\r\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

namespace Util {
    int getMyPath(OUT wchar_t* w_path, uint16_t max, const wchar_t* module_name);
	int getMyPath(OUT char* path, uint16_t max, const char* module_name);
	int isFileExist(const char* path);
    int isFileExist(const wchar_t* w_path);
    int getFileSize(const wchar_t* w_path);
    int setSleep(uint32_t millisecond);
	std::string ToString(double val);
    uint64_t get_tick_count();
	double getIniDouble(const wchar_t* AppName, const wchar_t* KeyName, double DefaultVal, const wchar_t* FilePath);
	std::string getIniRow(const char* AppName, const char* Key, const char* DefaultValue, const char* FileName);
	void getNowTime(OUT std::string& strTime);
	void logged(const wchar_t* fmt, ...);
	void logged(const char* fmt, ...);
	void logging(const char* fmt, ...);
	int replaceInString(std::string& str, const std::string& str_from, const std::string& str_to);
	int replaceInArrary(char* str, int count, const char* from, const char* to);
	void strTrim(std::string& str);
	bool strFind(const std::string& str, const char* str_find);
	std::wstring utf8_to_wstring(const std::string& str);
	std::string wstring_to_utf8(const std::wstring& str);
	std::vector<std::string> split(const std::string str, const char sc);
	void showcldebug(const char* fmt, ...);
	int decode64(const char* input, unsigned char* output);
	int timecmp(const char* time1, const char time2);
	int encode64(const unsigned char* input, char* output, int input_length);
}

#endif //_SAN_JCCOMMONAPI_H
