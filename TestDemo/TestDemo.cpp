// TestDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

enum JC_EXTERNAL_BAND{
	_DD800 = 0,
	_GSM900 = 1,
	_DCS1800 = 2,
	_PCS1900 = 3,
	_WCDMA2100 = 4,
	_LTE2600 = 5,
	_LTE700 = 6
};

//内部频段使用
enum JC_INTERNAL_BAND {
	LTE700 = 0,
	DD800 = 1,
	GSM900 = 2,
	DCS1800 = 3,
	PCS1900 = 4,
	WCDMA2100 = 5,
	LTE2600 = 6
};

//内部开关频段
enum JC_SWITCH_BAND {
	LTE700_A = 0,
	LTE700_B = 1,
	DD800_A = 2,
	DD800_B = 3,
	GSM900_A = 4,
	GSM900_B = 5,
	DCS1800_A = 6,
	DCS1800_B = 7,
	PCS1900_A = 8,
	PCS1900_B = 9,
	WCDMA2100_A = 10,
	WCDMA2100_B = 11,
	LTE2600_A = 12,
	LTE2600_B = 13
};


#define JC_CARRIER_TX1TX2 0
#define JC_CARRIER_TX1 1
#define JC_CARRIER_TX2 2

#define JC_DUTPORT_A 0
#define JC_DUTPORT_B 1
#define JC_COUP_TX1 0
#define JC_COUP_TX2 1

enum JC_DEVICE {
	SIGNAL1 = 0,
	SIGNAL2 = 1,
	ANALYZER = 2,
	SENSOR = 3,
	SWITCH = 4
};

#define INSTR_AG_U2000 0
#define INSTR_RS_NRT 1
#define INSTR_AG_N9000 2
#define INSTR_AG_N5180 3

#define INSTR_RS_NRPZ 10

#define JC_OFFSET_REAL 0
#define JC_OFFSET_DSP 1

typedef int(*pTest)(int, int);
//JIONTCOM_API int fnSetInit(ADDRESS_ cDeviceAddr);
typedef int(*pSetInit)(char*);
//JIONTCOM_API int fnSetExit();
typedef int(*pSetExit)();
//JIONTCOM_API int fnSetMeasBand(BYTE_ byBandIndex);
typedef int(*pSetMeasBand)(uint8_t);
//JIONTCOM_API int fnSetImAvg(BYTE_ byAvgTime);
typedef int(*pSetImAvg)(uint8_t);
//JIONTCOM_API int fnSetDutPort(BYTE_ byPort);
typedef int(*pSetDutPort)(uint8_t);
//JIONTCOM_API int fnSetImOrder(BYTE_ byImOrder);
typedef int(*pSetImOrder)(uint8_t);
//JIONTCOM_API int fnCheckReceiveChannel(BYTE_ byBandIndex, BYTE_ byPort);
typedef int(*pCheckReceiveChannel)(uint8_t, uint8_t);
//JIONTCOM_API int fnCheckTwoSignalROSC();
typedef int(*pCheckTwoSignalROSC)();
//JIONTCOM_API int fnSetTxPower(double dTxPower1, double dTxPower2,
//	double dPowerOffset1, double dPowerOffset2);
typedef int(*pSetTxPower)(double, double, double, double);
//JIONTCOM_API int fnSetTxFreqs(double dCarrierFreq1, double dCarrierFreq2, const UNIT_ cUnits);
typedef int(*pSetTxFreqs)(double, double, const char*);
//JIONTCOM_API int fnSetTxOn(BOOL_ bOn, BYTE_ byCarrier = 0);
typedef int(*pSetTxOn)(BOOL, uint8_t);
//JIONTCOM_API int fnGetImResult(JC_RETURN_VALUE dFreq, JC_RETURN_VALUE dPimResult, const UNIT_ cUnits);
typedef int(*pGetImResult)(double&, double&, const char*);
//JIONTCOM_API int fnSetSpan(int iSpan, const UNIT_ cUnits);
//JIONTCOM_API int fnSetRBW(int iRBW, const UNIT_ cUnits);
//JIONTCOM_API int fnSetVBW(int iVBW, const UNIT_ cUnits);
//JIONTCOM_API int fnSendCmd(BYTE_ byDevice, const CMD_ cmd, char* cResult, long& lCount);
//JIONTCOM_API int fnGetSpectrumType(char* cSpectrumType);
typedef int(*pGetSpectrumType)(char*);

//JC_API void  JcGetError(char* msg, size_t max);
typedef int(*pGetError)(char*, size_t);
//JC_API double JcGetAna(double freq_khz, bool isMax);
typedef double(*pJcGetAna)(double, bool);
//JC_API JcBool JcSetSig(JcInt8 byCarrier, double freq_khz, double pow_dbm);
typedef BOOL(*pJcSetSig)(uint8_t, double, double);
//JC_API double JcGetSen();
typedef double(*pJcGetSen)();
//JIONTCOM_API JcBool HwSetCoup(JcInt8 byCoup);
typedef BOOL(*pHwSetCoup)(uint8_t);

//JC_API long JcGetOffsetRxNum(BYTE_ byInternalBand);
typedef long(*pGetOffsetRxNum)(uint8_t);
//JC_API long JcGetOffsetTxNum(BYTE_ byInternalBand);
typedef long(*pGetOffsetTxNum)(uint8_t);
//JC_API JC_STATUS JcGetOffsetRx(JC_RETURN_VALUE offset_val,
//								 BYTE_ byInternalBand, BYTE_ byDutPort,
//								 double freq_mhz);
typedef int(*pGetOffsetRx)(double&, char, char, double);
//JC_API JC_STATUS JcGetOffsetTx(JC_RETURN_VALUE offset_val,
//								 BYTE_ byInternalBand, BYTE_ byDutPort,
//								 BYTE_ coup, BYTE_ real_or_dsp,
//								 double freq_mhz, double tx_dbm);
typedef int(*pGetOffsetTx)(double&, uint8_t, uint8_t, uint8_t, uint8_t, double, double);
//JC_API JC_STATUS JcGetOffsetVco(JC_RETURN_VALUE offset_vco, BYTE_ byInternalBand, BYTE_ byDutport);
typedef int(*pGetOffsetVco)(double&, uint8_t, uint8_t);
//JC_API JC_STATUS JcSetOffsetVco(BYTE_ byInternalBand, BYTE_ byDutport, double val);
typedef int(*pSetOffsetVco)(uint8_t, uint8_t, double);

typedef int(*pGetDllVersion)(int&, int&, int&, int&);

void Test_pim();
void Test_dll();

//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char* argv[])
{
	std::cout << "Welcome!" << std::endl;
	//std::thread t4([]() { std::cout << "Hello, C++11 thread\n"; });

	//Test_pim();
	for (int i = 0; i < 1; i++) {
		printf("==================No.%d=================\n", i);
		Test_dll();
	}
	getchar();
	return 0;
}

void Test_dll(){
	HINSTANCE hinst;
	hinst = LoadLibrary(_T("JcPimMultiBandV2.dll"));

	if (NULL == hinst){
		std::cout << "LoadLibrary Error!" << std::endl;
		return;
	}

	pTest test = (pTest)GetProcAddress(hinst, "gettestval");
	pSetInit setInit = (pSetInit)GetProcAddress(hinst, "fnSetInit");
	pSetMeasBand setMeasBand = (pSetMeasBand)GetProcAddress(hinst, "fnSetMeasBand");
	pSetExit setExit = (pSetExit)GetProcAddress(hinst, "fnSetExit");
	pSetImAvg setImAvg = (pSetImAvg)GetProcAddress(hinst, "fnSetImAvg");
	pSetDutPort setDutPort = (pSetDutPort)GetProcAddress(hinst, "fnSetDutPort");
	pSetImOrder setImOrder = (pSetImOrder)GetProcAddress(hinst, "fnSetImOrder");
	pCheckReceiveChannel checkReceiveChannel = (pCheckReceiveChannel)GetProcAddress(hinst, "fnCheckReceiveChannel");
	pCheckTwoSignalROSC checkTwoSignalROSC = (pCheckTwoSignalROSC)GetProcAddress(hinst, "fnCheckTwoSignalROSC");
	pSetTxPower setTxPower = (pSetTxPower)GetProcAddress(hinst, "fnSetTxPower");
	pSetTxFreqs setTxFreqs = (pSetTxFreqs)GetProcAddress(hinst, "fnSetTxFreqs");
	pSetTxOn setTxOn = (pSetTxOn)GetProcAddress(hinst, "fnSetTxOn");
	pGetImResult getImResult = (pGetImResult)GetProcAddress(hinst, "fnGetImResult");
	pGetSpectrumType getSpectrumType = (pGetSpectrumType)GetProcAddress(hinst, "fnGetSpectrumType");

	pHwSetCoup hwSetCoup = (pHwSetCoup)GetProcAddress(hinst, "HwSetCoup");
	pJcGetAna jcGetAna = (pJcGetAna)GetProcAddress(hinst, "JcGetAna");
	pJcSetSig jcSetSig = (pJcSetSig)GetProcAddress(hinst, "JcSetSig");
	pJcGetSen jcGetSen = (pJcGetSen)GetProcAddress(hinst, "JcGetSen");

	pGetError getError = (pGetError)GetProcAddress(hinst, "JcGetError");
	pGetOffsetRx getOffsetRx = (pGetOffsetRx)GetProcAddress(hinst, "JcGetOffsetRx");
	pGetOffsetRxNum getOffsetRxNum = (pGetOffsetRxNum)GetProcAddress(hinst, "JcGetOffsetRxNum");
	pGetOffsetTx getOffsetTx = (pGetOffsetTx)GetProcAddress(hinst, "JcGetOffsetTx");
	pGetOffsetTxNum getOffsetTxNum = (pGetOffsetTxNum)GetProcAddress(hinst, "JcGetOffsetTxNum");
	pGetOffsetVco getOffsetVco = (pGetOffsetVco)GetProcAddress(hinst, "JcGetOffsetVco");
	pSetOffsetVco setOffsetVco = (pSetOffsetVco)GetProcAddress(hinst, "JcSetOffsetVco");

	pGetDllVersion getDllVersion = (pGetDllVersion)GetProcAddress(hinst, "JcGetDllVersion");

	int v1, v2, v3, v4;
	getDllVersion(v1, v2, v3, v4);
	std::cout << "Version: "<<v1 << ',' << v2 << ',' << v3 << ',' << v4 << std::endl;

	int r = [test](){	
		return test(3, 4);
	}();

	std::cout << r << std::endl;
	std::string cmd = "this ext reference!";
	int a = cmd.find("Ext");
	if (a != -1){
		std::cout << "find ext" << std::endl;
	}
	
	//USB::0x0aad::0x000c::102838
	//USB0::0x0957::0x2B18::MY51020008::0::INSTR
	//GPIB0::12::INSTR
	bool isCont = true;
	//int s = setInit("0,0,0,0,0");
	int s = setInit("TCPIP0::192.168.1.3::5025::SOCKET,TCPIP0::192.168.1.4::5025::SOCKET,TCPIP0::192.168.1.5::inst0::INSTR,USB0::0x0957::0x2B18::MY51070014::0::INSTR,0");
	if (s == 0 && isCont == true) {
		std::cout << "init success!" << std::endl;
	}
	else {
		char msg[512] = { 0 };
		getError(msg, 512);
		std::cout << msg << std::endl;
	}

	BOOL B = jcSetSig(1, 930, -60);
	printf("set sig1: %d\n", B);
	B = jcSetSig(2, 960, -60);
	printf("set sig2: %d\n", B);
	for (int i = 0; i < 10; i++) {
		double ana = jcGetAna(930, false);
		printf("ana: %lf\n", ana);
		double sen = jcGetSen();
		printf("sen: %lf\n", sen);
	}

	//检测
	std::cout << std::endl;
	for (int i = 0; i < 7; i++) {
		long n = getOffsetRxNum(i);
		std::cout << i << "-Rx-Num: " << n << std::endl;
		n = getOffsetTxNum(i);
		std::cout << i << "-Tx-Num: " << n << std::endl;
	}
	double val = 0;
	s = getOffsetTx(val, JC_INTERNAL_BAND::LTE700, JC_DUTPORT_A, JC_COUP_TX2, JC_OFFSET_REAL, 728, 43);
	std::cout << "tx_offset = " << val << std::endl;
	s = getOffsetRx(val, JC_INTERNAL_BAND::LTE700, JC_DUTPORT_A, 715);
	std::cout << "rx_offset = " << val << std::endl;
	s = getOffsetVco(val, JC_INTERNAL_BAND::LTE700, JC_DUTPORT_A);
	std::cout << "vco_offset = " << val << std::endl;

	s = setExit();
	std::cout << "Exit!" << std::endl;

	FreeLibrary(hinst);
}

