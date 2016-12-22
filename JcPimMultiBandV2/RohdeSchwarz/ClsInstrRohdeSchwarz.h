#ifndef _CLS_ROHDESCHWARZ_HPP_
#define _CLS_ROHDESCHWARZ_HPP_

#include "../stdafx.h"

//ViStatus _VI_FUNC rsnrpz_init(ViRsrc resourceName,
//	ViBoolean IDQuery,
//	ViBoolean resetDevice,
//	ViSession *instrumentHandle);
typedef ViStatus (*p_rsnrpz_init)(ViRsrc, ViBoolean, ViBoolean, ViSession*);

//ViStatus _VI_FUNC rsnrpz_chan_initiate(ViSession instrumentHandle,
//	ViInt32 channel);
typedef ViStatus (*p_rsnrpz_chan_initiate)(ViSession, ViInt32);

//ViStatus _VI_FUNC rsnrpz_chan_isMeasurementComplete(ViSession instrumentHandle,
//	ViInt32 channel,
//	ViBoolean* measurementComplete);
typedef ViStatus(*p_rsnrpz_chan_isMeasurementComplete)(ViSession, ViInt32, ViBoolean*);

//ViStatus _VI_FUNC rsnrpz_meass_fetchMeasurement(ViSession instrumentHandle,
//	ViInt32 channel,
//	ViReal64* measurement);
typedef ViStatus(*p_rsnrpz_meass_fetchMeasurement)(ViSession, ViInt32, ViReal64*);

//ViStatus _VI_FUNC rsnrpz_chan_mode(ViSession instrumentHandle, ViInt32 channel,
//	ViInt32 measurementMode);
typedef ViStatus(*p_rsnrpz_chan_mode)(ViSession, ViInt32, ViInt32);

//ViStatus _VI_FUNC rsnrpz_chan_setInitContinuousEnabled(ViSession instrumentHandle,
//	ViInt32 channel,
//	ViBoolean continuousInitiate);
typedef ViStatus(*p_rsnrpz_chan_setInitContinuousEnabled)(ViSession, ViInt32, ViBoolean);

//ViStatus _VI_FUNC rsnrpz_chan_setContAvAperture(ViSession instrumentHandle,
//	ViInt32 channel,
//	ViReal64 contAvAperture);
typedef ViStatus(*p_rsnrpz_chan_setContAvAperture)(ViSession, ViInt32, ViReal64);

//ViStatus _VI_FUNC rsnrpz_chan_setContAvSmoothingEnabled(ViSession instrumentHandle,
//	ViInt32 channel,
//	ViBoolean contAvSmoothing);
typedef ViStatus(*p_rsnrpz_chan_setContAvSmoothingEnabled)(ViSession, ViInt32, ViBoolean);

//ViStatus _VI_FUNC rsnrpz_avg_configureAvgNSRatio(ViSession instrumentHandle,
//	ViInt32 channel,
//	ViReal64 maximumNoiseRatio,
//	ViReal64 upperTimeLimit);
typedef ViStatus(*p_rsnrpz_avg_configureAvgNSRatio)(ViSession, ViInt32, ViReal64, ViReal64);

//ViStatus _VI_FUNC rsnrpz_avg_configureAvgAuto(ViSession instrumentHandle,
//	ViInt32 channel, ViInt32 resolution);
typedef ViStatus(*p_rsnrpz_avg_configureAvgAuto)(ViSession, ViInt32, ViInt32);

//ViStatus _VI_FUNC rsnrpz_avg_configureAvgManual(ViSession instrumentHandle,
//	ViInt32 channel, ViInt32 count);
typedef ViStatus(*p_rsnrpz_avg_configureAvgManual)(ViSession, ViInt32, ViInt32);

//ViStatus _VI_FUNC rsnrpz_CloseSensor(ViSession instrumentHandle, ViInt32 channel);
typedef ViStatus(*p_rsnrpz_CloseSensor)(ViSession, ViInt32);

//ViStatus _VI_FUNC rsnrpz_close(ViSession instrumentHandle);
typedef ViStatus(*p_rsnrpz_close)(ViSession);

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