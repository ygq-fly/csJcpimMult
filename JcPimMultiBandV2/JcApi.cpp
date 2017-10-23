//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//华为API
//-------------------------------------------------------------------------
//int SetInit(char *csDeviceAddr)	初始化仪器	传入信号源、频谱仪、功率计地址，用逗号隔开,如格式：SG1Addr, SG2Addr, SAAddr, PMAddr
//int SetExit()	关闭仪器释放资源
//int SetMeasBand(BYTE byBandIndex)	选择频段	7个频段，参数例如： 0：DD800 1:900。。。。6 : LTE2600，7：LTE700
//int SetImAvg(BYTE byAvgTime)	设置平均次数	byAvgTime : 0 - None, 1 - Minimal, 2 - Normal, 3 - High, 4 - Maximum
//int SetDutPort(BYTE byPort)	设置测试端口	0：port1； 1：port2
//int SetImOrder(BYTE byImOrder)	设置IM测试阶数	3、5、7阶
//int SetTxPower(double dTxPower1,
//int double dTxPower2, double dPowerOffset1, double dPowerOffset2)	设置功率及差损补偿	如：SetTxPower(43, 43, 0.5，0.5)
//int SetTxFreqs(double dCarrierFreq1, double dCarrierFreq2, CString csUnits)	设置F1、F2频点，单位MHz	建议在这个接口里面
//                                                                          对两路功率分别进行功率监控，
//                                                                          如果功率不是43，则进行修正
//int SetTxOn(BOOL bOn, BYTE byCarrier = 0)	打开或关闭功率	第一个参数表示打开还是关闭；第二个参数：0表示F1F2都执行；1表示F1    2表示F2
//int GetImResult(double & dFreq, double& dPimResult, CString csUnits)	获取PIM测试结果	最后一个参数dBm
//int SetSpan(int iSpan, CString csUnit)	　	设置值和单位
//int SetRBW(int iRBW, CString csUnit)
//int SetVBW(int iVBW, CString csUnits)
//int SendCmd(BYTE byDevice, CString
//int csCmd, CString & csResult)	通用透传接口	byDevice : 仪器序号(00:sg1, 01 : sg2, 02 : sa,03 : Pm), csCmd : 仪器指令, csResult : 指令返回结果
//int GetSpectrumType(CString & csSpectrumType)	获取频谱仪类型
//-------------------------------------------------------------------------

#include "JcApi.h"
#include "stdafx.h"
#include "JcPimObject.h"
#include "MyUtil/JcCommonAPI.h"

#define __pobj JcPimObject::Instance()

#define SIGNAL_SOURCE_MAX_POW 8
#define OFFSET_TX_THREASOLD 0.05
#define OFFSET_SWITCH_NULL -1

//#define JC_OFFSET_TX_SINGLE_DEBUG
//#define JC_OFFSET_TX_DEBUG
//#define JC_OFFSET_RX_DEBUG

//功率计标识
#define INSTR_AG_U2000_SERIES 0
#define INSTR_RS_NRT_SERIES 1
#define INSTR_RS_NRPZ_SERIES 2

//信号源标识
#define INSTR_AG_MXG_SERIES 10
#define INSTR_RS_SM_SERIES 11
#define INSTR_TEK_TSG 12
#define INSTR_JCSIG 13

//频谱仪标识
#define INSTR_AG_MXA_SERIES 20
#define INSTR_RS_FS_SERIES 21
#define INSTR_JCSPE 23

#define ROSC_EXT 1
#define ROSC_INT 0


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//mian
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int fnSetInit2(const JC_ADDRESS cDeviceAddr, const char* cDirectory) {
	_startPath = Util::utf8_to_wstring(cDirectory);
	return fnSetInit(cDeviceAddr);
}
//初始化
int fnSetInit(const JC_ADDRESS cDeviceAddr) {
	//加载pim对象
	if (NULL != __pobj) {

		//strcpy(NULL, "123");
		//建立模块
		__pobj->InitConfig();
		//if (__pobj->CheckAuthorization() == false) {
		//	__pobj->strErrorInfo = "Authorize Fail: \n" + __pobj->strErrorInfo;
		//	Util::logged(L"授权已到期！");
		//	return JC_STATUS_NO_AUTORIZE;
		//}

		//分配地址
		std::istringstream iss(cDeviceAddr);
		//std::vector<std::string> vaddr;
		std::string stemp = "";
		while (std::getline(iss, stemp, ',')) {
			__pobj->vaddr.push_back(stemp);
		}
		//补位,默认开启开关
		if (__pobj->vaddr.size() == 4)
			__pobj->vaddr.push_back("1");

		//开始连接数据库
		bool isSqlConn = __pobj->InitBandSet();
		if (!isSqlConn) {
			__pobj->strErrorInfo = ("DB Connected: false(JcOffset.db no find!)\r\n");
			return JC_STATUS_NO_DATABASE;
		}
		//开始连接
		std::string strConnMsg = "";
		ViStatus s = viOpenDefaultRM(&__pobj->viDefaultRM);
		if (s) return JC_STATUS_ERROR;

		if (__pobj->vaddr.size() < 5) return JC_STATUS_ERROR;

		if ("0" != __pobj->vaddr[2]) {
			if (false == JcConnAna(const_cast<char*>(__pobj->vaddr[2].c_str())))
				Util::logged(L"fnSetInit: Connect SA Fail! (%s)", Util::utf8_to_wstring(__pobj->vaddr[2]).c_str());
		}

		if ("0" != __pobj->vaddr[0]){
			if (false == JcConnSig(0, const_cast<char*>(__pobj->vaddr[0].c_str())))
				Util::logged(L"fnSetInit: Connect SG1 Fail!(%s)", Util::utf8_to_wstring(__pobj->vaddr[0]).c_str());
		}

		if ("0" != __pobj->vaddr[1]){
			if (false == JcConnSig(1, const_cast<char*>(__pobj->vaddr[1].c_str())))
				Util::logged(L"fnSetInit: Connect SG2 Fail! (%s)", Util::utf8_to_wstring(__pobj->vaddr[1]).c_str());
		}

		if ("0" != __pobj->vaddr[3]) {
			if (false == JcConnSen(const_cast<char*>(__pobj->vaddr[3].c_str())))
				Util::logged(L"fnSetInit: Connect PowerMeter Fail! (%s)", Util::utf8_to_wstring(__pobj->vaddr[3]).c_str());
		}

		if ("0" != __pobj->vaddr[4]) {
			if (false == JcConnSwh())
				strConnMsg = __pobj->swh->SwitchGetError();
		}

		//判断连接
		__pobj->isAllConn = __pobj->device_status[0] & __pobj->device_status[1]
							& __pobj->device_status[2] & __pobj->device_status[3] & __pobj->device_status[4];
		__pobj->isAllConn &= isSqlConn;
		//记录错误信息
		__pobj->strErrorInfo = ("SIG1 Connected: " + std::to_string(__pobj->device_status[0]) + "\r\n");
		__pobj->strErrorInfo += ("SIG2 Connected: " + std::to_string(__pobj->device_status[1]) + "\r\n");
		__pobj->strErrorInfo += ("Spectrum Connected: " + std::to_string(__pobj->device_status[2]) + "\r\n");
		__pobj->strErrorInfo += ("Sensor Connected: " + std::to_string(__pobj->device_status[3]) + "\r\n");
		__pobj->strErrorInfo += strConnMsg;
		if (!isSqlConn)
			__pobj->strErrorInfo += ("DB Connected: " + std::to_string(isSqlConn) + "(JcOffset.db no find!)\r\n");

		//启用接收外部频段
		HwSetExtFlag(-1, "ATE");

		if (false == __pobj->isAllConn)
			return JC_STATUS_ERROR;
	}

	return 0;
}

//设置外部频段（ATE请设true，其他false）
void HwSetIsExtBand(JcBool isUse) {
	Util::logged(L"当前软件版本无法使用此DLL，请更新软件！");
	fnSetExit();
}

//设置外部标识
int HwSetExtFlag(int Build, const char* Flag) {
	if (Build > 0) {
		__pobj->isUseExtBand = false;
		__pobj->wstrLogFlag = L"MBP";
	}
	else {
		__pobj->isUseExtBand = true;
		__pobj->wstrLogFlag = L"ATE";
	}
	return 0;
}

//释放
int fnSetExit(){
	JcPimObject::release();
	//必须2s的延时才会关闭连接
	Util::setSleep(2000);
	return 0;
}

void HwSetExit(){
	JcPimObject::release();
}

//获取vco校准数值
int JcGetCalibrationTime(char* cTime, int len, JcInt8 byInternalBand, JcInt8 byDutport) {
	std::string sband = __pobj->GetBandString(byInternalBand);

	__pobj->offset.OffsetTime(cTime, len, sband.c_str(), byDutport);
	return 0;
}

//设置频段
int fnSetMeasBand(JcInt8 byBandIndex){
	return HwSetMeasBand(byBandIndex, byBandIndex, byBandIndex);
}

int HwSetMeasBand(JcInt8 byBandTx1, JcInt8 byBandTx2, JcInt8 byBandRx){
	if (byBandTx1 > (__pobj->now_num_band -1) ||
		byBandTx2 > (__pobj->now_num_band -1) ||
		byBandRx  > (__pobj->now_num_band -1))
		return JC_STATUS_ERROR;

	if (__pobj->isUseExtBand) {
		//__pobj->now_band = __pobj->GetExtBandToIntBand(byBandIndex);
		rf1->band = __pobj->GetExtBandToIntBand(byBandTx1);
		rf2->band = __pobj->GetExtBandToIntBand(byBandTx2);
		pim->band = __pobj->GetExtBandToIntBand(byBandRx);
	}
	else {
		//__pobj->now_band = byBandIndex;
		rf1->band = byBandTx1;
		rf2->band = byBandTx2;
		pim->band = byBandRx;
	}

	if (__pobj->now_mode == MODE_DPX) {
		rf1->band = 0;
		rf2->band = 0;
		pim->band = 0;
	}

	return 0;
}

//请先设置 HwSetMeasBand
int fnSetDutPort(JcInt8 byPort) {
	return HwSetDutPort(byPort, byPort, byPort);
}

int HwSetDutPort(JcInt8 byPortTx1, JcInt8 byPortTx2, JcInt8 byPortRx) {
	__pobj->WriteClDebug("", true);
	if (__pobj->now_mode == MODE_DPX){
		if (byPortTx1 > 8 || byPortTx2 > 8) 
			return JC_STATUS_ERROR_SET_SWITCH_FAIL;
	}
	else{
		if (byPortTx1 > 2 || byPortTx2 > 2) 
			return JC_STATUS_ERROR_SET_SWITCH_FAIL;
	}

	//dd调整值清空
	__pobj->isAdjust = true;
	rf1->dd = 0;
	rf2->dd = 0;

	rf1->dutport = byPortTx1;
	rf2->dutport = byPortTx2;
	pim->dutport = byPortRx;

	//Band转换开关参数 , byPort = JC_DUTPORT_A 或　JC_DUTPORT_B
	if (__pobj->now_mode == MODE_POI ) {
		rf1->switch_port = rf1->band;
		rf2->switch_port = rf2->band;
		pim->switch_port = pim->band;
	}
	else if (__pobj->now_mode == MODE_DPX) {
		rf1->switch_port = byPortTx1;
		rf2->switch_port = byPortTx2;
		pim->switch_port = byPortRx;
	}
	else {
		rf1->switch_port = rf1->band * 2 + rf1->dutport;
		rf2->switch_port = rf2->band * 2 + rf2->dutport;
		pim->switch_port = pim->band * 2 + pim->dutport;
	}

	JcBool b = JcSetSwitch(rf1->switch_port, rf2->switch_port, pim->switch_port, JC_COUP_TX2);
	//Util::setSleep(_coup_delay);
	if (b == FALSE)
		__pobj->strErrorInfo = "fnSetDutPort: Switch Set Error!\r\n";

	return (b == TRUE ? 0 : JC_STATUS_ERROR_SET_SWITCH_FAIL);
}

//设置阶数
int fnSetImOrder(JcInt8 byImOrder) {
	return HwSetImOrder(byImOrder, SUM_LOW, SUM_LESS);
}

int HwSetImOrder(JcInt8 byImOrder, JcInt8 byImLow, JcInt8 byImLess) {
	//设置当前测试互调阶数,默认3
	pim->im_order = byImOrder;
	pim->im_low = byImLow;
	pim->im_less = byImLess;
	//排除直接小于1阶，默认3阶
	if (pim->im_order <= 1)
		pim->im_order = 3;

	if ((pim->im_order % 2) == 0) {
		//正数阶
		pim->im_coefficients1 = pim->im_order / 2;
		pim->im_coefficients2 = pim->im_order / 2;
	}
	else {
		//奇数阶
		pim->im_coefficients1 = pim->im_order / 2 + 1;
		pim->im_coefficients2 = pim->im_order / 2;
	}

	return 0;
}

int HwSetImCoefficients(JcInt8 byImCo1, JcInt8 byImCo2, JcInt8 byImLow, JcInt8 byImLess) {
	pim->im_coefficients1 = byImCo1;
	pim->im_coefficients2 = byImCo2;
	pim->im_order = byImCo1 + byImCo2;
	pim->im_low = byImLow;
	pim->im_less = byImLess;

	return 0;
}

//设置pim平均此时
int fnSetImAvg(JcInt8 byAvgTime) {
	if (byAvgTime < 1) byAvgTime = 1;
	pim->imAvg = byAvgTime;
	//设置频谱仪平均
	__pobj->ana->InstrSetAvg(byAvgTime);
	return 0;
}

//voc检测(针对ATE)
int fnCheckReceiveChannel(JcInt8 byBandIndex, JcInt8 byPort) {
	//切开关
	fnSetMeasBand(byBandIndex);
	int s = fnSetDutPort(byPort);
	if (s <= -10000){
		Util::logged(L"fnVco: Set Switch fail! (Band-%d-%d)", (int)byBandIndex, (int)byPort);
		return JC_STATUS_ERROR_SET_SWITCH_FAIL;
	}
	//判断vco_enable
	if (__pobj->isUseExtBand){
		JcInt8 byTemp = __pobj->GetExtBandToIntBand(byBandIndex);
		if (__pobj->now_vco_enable[byTemp] == 0)
			return 0;
	}
	//延时
	Util::setSleep(500);
	//开始测量
	double real_val = 0;
	double vco_val = 0;
	if (HwGet_Vco(real_val, vco_val) == FALSE) {
		Util::logged(L"fnVco: check VCO fail! (%lf)", real_val - vco_val);
		return JC_STATUS_ERROR_CHECK_VCO_FAIL;	
	}

	return 0;
}

//时钟同步检测(针对ATE)
int fnCheckTwoSignalROSC() {
	if (JcGetSig_ExtRefStatus(JC_CARRIER_TX1) == FALSE) {
		Util::logged(L"fnCheckTwoSignalROSC: check ROSC fail! (SG1)");
		return JC_STATUS_ERROR_CHECK_SIG1_ROSC_FAIL;
	}

	if (JcGetSig_ExtRefStatus(JC_CARRIER_TX2) == FALSE) {
		Util::logged(L"fnCheckTwoSignalROSC: check ROSC fail! (SG2)");
		return JC_STATUS_ERROR_CHECK_SIG2_ROSC_FAIL;
	}
	return 0;
}

//设置rf1,rf2
int fnSetTxPower(double dTxPower1, double dTxPower2,
	double dPowerOffset1, double dPowerOffset2) {
	
	if (_free_tx_enable == 0) {
		rf1->pow_dbm = dTxPower1;
		rf2->pow_dbm = dTxPower2;
		rf1->offset_ext = dPowerOffset1;
		rf2->offset_ext = dPowerOffset2;
	}
	else {
		rf1->pow_dbm = 43;
		rf2->pow_dbm = 43;
		rf1->offset_ext = dTxPower1 + dPowerOffset1 - 43;
		rf2->offset_ext = dTxPower2 + dPowerOffset2 - 43;
	}

	return 0;
}

//设置频率(针对华为ATE优化)
JC_STATUS fnSetTxFreqs(double dCarrierFreq1, double dCarrierFreq2, const JC_UNIT cUnits) {
	//单位转换
	rf1->freq_khz = __pobj->TransKhz(dCarrierFreq1, cUnits);
	rf2->freq_khz = __pobj->TransKhz(dCarrierFreq2, cUnits);
	JC_STATUS js;
	//---------------------------------------------------------------------------------
	//设置功放2
	js = JcSetSig_Advanced(JC_CARRIER_TX2, true, rf1->dd);
	if (js) return js;
	//设置功放1
	js = JcSetSig_Advanced(JC_CARRIER_TX1, true, rf2->dd);
	if (js) return js;
	//计算pim互调频率，设置中心频率
	pim->freq_khz = __pobj->GetPimFreq();
	__pobj->ana->InstrSetCenterFreq(pim->freq_khz);

	char cLog[256] = { 0 };
	sprintf_s(cLog, "\r\nF1 = %lf, F2 = %lf\r\n"
		"P1 = %lf(%lf), P2 = %lf(%lf)\r\n"
		"offset1 = %lf, offset2 = %lf\r\n",
		rf1->freq_khz / 1000, rf2->freq_khz / 1000,
		rf1->offset_int + rf1->pow_dbm, rf1->pow_dbm,
		rf2->offset_int + rf2->pow_dbm, rf2->pow_dbm,
		rf1->offset_ext, rf2->offset_ext);
	__pobj->WriteClDebug(cLog);
	__pobj->WriteClDebug("   pim_band: " + std::to_string(pim->band) + "   name: " + __pobj->GetBandString(pim->band) + "\r\n");
	__pobj->WriteClDebug("   pim_freq: " + std::to_string(pim->freq_khz) + "\r\n");
	__pobj->WriteClDebug("   rf_dd1: " + std::to_string(rf1->dd) + "   rf_dd2: " + std::to_string(rf2->dd) + "\r\n");

	//dd调整标识
	if (__pobj->isAdjust) {
		//---------------------------------------------------------------------------------
		//开启功放2
		js = fnSetTxOn(true, JC_CARRIER_TX2);
		if (0 != js) return js;
		js = fnSetTxOn(false, JC_CARRIER_TX1);
		if (0 != js) return js;
		//---------------------------------------------------------------------------------
		//切换耦合器tx2开关
		if (HwSetCoup(JC_COUP_TX2) == FALSE) {
			fnSetTxOn(false, JC_CARRIER_TX1TX2);
			return -10000;
		}

		//检测tx2功率平稳度
		js = HwGetSig_Smooth(rf2->dd, JC_CARRIER_TX2);
		if (js <= -10000) {
			//关闭功放
			fnSetTxOn(false, JC_CARRIER_TX1TX2);
			if (js == JC_STATUS_ERROR_NO_FIND_POWER)
				Util::logged(_T("fnSetTxFreqs: TX2未检测到功率！请检功率输出！"));
			else
				Util::logged(_T("fnSetTxFreqs: TX2功率偏差过大！"));
			return js;
		}
		//---------------------------------------------------------------------------------
		//开启功放1
		js = fnSetTxOn(true, JC_CARRIER_TX1);
		if (0 != js) return js;
		js = fnSetTxOn(false, JC_CARRIER_TX2);
		if (0 != js) return js;
		//---------------------------------------------------------------------------------
		//切换耦合器tx1开关
		if (HwSetCoup(JC_COUP_TX1) == FALSE) {
			fnSetTxOn(false, JC_CARRIER_TX1TX2);
			return -10000;
		}

		//检测tx1功率平稳度
		js = HwGetSig_Smooth(rf1->dd, JC_CARRIER_TX1);
		if (js <= -10000) {
			//关闭功放
			fnSetTxOn(false, JC_CARRIER_TX1TX2);
			if (js == JC_STATUS_ERROR_NO_FIND_POWER)
				Util::logged(_T("fnSetTxFreqs: TX1未检测到功率！请检功率输出！"));
			else
				Util::logged(_T("fnSetTxFreqs: TX1功率偏差过大！"));

			return js;
		}
		//---------------------------------------------------------------------------------
		//关闭功放
		js = fnSetTxOn(false, JC_CARRIER_TX1TX2);
		if (0 != js) return js;

		__pobj->isAdjust = false;
	}
	//---------------------------------------------------------------------------------
	if (!_tx_fast_mode) {
		__pobj->isAdjust = true;
		rf1->dd = 0;
		rf1->dd = 0;
	}

	return 0;
}

//设置频率(针对非ATE)
JC_STATUS HwSetTxFreqs(double dCarrierFreq1, double dCarrierFreq2, const JC_UNIT cUnits) {
	//单位转换
	rf1->freq_khz = __pobj->TransKhz(dCarrierFreq1, cUnits);
	rf2->freq_khz = __pobj->TransKhz(dCarrierFreq2, cUnits);

	//设置功放
	JC_STATUS js;
	js = JcSetSig_Advanced(JC_CARRIER_TX1, true, 0);
	if (js) return js;
	js = JcSetSig_Advanced(JC_CARRIER_TX2, true, 0);
	if (js) return js;

	//计算pim互调频率，设置中心频率
	pim->freq_khz = __pobj->GetPimFreq();
	__pobj->ana->InstrSetCenterFreq(pim->freq_khz);
	//设置中心频率
	//pim->freq_khz = __pobj->GetPimFreq();
	//__pobj->ana->InstrSetCenterFreq(pim->freq_khz);
	char cLog[256] = { 0 };
	sprintf_s(cLog, "\r\nF1 = %lf, F2 = %lf\r\n"
						"P1 = %lf(%lf), P2 = %lf(%lf)\r\n"
						"offset1 = %lf, offset2 = %lf\r\n",
		rf1->freq_khz / 1000, rf2->freq_khz / 1000, 
		rf1->offset_int + rf1->pow_dbm, rf1->pow_dbm,
		rf2->offset_int + rf2->pow_dbm, rf2->pow_dbm,
		rf1->offset_ext, rf2->offset_ext);
	__pobj->WriteClDebug(cLog);

	return JC_STATUS_SUCCESS;
}

//开启功放
int fnSetTxOn(JcBool bOn, JcInt8 byCarrier){
	bool isSucc = false;
	bool isOn = bOn == 0 ? false : true;
	if (byCarrier == JC_CARRIER_TX1TX2) {
		isSucc = __pobj->sig1->InstrOpenPow(isOn);
		isSucc &= __pobj->sig2->InstrOpenPow(isOn);
	}
	else if (byCarrier == JC_CARRIER_TX1)
		isSucc = __pobj->sig1->InstrOpenPow(isOn);
	else if (byCarrier == JC_CARRIER_TX2)
		isSucc = __pobj->sig2->InstrOpenPow(isOn);

	//当阶数为0 时关闭功放
	if (pim->im_coefficients1 <= 0) {
		__pobj->sig1->InstrOpenPow(false);
	}
	if (pim->im_coefficients2 <= 0) {
		__pobj->sig2->InstrOpenPow(false);
	}

	return (isSucc ? 0 : JC_STATUS_ERROR_SET_TX_ONOFF_FAIL);
}

//获取互调结果
int fnGetImResult(JC_RETURN_VALUE dFreq, JC_RETURN_VALUE dPimResult, const JC_UNIT cUnits) {
	pim->freq_khz = __pobj->GetPimFreq();
	HwGetImResult(pim->freq_khz, dPimResult, cUnits);
	dFreq = __pobj->TransToUnit(pim->freq_khz, cUnits);
	return 0;
}

int HwGetImResult(double dFreq, JC_RETURN_VALUE dPimResult, const JC_UNIT cUnits) {
	//pim->freq_khz = __pobj->GetPimFreq();
	//获取pim模块内部校准
	double rxoff;
	JC_STATUS s = JcGetOffsetRx(rxoff, pim->band, pim->dutport, dFreq / 1000);
	if (s) rxoff = 0;
	//设置pim模块补偿(直接设置ana内置补偿)
	JcSetAna_RefLevelOffset(rxoff);
	//JcSetAna_RefLevelOffset(0);
	//获取互调,返回数据
	dPimResult = 0;
	for (int i = 0; i < _pim_avg; i++) {
		double temp = JcGetAna(dFreq, _spe_is_max);
		//double temp = JcGetAna(pim->freq_khz, false);
		if (temp == JC_STATUS_ERROR){
			Util::logged(L"fnGetImResult: Spectrum read error!");
			__pobj->strErrorInfo = "Spectrum read error!\r\n";
			return JC_STATUS_ERROR_READ_SPECTRUM_FAIL;
		}
		dPimResult += temp;
	}
	dPimResult = dPimResult / (double)_pim_avg;
	//防止华为ATE告警
	if (dPimResult < _out_of_val_range)
		dPimResult = _out_of_val_range;
	//dPimResult += rxoff;

	return 0;
}

int fnSetSpan(int iSpan, const JC_UNIT cUnits) {
	if (NULL == __pobj) return JC_STATUS_ERROR;
	__pobj->ana->InstrSetSpan(__pobj->TransKhz(iSpan, cUnits) * 1000);
	return 0;
}

int fnSetRBW(int iRBW, const JC_UNIT cUnits) {
	if (NULL == __pobj) return JC_STATUS_ERROR;
	__pobj->ana->InstrSetRbw(__pobj->TransKhz(iRBW, cUnits) * 1000);
	return 0;
}

int fnSetVBW(int iVBW, const JC_UNIT cUnits) {
	if (NULL == __pobj) return JC_STATUS_ERROR;
	__pobj->ana->InstrSetVbw(__pobj->TransKhz(iVBW, cUnits) * 1000);
	return 0;
}

//通用设备命令
int fnSendCmd(JcInt8 byDevice, const JC_CMD cmd, char* cResult, long& lCount) {
	bool b = 0;
	std::string scmd(cmd);
	int n = scmd.find('?');
	switch (byDevice) {
	case 0://SIG1
		if (n > 0)
			lCount = __pobj->sig1->InstrWriteAndRead(cmd, cResult);
		else
			b = __pobj->sig1->InstrWrite(cmd);
		break;
	case 1://SIG2
		if (n > 0)
			lCount = __pobj->sig2->InstrWriteAndRead(cmd, cResult);
		else
			b = __pobj->sig2->InstrWrite(cmd);
		break;
	case 2://SA
		if (n > 0)
			lCount = __pobj->sen->InstrWriteAndRead(cmd, cResult);
		else
			b = __pobj->sen->InstrWrite(cmd);
		break;
	case 3://PM
		if (n > 0)
			lCount = __pobj->ana->InstrWriteAndRead(cmd, cResult);
		else
			b = __pobj->ana->InstrWrite(cmd);
		break;
	default:
		break;
	}

	return 0;
}

//获取频谱型号
int fnGetSpectrumType(char* cSpectrumType) {
	long num =  __pobj->ana->InstrWriteAndRead("*IDN?\n", cSpectrumType);
	//if (num > 0)
	//	return 0;
	//else
	//	return JC_STATUS_ERROR_READ_SPECTRUM_IDN_FAIL;
	return num > 0 ? 0 : JC_STATUS_ERROR_READ_SPECTRUM_IDN_FAIL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//HW-API
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//废除
void HwSetBandEnable(int iBand, JcBool isEnable) {
	//if (iBand < 0 || iBand > 6)
	//	return;
	//_switch_enable[iBand] = isEnable;
}

//设置当前功放的耦合器
JcBool HwSetCoup(JcInt8 byCoup) {
	if (!__pobj->GetCoupEnable(byCoup == JC_COUP_TX1 ? rf1->band : rf2->band)) {
		char cLog[256] = { 0 };
		__pobj->WriteClDebug("Set Coup-" + std::to_string(byCoup) + ": enale = 0\r\n");
		return true;
	}
	__pobj->WriteClDebug("Set Coup-" + std::to_string(byCoup) + "): enale = 1\r\n");
	//int iSwitch = __pobj->now_band * 2 + __pobj->now_dut_port;
	JcBool r = JcSetSwitch(rf1->switch_port, rf2->switch_port, pim->switch_port, byCoup);
	Util::setSleep(_coup_delay);
	if (FALSE == r)  {
		__pobj->WriteClDebug("   (return: set Coup fail)\r\n");
		Util::logged(L"HwSetCoup(Coup-%d): set Coup fail!", (int)byCoup);
	}
	return r;
}

//读取当前功放功率值(tx1或tx2)
double HwGetCoup_Dsp(JcInt8 byCoup) {
	//阶数为0 时，不检测
	if (pim->im_coefficients1 == 0 && byCoup == JC_COUP_TX1)
		return 0;
	if (pim->im_coefficients2 == 0 && byCoup == JC_COUP_TX2)
		return 0;

	double val = 0;
	double tx_temp = 0;
	int tx_sensor_count = 3;
	if (byCoup == JC_COUP_TX1) {
		//所有校准数据以mhz为单位，注意转换
		int s = JcGetOffsetTx(val, rf1->band, rf1->dutport,
							  byCoup, OFFSET_DSP,
							  rf1->freq_khz / 1000, rf1->pow_dbm);
		if (s) return s;
		tx_temp = rf1->pow_dbm + rf1->offset_ext;
		tx_sensor_count = _tx1_sensor_count;
	}
	else if (byCoup == JC_COUP_TX2) {
		//所有校准数据以mhz为单位，注意转换
		int s = JcGetOffsetTx(val, rf2->band, rf2->dutport,
							  byCoup, OFFSET_DSP,
							  rf2->freq_khz / 1000, rf2->pow_dbm);
		if (s) return s;
		tx_temp = rf2->pow_dbm + rf2->offset_ext;
		tx_sensor_count = _tx2_sensor_count;
	}
	//读取功率计
	double sen = 0;
	for (int i = 0; i < tx_sensor_count; i++) {
		sen = JcGetSen();
		//__pobj->WriteClDebug("   Avg3rd_count: " + std::to_string(_tx_sensor_count) + " \r\n");
		__pobj->WriteClDebug("   Avg3rd_1_sensor: " + std::to_string(sen) + " \r\n");
	}
	//计算补偿
	sen += val;
	__pobj->WriteClDebug("   Avg3rd_1_value: " + std::to_string(sen) + " \r\n");

	//retest
	double dd = tx_temp - sen;
	if (dd > __pobj->now_tx_smooth_threasold || dd < (__pobj->now_tx_smooth_threasold * -1)) {
		Util::setSleep(100);

		////读3次后平均
		//sen = JcGetSen();
		//sen += JcGetSen();
		//sen += JcGetSen();
		//__pobj->WriteClDebug("   Avg3rd_2_sensor: " + std::to_string(sen / 3) + " \r\n");
		//sen = sen / 3 + val;

		for (int i = 0; i < 3; i++) {
			sen = JcGetSen();
			__pobj->WriteClDebug("   Avg3rd_2_sensor: " + std::to_string(sen) + " \r\n");
		}
		//计算补偿
		sen += val;

		__pobj->WriteClDebug("   Avg3rd_2_value: " + std::to_string(sen) + " \r\n");
	}
	return sen;
}

//vco检测
JcBool FnGet_Vco() {
	//开始测量
	double real_val = 0;
	double vco_val = 0;
	return HwGet_Vco(real_val, vco_val);

}

//读取vco
JcBool HwGet_Vco(double& real_val, double& vco_val) {
	real_val = -10000;
	vco_val = -10000;
	if (__pobj->now_mode == MODE_POI)
		return FALSE;

	//获取实际值
	JcBool b = JcGetVcoDsp(real_val, pim->switch_port);
	//获取校准值
	JcGetOffsetVco(vco_val, pim->band, pim->dutport);
	double dd = real_val - vco_val;
	//判断
	b = (dd > (__pobj->now_vco_threasold * -1) && dd < __pobj->now_vco_threasold);
	//if (!b) 
	//	Util::logged(L"HwVco: VCO Error (%lf)", dd);
	return b;
}

//检测功放稳定度(必须功放开启后检测) return dd
JC_STATUS HwGetSig_Smooth(JC_RETURN_VALUE dd, JcInt8 byCarrier) {
	//初始化dd
	dd = 0;
	//阶数为0 时，不调整
	if (pim->im_coefficients1 == 0 && byCarrier == JC_CARRIER_TX1) {
		return JC_STATUS_SUCCESS;
	}
	if (pim->im_coefficients2 == 0 && byCarrier == JC_CARRIER_TX2) {
		return JC_STATUS_SUCCESS;
	}

	int error_time = 0;
	JC_STATUS ret = JC_STATUS_SUCCESS;
	//tx显示值
	double tx_dsp = 0;
	//tx偏差值
	double tx_deviate = 0;

	__pobj->WriteClDebug("start smooth-tx-" + std::to_string(byCarrier) + "\r\n");
	//adjust
	for (int i = 0; i < _tx_adjust_count; i++) {
		__pobj->WriteClDebug("   No.: " + std::to_string(i + 1) + "\r\n");
		if (i == 0) {
			if (byCarrier == JC_CARRIER_TX2)
				Util::setSleep(_tx_delay + _tx_delay2);
			else
				Util::setSleep(_tx_delay);
		}
		else {
			Util::setSleep(_tx_delay);
		}

		if (byCarrier == JC_CARRIER_TX1) {
			tx_dsp = HwGetCoup_Dsp(JC_COUP_TX1);
			tx_deviate = rf1->pow_dbm + rf1->offset_ext - tx_dsp;
			__pobj->now_dsp_tx1 = tx_dsp;
		}
		else if (byCarrier == JC_CARRIER_TX2) {
			tx_dsp = HwGetCoup_Dsp(JC_COUP_TX2);
			tx_deviate = rf2->pow_dbm + rf2->offset_ext - tx_dsp;
			__pobj->now_dsp_tx2 = tx_dsp;
		}
		else
			return JC_STATUS_ERROR_SET_BOSH_USE_TX1TX2;
		//log
		__pobj->WriteClDebug("   tx_dsp: " + std::to_string(tx_dsp) + "\r\n");
		//未检测功率时
		if (tx_dsp <= 25) {
			__pobj->strErrorInfo = "   PowerSmooth: No find Power!\r\n";
			__pobj->WriteClDebug("   (tx_dsp <= 25)\r\n");
			__pobj->WriteClDebug("   (Return error!)\r\n");
			ret =  JC_STATUS_ERROR_NO_FIND_POWER;
			break;
		}
		//超出范围
		if (tx_deviate > __pobj->now_tx_smooth_threasold || tx_deviate < (__pobj->now_tx_smooth_threasold * -1)) {
			dd = 0;
			//检测错误后，关闭功放
			//FnSetTxOn(false, byCarrier);		
			__pobj->strErrorInfo = "   PowerSmooth: Power's Smooth out Allowable Range\r\n";
			error_time++;
			__pobj->WriteClDebug("   (tx_dsp out range, count:" + std::to_string(error_time) + ")\r\n");
			if (error_time == 2) {
				//检测错误2次后，结束
				__pobj->WriteClDebug("   (Return error!)\r\n");
				ret = JC_STATUS_ERROR_SET_TX_OUT_SMOOTH;
				break;
			}
			else {
				//检测错误后，尝试重启信号源
				__pobj->WriteClDebug("   (Try to restart!)\r\n");
				fnSetTxOn(false);
				JcNeedRetSwitch();
				if (byCarrier == JC_CARRIER_TX1) {
					//尝试抖动
					HwSetCoup(JC_COUP_TX2);
					HwSetCoup(JC_COUP_TX1);
					fnSetTxOn(true, JC_CARRIER_TX1);
				}
				else if (byCarrier == JC_CARRIER_TX2) {
					//尝试抖动
					HwSetCoup(JC_COUP_TX1);
					HwSetCoup(JC_COUP_TX2);
					fnSetTxOn(true, JC_CARRIER_TX2);
				}				
				__pobj->WriteClDebug("   (Complete restart!)\r\n");
				//Util::setSleep(_tx_delay);
				i--;
				continue;
			}
		} else {
			if (tx_deviate >= (__pobj->now_tx_smooth_accuracy * -1) && tx_deviate <= __pobj->now_tx_smooth_accuracy)
				break;

			if (i < 3)
				dd += tx_deviate * (1 - i * 0.2);
			else 
				dd += (tx_deviate * 0.5);

			double sig_val = 0;
			JcBool b = FALSE;
			if (byCarrier == JC_CARRIER_TX1) {
				sig_val = rf1->pow_dbm + rf1->offset_ext + rf1->offset_int + dd;
				sig_val += __pobj->now_mode_bandset[rf1->band]->fine_adjust1;
				b = JcSetSig(JC_CARRIER_TX1, rf1->freq_khz, sig_val);
			}
			else if (byCarrier == JC_CARRIER_TX2) {
				sig_val = rf2->pow_dbm + rf2->offset_ext + rf2->offset_int + dd;
				sig_val += __pobj->now_mode_bandset[rf2->band]->fine_adjust2;
				b = JcSetSig(JC_CARRIER_TX2, rf2->freq_khz, sig_val);			
			}
			if (b == FALSE) {
				ret = -10000;
				__pobj->WriteClDebug("   (tx_set: sig set error)\r\n");
				break;
			}
			__pobj->WriteClDebug("   tx_set: " + std::to_string(sig_val) + "\r\n");
			//Util::setSleep(_tx_delay);
		}
	}
	__pobj->WriteClDebug("   dd: " + std::to_string(dd) + "\r\n");

	return ret;
}

double HwGetDsp(JcInt8 byCarrier) {
	if (byCarrier == JC_CARRIER_TX1) {
		return __pobj->now_dsp_tx1;
	}
	else {
		return __pobj->now_dsp_tx2;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//扩展API
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int fnSetInit_origin(const JC_ADDRESS cDeviceAddr) {
	if (NULL != __pobj) {
		//分配地址
		std::istringstream iss(cDeviceAddr);
		//std::vector<std::string> vaddr;
		std::string stemp = "";
		while (std::getline(iss, stemp, ',')) {
			__pobj->vaddr.push_back(stemp);
		}

		//补位,默认开启开关
		if (__pobj->vaddr.size() == 4)
			__pobj->vaddr.push_back("0");

		//开始连接
		std::string strConnMsg = "";
		ViStatus s = viOpenDefaultRM(&__pobj->viDefaultRM);
		if (s) return JC_STATUS_ERROR;

		if (__pobj->vaddr.size() < 5) return JC_STATUS_ERROR;

		for (int i = 0; i < 5; i++) 
			__pobj->device_status[i] = true;

		_spe_preamp = 0;
		if ("0" != __pobj->vaddr[2]) {
			if (false == JcConnAna(const_cast<char*>(__pobj->vaddr[2].c_str())))
				Util::logged(L"fnSetInit: Connect SA Fail! (%s)", Util::utf8_to_wstring(__pobj->vaddr[2]).c_str());
		}

		if ("0" != __pobj->vaddr[0]){
			if (false == JcConnSig(0, const_cast<char*>(__pobj->vaddr[0].c_str())))
				Util::logged(L"fnSetInit: Connect SG1 Fail!(%s)", Util::utf8_to_wstring(__pobj->vaddr[0]).c_str());
		}

		if ("0" != __pobj->vaddr[1]){
			if (false == JcConnSig(1, const_cast<char*>(__pobj->vaddr[1].c_str())))
				Util::logged(L"fnSetInit: Connect SG2 Fail! (%s)", Util::utf8_to_wstring(__pobj->vaddr[1]).c_str());
		}

		if ("0" != __pobj->vaddr[3]) {
			if (false == JcConnSen(const_cast<char*>(__pobj->vaddr[3].c_str())))
				Util::logged(L"fnSetInit: Connect PowerMeter Fail! (%s)", Util::utf8_to_wstring(__pobj->vaddr[3]).c_str());
		}

		//判断连接
		__pobj->isAllConn = __pobj->device_status[0] & __pobj->device_status[1]
			& __pobj->device_status[2] & __pobj->device_status[3] & __pobj->device_status[4];
		//记录错误信息
		__pobj->strErrorInfo = ("SIG1 Connected: " + std::to_string(__pobj->device_status[0]) + "\r\n");
		__pobj->strErrorInfo += ("SIG2 Connected: " + std::to_string(__pobj->device_status[1]) + "\r\n");
		__pobj->strErrorInfo += ("Spectrum Connected: " + std::to_string(__pobj->device_status[2]) + "\r\n");
		__pobj->strErrorInfo += ("Sensor Connected: " + std::to_string(__pobj->device_status[3]) + "\r\n");
		__pobj->strErrorInfo += strConnMsg;

		if (false == __pobj->isAllConn)
			return JC_STATUS_ERROR;
	}

	return 0;
}

JcBool JcConnSig(JcInt8 byDevice, JC_ADDRESS cAddr) {
	ViSession vi = VI_NULL;
	ViStatus s = viOpen(__pobj->viDefaultRM, cAddr, VI_NULL, VI_NULL, &vi);
	if (s == VI_SUCCESS) {
		JcPimObject::Instance()->JcSetViAttribute(vi);
		char cIdn[128] = { 0 };
		int index = JcGetIDN(vi, cIdn);			
		//安捷伦
		if (index == INSTR_AG_MXG_SERIES || index == INSTR_JCSIG){
			if (byDevice == SIGNAL1) {
				__pobj->sig1 = std::make_shared<ClsSigAgN5181A>();
				__pobj->sig1->InstrSession(vi, cIdn);
			}
			else if (byDevice == SIGNAL2) {
				__pobj->sig2 = std::make_shared<ClsSigAgN5181A>();
				__pobj->sig2->InstrSession(vi, cIdn);
			}
			//if (index == INSTR_JCSIG)
			//	_protect_rx = OFFSET_JCPROTECT_RX;
		}
		//罗德斯瓦茨
		else if (index == INSTR_RS_SM_SERIES) {
			if (byDevice == SIGNAL1) {
				__pobj->sig1 = std::make_shared<ClsSigRsSMxSerial>(_sig_rosc);
				__pobj->sig1->InstrSession(vi, cIdn);
			}
			else if (byDevice == SIGNAL2) {
				__pobj->sig2 = std::make_shared<ClsSigRsSMxSerial>(ROSC_EXT);
				__pobj->sig2->InstrSession(vi, cIdn);
			}
		}
		else if (index == INSTR_TEK_TSG) {
			if (byDevice == SIGNAL1) {
				__pobj->sig1 = std::make_shared<ClsSigTekTsg4000>();
				__pobj->sig1->InstrSession(vi, cIdn);
			}
			else if (byDevice == SIGNAL2) {
				__pobj->sig2 = std::make_shared<ClsSigTekTsg4000>();
				__pobj->sig2->InstrSession(vi, cIdn);
			}
		}
		else 
			return FALSE;	
	}

	__pobj->device_status[byDevice] = !s;
	return !s;
}

JcBool JcConnAna(JC_ADDRESS cAddr) {
	ViSession vi = VI_NULL;
	ViStatus s = viOpen(__pobj->viDefaultRM, cAddr, VI_NULL, VI_NULL, &vi);
	if (s == VI_SUCCESS) {
		JcPimObject::Instance()->JcSetViAttribute(vi);
		char cIdn[128] = { 0 };
		int index = JcGetIDN(vi, cIdn);
		if (Util::strFind(cIdn, "FSC"))
			_sig_rosc = ROSC_INT; 
		bool isPreamp = _spe_preamp & 1;
		//安捷伦
		if (index == INSTR_AG_MXA_SERIES || index == INSTR_JCSPE) {
			__pobj->ana = std::make_shared<ClsAnaAgN9020A>(isPreamp);
			__pobj->ana->InstrSession(vi, cIdn);
		}
		//罗德斯瓦茨
		else if (index == INSTR_RS_FS_SERIES) {
			__pobj->ana = std::make_shared<ClsAnaRsFspSerial>(isPreamp);
			__pobj->ana->InstrSession(vi, cIdn);
		}
		else
			return FALSE;
		__pobj->ana->InstrSetModeAtt(_spe_pim_att, _spe_offset_att);
		__pobj->ana->InstrInit();
	}

	__pobj->device_status[ANALYZER] = !s;
	return !s;
}

JcBool JcConnSen(JC_ADDRESS cAddr) {
	ViSession vi = VI_NULL;
	bool isConn = false;
	//安捷伦仪表连接
	ViStatus s = viOpen(__pobj->viDefaultRM, cAddr, VI_NULL, VI_NULL, &vi);
	if (s == VI_SUCCESS) {
		JcPimObject::Instance()->JcSetViAttribute(vi);
		char cIdn[128] = { 0 };
		int index = JcGetIDN(vi, cIdn);
		if (index == INSTR_AG_U2000_SERIES) {
			__pobj->sen = std::make_shared<ClsSenAgU2000A>();
			__pobj->sen->InstrSession(vi, cIdn);
		}
		else if (index == INSTR_RS_NRT_SERIES) {
			__pobj->sen = std::make_shared<ClsSenRsNrt>();
			__pobj->sen->InstrSession(vi, cIdn);
		}
		else
			return FALSE;
		isConn = !s;
	} else {
		//尝试RS仪表连接
		__pobj->sen = std::make_shared<ClsSenRsNrpz>();
		isConn = __pobj->sen->InstrConnect(cAddr);
		////尝试tek仪表连接
		//if (!isConn) {
		//	__pobj->sen.reset();
		//	__pobj->sen = NULL;
		//	__pobj->sen = std::make_shared<ClsSenTekPsm>();
		//	isConn = __pobj->sen->InstrConnect(cAddr);
		//}
	}

	__pobj->device_status[SENSOR] = isConn;
	return isConn;
}

JcBool JcConnSwh() {
	__pobj->swh = std::make_shared<ClsJcSwitch>();
	std::locale old_loc = std::locale::global(std::locale(""));
	const wchar_t* src_wstr = _startPath.c_str();
	size_t buffer_size = _startPath.size() * 4 + 1;
	char buf[256] = { 0 };
	size_t n;
	wcstombs_s(&n, buf, 256, src_wstr, buffer_size);
	std::locale::global(old_loc);
	__pobj->device_status[4] = __pobj->swh->SwitchInit(__pobj->now_mode, 2, buf);
	return __pobj->device_status[SWITCH];
}

JcBool JcGetVcoDsp(JC_RETURN_VALUE vco, JcInt8 bySwitchBand) {
	if (NULL == __pobj) return false;

	double vco_freq_mhz; 
	if (__pobj->now_mode == MODE_POI) {
		int band = bySwitchBand / 2;
		vco_freq_mhz = __pobj->now_mode_bandset[band]->vco_a;
	}
	else  {
		int band = bySwitchBand / 2;
		if ((bySwitchBand % 2) == 0)
			vco_freq_mhz = __pobj->now_mode_bandset[band]->vco_a;
		else
			vco_freq_mhz = __pobj->now_mode_bandset[band]->vco_b;

		if (__pobj->now_mode == MODE_HUAWEI && vco_freq_mhz == 0) {
			if (bySwitchBand == 14)
				vco_freq_mhz = 1366;
			else if (bySwitchBand == 15)
				vco_freq_mhz = 1368;
			else
				vco_freq_mhz = 1334 + 2 * bySwitchBand;
		}
	}

	if (vco_freq_mhz == 0)
		return false;
	//__pobj->ana->InstrSetCenterFreq(vco_freq_mhz * 1000);
	__pobj->ana->InstrVcoSetting();
	Util::setSleep(_vco_delay);
	vco = __pobj->ana->InstrGetAnalyzer(vco_freq_mhz * 1000, true);
	__pobj->ana->InstrPimSetting();
	return vco >= -95 ? true : false;
}

//获取错误
void JcGetError(char* msg, unsigned int max) {
	if (NULL == __pobj) return;

	size_t n = __pobj->strErrorInfo.size();
	if (n > max) n = max;
	memcpy(msg, __pobj->strErrorInfo.c_str(), n);
	__pobj->strErrorInfo = "Not";
}

//获取各个模块状态
JcBool JcGetDeviceStatus(JcInt8 byDevice) {
	if (NULL == __pobj) return false;

	if (byDevice > 4 || byDevice < 0)
		return false;
	else
		return __pobj->device_status[byDevice];
}

//获取外部refence状态
JcBool JcGetSig_ExtRefStatus(JcInt8 byCarrier) {
	if (NULL == __pobj) return 0;
	bool isExt = false;

	if (byCarrier == JC_CARRIER_TX1)
		isExt = __pobj->sig1->InstrGetReferenceStatus();
	else if (byCarrier == JC_CARRIER_TX2)
		isExt = __pobj->sig2->InstrGetReferenceStatus();
	else if (byCarrier == JC_CARRIER_TX1TX2) {
		isExt = __pobj->sig1->InstrGetReferenceStatus();
		isExt &= __pobj->sig2->InstrGetReferenceStatus();
	}

	return isExt;
}

//设置功放参数 (打开关闭功放请使用FnSetTxOn())
JcBool JcSetSig(JcInt8 byCarrier, double freq_khz, double pow_dbm) {
	if (NULL == __pobj) return FALSE;

	bool b = false;
	if (byCarrier == JC_CARRIER_TX1)
		b = __pobj->sig1->InstrSetFreqPow(freq_khz, pow_dbm);
	else if (byCarrier == JC_CARRIER_TX2)
		b = __pobj->sig2->InstrSetFreqPow(freq_khz, pow_dbm);	
	else if (byCarrier == JC_CARRIER_TX1TX2) {
		b = __pobj->sig1->InstrSetFreqPow(freq_khz, pow_dbm);
		b &= __pobj->sig2->InstrSetFreqPow(freq_khz, pow_dbm);
	}

	if (false == b)
		Util::logged(L"JcSetSig: set sig fail! (sig-%d)", (int)byCarrier);

	return b;
}

//设置功放参数(高级)
JC_STATUS JcSetSig_Advanced(JcInt8 byCarrier, bool isOffset, double dOther) {
	if (NULL == __pobj) return JC_STATUS_ERROR;

	//初始化参数
	struct JcRFModule *rf;
	rf = byCarrier == JC_CARRIER_TX1 ? rf1 : rf2;
	JcInt8 byBand = rf->band;
	JcInt8 byPort = rf->dutport;
	double freq_khz = rf->freq_khz;
	double pow_dbm = rf->pow_dbm;
	double dExtOffset = isOffset ? (rf->offset_ext + dOther) : rf->offset_ext;
	
	double tx_true = pow_dbm;
	double internal_offset = 0;
	
	//开始获取内部校准
	JcInt8 coup = byCarrier - (JcInt8)1;
	//所有校准数据以mhz为单位，注意转换
	int s = JcGetOffsetTx(internal_offset, byBand, byPort, coup, JC_OFFSET_REAL, freq_khz / 1000, pow_dbm);
	if (s) {
		//Util::logged("offset:%lf, band:%d, port:%d, coup:%d, freq:%lf, pow:%lf", internal_offset, (int)byBand, (int)byPort, (int)coup, freq_khz / 1000, pow_dbm);
		__pobj->strErrorInfo = "SetTx" + std::to_string(byCarrier) + ": Read Offset's Data Error!\r\n";
		//返回错误
		return JC_STATUS_ERROR_GET_TX1_OFFSET - 1 + byCarrier;
	}
	//计算实际设置功率值
	tx_true = pow_dbm + dExtOffset + internal_offset;
	if (tx_true > SIGNAL_SOURCE_MAX_POW) {
		__pobj->strErrorInfo = "SetTx" + std::to_string(byCarrier) + ": SIG's Power out range（Maybe Offset's Data Error！）\r\n";
		return JC_STATUS_ERROR_SET_TX_OUT_RANGE;
	}
	if (byCarrier == JC_CARRIER_TX1) {
		rf1->offset_int = internal_offset;
		//微调
		tx_true += __pobj->now_mode_bandset[rf->band]->fine_adjust1;
		bool b = __pobj->sig1->InstrSetFreqPow(freq_khz, tx_true);
		if (false == b) {
			Util::logged(L"JcSetSig_Adv: set sig fail! (sig-%d)", (int)byCarrier);
			return -10000;
		}
	}
	else if (byCarrier == JC_CARRIER_TX2){
		rf2->offset_int = internal_offset;
		//微调
		tx_true += __pobj->now_mode_bandset[rf->band]->fine_adjust2;
		bool b = __pobj->sig2->InstrSetFreqPow(freq_khz, tx_true);
		if (false == b) {
			Util::logged(L"JcSetSig_Adv: set sig fail! (sig-%d)", (int)byCarrier);
			return -10000;
		}
	}
	return JC_STATUS_SUCCESS;
}

//读取当前SIG功率值(tx1或tx2) return sen
double JcGetSig_CoupDsp(JcInt8 byCoup, JcInt8 byBand, JcInt8 byPort,
						double freq_khz, double pow_dbm, double dExtOffset) {
	double val = 0;
	//所有校准数据以mhz为单位，注意转换
	int s = JcGetOffsetTx(val, byBand, byPort,
						  byCoup, OFFSET_DSP,
						  freq_khz / 1000, pow_dbm);
	if (s) val = 0;
	//读取功率计
	double sen = JcGetSen();
	//计算补偿
	sen += val;
	sen -= dExtOffset;
	return sen;
}

//获取功率计
double JcGetSen() {
	if (NULL == __pobj) return JC_STATUS_ERROR;
	return __pobj->sen->InstrGetSesnor();
}

//设置频谱REF LEVEL OFFSET
void JcSetAna_RefLevelOffset(double offset) {
	if (NULL == __pobj) return;
	__pobj->ana->InstrSetOffset(offset);
}

//获取频谱
double JcGetAna(double freq_khz, bool isMax){
	if (NULL == __pobj) return JC_STATUS_ERROR;
	return __pobj->ana->InstrGetAnalyzer(freq_khz, isMax);
}

//设置开关(iSwitchTx: 0 ~ 13)
//enum JC_HUAWEI_BAND {
//	LTE700_A = 0,
//	LTE700_B = 1,
//	DD800_A = 2,
//	DD800_B = 3,
//	GSM900_A = 4,
//	GSM900_B = 5,
//	DCS1800_A = 6,
//	DCS1800_B = 7,
//	PCS1900_A = 8,
//	PCS1900_B = 9,
//	WCDMA2100_A = 10,
//	WCDMA2100_B = 11,
//	LTE2600_A = 12,
//	LTE2600_B = 13
//};
void JcNeedRetSwitch() {
	//开始重置标识，only once
	_need_reset = true;
	JcSetSwitch(rf1->switch_port, rf2->switch_port, pim->switch_port, JC_COUP_TX2);
	_need_reset = false;
	Util::setSleep(_reset_delay);
	JcSetSwitch(rf1->switch_port, rf2->switch_port, pim->switch_port, JC_COUP_TX2);

}

JcBool JcSetSwitch(int iSwitchTx1, int iSwitchTx2, int iSwitchPim, int iSwitchCoup) {
	if (NULL == __pobj) return JC_STATUS_ERROR;
	//查找检测通道标号
	int coup = -1;
	if (__pobj->now_mode == MODE_POI) {
		int temp_iSwitchTx1 = iSwitchTx1;
		int temp_iSwitchTx2 = iSwitchTx2;

		////特殊频段处理(防止出现下面的频段同时出现)
		//if (iSwitchTx1 > 11 && iSwitchTx1 < 17)
		//	temp_iSwitchTx1 -= 5;
		//if (iSwitchTx2 > 11 && iSwitchTx2 < 17)
		//	temp_iSwitchTx2 -= 5;
		////新加一频
		//if (iSwitchTx1 == 12) {
		//	iSwitchTx1 = 17;
		//	temp_iSwitchTx1 = 12;
		//}
		//if (iSwitchTx2 == 12) {
		//	iSwitchTx2 = 17;
		//	temp_iSwitchTx2 = 12;
		//}
		//if (iSwitchPim == 12) {
		//	iSwitchPim = 17;
		//}

		//查找ID_POI检测通道标号
		//这里的iSwitch和band相配对
		//(配对信息在数据库中)
		//int coup1 = __pobj->now_mode_bandset[temp_iSwitchTx1].switch_coup1;
		//int coup2 = __pobj->now_mode_bandset[temp_iSwitchTx2].switch_coup2;
		//coup = iSwitchCoup == JC_COUP_TX1 ? coup1 : coup2;
		if (iSwitchCoup == JC_COUP_TX1 && temp_iSwitchTx1 >= 0) {
			coup = __pobj->now_mode_bandset[temp_iSwitchTx1]->switch_coup1;

			//根据tx_enable重设
			if (!__pobj->now_mode_bandset[temp_iSwitchTx1]->tx1_enable)
				iSwitchTx1 = -1;
		} else if (iSwitchCoup == JC_COUP_TX2 && temp_iSwitchTx2 >= 0) {
			coup = __pobj->now_mode_bandset[temp_iSwitchTx2]->switch_coup2;

			//根据tx_enable重设
			if (!__pobj->now_mode_bandset[temp_iSwitchTx2]->tx2_enable)
				iSwitchTx2 = -1;
		} else 
			coup = -1;

	} 
	else if (__pobj->now_mode == MODE_DPX) {
		coup = iSwitchCoup;
	}
	else {
		//查找ID_HUAWEI检测通道标号
		//这里的iSwitch和band不会配对
		if (iSwitchCoup == JC_COUP_TX1) 
		{
			if (iSwitchTx1 % 2 == 0)
				coup = iSwitchTx1 + JC_COUP_TX1;
			else
				coup = (iSwitchTx1 - 1) + JC_COUP_TX1;
		}
		else {
			if (iSwitchTx2 % 2 == 0)
				coup = iSwitchTx2 + JC_COUP_TX2;
			else
				coup = (iSwitchTx2 - 1) + JC_COUP_TX2;
		}
	}

	if (coup < -1) coup = -1;
	JcBool ret = __pobj->swh->SwitchExcut(iSwitchTx1, iSwitchTx2, iSwitchPim, coup, _need_reset);
	_need_reset = false;
	if (!ret) __pobj->strErrorInfo = "Switch: Excut Error!\r\n";
	return ret;
}

JcBool JcGetChannelEnable(int channel_num) {
	switch (channel_num)
	{
	case 1:
		return __pobj->now_mode_bandset[rf1->band]->tx1_enable;
	case 2:
		return __pobj->now_mode_bandset[rf2->band]->tx2_enable;
	case 3:
		return __pobj->now_mode_bandset[pim->band]->rx_enable;
	default:
		break;
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Rx 校准API
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//add 15-7-10
JC_STATUS JcSetOffsetTxIncremental(JcInt8 byInternalBand, JcInt8 byDutPort, JcInt8 coup, JcInt8 real_or_dsp, double Incremental) {
	std::string strBand = __pobj->GetBandString(byInternalBand);
	bool b = __pobj->offset.DbSetTxIncremental(strBand.c_str(), byDutPort, coup, real_or_dsp, Incremental);
	return (b ? 0 : -10000);
}

JC_STATUS JcGetOffsetBandInfo(int band_index, char* band_info){
	if (NULL == __pobj) return JC_STATUS_ERROR;
	
	char prefix[64] = { 0 };
	sprintf_s(prefix, "%s%d", __pobj->now_band_prefix.c_str(), band_index);
	return __pobj->offset.GetBandInfo(prefix, band_info);
}

int JcGetOffsetBandCount() {
	if (NULL == __pobj) return JC_STATUS_ERROR;
	return __pobj->now_num_band;
}

//获取Rx校准
JC_STATUS JcGetOffsetRx(JC_RETURN_VALUE offset_val,
						JcInt8 byInternalBand, JcInt8 byDutPort,
						double freq_mhz){
	//检查当前频段是否允许
	if (__pobj->now_mode_bandset[byInternalBand]->rx_enable == FALSE) {
		__pobj->strErrorInfo = "GetRxOffset: rx channel can not used";
		return JC_STATUS_ERROR;
	}

	//获取当前频段显示字符
	std::string sband = __pobj->GetBandString(byInternalBand);
	//计算校准数据点
	if (__pobj->now_mode == MODE_DPX)
		offset_val = __pobj->offset.OffsetRx_dpx(sband.c_str(), byDutPort, freq_mhz);
	else
		offset_val = __pobj->offset.OffsetRx(sband.c_str(), byDutPort, freq_mhz);
	if (offset_val == JC_STATUS_ERROR) {
		__pobj->strErrorInfo = "GetRxOffset: Read error!\r\n";
		return JC_STATUS_ERROR_GET_RX_OFFSET;
	}
	else 
		return JC_STATUS_SUCCESS;
}

//获取rx点数
long JcGetOffsetRxNum(JcInt8 byInternalBand){
	//获取当前频段显示字符
	std::string sband = __pobj->GetBandString(byInternalBand);
	//获取Rx校准点
	double Rxfreq[MAX_SIZE_FREQ] = { 0 };
	int freq_num = __pobj->offset.FreqHeader(OFFSET_RX, sband.c_str(), Rxfreq, MAX_SIZE_FREQ);
	return freq_num;
}

//自动校准接收Rx (校准前请确认连线)，（只使用JC_TX1来校准）
JC_STATUS JcSetOffsetRx(JcInt8 byInternalBand, JcInt8 byDutPort,
						double loss_db, Callback_Get_RX_Offset_Point pHandler) {
	//检查当前接收频段是否允许
	if (JcGetChannelEnable(JC_CARRIER_RX) == FALSE) {
		__pobj->strErrorInfo = "RxOffset: rx channel can not used";
		return JC_STATUS_ERROR;
	}

	//Band转换开关参数
	int iswitch = -1;
	if (__pobj->now_mode == MODE_POI)
		iswitch = byInternalBand;
	else
		iswitch = byInternalBand * 2 + byDutPort;
	//切换开关
	JcBool isSwhConn = JcSetSwitch(OFFSET_SWITCH_NULL, OFFSET_SWITCH_NULL, iswitch, OFFSET_SWITCH_NULL);
	if (isSwhConn == FALSE) {
		__pobj->strErrorInfo = "RxOffset: Switch Fail!\r\n";
		return JC_STATUS_ERROR;
	}

	//获取当前频段显示字符
	std::string sband = __pobj->GetBandString(byInternalBand);
	//获取Rx校准点
	double Rxfreq[MAX_SIZE_FREQ] = { 0 };
	int freq_num = __pobj->offset.FreqHeader(OFFSET_RX, sband.c_str(), Rxfreq, MAX_SIZE_FREQ);
	double off[MAX_SIZE_FREQ] = { 0 };
	__pobj->ana->InstrRxOffsetSetting();

	std::string strLog = "start offset-rx-" +
		std::to_string(byInternalBand) + "-" +
		std::to_string(byDutPort) + "\r\n";

	//设置保护值
	//JcSetSig(JC_CARRIER_TX1, Rxfreq[0] * 1000, OFFSET_PROTECT_RX);
	JcSetSig(JC_CARRIER_TX1, Rxfreq[0] * 1000, _protect_rx);
	//if (_protect_rx == OFFSET_JCPROTECT_RX)
	//	__pobj->ana->InstrSetAtt(30);
	//开启功放
	fnSetTxOn(true, JC_CARRIER_TX1);
	//VCO
	double vco = 0;
	if (JcGetVcoDsp(vco, byInternalBand * 2 + byDutPort) == false) {
		//__pobj->strErrorInfo = "RxOffset: vco < -90!\r\n";
		//return JC_STATUS_ERROR;
		//不返回错误继续
	}
	if (pHandler)
		pHandler(0, vco);
	JcSetOffsetVco(byInternalBand, byDutPort, vco);
	__pobj->ana->InstrSetCenterFreq(Rxfreq[0] * 1000);

	Util::setSleep(1000);
	for (int i = 0; i < freq_num; ++i) {		
		//设置
		JcSetSig(JC_CARRIER_TX1, Rxfreq[i] * 1000, _protect_rx);
		Util::setSleep(200);
		if (_protect_rx == OFFSET_JC_PROTECT_RX)
			Util::setSleep(300);
		//读取
		double v = JcGetAna(Rxfreq[i] * 1000, false);
		if (v == JC_STATUS_ERROR) {
			//关闭功放
			fnSetTxOn(false, JC_CARRIER_TX1);
			__pobj->strErrorInfo = "Spectrum read error!\r\n";
			return JC_STATUS_ERROR_READ_SPECTRUM_FAIL;
		}
		//计算规则: 目标值（OFFSET_PROTECT_RX） = 实际值（v） + 校准值 （off） +差损（loss_db）
		off[i] = _protect_rx - v - loss_db;
		if (off[i] > _protect_range_rx || off[i] < (_protect_range_rx*-1)) {
			//错误，关闭功放
			fnSetTxOn(false, JC_CARRIER_TX1);
			//__pobj->ana->InstrSetAvg(2);
			__pobj->strErrorInfo = "   RxOffset: No Find Power(" + std::to_string(_protect_rx) + ")!\r\n";
			strLog += __pobj->strErrorInfo;
			JcPimObject::Instance()->LoggingWrite(strLog.c_str());
			return JC_STATUS_ERROR;
		}
		//开始回调
		if (pHandler)
			pHandler(Rxfreq[i], off[i]);
#ifdef JC_OFFSET_RX_DEBUG
		std::cout << "Freq = " << Rxfreq[i] << "MHz " << std::endl;
		std::cout << "Now = " << v << "dBm ; Off = " << off[i] << std::endl;
#endif
	}
	//关闭功放
	fnSetTxOn(false, JC_CARRIER_TX1);
	//if (_protect_rx == OFFSET_JCPROTECT_RX)
	//	__pobj->ana->InstrSetAtt(0);
	JC_STATUS s;
	if (__pobj->now_mode == MODE_DPX)
		s = __pobj->offset.Store_dpx(OFFSET_RX, sband.c_str(), byDutPort, 0, 0, 0, Rxfreq, off, freq_num);
	else
		s = __pobj->offset.Store_v2(OFFSET_RX, sband.c_str(), byDutPort, 0, 0, 0, off, freq_num);
	if (s) {
		__pobj->strErrorInfo = "RxOffset: Save Error!\r\n";
		return JC_STATUS_ERROR;
	}

	return JC_STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Tx 校准API
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//获取tx校准(band:当前频段，dutport:当前测试端口，coup:当前测试功放)
JC_STATUS JcGetOffsetTx(JC_RETURN_VALUE offset_val,
						JcInt8 byInternalBand, JcInt8 byDutPort,
						JcInt8 coup, JcInt8 real_or_dsp,
						double freq_mhz, double tx_dbm) {

	//检查当前频段是否允许
	JcBool tx_enable = coup == JC_DUTPORT_A ?
		__pobj->now_mode_bandset[byInternalBand]->tx1_enable:
		__pobj->now_mode_bandset[byInternalBand]->tx2_enable;
	if (tx_enable == FALSE) {
		__pobj->strErrorInfo = "GetTxOffset: tx channel can not used, tx" + std::to_string(coup + 1);
		return JC_STATUS_ERROR;
	}

	//传输模式
	JcInt8 byTempDutPort = byDutPort;
	//if ((__pobj->isUseTransType == true) && (byDutPort == JC_DUTPORT_B))
	if ((__pobj->now_mode == MODE_TRANSMISSION) && (byDutPort == JC_DUTPORT_B))
		byTempDutPort = JC_DUTPORT_A;

	//获取当前频段显示字符
	std::string sband = __pobj->GetBandString(byInternalBand);
	//计算校准数据点
	if (__pobj->now_mode == MODE_DPX)
		offset_val = __pobj->offset.OffsetTx_dpx(sband.c_str(), byTempDutPort, coup, real_or_dsp, freq_mhz, tx_dbm);
	else
		offset_val = __pobj->offset.OffsetTx(sband.c_str(), byTempDutPort, coup, real_or_dsp, freq_mhz, tx_dbm);
	if (offset_val == JC_STATUS_ERROR) {
		offset_val = 0;
		__pobj->strErrorInfo = "TxOffset: Read data error!\r\n";
		return JC_STATUS_ERROR;
	}
	else 
		return JC_STATUS_SUCCESS;
}

//获取tx点数
long JcGetOffsetTxNum(JcInt8 byInternalBand) {
	//获取当前频段显示字符
	std::string sband = __pobj->GetBandString(byInternalBand);
	//获取Tx校准频点
	double txfreq[MAX_SIZE_FREQ] = { 0 };
	int freq_num = __pobj->offset.FreqHeader(OFFSET_TX, sband.c_str(), txfreq, MAX_SIZE_FREQ);
	return freq_num;
}

//自动校准Tx (校准前请确认连线)，(band:当前频段，dutport:当前测试端kou), 同时校准tx1,tx2
JC_STATUS JcSetOffsetTx(JcInt8 byInternalBand, JcInt8 byDutPort, double des_p_dbm, double loss_db,
						Callback_Get_TX_Offset_Point pHandler) {

	//获取当前频段显示字符
	std::string sband = __pobj->GetBandString(byInternalBand);
	//获取Tx校准频点
	//double txfreq[MAX_SIZE_FREQ] = { 0 };
	//int freq_num = __pobj->offset.FreqHeader(OFFSET_TX, sband.c_str(), txfreq, MAX_SIZE_FREQ);
	//double off_real[MAX_SIZE_FREQ] = { 0 };
	//double off_dsp[MAX_SIZE_FREQ] = { 0 };

	double* txfreq = NULL;
	int freq_num = __pobj->offset.FreqHeader(OFFSET_TX, sband.c_str(), &txfreq);
	if (freq_num <= 0) {
		if (txfreq) delete txfreq;
		return JC_STATUS_ERROR;
	}

	/*double* val = new double[freq_num];
	for (size_t i = 0; i < freq_num;i++)
	{
		val[i] = 43 + i;
	}*/
	/*__pobj->offset.Store_dpx(OFFSET_TX, sband.c_str(), byDutPort, 0, JC_OFFSET_REAL, des_p_dbm, txfreq, val, freq_num);
	return 0;*/

	double val= __pobj->offset.OffsetTx_dpx(sband.c_str(), 0, 0, 0, 728, 43);
	return 0;

	JC_STATUS ret = JcSetOffsetTx_FreeBand(byInternalBand, byDutPort, txfreq, freq_num, des_p_dbm, loss_db, pHandler);
	if (txfreq) delete txfreq;
	return  ret;
}

JC_STATUS JcSetOffsetTx_FreeBand(JcInt8 byInternalBand, JcInt8 byDutPort, double* freqs, int freq_num,
								 double des_p_dbm, double loss_db, Callback_Get_TX_Offset_Point pHandler) {
	if (freqs == NULL) {
		__pobj->strErrorInfo = "freqs is null";
		return JC_STATUS_ERROR;
	}

	//检查当前接收频段是否允许
	if (JcGetChannelEnable(JC_CARRIER_TX1) == FALSE && JcGetChannelEnable(JC_CARRIER_TX2) == FALSE) {
		__pobj->strErrorInfo = "TxOffset: tx1, tx2 channel can not used";
		return JC_STATUS_ERROR;
	}

	if (__pobj->ext_sen_index == 1) {
		//to do
		//外部传感器使用
	}
	else {
		//默认方式，需设置频谱
		__pobj->ana->InstrTxOffsetSetting();
		Util::setSleep(500);
	}

	//获取当前频段显示字符
	std::string sband = __pobj->GetBandString(byInternalBand);
	//获取Tx校准频点

	double* off_real = new double[freq_num];
	double* off_dsp = new double[freq_num];

	//Band转换开关参数
	int iswitch = -1;
	if (__pobj->now_mode == MODE_POI)
		iswitch = byInternalBand;
	else
		iswitch = byInternalBand * 2 + byDutPort;
	//----------------------------------------------------------------------------------------------
	//coup ==> JC_COUP_TX1 to JC_COUP_TX2
	for (int coup = 0; coup < 2; ++coup) {

#ifdef JC_OFFSET_TX_DEBUG
		std::cout << "\n";
		std::cout << "Start: " << coup << std::endl;
#endif
		//检查TX1TX2是否允许发射
		if (JcGetChannelEnable(coup + 1) == FALSE)
			continue;

		//----------------------------------------------------------------------------------------------
		//切换开关
		JcBool isSwhConn = JcSetSwitch(iswitch, iswitch, OFFSET_SWITCH_NULL, coup);
		if (isSwhConn == FALSE) {
			if (off_real) delete off_real;
			if (off_dsp) delete off_dsp;
			__pobj->strErrorInfo = "TxOffset: Switch-Coup Fail!\r\n";
			return JC_STATUS_ERROR;
		}
		//----------------------------------------------------------------------------------------------
		//开启功放,设置保护值
		std::shared_ptr<IfSignalSource> pow = coup == JC_COUP_TX1 ? __pobj->sig1 : __pobj->sig2;
		pow->InstrSetFreqPow(freqs[0] * 1000, _protect_tx);
		pow->InstrOpenPow(true);
		//----------------------------------------------------------------------------------------------
		Util::setSleep(400);
		//开始 自动生成 TX1 校准数据
		for (int i = 0; i < freq_num; ++i) {
			//单点校准
			int s = JcSetOffsetTx_Single(off_real[i], off_dsp[i], coup, des_p_dbm, freqs[i], loss_db);
			if (s) return s;
			//开始回调
			if (pHandler) pHandler(freqs[i], off_real[i], off_dsp[i]);

#ifdef JC_OFFSET_TX_DEBUG
			std::cout << "Run: " << freqs[i] << " MHz" << std::endl;
			std::cout << "Off = " << off_real[i] << " ; Sen= " << off_dsp[i] << std::endl;
#endif

		}
		//----------------------------------------------------------------------------------------------
		//关闭功放
		pow->InstrOpenPow(false);
		//----------------------------------------------------------------------------------------------
		//存储校准数据
		int s;
		if (__pobj->now_mode == MODE_DPX)
			s = __pobj->offset.Store_dpx(OFFSET_TX, sband.c_str(), byDutPort, coup, JC_OFFSET_REAL, des_p_dbm, off_real, freqs, freq_num);
		else
			s = __pobj->offset.Store_v2(OFFSET_TX, sband.c_str(), byDutPort, coup, JC_OFFSET_REAL, des_p_dbm, off_real, freq_num);
		if (s) {
			if (off_real) delete off_real;
			if (off_dsp) delete off_dsp;
			__pobj->strErrorInfo = "TxOffset: Tx_Real data save error!\r\n";
			return JC_STATUS_ERROR;
		}

		if (__pobj->now_mode == MODE_DPX)
			s = __pobj->offset.Store_dpx(OFFSET_TX, sband.c_str(), byDutPort, coup, JC_OFFSET_DSP, des_p_dbm, off_real, freqs, freq_num);
		else
			s = __pobj->offset.Store_v2(OFFSET_TX, sband.c_str(), byDutPort, coup, JC_OFFSET_DSP, des_p_dbm, off_dsp, freq_num);
		if (s) {
			if (off_real) delete off_real;
			if (off_dsp) delete off_dsp;
			__pobj->strErrorInfo = "TxOffset: Tx_Dsp data save error!\r\n";
			return JC_STATUS_ERROR;
		}
		//----------------------------------------------------------------------------------------------

#ifdef JC_OFFSET_TX_DEBUG
		std::cout << "Save Success!" << std::endl;
#endif
	}

	JcSetOffsetTime(byInternalBand, byDutPort);
	//还原频谱设置
	if (__pobj->ext_sen_index == 0)
		__pobj->ana->InstrPimSetting();

	if (off_real) delete off_real;
	if (off_dsp) delete off_dsp;
	return JC_STATUS_SUCCESS;
}

//自动校准Tx - single
JC_STATUS JcSetOffsetTx_Single(JC_RETURN_VALUE resulte,
							   JC_RETURN_VALUE resulte_sen,
							   int coup, 
							   double des_p_dbm, 
							   double des_f_mhz, 
							   double loss_db) {
	std::shared_ptr<IfSignalSource> pow = coup == JC_COUP_TX1 ? __pobj->sig1 : __pobj->sig2;
	//设置保护值
	pow->InstrSetFreqPow(des_f_mhz * 1000, _protect_tx);
	//-------------------------------------------------------------------------------------
	//pow->InstrOpenPow(true);
	//-------------------------------------------------------------------------------------

	if (__pobj->ext_sen_index == 1){
		//to do
		//外部传感器使用
	}
	else {
		//默认方式，需设置频谱		
		__pobj->ana->InstrTxOffsetSetting();
	}

	double p_true = _protect_tx;
	resulte = 0;
	std::string strLog = "start offset-tx\r\n";

	for (int i = 0; i < 8; i++) {
		//设置
		pow->InstrSetFreqPow(des_f_mhz * 1000, p_true);
		Util::setSleep(_tx_offset_delay);
		//读取
		double v = -10000;
		double r = 0;

		for (int n = 0; n < 3; n++) {
			if (__pobj->ext_sen_index == 1)
				//外部传感器，读取数值
				v = __pobj->ext_sen->InstrGetSesnor();
			else {
				//默认方式，读取频谱
				v = JcGetAna(des_f_mhz * 1000, false);
				if (v == JC_STATUS_ERROR){
					//关闭功放
					fnSetTxOn(false, JC_CARRIER_TX1);
					__pobj->strErrorInfo = "Spectrum read error!\r\n";
					return JC_STATUS_ERROR_READ_SPECTRUM_FAIL;
				}
			}

			strLog += "   freq: " + std::to_string(des_f_mhz) +
						"  val: " + std::to_string(v) + "\r\n";
			
			//判断1
			if (v <= _tx_no_power_limit) {
                Util::setSleep(1000);
                continue;
            }
	
			//判断2
			r = des_p_dbm - (v + loss_db);
			double temp = p_true + r;
			if (p_true >= SIGNAL_SOURCE_MAX_POW){
				Util::setSleep(1000);
				continue;
			}
			else
				break;
		}

		//检测
		if (v <= _tx_no_power_limit) {
			pow->InstrOpenPow(false);
			__pobj->strErrorInfo = "   TxOffset: This Channel can not find Power!\r\n";
			strLog += __pobj->strErrorInfo;
			JcPimObject::Instance()->LoggingWrite(strLog.c_str());
			if (v <= -10000)
				__pobj->strErrorInfo = "TxOffset: External Sensor read error!\r\n";
			return JC_STATUS_ERROR;
		}

#ifdef JC_OFFSET_TX_SINGLE_DEBUG
		std::cout << "No: " << i << std::endl;
		std::cout << "Set: " << des_f_mhz << " Mhz, " << p_true << "dBm\n";
		std::cout << "Get: " << v << std::endl;
		std::cout << "r = " << r << std::endl;
#endif

		//TX规则: 目标值（des_p_dbm） + 校准值（resulte） = 求实际值（p_true）
		resulte = p_true - des_p_dbm;
		if (r <= OFFSET_TX_THREASOLD && r >= (OFFSET_TX_THREASOLD * -1))
			break;
		
		if (i >= 4)
			r = r / 2;
		else
			p_true += (r * 0.9);

		if (p_true >= SIGNAL_SOURCE_MAX_POW) {
			pow->InstrOpenPow(false);
			__pobj->strErrorInfo = "   TxOffset: This Channel's power so big!\r\n";
			strLog += __pobj->strErrorInfo;
			JcPimObject::Instance()->LoggingWrite(strLog.c_str());
			return JC_STATUS_ERROR_SET_TX_OUT_RANGE;
		}	
	}

	Util::setSleep(_sensor_delay);
	double sen = JcGetSen();
	sen += JcGetSen();
	sen += JcGetSen();
	
	//TX_DSP规则: 实际值（sen） + 校准值（resulte_sen） = 求显示值（des_p_dbm）
	resulte_sen = des_p_dbm - sen / 3;
	//-------------------------------------------------------------------------------------
	//关闭功放
	//pow->InstrOpenPow(false);
	//-------------------------------------------------------------------------------------
#ifdef JC_OFFSET_TX_SINGLE_DEBUG
	std::cout << "sen = " << resulte_sen << std::endl;
	std::cout << "Complete Offset!" << std::endl;
#endif
	return JC_STATUS_SUCCESS;
}


//获取vco校准数值
JC_STATUS JcGetOffsetVco(JC_RETURN_VALUE offset_vco, JcInt8 byInternalBand, JcInt8 byDutport) {
	std::string sband = __pobj->GetBandString(byInternalBand);

	offset_vco = __pobj->offset.OffsetVco(sband.c_str(), byDutport);
	if (offset_vco <= -10000) {
		offset_vco = 0;
		return JC_STATUS_ERROR;
	}
	else
		return JC_STATUS_SUCCESS;
}

//保存vco校准数值
JC_STATUS JcSetOffsetVco(JcInt8 byInternalBand, JcInt8 byDutport, double val) {
	std::string sband = __pobj->GetBandString(byInternalBand);

	int s = __pobj->offset.Store_vco_single(sband.c_str(), byDutport, val);
	if (s) {
		__pobj->strErrorInfo = "VCO_Offset: VCO's data save error!\r\n";
		return JC_STATUS_ERROR;
	}
	return JC_STATUS_SUCCESS;
}

JC_STATUS JcSetOffsetTime(JcInt8 byInternalBand, JcInt8 byDutport) {
	std::string sband = __pobj->GetBandString(byInternalBand);
	time_t now = time(NULL);
	char date[128] = { 0 };
	struct tm p = { 0 };
	localtime_s(&p, &now);
	sprintf_s(date, "%d-%02d-%02d %02d:%02d:%02d", 1900 + p.tm_year, 1 + p.tm_mon, p.tm_mday,
										p.tm_hour, p.tm_min, p.tm_sec);
	int s = __pobj->offset.Store_calibration_time(sband.c_str(), byDutport, date);
	if (s) {
		__pobj->strErrorInfo = "database: time save error!\r\n";
		return JC_STATUS_ERROR;
	}
	return JC_STATUS_SUCCESS;
}

//设置自动校准tx的配置
JcBool JcSetOffsetTX_Config(int iAnalyzer, const JC_ADDRESS Device_Info) {
	__pobj->ext_sen_index = iAnalyzer;
	if (__pobj->ext_sen_index == 0) 
		return true;
	else if (__pobj->ext_sen_index == 1) {
		if (__pobj->isExtSenConn) return __pobj->isExtSenConn;
		//开始连接
		ViSession ext_visession;
		int index_sensor;
		//visa仪表连接
		ViStatus s = viOpen(__pobj->viDefaultRM, const_cast<char*>(Device_Info), VI_NULL, 5000, &ext_visession);
		if (s == VI_SUCCESS) {
			__pobj->isExtSenConn = true;
			index_sensor = JcGetIDN(ext_visession, NULL);
			if (index_sensor == INSTR_AG_U2000_SERIES) {
				__pobj->ext_sen = std::make_shared<ClsSenAgU2000A>();
				__pobj->ext_sen->InstrSession(ext_visession, "");
			}
			else if (index_sensor == INSTR_RS_NRT_SERIES) {
				__pobj->ext_sen = std::make_shared<ClsSenRsNrt>();
				__pobj->ext_sen->InstrSession(ext_visession, "");
			}
		}
		//RS仪表连接
		else {
			index_sensor = INSTR_RS_NRPZ_SERIES;
			__pobj->ext_sen = std::make_shared<ClsSenRsNrpz>();
			__pobj->isExtSenConn = __pobj->ext_sen->InstrConnect(Device_Info);
		}
	}
	else {
		__pobj->ext_sen_index = 0;
		__pobj->isExtSenConn = false;
	}

	return __pobj->isExtSenConn;
}

//设置自动校准tx的配置（关闭）
void JcSetOffsetTX_Config_Close() {
	if (__pobj->isExtSenConn) {
		__pobj->ext_sen->InstrClose();
		__pobj->ext_sen.reset();
		__pobj->isExtSenConn = false;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
//OTHER FUNC
//////////////////////////////////////////////////////////////////////////////////////////////

//废除
int JcGetSwtichEnable(int byInternalBandIndex){
	//if (byInternalBandIndex < 0 || byInternalBandIndex > 6)
	//	return 0;
	//return _switch_enable[byInternalBandIndex];
	return 0;
}

void JcFindRsrc() {
	char instrAddr[VI_FIND_BUFLEN];
	ViUInt32 num;
	ViFindList findlist;
	ViSession _defaultRm;
	ViStatus _status;

	_status = viOpenDefaultRM(&_defaultRm);
	_status = viFindRsrc(_defaultRm, const_cast<char*>("?*INSTR"), &findlist, &num, instrAddr);

	std::cout << "Num:  " << num << std::endl;
	std::cout << "Addr: " << instrAddr << std::endl;

	while (--num) {
		_status = viFindNext(findlist, instrAddr);
		std::cout << "Addr: " << instrAddr << std::endl;
	}

	_status = viClose(_defaultRm);
	fflush(stdin);
}

int JcGetIDN(unsigned long vi, OUT char* cIdn) {
	int iDeviceIDN = -1;
	unsigned char buf[1024] = { 0 };
	unsigned long retCount = 0;
	//int s = viQueryf(viSession, "*IDN?\n", "%#t", &retCount, buf);
	int s = viPrintf(vi, const_cast<char*>("*IDN?\n"));
	s = viRead(vi, buf, 1024, &retCount);
	
	if (retCount > 0) {
		if (NULL != cIdn)
			memcpy(cIdn, buf, retCount);
		std::string strIdn((char*)buf);
		//功率计
		if      (Util::strFind(strIdn, "U2000A")  || Util::strFind(strIdn, "U2001")  || 
				 Util::strFind(strIdn, "U2002A")  || Util::strFind(strIdn, "U2004"))
			iDeviceIDN = INSTR_AG_U2000_SERIES;
		else if (Util::strFind(strIdn, "NRT"))
			iDeviceIDN = INSTR_RS_NRT_SERIES;
		else if (Util::strFind(strIdn, "NRPZ"))
			iDeviceIDN = INSTR_RS_NRPZ_SERIES;
		//信号源
		else if (Util::strFind(strIdn, "N5171")  || Util::strFind(strIdn, "N5172")  || 
				 Util::strFind(strIdn, "N5181")  || Util::strFind(strIdn, "N5182")  || 
				 Util::strFind(strIdn, "N5183")  ||
				 Util::strFind(strIdn, "E4436")  || Util::strFind(strIdn, "E4437")  ||
				 Util::strFind(strIdn, "E4438")  || Util::strFind(strIdn, "E4433")  ||
				 Util::strFind(strIdn, "E4434")  || Util::strFind(strIdn, "E4435")  ||
				 Util::strFind(strIdn, "ESG"))
			iDeviceIDN = INSTR_AG_MXG_SERIES;
		else if (Util::strFind(strIdn, "SMA") || Util::strFind(strIdn, "SMB") ||
			Util::strFind(strIdn, "SMC") || Util::strFind(strIdn, "SMU"))
			iDeviceIDN = INSTR_RS_SM_SERIES;
		else if (Util::strFind(strIdn, "TSG4"))
			iDeviceIDN = INSTR_TEK_TSG;
		else if (Util::strFind(strIdn, "JCSIG"))
			iDeviceIDN = INSTR_JCSIG;
		//频谱仪
		else if (Util::strFind(strIdn, "N9000")  || Util::strFind(strIdn, "N9010")  || 
				 Util::strFind(strIdn, "N9020")  || Util::strFind(strIdn, "N9030")  || 
				 Util::strFind(strIdn, "N9038") || Util::strFind(strIdn, "E4443") ||
				 Util::strFind(strIdn, "E4445") || Util::strFind(strIdn, "E4440") ||
				 Util::strFind(strIdn, "E4407"))
			iDeviceIDN = INSTR_AG_MXA_SERIES;
		else if (Util::strFind(strIdn, "FSP")     || Util::strFind(strIdn, "FSU")     || 
				 Util::strFind(strIdn, "FSV")     || Util::strFind(strIdn, "FSC"))
			iDeviceIDN = INSTR_RS_FS_SERIES;
		else if (Util::strFind(strIdn, "JCSPE"))
			iDeviceIDN = INSTR_JCSPE;
		//NONE
		else
			Util::logged(L"JcGetIDN: The current device can not support! (%s)", Util::utf8_to_wstring(strIdn).c_str());
	}
	return iDeviceIDN;
}

int JcGetDllVersion(int &major, int &minor, int &build, int &revision) {
	
	DWORD   verBufferSize;
	char    verBuffer[2048];

	std::wstring wPath = _startPath + L"\\JcPimMultiBandV2.dll";
	verBufferSize = GetFileVersionInfoSizeW(wPath.c_str(), NULL);
	if (verBufferSize > 0 && verBufferSize <= sizeof(verBuffer)) {
		if (TRUE == GetFileVersionInfoW(wPath.c_str(), 0, verBufferSize, verBuffer)) {
			UINT length;
			VS_FIXEDFILEINFO *verInfo = NULL;

			if (TRUE == VerQueryValue(
				verBuffer,
				TEXT("\\"),
				reinterpret_cast<LPVOID*>(&verInfo),
				&length)) {
				major = HIWORD(verInfo->dwProductVersionMS);
				minor = LOWORD(verInfo->dwProductVersionMS);
				build = HIWORD(verInfo->dwProductVersionLS);
				revision = LOWORD(verInfo->dwProductVersionLS);
				return true;
			}
		}
	}

	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////
//VNA 测试 ！(请无视)
//////////////////////////////////////////////////////////////////////////////////////////////

void testcb(Callback_Get_RX_Offset_Point pHandler) {
	double d = 1;
	double f = 930;

	for (int i = 0; i < 5; ++i) {
		d += i;
		f++;
		Util::setSleep(100);
		if (pHandler != 0)
			pHandler(f, d);
	}
}

int gettestval(int a, int b) {
	double d = 637;
	return a + b;
}
