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

#include "JcOffsetDB.h"

#include "stdafx.h"

#define SMOOTH_VCO_THREASOLD 2
#define SMOOTH_TX_THREASOLD 2
#define SMOOTH_TX_ACCURACY 0.15

//static int _switch_enable[7] = { 1, 1, 1, 1, 1, 1, 1 };
static int _debug_enable = 0;

//dll???????????
static std::wstring _startPath = [](){
	wchar_t wcBuff[512] = { 0 };
	Util::getMyPath(wcBuff, 256, L"JcPimMultiBandV2.dll");
	//char cbuff[512] = { 0 };
	//Util::getMyPath(cbuff, 256, "JcPimMultiBandV2.dll");
	std::wstring wsPath_ini = std::wstring(wcBuff) + L"\\JcConfig.ini";
	_debug_enable = GetPrivateProfileIntW(L"Settings", L"debug", 0, wsPath_ini.c_str());

	return std::wstring(wcBuff);
}();

struct JcBandModule {
	std::string band_name;
	//?????????1, (????1???,??????-1)
	int switch_coup1;
	//?????????2, (????1???,??????-1)
	int switch_coup2;
	//??????
	double tx1_start;
	double tx1_stop;
	double tx2_start;
	double tx2_stop;
	double rx_start;
	double rx_stop;
	bool tx1_enable;
	bool tx2_enable;
	//???????
	double power_min;
	double power_max;
};

//rf????????????(15/6/5???)
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
	//rf??????(?????????), ????0???
	uint8_t band;
	//rf?????????, ????0???
	uint8_t dutport;
	//rf?????????, ????0???
	uint8_t switch_port;
	//rf?????????, ????0???
	//??????
	uint8_t switch_coup;
	//rf?????????KHZ
	double freq_khz;
	//rf??????????dBm
	double pow_dbm;
	//?????????
	double offset_ext;
	//??????????
	double offset_int;
	//other
	double dd;
} *rf1, *rf2;

//pim????????????(15/6/5???)
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
	//pim??????(???????????)
	uint8_t band;
	//pim?????????
	uint8_t dutport;
	//pim???????
	uint8_t switch_port;
	//pim??????????KHZ
	double freq_khz;
	//pim???????(f1 *M - f2 *N ?? f2 *M- f1 *N ??????)
	bool is_high_pim;
	//pim?????(f1 *M + f2 *N ?? f1 *M- f2 *N ??????)
	uint8_t is_less_pim;
	//pim????
	uint8_t order;
	//pim???????????
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

	//vco???????
	double now_vco_threasold;
	//???????????
	double now_tx_smooth_threasold;
	//????????
	double now_tx_smooth_accuracy;
	//???????????(???ATE)
	bool isUseExtBand;
    //?????
	//0-??????????? 1-?????????? 2-POI??
	//isUseTransType 
	uint8_t now_mode;
	//????????????????
	//???????????????
	std::vector<JcBandModule> now_mode_bandset;
	//??????????
	int now_num_band;
	int now_num_port;
	//???????
	std::string strErrorInfo;
	std::wstring wstrLogPath;
	std::wstring wstrLogFlag;

public:
	//?????????????
	//0-SIG1,1-SIG2,2-ANA,3-SEN,4-SWH
	bool device_status[5];
	bool isAllConn;
	//??????
	std::vector<std::string> vaddr;
	//vi??????????
	ViSession viDefaultRM;
	//pim??????????
	std::shared_ptr<IfAnalyzer> ana;
	//?????????????
	std::shared_ptr<IfSensor> sen;
	//rf1??????????
	std::shared_ptr<IfSignalSource> sig1;
	//rf2??????????
	std::shared_ptr<IfSignalSource> sig2;
	//...(test)
	std::shared_ptr<IfVna> vna;
	//???????
	std::shared_ptr<ClsJcSwitch> swh;
	//?????
	JcOffsetDB offset;

	//????????(???)
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
		, isUseExtBand(true)
		, strErrorInfo("Not")
		, viDefaultRM(VI_NULL)
		, isExtSenConn(false)
		, ext_sen_index(0)
		, wstrLogFlag(L"MBP")
		, offset()
	{
		//?????(15/6/5???)
		rf1 = new JcRFModule;
		rf2 = new JcRFModule;
		pim = new JcPimModule;

		//???????
		for (int i = 0; i < 5; ++i) {
			device_status[i] = false;
		}
		for (int i = 0; i < 10; ++i){
			now_vco_enable[i] = 1;
		}

		//??????????????
		std::wstring wsPath_ini = _startPath + L"\\JcConfig.ini";

		//???VCO_ENABLE
		for (int i = 0; i < 7; i++){
			wchar_t key[10] = { 0 };
			swprintf_s(key, L"vco_band%d", i);
			now_vco_enable[i] = GetPrivateProfileIntW(L"VCO_Enable", key, 1, wsPath_ini.c_str());
		}

		//???PATH
		wchar_t temp[1024] = { 0 };
		GetPrivateProfileStringW(L"PATH", L"logging_file_path", L"", temp, 1024, wsPath_ini.c_str());
		wstrLogPath = std::wstring(temp);

		//???SETTINGS
		int iTransType = GetPrivateProfileIntW(L"Settings", L"type_trans", 0, wsPath_ini.c_str());
		double vco_limit = Util::getIniDouble(L"Settings", L"vco_limit", SMOOTH_VCO_THREASOLD, wsPath_ini.c_str());
		double tx_smooth = Util::getIniDouble(L"Settings", L"tx_smooth", SMOOTH_TX_THREASOLD, wsPath_ini.c_str());
		double tx_accuracy = Util::getIniDouble(L"Settings", L"tx_accuracy", SMOOTH_TX_ACCURACY, wsPath_ini.c_str());

		//????SETTINGS
		now_mode = (iTransType < 0 || iTransType > 2) ? MODE_HUAWEI : iTransType;
		now_vco_threasold = vco_limit <= 0 ? SMOOTH_VCO_THREASOLD : vco_limit;
		now_tx_smooth_threasold = tx_smooth <= 0 ? SMOOTH_TX_THREASOLD : tx_smooth;
		now_tx_smooth_accuracy = tx_accuracy <= 0 ? SMOOTH_TX_ACCURACY : tx_accuracy;

		//???POI????????
		std::string hw_band_set[7] = huawei_sql_body;
		std::string poi_band_set[12] = poi_sql_body;

		std::string bandfreq_path = Util::wstring_to_utf8(_startPath + L"\\JcBandFreq.ini");
		now_num_band = now_mode == MODE_POI ? 12 : 7;
		for (int i = 0; i < now_num_band; i++) 
		{
			std::string band_row;
			if (now_mode == MODE_POI) 
			{
				//char key[16] = { 0 };
				//sprintf_s(key, "band%d", i);
				//wchar_t val[1024] = { 0 };
				//band_row = Util::getIniRow("Poi Band Set", key, "", bandfreq_path.c_str());
				band_row = poi_band_set[i];
			}
			else
			{
				band_row = hw_band_set[i];
			}
		
			std::vector<std::string> band_items = Util::split(band_row, ',');
			JcBandModule bm;
			bm.band_name = Util::split(band_items[0], '\'')[1];
			int tx_enable = atoi(band_items[1].c_str());
			bm.tx1_enable = (tx_enable == 1 || tx_enable == 12);
			bm.tx2_enable = (tx_enable >= 2);
			bm.switch_coup1 = atoi(band_items[2].c_str());
			bm.switch_coup2 = atoi(band_items[3].c_str());
			bm.tx1_start = atof(band_items[4].c_str());
			bm.tx1_stop = atof(band_items[5].c_str());
			bm.tx2_start = bm.tx1_start;
			bm.tx2_stop = bm.tx1_stop;
			bm.rx_start = atof(band_items[6].c_str());
			bm.rx_stop = atof(band_items[7].c_str());
			now_mode_bandset.push_back(bm);
		}		
		isAllConn = false;
	}

	~JcPimObject() {}

public:
	//????vi???????
	void JcSetViAttribute(ViSession vi){
		char cInfo[32] = { 0 };
		int s = viGetAttribute(vi, 0xBFFF0001UL, &cInfo);
		//??????:0x3FFF001AUL
		s = viSetAttribute(vi, 0x3FFF001AUL, TIMEOUT_VALUE);
		//write by san
		if (0 == strcmp(cInfo, "INSTR")) {
			//???????????????0x3FFF0171UL
			//memset(cInfo, 0, sizeof(cInfo));
			//s = viGetAttribute(vi, 0x3FFF0171UL, &cInfo);
			//1-gpib;2-vxi;3-gpib_vxi;4-asrl(????);5-pxi;6-tcpip;7-usb
			//..todo
		}
		else if (0 == strcmp(cInfo, "SOCKET")) {
			//????TERM_CHAR?????????:0x3FFF0018UL??(windows????????/r)
			//s = viSetAttribute(vi, 0x3FFF0018UL, LINEFEED_CHAR);

			//????TERM_CHAR(?????????):0x3FFF0038UL
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

	//??????????(???ATE)
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

	//??????????????(15/6/5???)
	double GetPimFreq() {
		double dFreq = 0;
		//????F1,F2
		double dF1 = pim->is_high_pim ? rf1->freq_khz : rf2->freq_khz;
		double dF2 = pim->is_high_pim ? rf2->freq_khz : rf1->freq_khz;
		//???????
		int ord1 = pim->order == 2 ? 1 : (pim->order / 2 + 1);
		int ord2 = pim->order == 2 ? 1 : (pim->order / 2);
		//??????
		if (pim->is_less_pim == 0)
			dFreq = dF1 * ord1 - dF2 * ord2;
		else
			dFreq = dF1 * ord1 + dF2 * ord2;

		return abs(dFreq);
	}

	//???
	void LoggingWrite(std::string strLog) {
		std::string strTime;
		Util::getNowTime(strTime);
		strLog = "==>(" + strTime + ")" + strLog;
		std::wstring log_path = wstrLogPath + L"log_" + wstrLogFlag;
		//???????
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