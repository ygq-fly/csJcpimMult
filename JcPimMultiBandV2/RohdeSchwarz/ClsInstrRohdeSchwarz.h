#ifndef _CLS_ROHDESCHWARZ_HPP_
#define _CLS_ROHDESCHWARZ_HPP_

#include "../stdafx.h"

class ClsInstrRohdeSchwarz
{
public:
	ClsInstrRohdeSchwarz()
		:_viStatus(VI_NULL),
		_viDefaultRM(VI_NULL),
		_viSession(VI_NULL),
		_isConn(false),
		_esr(0)
	{}

	~ClsInstrRohdeSchwarz() {}

public:
	//��ʼ����
	bool RsConnect(const char* c_addr) {
		if (_isConn) return true;

		_viStatus = rsnrpz_init(const_cast<char*>(c_addr), 1, 1, &_viSession);

		_isConn = !_viStatus;
		return _isConn;
	}

	//��ʼ����
	void RsSession(ViSession viConnectedSession) {
		_viSession = viConnectedSession;
		_isConn = true;
	}

	//д������
	bool RsWrite(const char* c_cmd) {
		return !_viStatus;
	}

	//д�벢�ȴ���ȡ�����ض�ȡ�ֽڳ��ȣ�
	long RsWriteAndRead(const char* c_cmd, char* rbuf) {


		return 0;
	}

	//��������״��
	bool RsConnStatus() const {
		return _isConn;
	}

	//���ش�����Ϣ
	ViStatus RsError() const {
		return _viSession;
	}

	void RsClose(){

	}

	//�ȴ�
	void RsWait() {

	}

protected:
	bool _isConn;
	ViStatus _viStatus;
	ViSession _viDefaultRM;
	ViSession _viSession;
	ViUInt16 _esr;
};

#endif