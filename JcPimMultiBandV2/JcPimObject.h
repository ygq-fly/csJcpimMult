#ifndef _SAN_JCPIMPARAM_H_
#define _SAN_JCPIMPARAM_H_

#include "ClsVnaAgE5062A.hpp"
#include "ClsSigAgN5181A.hpp"
#include "ClsAnaAgN9020A.hpp"
#include "ClsSenAgU2000A.hpp"

#include "RohdeSchwarz/ClsSenRsNrpz.hpp"
#include "RohdeSchwarz/ClsSenRsNrt.hpp"
#include "RohdeSchwarz/ClsAnaRsFspSerial.h"
#include "RohdeSchwarz/ClsSigRsSMxSerial.h"

#include "Tektronix\ClsSigTekTSG4000.h"

#include "JcOffsetDB.h"
#include "Switch/ClsJcSwitch.h"

#include "stdafx.h"

#define SMOOTH_VCO_THREASOLD 2
#define SMOOTH_TX_THREASOLD 2
#define SMOOTH_TX_ACCURACY 0.15

#define SUM_LOW	       0
#define SUM_HIGH	   1
#define SUM_LESS       0
#define SUM_ADD        1

//static int _switch_enable[7] = { 1, 1, 1, 1, 1, 1, 1 };
static int _debug_enable = 0;
static int _tx_delay = 400;

//dll加载初始化地址
static std::wstring _startPath = [](){
	wchar_t wcBuff[512] = { 0 };
	Util::getMyPath(wcBuff, 256, L"JcPimMultiBandV2.dll");
	//char cbuff[512] = { 0 };
	//Util::getMyPath(cbuff, 256, "JcPimMultiBandV2.dll");
	std::wstring wsPath_ini = std::wstring(wcBuff) + L"\\JcConfig.ini";
	_debug_enable = GetPrivateProfileIntW(L"Settings", L"tx_debug", 0, wsPath_ini.c_str());
	_tx_delay = GetPrivateProfileIntW(L"Settings", L"tx_delay", 400, wsPath_ini.c_str());
	//防止tx_delay小于200
	_tx_delay = _tx_delay < 200 ? 200 : _tx_delay;

	return std::wstring(wcBuff);
}();

struct JcBandModule {
	std::string band_name;
	//当前band检测通道1, (标号1开始,使用时需-1)
	int switch_coup1;
	//当前band检测通道2, (标号1开始,使用时需-1)
	int switch_coup2;
	//band频率范围
	double tx1_start;
	double tx1_stop;
	double tx2_start;
	double tx2_stop;
	double rx_start;
	double rx_stop;
	bool tx1_enable;
	bool tx2_enable;
	bool rx_enable;
	//vco
	double vco_a;
	double vco_b;
	//功率范围
	double power_min;
	double power_max;
};

//rf发射模块参数类
struct JcRFModule {
	JcRFModule() 
		: band(0)
		, dutport(0)
		, switch_port(0)
		, switch_coup(0)
		, pow_dbm(-60)
		, freq_khz(637)
		, offset_ext(0)
		, offset_int(0)
		, dd(0)
	{}
	//rf当前频段(物理模块位置), 标号从0开始
	uint8_t band;
	//rf当前频段端口, 标号从0开始
	uint8_t dutport;
	//rf当前输出通道, 标号从0开始
	uint8_t switch_port;
	//rf当前检测通道, 标号从0开始
	//预留
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
} *rf1, *rf2;

//pim接收模块参数类
struct JcPimModule {
	JcPimModule()
		: band(0)
		, dutport(0)
		, switch_port(0)
		, freq_khz(0)
		, im_low(SUM_LOW)
		, im_less(SUM_LESS)
		, im_order(3)
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
	uint8_t im_low;
	//pim计算公式(f1 *M + f2 *N 和 f1 *M- f2 *N 的区别)
	uint8_t im_less;
	//pim阶数
	uint8_t im_order;
	//pim接收平均次数
	uint8_t imAvg;
} *pim;

//?????
struct JcPimObject
{
#define LINEFEED_CHAR 0x0D
#define TIMEOUT_VALUE 10000
public:
	//vco??????????
	int now_vco_enable[10];

	//vco门限
	double now_vco_threasold;
	//校准功率门限
	double now_tx_smooth_threasold;
	//校准功率精度
	double now_tx_smooth_accuracy;
	//external band (only support ATE)
	bool isUseExtBand;
    //mode:
	//0-huwei 1-trans 2-POI
	//isUseTransType 
	uint8_t now_mode;
	//bande_set
	//仪表物理模块频段信息，按仪表物理模块安装循序
	//开关切换后rf和pim工作模块将调用该信息
	std::vector<JcBandModule> now_mode_bandset;
	//band_number
	int now_num_band;
	int now_num_port;
	//???????
	std::string strErrorInfo;
	std::wstring wstrLogPath;
	std::wstring wstrLogFlag;

public:
	//连接地址
	//0-SIG1,1-SIG2,2-ANA,3-SEN,4-SWH
	bool device_status[5];
	bool isAllConn;
	//vi地址集合
	std::vector<std::string> vaddr;
	//vi资源管理参数
	ViSession viDefaultRM;
	//pim_module device
	std::shared_ptr<IfAnalyzer> ana;
	//rf detect channel
	std::shared_ptr<IfSensor> sen;
	//rf1_module device
	std::shared_ptr<IfSignalSource> sig1;
	//rf2_module device
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
		, now_mode(MODE_HUAWEI)//isUseTransType(false)
		, now_num_band(7)
		, isUseExtBand(true)
		, strErrorInfo("Not")
		, viDefaultRM(VI_NULL)
		, isExtSenConn(false)
		, ext_sen_index(0)
		, wstrLogFlag(L"MBP")
		, offset()
	{
		//INIT
		rf1 = new JcRFModule;
		rf2 = new JcRFModule;
		pim = new JcPimModule;

		//INIT status
		for (int i = 0; i < 5; ++i) {
			device_status[i] = false;
		}
		for (int i = 0; i < 10; ++i){
			now_vco_enable[i] = 1;
		}

		//SET INI PATH
		std::wstring wsPath_ini = _startPath + L"\\JcConfig.ini";

		//INIT VCO_ENABLE
		for (int i = 0; i < 7; i++){
			wchar_t key[10] = { 0 };
			swprintf_s(key, L"vco_band%d", i);
			now_vco_enable[i] = GetPrivateProfileIntW(L"VCO_Enable", key, 1, wsPath_ini.c_str());
		}

		//GET PATH
		wchar_t temp[1024] = { 0 };
		GetPrivateProfileStringW(L"PATH", L"logging_file_path", L"", temp, 1024, wsPath_ini.c_str());
		wstrLogPath = std::wstring(temp);

		//GET SETTINGS
		int iTransType = GetPrivateProfileIntW(L"Settings", L"type_trans", 0, wsPath_ini.c_str());
		double vco_limit = Util::getIniDouble(L"Settings", L"vco_limit", SMOOTH_VCO_THREASOLD, wsPath_ini.c_str());
		double tx_smooth = Util::getIniDouble(L"Settings", L"tx_smooth", SMOOTH_TX_THREASOLD, wsPath_ini.c_str());
		double tx_accuracy = Util::getIniDouble(L"Settings", L"tx_accuracy", SMOOTH_TX_ACCURACY, wsPath_ini.c_str());

		//SET SETTINGS
		now_mode = (iTransType < 0 || iTransType > 2) ? MODE_HUAWEI : iTransType;
		now_vco_threasold = vco_limit <= 0 ? SMOOTH_VCO_THREASOLD : vco_limit;
		now_tx_smooth_threasold = tx_smooth <= 0 ? SMOOTH_TX_THREASOLD : tx_smooth;
		now_tx_smooth_accuracy = tx_accuracy <= 0 ? SMOOTH_TX_ACCURACY : tx_accuracy;

		isAllConn = false;
	}

	~JcPimObject() {}

public:
	//连接数据库，初始化参数
	bool InitBandSet(){
		std::string sPath = Util::wstring_to_utf8(_startPath + L"\\JcOffset.db");
		bool b = offset.DbConnect(sPath.c_str());
		if (b == false) {
			Util::logged(L"fnSetInit: file error(JcOffset.db)");
			return false;
		}
		//数据库初始化
		offset.DbInit(now_mode);

		int ret = 0;
		if (now_mode == MODE_POI)
			now_num_band = offset.GetBandCount("poi");
		else if (now_mode == MODE_HUAWEI)
			now_num_band = offset.GetBandCount("hw");
		
		for (int i = 0; i < now_num_band; i++)
		{
			std::string band_row;
			char prefix[64] = { 0 };
			char band_info[1024] = { 0 };
			if (now_mode == MODE_POI)
			{
				sprintf_s(prefix, "poi%d", i + 1);
				ret = offset.GetBandInfo(prefix, band_info);
				band_row = std::string(band_info);
			}
			else
			{
				sprintf_s(prefix, "hw%d", i + 1);
				ret = offset.GetBandInfo(prefix, band_info);
				band_row = std::string(band_info);
			}
			Util::strTrim(band_row);
			band_row.erase(std::remove(band_row.begin(), band_row.end(), '\''), band_row.end());
			std::vector<std::string> band_items = Util::split(band_row, ',');
			if (ret < 0 || band_items.size() < 11) {
				Util::logged("fnInitBandSet: band's info error!");
				return false;
			}

			JcBandModule bm;
			bm.band_name = band_items[1];

			bm.tx1_start = atof(band_items[2].c_str());
			bm.tx1_stop = atof(band_items[3].c_str());
			bm.tx2_start = bm.tx1_start;
			bm.tx2_stop = bm.tx1_stop;
			bm.rx_start = atof(band_items[4].c_str());
			bm.rx_stop = atof(band_items[5].c_str());

			bm.vco_a = atof(band_items[6].c_str());
			bm.vco_b = atof(band_items[7].c_str());

			int channel_enable = std::stoi(band_items[8].c_str(), 0, 16);
			bm.tx1_enable = (channel_enable & 0x100) >> 8;
			bm.tx2_enable = (channel_enable & 0x010) >> 4;
			bm.rx_enable = channel_enable & 0x001;
			bm.switch_coup1 = atoi(band_items[9].c_str());
			bm.switch_coup2 = atoi(band_items[10].c_str());

			now_mode_bandset.push_back(bm);
		}
		return true;
	}
	//vi_attribute
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

	//???????khz
	double TransKhz(double val, char* cUnits) {
		std::string sUnits(cUnits);
		std::transform(sUnits.begin(), sUnits.end(), sUnits.begin(), ::tolower);

		if (sUnits == "hz")
			return val / 1000;
		else if (sUnits == "mhz")
			return val * 1000;
		else if (sUnits == "ghz")
			return  val * 1000 * 1000;
		else
			return val;
	}

	//???????
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

	//??????????(ture: support ATE)
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
		if (now_mode == MODE_POI)
			return byExtMeasBand;

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
		default: return 0 /*"LTE700"*/;
		}

		return sband;
	}

	//???band????(15/6/5???)
	std::string GetBandString(const uint8_t& MeasBand) {
		std::string sband;
		//switch (MeasBand)
		//{
		//case 0: sband = "LTE700"; break;
		//case 1: sband = "DD800"; break;
		//case 2: sband = "EGSM900"; break;
		//case 3: sband = "DCS1800"; break;
		//case 4: sband = "PCS1900"; break;
		//case 5: sband = "WCDMA2100"; break;
		//case 6: sband = "LTE2600"; break;
		//default:return "LTE700";
		//}
		return now_mode_bandset[MeasBand].band_name;
	}

	//各种互调公式计算(15/6/5新加)
	double GetPimFreq() {
		double dFreq = 0;
		//????F1,F2
		double dF1, dF2;
		if (pim->im_low == SUM_LOW) {
			dF1 = rf1->freq_khz;
			dF2 = rf2->freq_khz;
		}
		else {
			dF1 = rf2->freq_khz;
			dF2 = rf1->freq_khz;
		}
		//DD800例外
		if (GetBandString(pim->band) == "DD800") {
			dF1 = rf2->freq_khz;
			dF2 = rf1->freq_khz;
		}
		//例外:2F1/2F2
		if (pim->im_order == 0)
			return 2 * dF1;
		//设置阶数
		int ord1, ord2;
		//正数阶
		if ((pim->im_order % 2) == 0)
		{
			ord1 = pim->im_order / 2;
			ord2 = pim->im_order / 2;
		}
		else//奇数阶
		{
			ord1 = pim->im_order / 2 + 1;
			ord2 = pim->im_order / 2;
		}
		//设置算法
		if (pim->im_less == SUM_LESS)
			dFreq = dF1 * ord1 - dF2 * ord2;
		else
			dFreq = dF1 * ord1 + dF2 * ord2;

		//Util::logged("f1=%lf,f2=%lf,order=%d, low=%d, less=%d, ord1=%d, ord2=%d",
		//	dF1 / 1000, dF2 / 1000,
		//	(int)pim->im_order, (int)pim->im_low, (int)pim->im_less,
		//	ord1, ord2);
		return abs(dFreq);
	}

	//???
	void LoggingWrite(std::string strLog) {
		std::string strTime;
		Util::getNowTime(strTime);
		strLog = "==>(" + strTime + ")" + strLog;
		std::wstring log_path = wstrLogPath + L"log_" + wstrLogFlag;
		//暂时关闭！
		//Util::logging(log_path.c_str(), strLog.c_str());
	}

	void WriteClDebug(std::string strLog, bool isCls = false) {
		if (_debug_enable == 0)
			return;
		if (isCls)
			Util::showcldebug("");
		Util::showcldebug(strLog.c_str());
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