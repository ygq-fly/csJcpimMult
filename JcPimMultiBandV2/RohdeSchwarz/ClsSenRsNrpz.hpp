//
//请注意！！！！！！
//为了兼容华为ATE系统
//需求nrpz32.dll
//版本2,1,5,0
//san@jointcom.com
//
#ifndef _CLS_INSTRRSNRPZ_HPP_
#define _CLS_INSTRRSNRPZ_HPP_

#include "ClsInstrRohdeSchwarz.h"
#include "../IfSensor.hpp"

class ClsSenRsNrpz :public ClsInstrRohdeSchwarz, Implements_ IfSensor
{
#define CHANNEL1 (1)
#define VERROR -10000
#define eNULL 0
public:
	ClsSenRsNrpz()
		:_freq_now(eNULL),
		ClsInstrRohdeSchwarz()
	{}

	~ClsSenRsNrpz() {}

public:
	bool InstrConnect(const char* c_addr){
		if (_isConn) return true;

		//ViInt32 sensorCount;
		//ViChar	SensorName[128],
		//		SensorType[128],
		//		SensorSerial[128];
		//_viStatus = rsnrpz_GetSensorCount(VI_NULL, &sensorCount);
		//_viStatus = rsnrpz_GetSensorInfo(VI_NULL, 1, SensorName, SensorType, SensorSerial);
		//_viStatus = rsnrpz_init(SensorName, VI_TRUE, VI_TRUE, &_viSession);

		//USB USB::vendor Id::product Id::serial number
		_viStatus = rsnrpz_init(const_cast<char*>(c_addr), 0, 0, &_viSession);
		PrintError(_viStatus, _viSession);
		_isConn = !_viStatus;
		if (_isConn) ConfigContAv(_viSession);
		return _isConn;
	}

	void InstrSession(unsigned long viConnectedSession, const char* cIdn) {
		_viSession = viConnectedSession;
		_isConn = true;
	}

	bool InstrWrite(const char* c_cmd) {
		//To do...
		return 0;
	}

	long InstrWriteAndRead(const char* c_cmd, char* rbuf) {
		//To do...
		return 0;
	}

	bool InstrConnStatus() const {
		return _isConn;
	}

	void InstrClose() {
		PrintError(rsnrpz_close(_viSession), _viSession);
	}

public:
	//读取
	double InstrGetSesnor() {
		//开始读取
		return StartMeasurement(_viSession);
	}

	std::string InstrGetIdn(){
		return "nrpz";
	}

private:
	void PrintError(long err, long session)
	{
		//if (err < VI_SUCCESS)
		//{
		//	long ec = 0;
		//	ViChar  error_message[256] = "";
		//	rsnrpz_error_message(session, err, error_message);
		//	printf("Error: %s\n", error_message);

		//	rsnrpz_error_query(session, &ec, error_message);
		//	if (ec != VI_SUCCESS) {
		//		printf("Sensor error: %s\r\n", error_message);
		//	}
		//}

	}

	int IsMeasurementComplete(unsigned long sid)
	{
		unsigned short complete1 = 0;
		_viStatus = rsnrpz_chan_isMeasurementComplete(sid, CHANNEL1, &complete1);
		PrintError(_viStatus, _viSession);
		return complete1;
	}

	//配置
	void ConfigContAv(unsigned long sid)
	{
		unsigned short SmoothingState = 1;
		double ApertureTime = 10e-3; //10ms Aperture Time
		double maximumNoiseRatio = 0.1;// N/S = 0.01dB
		double upperTimeLimit = 0100;//30; //max. measure time = 30s

		//设置mode
		_viStatus = rsnrpz_chan_mode(sid, CHANNEL1, RSNRPZ_SENSOR_MODE_CONTAV);
		PrintError(_viStatus, _viSession);
		//设置被动触发
		_viStatus = rsnrpz_chan_setInitContinuousEnabled(sid, CHANNEL1, 0);
		PrintError(_viStatus, _viSession);
		// Aperture Time:
		_viStatus = rsnrpz_chan_setContAvAperture(sid, CHANNEL1, ApertureTime);
		PrintError(_viStatus, _viSession);
		// Smoothing
		_viStatus = rsnrpz_chan_setContAvSmoothingEnabled(sid, CHANNEL1, SmoothingState);
		PrintError(_viStatus, _viSession);
		// Averaging: AutoAveraging, with Noise to Signal ratio = 0.01 dB
		_viStatus = rsnrpz_avg_configureAvgNSRatio(sid, CHANNEL1, maximumNoiseRatio, upperTimeLimit);
		PrintError(_viStatus, _viSession);
		//自动平均
		//_viStatus = rsnrpz_avg_configureAvgAuto(sid, CHANNEL1, 3);
		//PrintError(_viStatus, _viSession);
		//手动平均
		_viStatus = rsnrpz_avg_configureAvgManual(sid, CHANNEL1, 4);
		PrintError(_viStatus, _viSession);
	}

	//低级方法读取
	double StartMeasurement(unsigned long sid)
	{
		double val = 0.0f;
		long   avgcount = -1;

		//开始触发
		_viStatus = rsnrpz_chan_initiate(sid, CHANNEL1);
		PrintError(_viStatus, _viSession);

		unsigned short complete = 0;
		while (complete == 0)
		{
			//检查状态
			_viStatus = rsnrpz_chan_isMeasurementComplete(sid, CHANNEL1, &complete);
			PrintError(_viStatus, _viSession);
			//Sleep(199);
			Util::setSleep(20);
		}
		//获取值
		_viStatus = rsnrpz_meass_fetchMeasurement(sid, CHANNEL1, &val);
		PrintError(_viStatus, _viSession);
		//返回
		return 30.0 + 10.0 * log10(fabs(val) + 1e-32);
	}

	//高级方法读取(无效)
	double ReadMeasurement(unsigned long sid)
	{
		double result = 0.0f;
		_viStatus = rsnrpz_meass_readMeasurement(sid, CHANNEL1, 5000, &result);
		PrintError(_viStatus, _viSession);
		return result;
	}

private:
	double _freq_now;
};

#endif