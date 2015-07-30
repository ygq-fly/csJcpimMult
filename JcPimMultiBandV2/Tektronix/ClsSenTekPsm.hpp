//
//请注意！！！！！！
//只支持32位
//
#ifndef SAN_CLSSENTEKPSM_HPP_
#define SAN_CLSSENTEKPSM_HPP_

#include "../IfSensor.hpp"
#include "../stdafx.h"

typedef long(*p_LB_SensorCnt) (void);
typedef long(*p_LB_GetAddress_Idx) (long idx);
typedef long(*p_LB_InitializeSensor_Idx) (long idx);
typedef long(*p_LB_MeasureCW)(long addr, double* CW);

class ClsSenTekPsm : Implements_ IfSensor
{
#define CHANNEL1 (1)
#define VERROR -10000
#define eNULL 0
public:
	ClsSenTekPsm()
		:m_freq_now(eNULL)
		,m_isConn(false)
		, m_addr(0)
	{}

	~ClsSenTekPsm() {}

public:
	bool InstrConnect(const char* c_addr){
		if (m_isConn) return true;

		m_isConn = false;
		HINSTANCE hinst = LoadLibrary(L"LB_API2.dll");
		if (hinst == NULL) {
			DWORD err = GetLastError();
			char info[128] = { 0 };
			sprintf_s(info, "LB_API2.dll Error: %d", err);
			MessageBoxA(GetForegroundWindow(), info, "Error", MB_TOPMOST);
			return false;
		}

		fn_LB_SensorCnt = (p_LB_SensorCnt)GetProcAddress(hinst, "LB_SensorCnt");
		fn_LB_GetAddress_Idx = (p_LB_GetAddress_Idx)GetProcAddress(hinst, "LB_GetAddress_Idx");
		fn_LB_InitializeSensor_Idx = (p_LB_InitializeSensor_Idx)GetProcAddress(hinst, "LB_InitializeSensor_Idx");
		fn_LB_MeasureCW = (p_LB_MeasureCW)GetProcAddress(hinst, "LB_MeasureCW");

		if (fn_LB_SensorCnt() == 0)
			return false;

		m_addr = fn_LB_GetAddress_Idx(1);
		if (fn_LB_InitializeSensor_Idx(1) > 0)
			m_isConn = true;
		return m_isConn;
	}

	void InstrSession(unsigned long viConnectedSession, const char* cIdn) {
		//
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
		return m_isConn;
	}

	void InstrClose() {
	}

public:
	//读取
	double InstrGetSesnor() {
		double val = -10000;
		int ret = fn_LB_MeasureCW(m_addr, &val);
		return val;
	}

	std::string InstrGetIdn(){
		return "psm";
	}

private:
	void PrintError(long err, long session)
	{

	}


private:
	double m_freq_now;
	bool m_isConn;
	long m_addr;
	p_LB_SensorCnt fn_LB_SensorCnt;
	p_LB_GetAddress_Idx fn_LB_GetAddress_Idx;
	p_LB_InitializeSensor_Idx fn_LB_InitializeSensor_Idx;
	p_LB_MeasureCW  fn_LB_MeasureCW;
};

#endif