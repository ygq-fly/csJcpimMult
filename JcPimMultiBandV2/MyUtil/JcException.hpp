//
// Created by san on 15/3/30.
//“Ï≥£¿‡
//

#ifndef _SAN_JCEXCEPTION_H
#define _SAN_JCEXCEPTION_H

#include "..\stdafx.h"

class JcException {
public:
	JcException(uint32_t error_code, const char* error_msg)
	{
		m_error_code = error_code;
		m_error_msg = error_msg;
	}

	virtual ~JcException() {}

	uint32_t GetErrorCode() { return m_error_code; }
	char* GetErrorMsg() { return (char*)m_error_msg.c_str(); }
private:

	uint32_t	m_error_code;
	std::string		m_error_msg;
};

#endif //_SAN_JCEXCEPTION_H


