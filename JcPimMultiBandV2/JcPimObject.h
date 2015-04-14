#ifndef _SAN_JCPIMPARAM_H_
#define _SAN_JCPIMPARAM_H_

#include "ClsVnaAgE5062A.hpp"
#include "ClsSigAgN5181A.hpp"
#include "ClsAnaAgN9020A.hpp"
#include "ClsSenAgU2000A.hpp"
#include "ClsJcSwitch.h"

#include "ClsSenRsNrpz.hpp"
#include "ClsSenRsNrt.hpp"

#include "JcOffsetDB.hpp"

#include "stdafx.h"

struct JcPimObject
{
public:
	char now_band;
	char now_dut_port;
	char now_order;
	char now_imAvg;

	double now_txPow1;
	double now_txPow2;
	//单位KHZ
	double now_txFreq1;
	//单位KHZ
	double now_txFreq2;

	//外部补偿1
	double offset_txPow1;
	//外部补偿2
	double offset_txPow2;

	//内部校准1
	double offset_internal_txPow1;
	double dd1;
	//内部校准2
	double offset_internal_txPow2;
	double dd2;

	bool isAllConn;
	bool isSwhConn;
	bool isExtSenConn;
	bool now_status[4];
	int now_vco_enbale[10];
	int now_vco_threasold;
	int debug_time;
	bool isUseExtBand;
	std::string strErrorInfo;	

public:
	ViSession viDefaultRM;
	std::shared_ptr<IfAnalyzer> ana;
	std::shared_ptr<IfSensor> sen;
	std::shared_ptr<IfSignalSource> sig1;
	std::shared_ptr<IfSignalSource> sig2;
	std::shared_ptr<IfVna> vna;
	std::shared_ptr<ClsJcSwitch> swh;

	JcOffsetDB offset;
	//外部传感器
	int ext_sen_index;
	std::shared_ptr<IfSensor> ext_sen;

private:
	JcPimObject()
		: now_band(0),
		now_dut_port(0),
		now_order(3),
		now_imAvg(1),
		now_txPow1(-60),
		now_txPow2(-60),
		now_txFreq1(200),
		now_txFreq2(200),
		offset_txPow1(0),
		offset_txPow2(0),
		offset_internal_txPow1(0),
		dd1(0),
		dd2(0),
		offset_internal_txPow2(0),
		isAllConn(false),
		isSwhConn(false),
		isExtSenConn(false),
		now_vco_threasold(10),
		debug_time(200),
		isUseExtBand(true),
		strErrorInfo("Not"),
		viDefaultRM(VI_NULL),
		ext_sen_index(0)
		
	{
		for (int i = 0; i < 4; ++i) {
			now_status[i] = false;
		}

		for (int i = 0; i < 10; ++i){
			now_vco_enbale[i] = 1;
		}	
	}

	~JcPimObject() {}

public:
	double TransKhz(double val, char* cUnits) {
		std::string sUnits(cUnits);
		std::transform(sUnits.begin(), sUnits.end(), sUnits.begin(), ::tolower);

		double _val = val;
		if (sUnits == "hz")
			_val = val / 1000;
		else if (sUnits == "mhz")
			_val = val * 1000;
		else if (sUnits == "ghz")
			_val = val * 1000 * 1000;

		return _val;
	}

	double TransToUnit(double val_khz, char* cUnits) {
		std::string sUnits(cUnits);
		std::transform(sUnits.begin(), sUnits.end(), sUnits.begin(), ::tolower);

		double _val = val_khz;
		if (sUnits == "hz")
			_val = val_khz * 1000;
		else if (sUnits == "mhz")
			_val = val_khz / 1000;
		else if (sUnits == "ghz")
			_val = val_khz / 1000 / 1000;

		return _val;
	}

	//enum JC_EXTERNAL_BAND{
	//	_DD800 = 0,
	//	_GSM900 = 1,
	//	_DCS1800 = 2,
	//	_PCS1900 = 3,
	//	_WCDMA2100 = 4,
	//	_LTE2600 = 5,
	//	_LTE700 = 6
	//};
	uint8_t GetExtBandToIntBand(const uint8_t& byExtMeasBand) {
		uint8_t sband;
		switch (byExtMeasBand)
		{
		
		case 0: sband =1 /*"DD800"*/; break;
		case 1: sband =2 /*"EGSM900"*/; break;
		case 2: sband =3 /*"DCS1800"*/; break;
		case 3: sband =4 /*"PCS1900"*/; break;
		case 4: sband =5 /*"WCDMA2100"*/; break;
		case 5: sband =6 /*"LTE2600"*/; break;
		case 6: sband =0 /*"LTE700"*/; break;
		default: return 0;
		}
		return sband;
	}

	std::string GetBandString(const uint8_t& MeasBand) {
		std::string sband;
		switch (MeasBand)
		{
		case 0: sband = "LTE700"; break;
		case 1: sband = "DD800"; break;
		case 2: sband = "EGSM900"; break;
		case 3: sband = "DCS1800"; break;
		case 4: sband = "PCS1900"; break;
		case 5: sband = "WCDMA2100"; break;
		case 6: sband = "LTE2600"; break;
		default:return "LTE700";
		}
		return sband;
	}

	double GetPimFreq() {
		double dFreq = 0;
		//计算阶数
		if (now_band == 1) {
			if (now_order == 3)
				dFreq = now_txFreq2 * 2 - now_txFreq1 * 1;
			else if (now_order == 5)
				dFreq = now_txFreq2 * 3 - now_txFreq1 * 2;
			else if (now_order == 7)
				dFreq = now_txFreq2 * 4 - now_txFreq1 * 3;
			else if (now_order == 9)
				dFreq = now_txFreq2 * 5 - now_txFreq1 * 4;
			else if (now_order == 11)
				dFreq = now_txFreq2 * 6 - now_txFreq1 * 5;
			else
				dFreq = now_txFreq2 * 2 - now_txFreq1 * 1;
		}
		else {
			if (now_order == 3)
				dFreq = now_txFreq1 * 2 - now_txFreq2 * 1;
			else if (now_order == 5)
				dFreq = now_txFreq1 * 3 - now_txFreq2 * 2;
			else if (now_order == 7)
				dFreq = now_txFreq1 * 4 - now_txFreq2 * 3;
			else if (now_order == 9)
				dFreq = now_txFreq1 * 5 - now_txFreq2 * 4;
			else if (now_order == 11)
				dFreq = now_txFreq1 * 6 - now_txFreq2 * 5;
			else 
				dFreq = now_txFreq1 * 2 - now_txFreq2 * 1;
		}
		return dFreq;
	}

	std::string ToString(double val) {
		auto str = std::to_string(val);
		str.erase(str.find_last_not_of('0') + 1, std::string::npos);
		return str;
	}

	//Sigleton model
private:
	static JcPimObject* _singleton;

public:
	static JcPimObject* Instance(){
		if (NULL == _singleton)
			_singleton = new JcPimObject;

		return _singleton;
	}

	static void release() {
		if (NULL != _singleton) {
			if (_singleton->swh)
				_singleton->swh->SwitchClose();
			if (_singleton->ana)
				_singleton->ana->InstrClose();
			if (_singleton->sen)
				_singleton->sen->InstrClose();
			if (_singleton->sig1)
				_singleton->sig1->InstrClose();
			if (_singleton->sig2)
				_singleton->sig2->InstrClose();
			if (_singleton->vna)
				_singleton->vna->InstrClose();
			viClose(_singleton->viDefaultRM);

			_singleton->ana.reset();
			_singleton->sen.reset();
			_singleton->sig1.reset();
			_singleton->sig2.reset();
			_singleton->vna.reset();
			_singleton->swh.reset();

			delete _singleton;
			_singleton = NULL;
		}
	}
};

JcPimObject* JcPimObject::_singleton = NULL;

#endif