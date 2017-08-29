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
#include "Tektronix\ClsSenTekPsm.hpp"

#include "JcOffsetDB.h"
#include "Switch/ClsJcSwitch.h"

#include "stdafx.h"
#include <WinCrypt.h>

#define SMOOTH_VCO_THREASOLD 2
#define SMOOTH_TX_THREASOLD 2
#define SMOOTH_TX_ACCURACY 0.15

#define OFFSET_JC_PROTECT_RX -10

#define OFFSET_PROTECT_TX -10
#define OFFSET_PROTECT_RX -90
#define OFFSET_PROTECT_RX_RANGE 10
#define OFFSET_STEP_RX 1
#define OFFSET_STEP_TX 1

#define SUM_LOW	       0
#define SUM_HIGH	   1
#define SUM_LESS       0
#define SUM_ADD        1
#define DES_SIZE	   1024	
#define MAX_SIZE_FREQ  1024	

//debug模式
static int _debug_enable = 0;
//保护门限相关
static int _protect_tx = OFFSET_PROTECT_TX;
static int _protect_rx = OFFSET_PROTECT_RX;
static int _protect_range_rx = OFFSET_PROTECT_RX_RANGE;
static int _tx_no_power_limit = -50;
static double _out_of_val_range = -149;
//延时相关
static bool _need_reset = false;
static int _tx_delay = 10;
static int _tx_delay2 = 0;
static int _coup_delay = 300;
static int _reset_delay = 500;
static int _sensor_delay = 500;
static int _tx_offset_delay = 100;
static int _vco_delay = 100;
//设置相关
static int _free_tx_enable = 1;
static int _tx_step = 1;
static int _tx_adjust_count = 1;
static int _tx1_sensor_count = 4;
static int _tx2_sensor_count = 8;

//false通用（无关是否有耦合器开关），true针对华为无耦合器开关
//static bool _tx_coup_enable = true;
bool _tx_fast_mode = false;

static int _rx_step = 1;
static int _pim_avg = 1;
//sig的rosc
static int _sig_rosc = 0;
//spe读取模式
static int _spe_is_max = 0;
//spe测试预设预放
static int _spe_preamp = 1;
//spe测试预设att
static int _spe_pim_att = -1;
//sep校准预设att
static int _spe_offset_att = -1;

static std::string _serial;
const char DESkeys[] = "jointcom";
const char DESiv[8] = { 0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF };

//dll加载初始化地址
static std::wstring _startPath = [](){
	wchar_t wcBuff[512] = { 0 };
	Util::getMyPath(wcBuff, 256, L"JcPimMultiBandV2.dll");
	//char cbuff[512] = { 0 };
	//Util::getMyPath(cbuff, 256, "JcPimMultiBandV2.dll");

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
	//微调
	double fine_adjust1;
	double fine_adjust2;
	//enable
	bool vco_enable;
	bool coup_enable;
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
		, im_coefficients1(2)
		, im_coefficients2(1)
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
	//pim互调系数
	uint8_t im_coefficients1;
	uint8_t im_coefficients2;
} *pim;

//?????
class JcPimObject : public Singleton<JcPimObject>
{
#define LINEFEED_CHAR 0x0D
#define TIMEOUT_VALUE 10000
public:
	//vco??????????
	int now_vco_enable[15];
	int now_coup_enable[15];

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
	std::vector<std::shared_ptr<JcBandModule>> now_mode_bandset;
	//band_number
	std::string now_band_prefix;
	int now_num_band;
	int now_num_port;
	//???????
	std::string strErrorInfo;
	std::wstring wstrLogPath;
	std::wstring wstrLogFlag;

	double now_dsp_tx1;
	double now_dsp_tx2;

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
	//调整标识
	bool isAdjust;

	//外部传感器(预留)
	bool isExtSenConn;
	int ext_sen_index;
	std::shared_ptr<IfSensor> ext_sen;

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
		, isAdjust(true)
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

		//SET INI PATH
		std::wstring wsPath_ini = _startPath + L"\\JcConfig.ini";

		//INIT VCO_ENABLE
		int len = sizeof(now_vco_enable) / sizeof(int);
		for (int i = 0; i < len; i++){
			wchar_t key[32] = { 0 };
			swprintf_s(key, L"vco_band%d", i);
			now_vco_enable[i] = GetPrivateProfileIntW(L"VCO_Enable", key, 1, wsPath_ini.c_str());
		}
		//INIT COUP_ENABLE
		//for (int i = 0; i < len; i++){
		//	wchar_t key[32] = { 0 };
		//	swprintf_s(key, L"coup_band%d", i);
		//	now_coup_enable[i] = GetPrivateProfileIntW(L"COUP_Enable", key, 1, wsPath_ini.c_str());
		//}

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
		now_mode = (iTransType < 0 || iTransType > 4) ? MODE_HUAWEI : iTransType;
		now_vco_threasold = vco_limit <= 0 ? SMOOTH_VCO_THREASOLD : vco_limit;
		now_tx_smooth_threasold = tx_smooth <= 0 ? SMOOTH_TX_THREASOLD : tx_smooth;
		now_tx_smooth_accuracy = tx_accuracy <= 0 ? SMOOTH_TX_ACCURACY : tx_accuracy;

		isAllConn = false;
	}

	~JcPimObject() {
			if (swh) swh->SwitchClose();
			if (ana) ana->InstrClose();
			if (sen) sen->InstrClose();
			if (vna) vna->InstrClose();

			if (sig1) {
				if (sig1->InstrConnStatus())
					sig1->InstrOpenPow(false);
				sig1->InstrClose();
			}
			if (sig2) {
				if (sig2->InstrConnStatus())
					sig2->InstrOpenPow(false);
				sig2->InstrClose();
			}

			viClose(viDefaultRM);

			ana.reset();
			sen.reset();
			sig1.reset();
			sig2.reset();
			vna.reset();
			swh.reset();

			delete rf1;
			delete rf2;
			delete pim;
	}

public:
	void InitConfig() {
		std::wstring wsPath_ini = _startPath + L"\\JcConfig.ini";
		//读取配置
		_debug_enable = GetPrivateProfileIntW(L"Settings", L"tx_debug", 0, wsPath_ini.c_str());
		_free_tx_enable = GetPrivateProfileIntW(L"Settings", L"tx_limit", 1, wsPath_ini.c_str());

		_protect_tx = GetPrivateProfileIntW(L"Settings", L"tx_protect_value", OFFSET_PROTECT_TX, wsPath_ini.c_str());
		_protect_rx = GetPrivateProfileIntW(L"Settings", L"rx_protect_value", OFFSET_PROTECT_RX, wsPath_ini.c_str());
		_protect_range_rx = GetPrivateProfileIntW(L"Settings", L"rx_protect_range", OFFSET_PROTECT_RX_RANGE, wsPath_ini.c_str());
		_tx_step = GetPrivateProfileIntW(L"Settings", L"tx_step", OFFSET_STEP_TX, wsPath_ini.c_str());
		_rx_step = GetPrivateProfileIntW(L"Settings", L"rx_step", OFFSET_STEP_RX, wsPath_ini.c_str());

		_tx_no_power_limit = GetPrivateProfileIntW(L"Settings", L"tx_no_power_limit", _tx_no_power_limit, wsPath_ini.c_str());
		_out_of_val_range = Util::getIniDouble(L"Settings", L"out_of_val_range", _out_of_val_range, wsPath_ini.c_str());

		_tx_delay = GetPrivateProfileIntW(L"Settings", L"tx_delay", _tx_delay, wsPath_ini.c_str());
		_tx_delay2 = GetPrivateProfileIntW(L"Settings", L"tx_delay2", _tx_delay2, wsPath_ini.c_str());
		_coup_delay = GetPrivateProfileIntW(L"Settings", L"coup_delay", _coup_delay, wsPath_ini.c_str());
		_sensor_delay = GetPrivateProfileIntW(L"Settings", L"sensor_delay", _sensor_delay, wsPath_ini.c_str());
		_reset_delay = GetPrivateProfileIntW(L"Settings", L"reset_delay", _reset_delay, wsPath_ini.c_str());
		_tx_offset_delay = GetPrivateProfileIntW(L"Settings", L"tx_offset_delay", _tx_offset_delay, wsPath_ini.c_str());
		_vco_delay = GetPrivateProfileIntW(L"Settings", L"vco_delay", _vco_delay, wsPath_ini.c_str());

		_pim_avg = GetPrivateProfileIntW(L"Settings", L"pim_avg", 1, wsPath_ini.c_str());
		_sig_rosc = GetPrivateProfileIntW(L"Settings", L"sig_rosc", 1, wsPath_ini.c_str());
		_spe_is_max = GetPrivateProfileIntW(L"Settings", L"spe_is_max", 0, wsPath_ini.c_str());
		_spe_preamp = GetPrivateProfileIntW(L"Settings", L"spe_preamp", 1, wsPath_ini.c_str());

		_spe_pim_att = GetPrivateProfileIntW(L"Settings", L"spe_pim_att", _spe_pim_att, wsPath_ini.c_str());
		_spe_offset_att = GetPrivateProfileIntW(L"Settings", L"spe_offset_att", _spe_offset_att, wsPath_ini.c_str());

		_tx_no_power_limit = _tx_no_power_limit > 0 ? 0 : _tx_no_power_limit;

		//_tx_coup_enable = GetPrivateProfileIntW(L"Settings", L"tx_coup_enable", 1, wsPath_ini.c_str());
		_tx_adjust_count = GetPrivateProfileIntW(L"Settings", L"tx_adjust_count", _tx_adjust_count, wsPath_ini.c_str());
		_tx1_sensor_count = GetPrivateProfileIntW(L"Settings", L"tx1_sensor_count", _tx1_sensor_count, wsPath_ini.c_str());
		_tx2_sensor_count = GetPrivateProfileIntW(L"Settings", L"tx2_sensor_count", _tx2_sensor_count, wsPath_ini.c_str());
		_tx_fast_mode = GetPrivateProfileIntW(L"Settings", L"tx_fast_mode", 0, wsPath_ini.c_str());

		//最低数值保护
		if (_protect_tx > OFFSET_PROTECT_TX) _protect_tx = OFFSET_PROTECT_TX;
		if (_protect_range_rx < 10) _protect_range_rx = 10;
		if (_protect_rx > OFFSET_PROTECT_RX) _protect_rx = OFFSET_PROTECT_RX;
		//_protect_tx = _protect_tx > OFFSET_PROTECT_TX ? OFFSET_PROTECT_TX : _protect_tx;
		//_protect_range_rx = _protect_range_rx < 10 ? 10 : _protect_range_rx;
		//_protect_rx = _protect_rx > 0 ? OFFSET_PROTECT_RX : _protect_rx;

		if (_tx_delay < 0) _tx_delay = 10;
		if (_tx_delay2 < 0) _tx_delay2 = 0;
		if (_coup_delay < 300) _coup_delay = 300;
		if (_sensor_delay < 500) _sensor_delay = 500;
		if (_reset_delay < 500) _reset_delay = 500;
		if (_tx_offset_delay < 100) _tx_offset_delay = 100;
		if (_vco_delay < 100) _vco_delay = 100;
		//_coup_delay = _coup_delay < 300 ? 300 : _coup_delay;
		//_sensor_delay = _sensor_delay < 500 ? 500 : _sensor_delay;
		//_reset_delay = _reset_delay < 500 ? 500 : _reset_delay;
		//_tx_offset_delay = _tx_offset_delay < 100 ? 100 : _tx_offset_delay;
		//_vco_delay = _vco_delay < 100 ? 100 : _vco_delay;

		_pim_avg = _pim_avg < 1 ? 1 : _pim_avg;
		_spe_is_max = _spe_is_max == 0 ? 0 : 1;
		_spe_preamp = _spe_preamp == 0 ? 0 : 1;

		if (_tx_adjust_count < 1) _tx_adjust_count = 1;
		if (_tx1_sensor_count < 1) _tx1_sensor_count = 1;
		if (_tx2_sensor_count < 1) _tx2_sensor_count = 1;

		wchar_t wcSerial[1024] = { 0 };
		GetPrivateProfileStringW(L"SN", L"sn", L" ", wcSerial, 1024, wsPath_ini.c_str());
		_serial = Util::wstring_to_utf8(std::wstring(wcSerial));
	}

	//连接数据库，初始化参数
	bool InitBandSet(){
		std::wstring ini_Path = _startPath + L"\\JcConfig.ini";
		std::wstring db_Path = _startPath + L"\\JcOffset.db";
		std::string sPath = Util::wstring_to_utf8(_startPath + L"\\JcOffset.db");
		
		if (Util::isFileExist(db_Path.c_str()) == -1) {
			Util::logged(L"fnSetInit: file not exist(%s)", db_Path.c_str());
			return false;
		}
		if (!offset.DbConnect(sPath.c_str())) {
			return false;
		}
		//数据库初始化
		offset.SetOffsetStep(_tx_step, _rx_step);
		offset.DbInit(now_mode);

		int ret = 0;
		
		//获取频段数量
		if (now_mode == MODE_POI) {
			now_num_band = offset.GetBandCount(poi_flag);
			now_band_prefix = poi_flag;
		}
		else if (now_mode == MODE_NEWPOI) {
			now_num_band = offset.GetBandCount(NewPoi_flag);
			now_band_prefix = NewPoi_flag;
		}
		else if (now_mode == MODE_NEWHUAWEI) {
			now_num_band = offset.GetBandCount(NewHuawei_flag);
			now_band_prefix = NewHuawei_flag;
		}
		else if (now_mode == MODE_DPX) {
			now_num_band = offset.GetBandCount(Dpx_flag);
			now_band_prefix = Dpx_flag;
		}
		else {
			now_num_band = offset.GetBandCount(huawei_flag);
			now_band_prefix = huawei_flag;
		}
		
		for (int i = 0; i < now_num_band; i++)
		{//获取单独频段信息
			char prefix[64] = { 0 };
			char band_info[1024] = { 0 };
			sprintf_s(prefix, "%s%d", now_band_prefix.c_str(), i + 1);
			ret = offset.GetBandInfo(prefix, band_info);

			std::string band_row = std::string(band_info);
			Util::strTrim(band_row);
			band_row.erase(std::remove(band_row.begin(), band_row.end(), '\''), band_row.end());
			std::vector<std::string> band_items = Util::split(band_row, ',');
			if (ret < 0 || band_items.size() < 11) {
				Util::logged("fnInitBandSet: band's info error!");
				return false;
			}
			//初始化频段信息
			//JcBandModule* bm = new JcBandModule;
			std::shared_ptr<JcBandModule> bm = std::make_shared<JcBandModule>();
			bm->band_name = band_items[1];

			//REAL字段
			bm->tx1_start = atof(band_items[2].c_str());
			bm->tx1_stop = atof(band_items[3].c_str());
			bm->tx2_start = bm->tx1_start;
			bm->tx2_stop = bm->tx1_stop;
			bm->rx_start = atof(band_items[4].c_str());
			bm->rx_stop = atof(band_items[5].c_str());

			bm->vco_a = atof(band_items[6].c_str());
			bm->vco_b = atof(band_items[7].c_str());

			//TEXT字段
			int channel_enable = std::stoi(band_items[8].c_str(), 0, 16);
			bm->tx1_enable = (channel_enable & 0x100) >> 8;
			bm->tx2_enable = (channel_enable & 0x010) >> 4;
			bm->rx_enable = channel_enable & 0x001;

			//INTEGER字段
			bm->switch_coup1 = atoi(band_items[9].c_str()) - 1;
			bm->switch_coup2 = atoi(band_items[10].c_str()) - 1;

			bm->vco_enable = atoi(band_items[11].c_str());
			bm->coup_enable = atoi(band_items[12].c_str());

			//ini配置
			wchar_t keyname[64] = { 0 };
			swprintf_s(keyname, L"band%d_tx1", i);
			bm->fine_adjust1 = Util::getIniDouble(L"Adjust", keyname, 0, ini_Path.c_str());

			memset(keyname, 0x00, sizeof(keyname));
			swprintf_s(keyname, L"band%d_tx2", i);
			bm->fine_adjust2 = Util::getIniDouble(L"Adjust", keyname, 0, ini_Path.c_str());

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
		if (now_mode != MODE_HUAWEI && now_mode != MODE_TRANSMISSION)
			return byExtMeasBand;

		uint8_t sband;
		switch (byExtMeasBand) {		
		case 0: sband =1 /*"DD800"*/; break;
		case 1: sband =2 /*"EGSM900"*/; break;
		case 2: sband =3 /*"DCS1800"*/; break;
		case 3: sband =4 /*"PCS1900"*/; break;
		case 4: sband =5 /*"WCDMA2100"*/; break;
		case 5: sband =6 /*"LTE2600"*/; break;
		case 6: sband =0 /*"LTE700"*/; break;
		case 7: sband =7 /*"Gpp1500"*/; break;
		default: return byExtMeasBand /*"LTE700"*/;
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
		return now_mode_bandset[MeasBand]->band_name;
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
		//if (GetBandString(pim->band) == "DD800") {
		//	dF1 = rf2->freq_khz;
		//	dF2 = rf1->freq_khz;
		//}
		if (now_mode == MODE_HUAWEI || now_mode == MODE_TRANSMISSION) {
			//DD800例外
			//if (pim->band == 1) {
			if (Util::strFind(GetBandString(pim->band), "DD800")) {
				WriteClDebug("GetPimFreq: DD800 => PIM of High");
				dF1 = rf2->freq_khz;
				dF2 = rf1->freq_khz;
			}
		}
		//例外:2F1/2F2
		if (pim->im_order == 0)
			return 2 * dF1;

		//设置算法
		if (pim->im_less == SUM_LESS)
			dFreq = dF1 * pim->im_coefficients1 - dF2 * pim->im_coefficients2;
		else
			dFreq = dF1 * pim->im_coefficients1 + dF2 * pim->im_coefficients2;

		//Util::logged("f1=%lf,f2=%lf,order=%d, low=%d, less=%d, ord1=%d, ord2=%d",
		//	dF1 / 1000, dF2 / 1000,
		//	(int)pim->im_order, (int)pim->im_low, (int)pim->im_less,
		//	ord1, ord2);
		return abs(dFreq);
	}

	bool GetCoupEnable(const uint8_t& MeasBand) {
		JcInt8 byTemp = isUseExtBand ? GetExtBandToIntBand(MeasBand) : MeasBand;
		//return now_coup_enable[byTemp] != 0;
		return now_mode_bandset[byTemp]->coup_enable;
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

	bool CheckAuthorization() {
		bool result = false;
		FILE * pFile = NULL;
		char cAuthorFileName[1024] = { 0 };
		Util::getMyPath(cAuthorFileName, 1024, "JcPimMultiBandV2.dll");
		std::string strAuthorFileName(cAuthorFileName);
		strAuthorFileName += "\\Key\\Key";
		fopen_s(&pFile, strAuthorFileName.c_str(), "rb");
		if (!pFile)
		{
			strErrorInfo = "Authorize File no Find!\n";
			return false;
		}
		char cAuthorValue[DES_SIZE] = { 0 };
		size_t n = fread(cAuthorValue, sizeof *cAuthorValue, sizeof(cAuthorValue), pFile);
		fclose(pFile);
		pFile = NULL;
		n = Util::replaceInArrary(cAuthorValue, n, "\r\n", "");
		cAuthorValue[n] = '\0';
		if (n > 0) 
			GetXor(cAuthorValue, n);
		else {
			strErrorInfo = "Authorize Read Error!\n";
			return false;
		}
		char cAuthorValueFromBase64[DES_SIZE] = { 0 };
		unsigned long length = Util::decode64(cAuthorValue, (unsigned char*)cAuthorValueFromBase64);
		CodeDes(true, cAuthorValueFromBase64, &length, DES_SIZE);
		cAuthorValueFromBase64[length] = '\0';
		std::vector<std::string> items = Util::split(std::string(cAuthorValueFromBase64), '#');
		if (items.size() != 6) {
			strErrorInfo = "Authorize Content Error!\n";
			return false;
		}
		std::string Dates = items[0];
		std::string Datee = items[1];
		std::string Type = items[2];
		std::string Days = items[3];
		std::string Day = items[4];
		std::string Needcheck = items[5];
		if (strcmp(Needcheck.c_str(), "1") == 0) {
			std::transform(_serial.begin(), _serial.end(), _serial.begin(), ::tolower);
			if (strcmp(Type.c_str(), _serial.c_str()) == 0) {
				struct tm tm1 = { 0 };
				if (Util::strFind(Dates, "-"))
					sscanf_s(Dates.c_str(), "%d-%d-%d", &(tm1.tm_year), &(tm1.tm_mon), &(tm1.tm_mday));
				else
					sscanf_s(Dates.c_str(), "%d/%d/%d", &(tm1.tm_year), &(tm1.tm_mon), &(tm1.tm_mday));
				tm1.tm_year -= 1900;
				tm1.tm_mon--;
				tm1.tm_isdst = -1;
				time_t time1 = mktime(&tm1);
				time_t time2 = time(NULL);
				struct tm tm3 = { 0 };
				if (Util::strFind(Datee, "-"))
					sscanf_s(Datee.c_str(), "%d-%d-%d", &(tm3.tm_year), &(tm3.tm_mon), &(tm3.tm_mday));
				else
					sscanf_s(Datee.c_str(), "%d/%d/%d", &(tm3.tm_year), &(tm3.tm_mon), &(tm3.tm_mday));
				tm3.tm_year -= 1900;
				tm3.tm_mon--;
				tm3.tm_isdst = -1;
				time_t time3 = mktime(&tm3);
				if (time1 != -1 && time2 != -1 && time3 != -1) {
					if (time2 >= time1) {
						if (time2 <= time3) {
							int iDay = ceil((time2 - time1) / 86400.0);
							if (atoi(Day.c_str()) <= iDay) {
								std::string strAuthorValueToWrite = Dates + "#" + Datee + "#" + Type + "#" + Days + "#" + 
									std::to_string(iDay) + "#" + Needcheck;
								char cAuthorValueToWrite[DES_SIZE] = { 0 };
								sprintf_s(cAuthorValueToWrite, "%s#%s#%s#%s#%d#%s", 
									Dates.c_str(), Datee.c_str(), Type.c_str(), Days.c_str(), iDay, Needcheck.c_str());
								unsigned long lengthToWrite = strlen(cAuthorValueToWrite);
								CodeDes(false, cAuthorValueToWrite, &lengthToWrite, DES_SIZE);
								char cAuthorValueToWriteToBase64[DES_SIZE] = { 0 };
								int lengthToWriteToBase64 =  Util::encode64((unsigned char*)cAuthorValueToWrite, cAuthorValueToWriteToBase64, lengthToWrite);
								GetXor(cAuthorValueToWriteToBase64, lengthToWriteToBase64);
								if (fopen_s(&pFile, strAuthorFileName.c_str(), "wb") != 0)
									return false;
								fwrite(cAuthorValueToWriteToBase64, sizeof(char), lengthToWriteToBase64, pFile);
								fclose(pFile);
								result = true;
							} else 
								strErrorInfo = "System Time is modified!\n";
						}else
							strErrorInfo = "System Time > Authorize time(" + Datee + ")!\n";
					}else
						strErrorInfo = "System Time < Factory time(" + Dates + ")!\n";
				}else
					strErrorInfo = "Authorize File Error!\n";
			}else
				strErrorInfo = "Authorize Serial Number Error!\n";
		}
		return result;
	}

	void GetXor(char* input, int length) {
		char UnDefine[] = "zjn934";
		for (int i = 0; i < length; i++)
			input[i] ^= UnDefine[i++ % (sizeof(UnDefine) - 1)];
	}

	int CodeDes(bool isDecode, char* input, unsigned long* length, int max) {
#ifdef _MSC_VER
		//为了兼容c#，改用vc代码
		HCRYPTPROV hProv = NULL;
		HCRYPTKEY hSessionKey = NULL;
		DWORD dwDataLen = 0;
		KeyBlob blob;
		blob.header.bType = PLAINTEXTKEYBLOB;
		blob.header.bVersion = CUR_BLOB_VERSION;
		blob.header.reserved = 0;
		blob.header.aiKeyAlg = CALG_DES;
		blob.cbKeySize = 8;
		memcpy(blob.rgbKeyData, DESkeys, 8);
		BYTE iv[9] = { 0 };
		memcpy(iv, DESiv, 8);
		CryptAcquireContext(&hProv, NULL, MS_DEF_PROV, PROV_RSA_FULL, 0);
		CryptImportKey(hProv, (BYTE*)&blob, sizeof(blob), 0, 0, &hSessionKey);
		CryptSetKeyParam(hSessionKey, KP_IV, (BYTE*)iv, 0);
		BOOL ret = FALSE;
		if (isDecode)
			ret = CryptDecrypt(hSessionKey, NULL, TRUE, 0, (BYTE*)input, length);
		else
			ret = CryptEncrypt(hSessionKey, NULL, TRUE, 0, (BYTE*)input, length, max);
		CryptDestroyKey(hSessionKey);
		CryptReleaseContext(hProv, 0);
#else
		int ret = 0;
#endif	
		return ret;
	}

private:
	typedef struct
	{
		BLOBHEADER header;
		DWORD cbKeySize;
		BYTE rgbKeyData[8];
	}KeyBlob;
};

#endif