#ifndef _CLS_CLSTCPNET_HPP_
#define _CLS_CLSTCPNET_HPP_

#include "stdafx.h"
#include "MyUtil/JcCommonAPI.h"
#include "MyUtil/TcpClient.h"

class ClsTcpNet
{
public:
	ClsTcpNet()
		:_viStatus(VI_NULL),
		_viDefaultRM(VI_NULL),
		_viSession(VI_NULL),
		_isConn(false),
		_esr(0),
		_strIDN("")
	{}

	~ClsTcpNet() {}

public:
	//��ʼ����
	bool AgConnect(const char* c_addr) {
		if (_isConn) return true;

		return _isConn;
	}

	//��ʼ����
	void AgSession(ViSession viConnectedSession, std::string Idn) {

		_isConn = true;
	}

	//д������
	bool AgWrite(const char* c_cmd) {

		return !_viStatus;
	}

	//д�벢�ȴ���ȡ�����ض�ȡ�ֽڳ��ȣ�
	long AgWriteAndRead(const char* c_cmd, char* rbuf) {

		return 0;
	}

	//��������״��
	bool AgConnStatus() const {
		return _isConn;
	}

	//���ش�����Ϣ
	ViStatus AgError() const {
		return _viSession;
	}

	void AgClose() {
		_isConn = false;
	}

	//�ȴ�
	bool AgWait() {

		return true;
	}

	void PrintError(long err)
	{
		if (err < VI_SUCCESS)
		{
			unsigned long retCount = 0;
			unsigned char error_message[256] = { 0 };

			_viStatus = viPrintf(_viSession, const_cast<char*>("SYST:ERR?\n"));
			_viStatus = viRead(_viSession, error_message, 256, &retCount);
			//viQueryf(_viSession, "")
			printf("Error: %s\n", error_message);
		}
	}

protected:
	bool _isConn;
	ViStatus _viStatus;
	ViSession _viDefaultRM;
	ViSession _viSession;
	ViUInt16 _esr;
	std::string _strIDN;
};

#endif