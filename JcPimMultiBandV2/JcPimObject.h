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

//dll加载初始化地址
static std::wstring _startPath = [](){
	wchar_t wcBuff[512] = { 0 };
	Util::getMyPath(wcBuff, 256, L"JcPimMultiBandV2.dll");
	std::wstring wsPath_ini = std::wstring(wcBuff) + L"\\JcConfig.ini";
	_debug_enable = GetPrivateProfileIntW(L"Settings", L"debug", 0, wsPath_ini.c_str());

	return std::wstring(wcBuff);
}();

//rf发射模块参数类(15/6/5新加)
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
	//rf当前频段(物理模块位置)
	uint8_t band;
	//rf当前频段端口
	uint8_t dutport;
	//rf当前输出通道
	uint8_t switch_port;
	//rf当前检测通道
	uint8_t switch_coup;
	//rf输出频率单位KHZ
	double freq_khz;
	//rf输出功率单位dBm
	double pow_dbm;
	//外部校准参数
	double offset_ext;
	//内部校准参数
	double offset_int;
	//other
	double dd;

	double freq_min;
	double freq_max;
	double power_min;
	double power_max;
} *rf1, *rf2;

//pim接收模块参数类(15/6/5新加)
struct JcPimModule {
	JcPimModule()
		: band(0)
		, dutport(0)
		, switch_port(0)
		, freq_khz(0)
		, is_high_pim(true)
		, is_less_pim(0)
		, order(3)
		, imAvg(1)
	{}
	//pim当前频段(物理模块位置)
	uint8_t band;
	//pim当前频段端口
	uint8_t dutport;
	//pim接收通道
	uint8_t switch_port;
	//pim接收频率单位KHZ
	double freq_khz;
	//pim高频互调(f1 *M - f2 *N 和 f2 *M- f1 *N 的区别)
	bool is_high_pim;
	//pim计算公式(f1 *M + f2 *N 和 f1 *M- f2 *N 的区别)
	uint8_t is_less_pim;
	//pim阶数
	uint8_t order;
	//pim接收平均次数
	uint8_t imAvg;
} *pim;

//仪表类
struct JcPimObject
{
#define LINEFEED_CHAR 0x0D
#define TIMEOUT_VALUE 10000
public:
	//vco功能启用标志
	int now_vco_enable[10];

	//vco检测门限
	double now_vco_threasold;
	//功率调整门限
	double now_tx_smooth_threasold;
	//功率精测度
	double now_tx_smooth_accuracy;
	//启用外部频段名(针对ATE)
	bool isUseExtBand;
    //仪表模式
	//0-华为互调模式， 1-支持传输模式， 2-POI模式
	//isUseTransType 
	uint8_t now_mode;
	//错误信息
	std::string strErrorInfo;
	std::wstring wstrLogPath;
	std::wstring wstrLogFlag;

public:
	//各个设备连接状态
	//0-SIG1,1-SIG2,2-ANA,3-SEN,4-SWH
	bool device_status[5];
	bool isAllConn;
	//连接地址
	std::vector<std::string> vaddr;
	//vi资源管理参数
	ViSession viDefaultRM;
	//pim模块使用的设备
	std::shared_ptr<IfAnalyzer> ana;
	//检测模块使用的设备
	std::shared_ptr<IfSensor> sen;
	//rf1模块使用的设备
	std::shared_ptr<IfSignalSource> sig1;
	//rf2模块使用的设备
	std::shared_ptr<IfSignalSource> sig2;
	//...(test)
	std::shared_ptr<IfVna> vna;
	//开关模块
	std::shared_ptr<ClsJcSwitch> swh;
	//数据库
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
		, now_mode(0)//isUseTransType(false)
		, isUseExtBand(true)
		, strErrorInfo("Not")
		, viDefaultRM(VI_NULL)
		, isExtSenConn(false)
		, ext_sen_index(0)
		, wstrLogFlag(L"MBP")
	{
		//初始化(15/6/5新加)
		rf1 = new JcRFModule;
		rf2 = new JcRFModule;
		pim = new JcPimModule;
		//状态初始化
		for (int i = 0; i < 5; ++i) {
			device_status[i] = false;
		}
		for (int i = 0; i < 10; ++i){
			now_vco_enable[i] = 1;
		}
		//设置配置文件地址
		std::wstring wsPath_ini = _startPath + L"\\JcConfig.ini";
		//获取VCO_ENABLE
		for (int i = 0; i < 7; i++){
			wchar_t key[10] = { 0 };
			swprintf_s(key, L"vco_band%d", i);
			now_vco_enable[i] = GetPrivateProfileIntW(L"VCO_Enable", key, 1, wsPath_ini.c_str());
		}
		//获取PATH
		wchar_t temp[1024] = { 0 };
		GetPrivateProfileStringW(L"PATH", L"logging_file_path", L"", temp, 1024, wsPath_ini.c_str());
		wstrLogPath = std::wstring(temp);
		//获取SETTINGS
		int iTransType = GetPrivateProfileIntW(L"Settings", L"type_trans", 0, wsPath_ini.c_str());
		double vco_limit = Util::getIniDouble(L"Settings", L"vco_limit", SMOOTH_VCO_THREASOLD, wsPath_ini.c_str());
		double tx_smooth = Util::getIniDouble(L"Settings", L"tx_smooth", SMOOTH_TX_THREASOLD, wsPath_ini.c_str());
		double tx_accuracy = Util::getIniDouble(L"Settings", L"tx_accuracy", SMOOTH_TX_ACCURACY, wsPath_ini.c_str());
		//设置SETTINGS
		now_mode = (iTransType < 0 || iTransType > 2) ? MODE_HUAWEI : iTransType;
		now_vco_threasold = vco_limit <= 0 ? SMOOTH_VCO_THREASOLD : vco_limit;
		now_tx_smooth_threasold = tx_smooth <= 0 ? SMOOTH_TX_THREASOLD : tx_smooth;
		now_tx_smooth_accuracy = tx_accuracy <= 0 ? SMOOTH_TX_ACCURACY : tx_accuracy;
	}

	~JcPimObject() {}

public:
	//设置vi连接参数
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

	//转换单位khz
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

	//转换单位
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

	//设置外部参数(针对ATE)
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

	//获取band名称
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

	//各种互调公式计算(15/6/5新加)
	double GetPimFreq() {
		double dFreq = 0;
		//设置F1,F2
		double dF1 = pim->is_high_pim ? rf1->freq_khz : rf2->freq_khz;
		double dF2 = pim->is_high_pim ? rf2->freq_khz : rf1->freq_khz;
		//设置阶数
		int ord1 = pim->order == 2 ? 1 : (pim->order / 2 + 1);
		int ord2 = pim->order == 2 ? 1 : (pim->order / 2);
		//设置算法
		if (pim->is_less_pim == 0)
			dFreq = dF1 * ord1 - dF2 * ord2;
		else
			dFreq = dF1 * ord1 + dF2 * ord2;

		return abs(dFreq);
	}

	//日志
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