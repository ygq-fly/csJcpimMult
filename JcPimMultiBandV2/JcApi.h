#ifndef _SAN_JCAPI_H_
#define _SAN_JCAPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#define JIONTCOM_API __declspec(dllexport) 
#define JC_API __declspec(dllexport) 

#define JC_DEVICE_NOT_ENABLE "0"

#define JC_RETURN_VALUE double&
#define JC_RETURN_NUM int&
#define JC_STATUS int
#define JC_STATUS_SUCCESS 0
#define JC_STATUS_ERROR -10000
#define JC_STATUS_ERROR_GET_TX1_OFFSET      -10001
#define JC_STATUS_ERROR_GET_TX2_OFFSET      -10002
#define JC_STATUS_ERROR_GET_RX_OFFSET       -10003
#define JC_STATUS_ERROR_SET_TX_OUT_RANGE    -10004
#define JC_STATUS_ERROR_SET_TX_OUT_SMOOTH   -10005
#define JC_STATUS_ERROR_SET_BOSH_USE_TX1TX2 -10006
#define JC_STATUS_NO_AUTORIZE               -10007
#define JC_STATUS_NO_DATABASE               -10008

#define JC_STATUS_ERROR_SET_IM_FAIL -10010
#define JC_STATUS_ERROR_SET_TX_ONOFF_FAIL -10011
#define JC_STATUS_ERROR_CHECK_SIG1_ROSC_FAIL -10012
#define JC_STATUS_ERROR_CHECK_SIG2_ROSC_FAIL -10013
#define JC_STATUS_ERROR_SET_SWITCH_FAIL -10014
#define JC_STATUS_ERROR_CHECK_VCO_FAIL -10015
#define JC_STATUS_ERROR_READ_SPECTRUM_FAIL -10016
#define JC_STATUS_ERROR_READ_SPECTRUM_IDN_FAIL -10017
#define JC_STATUS_ERROR_NO_FIND_POWER -10018

#define JC_STATUS_ERROR_SIG1_CONN_FAIL -11000
#define JC_STATUS_ERROR_SIG2_CONN_FAIL -11001
#define JC_STATUS_ERROR_SA_CONN_FAIL -11002
#define JC_STATUS_ERROR_PM_CONN_FAIL -11003
#define JC_STATUS_ERROR_SWITCH_CONN_FAIL -11004
#define JC_STATUS_ERROR_DATABASE_CONN_FAIL -11005

#define JC_OFFSET_REAL 0
#define JC_OFFSET_DSP 1

#define JC_EXT_FLAG -1

typedef unsigned char JcInt8;
typedef int JcBool;
typedef char* JC_ADDRESS;
typedef char* JC_CMD;
typedef char* JC_UNIT;

//提供普通C方式校准回调，获取每个校准点值
//(如回调成员函数请使用bind来绑定该成员this指针）
typedef void(*Callback_Get_RX_Offset_Point)(double offset_freq, double Offset_val);
typedef void(*Callback_Get_TX_Offset_Point)(double offset_freq, double Offset_real_val, double Offset_dsp_val);

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

#define JC_CARRIER_TX1TX2 0
#define JC_CARRIER_TX1 1
#define JC_CARRIER_TX2 2
#define JC_CARRIER_RX 3

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

/////////////////////////////////////////////////////////////////////////////////////////////
//基本API
/////////////////////////////////////////////////////////////////////////////////////////////

//设置初始化地址：格式为逗号隔开SG1addr,SG2Addr,SAAddr,PMAddr
JIONTCOM_API int fnSetInit2(const JC_ADDRESS cDeviceAddr, const char* cDirectory);
JIONTCOM_API int fnSetInit(const JC_ADDRESS cDeviceAddr);
JIONTCOM_API void HwSetIsExtBand(JcBool isUse);
JIONTCOM_API int HwSetExtFlag(int Build, const char* Flag);
//关闭连接
JIONTCOM_API int fnSetExit();
JIONTCOM_API void HwSetExit();
//设置频段
JIONTCOM_API int fnSetMeasBand(JcInt8 byBandIndex);
JIONTCOM_API int HwSetMeasBand(JcInt8 byBandTx1, JcInt8 byBandTx2, JcInt8 byBandRx);
//设置测试端口(请先设置频段后)
JIONTCOM_API int fnSetDutPort(JcInt8 byPort);
JIONTCOM_API int HwSetDutPort(JcInt8 byPortTx1, JcInt8 byPortTx2, JcInt8 byPortRx);
//设置互调阶数
JIONTCOM_API int fnSetImOrder(JcInt8 byImOrder);
JIONTCOM_API int HwSetImOrder(JcInt8 byImOrder, JcInt8 byImLow, JcInt8 byImLess);
JIONTCOM_API int HwSetImCoefficients(JcInt8 byImCo1, JcInt8 byImCo2, JcInt8 byImLow, JcInt8 byImLess);
//设置平均次数
JIONTCOM_API int fnSetImAvg(JcInt8 byAvgTime);
//检测VCO
JIONTCOM_API int fnCheckReceiveChannel(JcInt8 byBandIndex, JcInt8 byPort);
//检测ext
JIONTCOM_API int fnCheckTwoSignalROSC();
//设置tx功率
JIONTCOM_API int fnSetTxPower(double dTxPower1, double dTxPower2,
							   double dPowerOffset1, double dPowerOffset2);
//设置tx频率
JIONTCOM_API int fnSetTxFreqs(double dCarrierFreq1, double dCarrierFreq2, const JC_UNIT cUnits);
JIONTCOM_API JC_STATUS HwSetTxFreqs(double dCarrierFreq1, double dCarrierFreq2, const JC_UNIT cUnits);
//tx开启
JIONTCOM_API int fnSetTxOn(JcBool bOn, JcInt8 byCarrier = 0);
//获取互调值
JIONTCOM_API int fnGetImResult(JC_RETURN_VALUE dFreq, JC_RETURN_VALUE dPimResult, const JC_UNIT cUnits);
JIONTCOM_API int HwGetImResult(double dFreq, JC_RETURN_VALUE dPimResult, const JC_UNIT cUnits);
//设置span
JIONTCOM_API int fnSetSpan(int iSpan, const JC_UNIT cUnits);
//设置rbw
JIONTCOM_API int fnSetRBW(int iRBW, const JC_UNIT cUnits);
//设置vbw
JIONTCOM_API int fnSetVBW(int iVBW, const JC_UNIT cUnits);
//通用透传接口
JIONTCOM_API int fnSendCmd(JcInt8 byDevice, const JC_CMD cmd, char* cResult, long& lCount);
//获取频谱仪类型
JIONTCOM_API int fnGetSpectrumType(char* cSpectrumType);
//Other
JIONTCOM_API void HwSetBandEnable(int iBand, JcBool isEnable); 
JIONTCOM_API JcBool HwSetCoup(JcInt8 byCoup);
JIONTCOM_API double HwGetCoup_Dsp(JcInt8 byCoup);
JIONTCOM_API JcBool FnGet_Vco();
JIONTCOM_API JcBool HwGet_Vco(double& real_val, double& vco_val);
JIONTCOM_API JC_STATUS HwGetSig_Smooth(JC_RETURN_VALUE dd, JcInt8 byCarrier);
JIONTCOM_API double HwGetDsp(JcInt8 byCarrier);

/////////////////////////////////////////////////////////////////////////////////////////////
//扩展API
/////////////////////////////////////////////////////////////////////////////////////////////
JC_API int fnSetInit_origin(const JC_ADDRESS cDeviceAddr);
JC_API JcBool JcConnSig(JcInt8 byDevice, JC_ADDRESS cAddr);
JC_API JcBool JcConnAna(JC_ADDRESS cAddr);
JC_API JcBool JcConnSen(JC_ADDRESS cAddr);
JC_API JcBool JcConnSwh();
JC_API JcBool JcGetVcoDsp(JC_RETURN_VALUE vco, JcInt8 bySwitchBand);
JC_API void  JcGetError(char* msg, unsigned int max);
JC_API JcBool JcGetDeviceStatus(JcInt8 byDevic);

JC_API JcBool JcGetSig_ExtRefStatus(JcInt8 byCarrier);
JC_API JcBool JcSetSig(JcInt8 byCarrier, double freq_khz, double pow_dbm);
JC_API JC_STATUS JcSetSig_Advanced(JcInt8 byCarrier, bool isOffset, double dOther);
JC_API double JcGetSig_CoupDsp(JcInt8 byCoup, JcInt8 byBand, JcInt8 byPort,
							   double freq_khz, double pow_dbm, double dExtOffset);
JC_API double JcGetSen();
JC_API void   JcSetAna_RefLevelOffset(double offset);
JC_API double JcGetAna(double freq_khz, bool isMax);
JC_API void JcNeedRetSwitch();
JC_API JcBool JcSetSwitch(int iSwitchTx1, int iSwitchTx2,
						  int iSwitchPim, int iSwitchDet);
JC_API JcBool JcGetChannelEnable(int channel_num);

/////////////////////////////////////////////////////////////////////////////////////////////
//OFFSET-API
/////////////////////////////////////////////////////////////////////////////////////////////
JC_API JC_STATUS JcSetOffsetTxIncremental(JcInt8 byInternalBand, JcInt8 byDutPort, JcInt8 coup, JcInt8 real_or_dsp, double Incremental);
JC_API JC_STATUS JcGetOffsetBandInfo(int band_index, char* band_info);
JC_API int JcGetOffsetBandCount();

JC_API JC_STATUS JcGetOffsetRx(JC_RETURN_VALUE offset_val,
							   JcInt8 byInternalBand, JcInt8 byDutPort,
							   double freq_mhz);
JC_API long JcGetOffsetRxNum(JcInt8 byInternalBand);
JC_API JC_STATUS JcSetOffsetRx(JcInt8 byInternalBand, JcInt8 byDutPort,
							   double loss_db, Callback_Get_RX_Offset_Point pHandler);

JC_API JC_STATUS JcGetOffsetTx(JC_RETURN_VALUE offset_val,
							   JcInt8 byInternalBand, JcInt8 byDutPort,
							   JcInt8 coup, JcInt8 real_or_dsp,
							   double freq_mhz, double tx_dbm);
JC_API long JcGetOffsetTxNum(JcInt8 byInternalBand);
JC_API JC_STATUS JcSetOffsetTx(JcInt8 byInternalBand, JcInt8 byDutPort,
							   double des_p_dbm, double loss_db, Callback_Get_TX_Offset_Point pHandler);
JC_API JC_STATUS JcSetOffsetTx_FreeBand(JcInt8 byInternalBand, JcInt8 byDutPort, double* freqs, int freq_num,
							   double des_p_dbm, double loss_db, Callback_Get_TX_Offset_Point pHandler);
JC_API JC_STATUS JcSetOffsetTx_Single(JC_RETURN_VALUE resulte, JC_RETURN_VALUE resulte_sen,
									  int coup,
									  double des_p_dbm, double des_f_mhz,
									  double loss_db);

JC_API JC_STATUS JcGetOffsetVco(JC_RETURN_VALUE offset_vco, JcInt8 byInternalBand, JcInt8 byDutport);
JC_API JC_STATUS JcSetOffsetVco(JcInt8 byInternalBand, JcInt8 byDutport, double val);
JC_API JC_STATUS JcGetCalibrationTime(char* time,int len, JcInt8 byInternalBand, JcInt8 byDutport);
JC_API JC_STATUS JcSetOffsetTime(JcInt8 byInternalBand, JcInt8 byDutport);
JC_API JcBool JcSetOffsetTX_Config(int iAnalyzer, const JC_ADDRESS Device_Info);
JC_API void  JcSetOffsetTX_Config_Close();

///////////////////////////////////////////////////////////////////////////////////////////
//other
///////////////////////////////////////////////////////////////////////////////////////////

JC_API int JcGetSwtichEnable(int byInternalBandIndex);
JC_API void JcFindRsrc();
JC_API int JcGetDllVersion(int &major, int &minor, int &build, int &revision);
JC_API int  JcGetIDN(unsigned long viSession, char* cIdn);

///////////////////////////////////////////////////////////////////////////////////////////
//测试（请无视）
///////////////////////////////////////////////////////////////////////////////////////////


JC_API void testcb(Callback_Get_RX_Offset_Point pHandler);
JC_API int gettestval(int a, int b);

#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif

#endif