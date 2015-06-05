#ifndef _SAN_JCPIMPARAM_H_
#define _SAN_JCPIMPARAM_H_

#include "ClsVnaAgE5062A.hpp"
#include "ClsSigAgN5181A.hpp"
#include "ClsAnaAgN9020A.hpp"
#include "ClsSenAgU2000A.hpp"
#include "ClsJcSwitch.h"

#include "ClsSenRsNrpz.hpp"
#include "ClsSenRsNrt.hpp"

#include "RohdeSchwarz/ClsAnaRsFspSerial.h"
#include "RohdeSchwarz/ClsSigRsSMxSerial.h"

#include "JcOffsetDB.hpp"

#include "stdafx.h"

#define SMOOTH_VCO_THREASOLD 2
#define SMOOTH_TX_THREASOLD 2
#define SMOOTH_TX_ACCURACY 0.15

//static int _switch_enable[7] = { 1, 1, 1, 1, 1, 1, 1 };
static int _debug_enable = 0;

static std::wstring _startPath = [](){
	wchar_t wcBuff[512] = { 0 };
	Util::getMyPath(wcBuff, 256, L"JcPimMultiBandV2.dll");
	std::wstring wsPath_ini = std::wstring(wcBuff) + L"\\JcConfig.ini";
	_debug_enable = GetPrivateProfileIntW(L"Settings", L"debug", 0, wsPath_ini.c_str());

	return std::wstring(wcBuff);
}();

struct JcRFModule {
	JcRFModule() 
		: band(0)
		, dutport(0)
		, switch_port(0)
		, pow_dbm(-60)
		, freq_khz(200)
		, offset_ext(0)
		, offset_int(0)
		, dd(0)
	{}
	uint8_t band;
	uint8_t dutport;
	//输出通道
	uint8_t switch_port;
	//检测通道
	uint8_t switch_coup;
	//RF单位KHZ
	double freq_khz;
	//RF单位dBm
	double pow_dbm;
	double offset_ext;
	double offset_int;
	double dd;

	double freq_min;
	double freq_max;
	double power_min;
	double power_max;
} *rf1, *rf2;

struct JcPimModule {
	JcPimModule()
		: band(0)
		, dutport(0)
		, switch_port(0)
		, freq_khz(0)
		, is_high_pim(true)
		, order(3)
		, imAvg(1)
	{}
	uint8_t band;
	uint8_t dutport;
	//接收通道
	uint8_t switch_port;
	//PIM单位KHZ
	double freq_khz;

	bool is_high_pim;
	uint8_t order;
	uint8_t imAvg;
} *pim;

struct JcPimObject
{
#define LINEFEED_CHAR 0x0D
#define TIMEOUT_VALUE 10000
public:
	int now_vco_enable[10];
	double now_vco_threasold;
	double now_tx_smooth_threasold;
	double now_tx_smooth_accuracy;
	//启用外部频段名（针对华为）
	bool isUseExtBand;
    //启用传输模式
	bool isUseTransType;
	std::string strErrorInfo;
	std::wstring wstrLogPath;
	std::wstring wstrLogFlag;

public:
	bool isAllConn;
	//0-SIG1,1-SIG2,2-ANA,3-SEN,4-SWH
	bool device_status[5];
	ViSession viDefaultRM;
	std::shared_ptr<IfAnalyzer> ana;
	std::shared_ptr<IfSensor> sen;
	std::shared_ptr<IfSignalSource> sig1;
	std::shared_ptr<IfSignalSource> sig2;
	std::shared_ptr<IfVna> vna;
	std::shared_ptr<ClsJcSwitch> swh;
	std::vector<std::string> vaddr;

	JcOffsetDB offset;
	//外部传感器(预留)
	bool isExtSenConn;
	int ext_sen_index;
	std::shared_ptr<IfSensor> ext_sen;

private:
	JcPimObject()
		: isAllConn(false)
		, now_vco_threasold(SMOOTH_VCO_THREASOLD)
		, now_tx_smooth_threasold(SMOOTH_TX_THREASOLD)
		, now_tx_smooth_accuracy(SMOOTH_TX_ACCURACY)
		, isUseTransType(false)
		, isUseExtBand(true)
		, strErrorInfo("Not")
		, viDefaultRM(VI_NULL)
		, isExtSenConn(false)
		, ext_sen_index(0)
		, wstrLogFlag(L"MBP")
	{
		rf1 = new JcRFModule;
		rf2 = new JcRFModule;
		pim = new JcPimModule;

		for (int i = 0; i < 5; ++i) {
			device_status[i] = false;
		}

		for (int i = 0; i < 10; ++i){
			now_vco_enable[i] = 1;
		}

		std::wstring wsPath_ini = _startPath + L"\\JcConfig.ini";
		//VCO_ENABLE
		for (int i = 0; i < 7; i++){
			wchar_t key[10] = { 0 };
			swprintf_s(key, L"vco_band%d", i);
			now_vco_enable[i] = GetPrivateProfileIntW(L"VCO_Enable", key, 1, wsPath_ini.c_str());
		}
		//PATH
		wchar_t temp[1024] = { 0 };
		GetPrivateProfileStringW(L"PATH", L"logging_file_path", L"", temp, 1024, wsPath_ini.c_str());
		wstrLogPath = std::wstring(temp);
		//SETTINGS
		double vco_limit = Util::getIniDouble(L"Settings", L"vco_limit", SMOOTH_VCO_THREASOLD, wsPath_ini.c_str());
		double tx_smooth = Util::getIniDouble(L"Settings", L"tx_smooth", SMOOTH_TX_THREASOLD, wsPath_ini.c_str());
		double tx_accuracy = Util::getIniDouble(L"Settings", L"tx_accuracy", SMOOTH_TX_ACCURACY, wsPath_ini.c_str());
		int iUseTransType = GetPrivateProfileIntW(L"Settings", L"type_trans", 0, wsPath_ini.c_str());
		now_vco_threasold = vco_limit <= 0 ? SMOOTH_VCO_THREASOLD : vco_limit;
		now_tx_smooth_threasold = tx_smooth <= 0 ? SMOOTH_TX_THREASOLD : tx_smooth;
		now_tx_smooth_accuracy = tx_accuracy <= 0 ? SMOOTH_TX_ACCURACY : tx_accuracy;
		isUseTransType = iUseTransType & 1;
	}

	~JcPimObject() {}

public:
	void JcSetViAttribute(ViSession vi){
		char cInfo[32] = { 0 };
		int s = viGetAttribute(vi, 0xBFFF0001UL, &cInfo);
		//超时时间:0x3FFF001AUL
		s = viSetAttribute(vi, 0x3FFF001AUL, TIMEOUT_VALUE);
		//write by san
		if (0 == strcmp(cInfo, "INSTR")) {
			//获取设备连接类型：0x3FFF0171UL
			//memset(cInfo, 0, sizeof(cInfo));
			//s = viGetAttribute(vi, 0x3FFF0171UL, &cInfo);
			//1-gpib;2-vxi;3-gpib_vxi;4-asrl(串口);5-pxi;6-tcpip;7-usb
			//..todo
		}
		else if (0 == strcmp(cInfo, "SOCKET")) {
			//设置TERM_CHAR返回结束码:0x3FFF0018UL，(windows可以设置/r)
			//s = viSetAttribute(vi, 0x3FFF0018UL, LINEFEED_CHAR);

			//设置TERM_CHAR(必须要设置):0x3FFF0038UL
			s = viSetAttribute(vi, 0x3FFF0038UL, VI_TRUE);
		}
	}

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
		int ord = pim->order / 2;
		if (pim->is_high_pim)
			dFreq = rf1->freq_khz * (ord + 1) - rf2->freq_khz * ord;
		else
			dFreq = rf2->freq_khz * (ord + 1) - rf1->freq_khz * ord;
		return dFreq;
	}

	void LoggingWrite(std::string strLog) {
		std::string strTime;
		Util::getNowTime(strTime);
		strLog = "==>(" + strTime + ")" + strLog;
		std::wstring log_path = wstrLogPath + L"log_" + wstrLogFlag;
		//暂时关闭！
		//Util::logging(log_path.c_str(), strLog.c_str());
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

			delete rf1;
			delete rf2;
			delete pim;

			delete _singleton;
			_singleton = NULL;
		}
	}
};

JcPimObject* JcPimObject::_singleton = NULL;

#endif