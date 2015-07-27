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
	//开始连接
	bool RsConnect(const char* c_addr) {
		if (_isConn) return true;

		_viStatus = rsnrpz_init(const_cast<char*>(c_addr), 1, 1, &_viSession);

		_isConn = !_viStatus;
		return _isConn;
	}

	//开始连接
	void RsSession(ViSession viConnectedSession) {
		_viSession = viConnectedSession;
		_isConn = true;
	}

	//写入命令
	bool RsWrite(const char* c_cmd) {
		return !_viStatus;
	}

	//写入并等待读取（返回读取字节长度）
	long RsWriteAndRead(const char* c_cmd, char* rbuf) {


		return 0;
	}

	//返回连接状况
	bool RsConnStatus() const {
		return _isConn;
	}

	//返回错误信息
	ViStatus RsError() const {
		return _viSession;
	}

	void RsClose(){

	}

	//等待
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