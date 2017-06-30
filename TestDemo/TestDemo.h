//
// Created by san on 15/5/7.
//

#ifndef JCPIMMULTIBANDV2_TESTDEMO_H
#define JCPIMMULTIBANDV2_TESTDEMO_H

#include "stdafx.h"

#define JC_CARRIER_TX1TX2 0
#define JC_CARRIER_TX1 1
#define JC_CARRIER_TX2 2

#define JC_DUTPORT_A 0
#define JC_DUTPORT_B 1
#define JC_COUP_TX1 0
#define JC_COUP_TX2 1

#define INSTR_AG_U2000 0
#define INSTR_RS_NRT 1
#define INSTR_AG_N9000 2
#define INSTR_AG_N5180 3

#define INSTR_RS_NRPZ 10

#define JC_OFFSET_REAL 0
#define JC_OFFSET_DSP 1

#define JC_EXT_FLAG -1

typedef void(*Callback_Get_RX_Offset_Point)(double offset_freq, double Offset_val);
typedef void(*Callback_Get_TX_Offset_Point)(double offset_freq, double Offset_real_val, double Offset_dsp_val);

typedef int(*pTest)(int, int);
//JIONTCOM_API int fnSetInit(ADDRESS_ cDeviceAddr);
typedef int(*pSetInit2)(const char*, const char*);
typedef int(*pSetInit)(const char*);
typedef void(*pHwSetIsExtBand)(BOOL);
//JIONTCOM_API int HwSetExtFlag(int Build, const char* Flag);
typedef int(*pHwSetExtFlag)(int, const char*);
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
//int HwSetImCoefficients(JcInt8 byImCo1, JcInt8 byImCo2, JcInt8 byImLow, JcInt8 byImLess)
typedef int(*pSetImCoefficients)(uint8_t, uint8_t, uint8_t, uint8_t);
//JIONTCOM_API int fnCheckReceiveChannel(BYTE_ byBandIndex, BYTE_ byPort);
typedef int(*pCheckReceiveChannel)(uint8_t, uint8_t);
//JIONTCOM_API int fnCheckTwoSignalROSC();
typedef int(*pCheckTwoSignalROSC)();
//JIONTCOM_API int fnSetTxPower(double dTxPower1, double dTxPower2,
//	double dPowerOffset1, double dPowerOffset2);
typedef int(*pSetTxPower)(double, double, double, double);
//JIONTCOM_API int fnSetTxFreqs(double dCarrierFreq1, double dCarrierFreq2, const UNIT_ cUnits);
typedef int(*pSetTxFreqs)(double, double, const char*);
typedef int(*pHwSetTxFreqs)(double, double, const char*);
//HwGetSig_Smooth(JC_RETURN_VALUE dd, JcInt8 byCarrier)
typedef int(*pHwGetSig_Smooth)(double&, uint8_t);
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
typedef BOOL(*pJcSetSwitch)(int, int, int, int);
//JIONTCOM_API JcBool HwSetCoup(JcInt8 byCoup);
typedef BOOL(*pHwSetCoup)(uint8_t);

//JC_API void   JcSetAna_RefLevelOffset(double offset);
typedef void(*pJcSetAna_RefLevelOffset)(double);
//JC_API JcBool JcGetSig_ExtRefStatus(JcInt8 byCarrier);
typedef BOOL(*pJcGetSig_ExtRefStatus)(uint8_t);

//JC_API long JcGetOffsetRxNum(BYTE_ byInternalBand);
typedef long(*pGetOffsetRxNum)(uint8_t);
//JC_API long JcGetOffsetTxNum(BYTE_ byInternalBand);
typedef long(*pJcGetOffsetTxNum)(uint8_t);
//JC_API JC_STATUS JcGetOffsetRx(JC_RETURN_VALUE offset_val,
//								 BYTE_ byInternalBand, BYTE_ byDutPort,
//								 double freq_mhz);
typedef int(*pGetOffsetRx)(double&, char, char, double);
//JC_API JC_STATUS JcGetOffsetTx(JC_RETURN_VALUE offset_val,
//								 BYTE_ byInternalBand, BYTE_ byDutPort,
//								 BYTE_ coup, BYTE_ real_or_dsp,
//								 double freq_mhz, double tx_dbm);
typedef int(*pJcGetOffsetTx)(double&, uint8_t, uint8_t, uint8_t, uint8_t, double, double);
typedef int(*pJcSetOffsetTx)(char, char,double, double, Callback_Get_TX_Offset_Point);
//JC_STATUS JcSetOffsetRx(JcInt8 byInternalBand, JcInt8 byDutPort,
//	double loss_db, Callback_Get_RX_Offset_Point pHandler);
typedef int(*pJcSetOffsetRx)(char, char, double, Callback_Get_RX_Offset_Point);
//JC_API JC_STATUS JcGetOffsetVco(JC_RETURN_VALUE offset_vco, BYTE_ byInternalBand, BYTE_ byDutport);
typedef int(*pGetOffsetVco)(double&, uint8_t, uint8_t);
//JC_API JC_STATUS JcSetOffsetVco(BYTE_ byInternalBand, BYTE_ byDutport, double val);
typedef int(*pSetOffsetVco)(uint8_t, uint8_t, double);

typedef int(*pGetDllVersion)(int&, int&, int&, int&);

//JC_API void testcb(Callback_Get_RX_Offset_Point pHandler);
typedef void(*ptestcb)(Callback_Get_RX_Offset_Point);

//void JcSetOffsetTxIncremental(JcInt8 byInternalBand, JcInt8 byDutPort, JcInt8 coup, double Incremental)
typedef int(*pJcSetOffsetTxIncremental)(char, char, char, char, double);
//JC_STATUS JcGetOffsetBandInfo(int band_index, char* band_info)
typedef int(*pJcGetOffsetBandInfo)(int, char*);
//JC_API JC_STATUS JcSetOffsetTx_Single(JC_RETURN_VALUE resulte, JC_RETURN_VALUE resulte_sen,
//	int coup,
//	double des_p_dbm, double des_f_mhz,
//	double loss_db);
typedef int(*pJcSetOffsetTx_Single)(double&, double&, int, double, double, double);

//int JcGetCalibrationTime(char* cTime, int len, JcInt8 byInternalBand, JcInt8 byDutport)
typedef int(*pJcGetCalibrationTime)(char*, int, char, char);
//JC_API JC_STATUS JcSetOffsetTime(JcInt8 byInternalBand, JcInt8 byDutport, double val);
typedef int(*pJcSetOffsetTime)(char, char);

#endif //JCPIMMULTIBANDV2_TESTDEMO_H
