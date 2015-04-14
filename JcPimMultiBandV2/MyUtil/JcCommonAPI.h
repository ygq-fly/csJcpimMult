//
// Created by san on 15/3/30.
//

#ifndef _SAN_JCCOMMONAPI_H
#define _SAN_JCCOMMONAPI_H

#include "../stdafx.h"

#define log(fmt, ...) Util::logging("<%s>|<%d>|<%s>," fmt "\r\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

namespace Util {
    int getMyPath(OUT wchar_t* w_path, uint16_t max, const wchar_t* module_name);
    int getMyParentPath(OUT wchar_t* w_path, uint16_t max);
    int isFileExist(const wchar_t* w_path);
    int getFileSize(const wchar_t* w_path);
    int saveBitmapToFile(HBITMAP hbitmap, const wchar_t* wFileName);
    int setSleep(uint32_t millisecond);
    uint64_t get_tick_count();
    void logging(const char* fmt, ...);
}

#endif //CLION_ONE_JCCOMMONAPI_H
