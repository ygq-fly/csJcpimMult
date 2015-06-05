#include "JcApi.h"
#include "stdafx.h"
#include "JcPimObject.h"
#include "MyUtil/JcCommonAPI.h"

#define __pobj JcPimObject::Instance()

#define OFFSET_PROTECT_TX -10
#define OFFSET_PROTECT_RX -90
#define SIGNAL_SOURCE_MAX_POW 8

#define OFFSET_TX_THREASOLD 0.05

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

//频谱仪标识
#define INSTR_AG_MXA_SERIES 20
#define INSTR_RS_FS_SERIES 21

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//初始化与释放
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int fnSetInit(const JC_ADDRESS cDeviceAddr) {
	bool isSqlConn = false;
	//加载pim对象
	if (NULL != __pobj) {

		//建立模块
		//__pobj->vna = std::make_shared<ClsVnaAgE5062A>();

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
#ifdef WIN32
		//std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		//std::string sPath = conv.to_bytes(_startPath + L"\\JcOffset.db");
		std::string sPath = Util::wstring_to_utf8(_startPath + L"\\JcOffset.db");
		isSqlConn = __pobj->offset.Dbconnect(sPath.c_str());
		if (!isSqlConn) {
			Util::logged(L"fnSetInit: DB Connected Error!");
			//return JC_STATUS_ERROR_DATABASE_CONN_FAIL;
		}
#else
		//isconn = __pobj->offset.Dbconnect("D:\\Sync_ProJects\\Jointcom\\JcPimMultiBandV2\\Debug\\JcOffset.db");
#endif

		//开始连接
		std::string strConnMsg = "";
		ViStatus s = viOpenDefaultRM(&__pobj->viDefaultRM);
		if (s) return false;

		if (__pobj->vaddr.size() < 5) return JC_STATUS_ERROR;

		if ("0" != __pobj->vaddr[0]){
			if (false == JcConnSig(0, const_cast<char*>(__pobj->vaddr[0].c_str())))
				Util::logged(L"fnSetInit: Connect SG1 Fail! (%s)", Util::utf8_to_wstring(__pobj->vaddr[0]).c_str());
		}

		if ("0" != __pobj->vaddr[1]){
			if (false == JcConnSig(1, const_cast<char*>(__pobj->vaddr[1].c_str())))
				Util::logged(L"fnSetInit: Connect SG2 Fail! (%s)", Util::utf8_to_wstring(__pobj->vaddr[1]).c_str());
		}

		if ("0" != __pobj->vaddr[3]) {
			if (false == JcConnSen(const_cast<char*>(__pobj->vaddr[3].c_str())))
				Util::logged(L"fnSetInit: Connect PowerMeter Fail! (%s)", Util::utf8_to_wstring(__pobj->vaddr[3]).c_str());
		}

		if ("0" != __pobj->vaddr[2]) {
			if (false == JcConnAna(const_cast<char*>(__pobj->vaddr[2].c_str())))
				Util::logged(L"fnSetInit: Connect SA Fail! (%s)", Util::utf8_to_wstring(__pobj->vaddr[2]).c_str());
		}

		if ("0" != __pobj->vaddr[4]) {
			if (false == JcConnSwh())
				//strConnMsg = __pobj->swh->SwitchGetInfo();
				strConnMsg = "Switch Init: LoadMap Error!";
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
		HwSetIsExtBand(TRUE);

		if (false == __pobj->isAllConn)
			return JC_STATUS_ERROR;
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//华为API
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

int fnSetMeasBand(JcInt8 byBandIndex){
	if (__pobj->isUseExtBand) {
		//__pobj->now_band = __pobj->GetExtBandToIntBand(byBandIndex);
		rf1->band = __pobj->GetExtBandToIntBand(byBandIndex);
		rf2->band = __pobj->GetExtBandToIntBand(byBandIndex);
		pim->band = __pobj->GetExtBandToIntBand(byBandIndex);
	}
	else {
		//__pobj->now_band = byBandIndex;
		rf1->band = byBandIndex;
		rf2->band = byBandIndex;
		pim->band = byBandIndex;
	}

	if (pim->band == 1)
		pim->is_high_pim = false;
	return 0;
}

//请先设置 HwSetMeasBand
int fnSetDutPort(JcInt8 byPort) {
	rf1->dd = 0;
	rf2->dd = 0;
	//__pobj->now_dut_port = byPort;
	rf1->dutport = byPort;
	rf2->dutport = byPort;
	pim->dutport = byPort;

	//Band转换开关参数 , byPort = JC_DUTPORT_A 或　JC_DUTPORT_B
	//int iSwitch = __pobj->now_band * 2 + byPort;
	rf1->switch_port = rf1->band * 2 + rf1->dutport;
	rf2->switch_port = rf2->band * 2 + rf2->dutport;
	pim->switch_port = pim->band * 2 + pim->dutport;

	JcBool b = JcSetSwitch(rf1->switch_port, rf2->switch_port, pim->switch_port, JC_COUP_TX2);
	//if (TRUE == b)
	//	return 0;
	//else
	//	return JC_STATUS_ERROR_SET_SWITCH_FAIL;
	return (b == TRUE ? 0 : JC_STATUS_ERROR_SET_SWITCH_FAIL);
}

int fnSetImAvg(JcInt8 byAvgTime) {
	if (byAvgTime < 1) byAvgTime = 1;
	pim->imAvg = byAvgTime;
	//设置频谱仪平均
	__pobj->ana->InstrSetAvg(byAvgTime);
	return 0;
}

int fnSetImOrder(JcInt8 byImOrder) {
	//设置当前测试互调阶数,默认3
	pim->order = byImOrder > 1 ? byImOrder : 3;

	//return JC_STATUS_ERROR_SET_IM_FAIL;
	return 0;
}

int fnCheckReceiveChannel(JcInt8 byBandIndex, JcInt8 byPort) {
	if (__pobj->isUseExtBand){
		JcInt8 byTemp = __pobj->GetExtBandToIntBand(byBandIndex);
		if (__pobj->now_vco_enable[byTemp] == 0)
			return 0;
	}
	
	fnSetMeasBand(byBandIndex);
	int s = fnSetDutPort(byPort);
	if (s <= -10000){
		Util::logged(L"fnVco: Set Switch Error!");
		return JC_STATUS_ERROR_SET_SWITCH_FAIL;
	}

	Util::setSleep(500);

	//开始测量
	double real_val = 0;
	double vco_val = 0;

	if (HwGet_Vco(real_val, vco_val) == FALSE){
		Util::logged(L"fnVco: VCO Error (%lf)", real_val - vco_val);
		return JC_STATUS_ERROR_CHECK_VCO_FAIL;	
	}
	else
		return 0;
}

int fnCheckTwoSignalROSC() {
	if (JcGetSig_ExtRefStatus(JC_CARRIER_TX1) == FALSE) {
		Util::logged(L"fnCheckTwoSignalROSC: (SG1) Check Fail!");
		return JC_STATUS_ERROR_CHECK_SIG1_ROSC_FAIL;
	}

	if (JcGetSig_ExtRefStatus(JC_CARRIER_TX2) == FALSE) {
		Util::logged(L"fnCheckTwoSignalROSC: (SG2) Check Fail!");
		return JC_STATUS_ERROR_CHECK_SIG2_ROSC_FAIL;
	}
	return 0;
}

int fnSetTxPower(double dTxPower1, double dTxPower2,
	double dPowerOffset1, double dPowerOffset2) {
	//__pobj->now_txPow1 = dTxPower1;
	//__pobj->now_txPow2 = dTxPower2;
	////设置外部校准
	//__pobj->offset_txPow1 = dPowerOffset1;
	//__pobj->offset_txPow2 = dPowerOffset2;
	rf1->pow_dbm = 43;
	rf2->pow_dbm = 43;
	rf1->offset_ext = dTxPower1 + dPowerOffset1 - 43;
	rf2->offset_ext = dTxPower1 + dPowerOffset2 - 43;

	return 0;
}

//设置频率
JC_STATUS fnSetTxFreqs(double dCarrierFreq1, double dCarrierFreq2, const JC_UNIT cUnits) {
	//单位转换
	rf1->freq_khz = __pobj->TransKhz(dCarrierFreq1, cUnits);
	rf2->freq_khz = __pobj->TransKhz(dCarrierFreq2, cUnits);
	
	JC_STATUS js;
	double dd = 0;
	//---------------------------------------------------------------------------------
	//设置功放2
	js = JcSetSig_Advanced(JC_CARRIER_TX2, true, dd);
	if (js) return js;
	//设置功放1
	js = JcSetSig_Advanced(JC_CARRIER_TX1, true, dd);
	if (js) return js;
	//---------------------------------------------------------------------------------
	//开启功放
	js = fnSetTxOn(true, JC_CARRIER_TX1TX2);
	if (0 != js) return js;
	//---------------------------------------------------------------------------------
	//切换耦合器tx2开关
	JcBool b = HwSetCoup(JC_COUP_TX2);
	if (FALSE == b) {
		//关闭功放
		fnSetTxOn(false, JC_CARRIER_TX1TX2);
		return -10000;
	}
	Util::setSleep(100);
	//检测tx1功率平稳度
	js = HwGetSig_Smooth(dd, JC_CARRIER_TX2);
	if (js <= -10000) {
		//关闭功放
		fnSetTxOn(false, JC_CARRIER_TX1TX2);
		if (js == JC_STATUS_ERROR_NO_FIND_POWER)
			Util::logged(_T("错误： TX2未检测到功率！请检功率输出！"));
		else
			Util::logged(_T("错误： TX2功率偏差过大！"));
		return js;
	}
	//---------------------------------------------------------------------------------
	//切换耦合器tx1开关
	b = HwSetCoup(JC_COUP_TX1);
	if (FALSE == b) {
		//关闭功放
		fnSetTxOn(false, JC_CARRIER_TX1TX2);
		return -10000;
	}
	Util::setSleep(100);
	//检测tx2功率平稳度
	js = HwGetSig_Smooth(dd, JC_CARRIER_TX1);
	if (js <= -10000) {
		//关闭功放
		fnSetTxOn(false, JC_CARRIER_TX1TX2);
		if (js == JC_STATUS_ERROR_NO_FIND_POWER)
			Util::logged(_T("错误： TX1未检测到功率！请检功率输出！"));
            //Util::logged("Tx1_Error: Can not find Power!");
		else
			Util::logged(_T("错误： TX1功率偏差过大！"));
            //Util::logged("Tx1_Error: Power out range!");
		return js;
	}
	//---------------------------------------------------------------------------------
	//关闭功放
	js = fnSetTxOn(false, JC_CARRIER_TX1TX2);
	if (0 != js) return js;
	//---------------------------------------------------------------------------------
	//设置中心频率
	double freq_pim_khz = __pobj->GetPimFreq();
	__pobj->ana->InstrSetCenterFreq(freq_pim_khz);

	return 0;
}

//开启功放
int fnSetTxOn(JcBool bOn, JcInt8 byCarrier){
	bool isSucc = false;
	bool isOn = bOn == 0 ? false : true;
	if (byCarrier == JC_CARRIER_TX1TX2){
		isSucc = __pobj->sig1->InstrOpenPow(isOn);
		isSucc &= __pobj->sig2->InstrOpenPow(isOn);
	}
	else if (byCarrier == JC_CARRIER_TX1)
		isSucc = __pobj->sig1->InstrOpenPow(isOn);
	else if (byCarrier == JC_CARRIER_TX2)
		isSucc = __pobj->sig2->InstrOpenPow(isOn);

	return (isSucc ? 0 : JC_STATUS_ERROR_SET_TX_ONOFF_FAIL);
}

int fnGetImResult(JC_RETURN_VALUE dFreq, JC_RETURN_VALUE dPimResult, const JC_UNIT cUnits) {
	pim->freq_khz = __pobj->GetPimFreq();
	//获取内部校准
	double rxoff;
	JC_STATUS s = JcGetOffsetRx(rxoff, pim->band, pim->dutport, pim->freq_khz / 1000);
	if (s) rxoff = 0;
	//获取互调
	//dPimResult = __pobj->ana->InstrGetAnalyzer(dFreq, false);
	JcSetAna_RefLevelOffset(rxoff);
	dPimResult = JcGetAna(pim->freq_khz, false);
	//返回数据
	//dPimResult += rxoff;
	dFreq = __pobj->TransToUnit(pim->freq_khz, cUnits);
	if (dPimResult == JC_STATUS_ERROR){
		Util::logged(L"fnGetImResult: Spectrum Read Error!");
		__pobj->strErrorInfo = "Spectrum read error!\r\n";
		return JC_STATUS_ERROR_READ_SPECTRUM_FAIL;
	}
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

int fnSendCmd(JcInt8 byDevice, const JC_CMD cmd, char* cResult, long& lCount) {
	bool b = 0;
	int num = 0;
	std::string scmd(cmd);
	int n = scmd.find('?');
	switch (byDevice)
	{
	case 0://SIG1
		if (n > 0)
			num = __pobj->sig1->InstrWriteAndRead(cmd, cResult);
		else
			b = __pobj->sig1->InstrWrite(cmd);
		break;
	case 1://SIG2
		if (n > 0)
			num = __pobj->sig2->InstrWriteAndRead(cmd, cResult);
		else
			b = __pobj->sig2->InstrWrite(cmd);
		break;
	case 2://SA
		if (n > 0)
			num = __pobj->sen->InstrWriteAndRead(cmd, cResult);
		else
			b = __pobj->sen->InstrWrite(cmd);
		break;
	case 3://PM
		if (n > 0)
			num = __pobj->ana->InstrWriteAndRead(cmd, cResult);
		else
			b = __pobj->ana->InstrWrite(cmd);
		break;
	default:
		break;
	}

	return 0;
}

int fnGetSpectrumType(char* cSpectrumType) {
	long num =  __pobj->ana->InstrWriteAndRead("*IDN?\n", cSpectrumType);
	if (num > 0)
		return 0;
	else
		return JC_STATUS_ERROR_READ_SPECTRUM_IDN_FAIL;
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

void HwSetExit(){
	JcPimObject::release();
}

//vco检测
JcBool FnGet_Vco() {
	//开始测量
	double real_val = 0;
	double vco_val = 0;
	return HwGet_Vco(real_val, vco_val);

}

void HwSetIsExtBand(JcBool isUse) {
	if (isUse == FALSE) {
		__pobj->isUseExtBand = false;
		__pobj->wstrLogFlag = L"MBP";
	}
	else {
		__pobj->isUseExtBand = true;
		__pobj->wstrLogFlag = L"ATE";
	}
}

//设置当前功放的耦合器
JcBool HwSetCoup(JcInt8 byCoup) {
	//int iSwitch = __pobj->now_band * 2 + __pobj->now_dut_port;
	JcBool r = JcSetSwitch(rf1->switch_port, rf2->switch_port, pim->switch_port, byCoup);
	Util::setSleep(250);
	if (FALSE == r) 
		Util::logged(L"HwSetCoup: Switch-Coup-%d Error!", (int)byCoup);		
	
	return r;
}

//读取当前功放功率值(tx1或tx2)
double HwGetCoup_Dsp(JcInt8 byCoup) {
	double val = 0;
	double tx_temp = 0;
	if (byCoup == JC_COUP_TX1) {
		//所有校准数据以mhz为单位，注意转换
		int s = JcGetOffsetTx(val, rf1->band, rf1->dutport,
							  byCoup, OFFSET_DSP,
							  rf1->freq_khz / 1000, rf1->pow_dbm);
		if (s) return s;
		tx_temp = rf1->pow_dbm + rf1->offset_ext;
	}
	else if (byCoup == JC_COUP_TX2) {
		//所有校准数据以mhz为单位，注意转换
		int s = JcGetOffsetTx(val, rf2->band, rf2->dutport,
							  byCoup, OFFSET_DSP,
							  rf2->freq_khz / 1000, rf2->pow_dbm);
		if (s) return s;
		tx_temp = rf2->pow_dbm + rf2->offset_ext;
	}
	//读取功率计
	double sen = JcGetSen();
	if (Util::strFind(__pobj->sen->InstrGetIdn(), "nrpz")) {
		sen += val;
	}
	else {
		sen += JcGetSen();
		sen += JcGetSen();
		//计算补偿
		sen = sen / 3 + val;
	}

	std::string strLog = "start Dsp-Coup-" + std::to_string(byCoup) + "\r\n";
	strLog += "   Avg3rd_1: " + std::to_string(sen) + " \r\n";

	double dd = tx_temp - sen;
	if (dd > __pobj->now_tx_smooth_threasold || dd < (__pobj->now_tx_smooth_threasold * -1)) {
		Util::setSleep(100);
		//读2次后平均
		sen = JcGetSen();
		sen += JcGetSen();
		sen += JcGetSen();
		sen = sen / 3 + val;
		strLog += "   Avg3rd_2: " + std::to_string(sen) + " \r\n";
		JcPimObject::Instance()->LoggingWrite(strLog.c_str());
	}
	return sen;
}

JcBool HwGet_Vco(double& real_val, double& vco_val) {
	//获取实际值
	JcBool b = JcGetVcoDsp(real_val, pim->switch_port);
	//获取校准值
	JcGetOffsetVco(vco_val, pim->band, pim->dutport);
	double dd = real_val - vco_val;

	b = (dd > (__pobj->now_vco_threasold * -1) && dd < __pobj->now_vco_threasold);
	//if (!b) 
	//	Util::logged(L"HwVco: VCO Error (%lf)", dd);
	return b;
}

//检测功放稳定度(必须功放开启后检测) return dd
JC_STATUS HwGetSig_Smooth(JC_RETURN_VALUE dd, JcInt8 byCarrier){
	double tx_dsp = 0;
	dd = 0;
	double tx_deviate = 0;
	std::string strLog = "start smooth-tx-" + std::to_string(byCarrier) + "\r\n";

	for (int i = 0; i < 4; i++){
		if (i == 0)
			Util::setSleep(100);
		if (byCarrier == JC_CARRIER_TX1) {
			//获取检测功率
			tx_dsp = HwGetCoup_Dsp(JC_COUP_TX1);
			//获取偏差值
			tx_deviate = rf1->pow_dbm + rf1->offset_ext - tx_dsp;	
		}
		else if (byCarrier == JC_CARRIER_TX2) {
			tx_dsp = HwGetCoup_Dsp(JC_COUP_TX2);
			tx_deviate = rf2->pow_dbm + rf2->offset_ext - tx_dsp;
		}
		else
			return JC_STATUS_ERROR_SET_BOSH_USE_TX1TX2;

		strLog += "   dd: " + std::to_string(dd) + "\r\n";
		strLog += "   No.: " + std::to_string(i) + "\r\n";
		strLog += "   tx_dsp: " + std::to_string(tx_dsp) + "\r\n";
		strLog += "   tx_deviate: " + std::to_string(tx_deviate) + "\r\n";

		//未检测功率时
		if (tx_dsp <= 33){
			__pobj->strErrorInfo = "   PowerSmooth: No find Power!\r\n";
			strLog += __pobj->strErrorInfo;
			JcPimObject::Instance()->LoggingWrite(strLog.c_str());
			return JC_STATUS_ERROR_NO_FIND_POWER;
		}

		if (tx_deviate > __pobj->now_tx_smooth_threasold || tx_deviate < (__pobj->now_tx_smooth_threasold * -1)) {
			rf1->dd = 0;
			rf2->dd = 0;
			//检测错误后，关闭功放
			//FnSetTxOn(false, byCarrier);		
			__pobj->strErrorInfo = "   PowerSmooth: Power's Smooth out Allowable Range\r\n";
			strLog += __pobj->strErrorInfo;
			JcPimObject::Instance()->LoggingWrite(strLog.c_str());
			return JC_STATUS_ERROR_SET_TX_OUT_SMOOTH;
		}
		else {
			if (tx_deviate >= (__pobj->now_tx_smooth_accuracy * -1) && tx_deviate <= __pobj->now_tx_smooth_accuracy)
				return JC_STATUS_SUCCESS;

			if (i == 0)
				dd += tx_deviate * 0.9;
			else
				dd += (tx_deviate * 0.6);

			double sig_val = 0;
			if (byCarrier == JC_CARRIER_TX1) {
				rf1->dd = dd;
				sig_val = rf1->pow_dbm + rf1->offset_ext + rf1->offset_int + dd;
				JcBool b = JcSetSig(JC_CARRIER_TX1, rf1->pow_dbm, sig_val);
				if (FALSE == b)
					return -10000;
			}
			else if (byCarrier == JC_CARRIER_TX2) {
				rf2->dd = dd;
				sig_val = rf2->pow_dbm + rf2->offset_ext + rf2->offset_int + dd;
				JcBool b = JcSetSig(JC_CARRIER_TX2, rf2->pow_dbm, sig_val);
				if (FALSE == b)
					return -10000;
			}
			strLog += "   sig_val: " + std::to_string(sig_val) + "\r\n";
			Util::setSleep(200);
		}
	}
	return JC_STATUS_SUCCESS;
}

//设置频率
JC_STATUS HwSetTxFreqs(double dCarrierFreq1, double dCarrierFreq2, const JC_UNIT cUnits) {
	//单位转换
	rf1->freq_khz = __pobj->TransKhz(dCarrierFreq1, cUnits);
	rf2->freq_khz = __pobj->TransKhz(dCarrierFreq2, cUnits);

	//设置功放
	JC_STATUS js;
	js = JcSetSig_Advanced(JC_CARRIER_TX1, true, rf1->dd);
	if (js) return js;
	js = JcSetSig_Advanced(JC_CARRIER_TX2, true, rf2->dd);
	if (js) return js;
	//设置中心频率
	pim->freq_khz = __pobj->GetPimFreq();
	__pobj->ana->InstrSetCenterFreq(pim->freq_khz);

	return JC_STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//扩展API
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


JcBool JcConnSig(JcInt8 byDevice, JC_ADDRESS cAddr) {
	ViSession vi = VI_NULL;
	ViStatus s = viOpen(__pobj->viDefaultRM, cAddr, VI_NULL, VI_NULL, &vi);
	if (s == VI_SUCCESS) {
		JcPimObject::Instance()->JcSetViAttribute(vi);
		char cIdn[128] = { 0 };
		int index = JcGetIDN(vi, cIdn);
		//安捷伦
		if (index == INSTR_AG_MXG_SERIES){
			if (byDevice == JC_DEVICE::SIGNAL1) {
				__pobj->sig1 = std::make_shared<ClsSigAgN5181A>();
				__pobj->sig1->InstrSession(vi, cIdn);
			}
			else if (byDevice == JC_DEVICE::SIGNAL2) {
				__pobj->sig2 = std::make_shared<ClsSigAgN5181A>();
				__pobj->sig2->InstrSession(vi, cIdn);
			}
		}
		//罗德斯瓦茨
		else if (index == INSTR_RS_SM_SERIES) {
			if (byDevice == JC_DEVICE::SIGNAL1) {
				__pobj->sig1 = std::make_shared<ClsSigRsSMxSerial>();
				__pobj->sig1->InstrSession(vi, cIdn);
			}
			else if (byDevice == JC_DEVICE::SIGNAL2) {
				__pobj->sig2 = std::make_shared<ClsSigRsSMxSerial>();
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
		//安捷伦
		if (index == INSTR_AG_MXA_SERIES) {
			__pobj->ana = std::make_shared<ClsAnaAgN9020A>();
			__pobj->ana->InstrSession(vi, cIdn);
		}
		//罗德斯瓦茨
		else if (index == INSTR_RS_FS_SERIES) {
			__pobj->ana = std::make_shared<ClsAnaRsFspSerial>();
			__pobj->ana->InstrSession(vi, cIdn);
		}
		else
			return FALSE;
	}

	__pobj->device_status[JC_DEVICE::ANALYZER] = !s;
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
	}
	//RS仪表连接
	else {
		//int index = INSTR_RS_NRPZ_SERIES;
		__pobj->sen = std::make_shared<ClsSenRsNrpz>();
		isConn = __pobj->sen->InstrConnect(cAddr);
	}

	__pobj->device_status[JC_DEVICE::SENSOR] = isConn;
	return isConn;
}

JcBool JcConnSwh() {
	__pobj->swh = std::make_shared<ClsJcSwitch>();
	bool isConn = false;
	if (__pobj->swh->SwitchInit()){
		//for (int i = 0; i < 7; ++i) {
		//	if (_switch_enable[i] == 0)
		//		__pobj->swh->SwitchSetEnable(i, false);
		//	else
		//		__pobj->swh->SwitchSetEnable(i, true);
		//}

		//开始连接
		isConn = __pobj->swh->SwitchConnect();
	}
	
	__pobj->device_status[4] = isConn;
	return isConn;
}

JcBool JcGetVcoDsp(JC_RETURN_VALUE vco, JcInt8 bySwitchBand) {
	if (NULL == __pobj) return false;

	double vco_freq_mhz = 1334 + 2 * bySwitchBand;
	//__pobj->ana->InstrSetCenterFreq(vco_freq_mhz * 1000);
	__pobj->ana->InstrVcoSetting();
	Util::setSleep(100);
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

//设置功放参数 (打开关闭功放请使用HwSetTxOn())
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
		Util::logged(L"JcSetSig: sig(%d) Error!", (int)byCarrier);

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
		bool b = __pobj->sig1->InstrSetFreqPow(freq_khz, tx_true);
		if (false == b) {
			Util::logged(L"JcSetSig: (%d)Error!", (int)byCarrier);
			return -10000;
		}
	}
	else if (byCarrier == JC_CARRIER_TX2){
		rf2->offset_int = internal_offset;
		bool b = __pobj->sig2->InstrSetFreqPow(freq_khz, tx_true);
		if (false == b) {
			Util::logged(L"JcSetSig: (%d)Error!", (int)byCarrier);
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
JcBool JcSetSwitch(int iSwitchTx1, int iSwitchTx2,
				  int iSwitchPim, int iSwitchCoup) {
	if (NULL == __pobj) {
		__pobj->strErrorInfo = "object: Not init!\r\n";
		return false; 
	}
	if (false == __pobj->device_status[4]) {
		__pobj->strErrorInfo = "Switch: All not connected\r\n";
		return false;
	}

	int coup = 0;
	if (iSwitchTx1 % 2 == 0)
		coup = iSwitchTx1 + iSwitchCoup;
	else
		coup = (iSwitchTx1 - 1) + iSwitchCoup;
	bool isSucc = __pobj->swh->SwitchExcut(iSwitchTx1, iSwitchTx2, iSwitchPim, coup);
	if (!isSucc) __pobj->strErrorInfo = "Switch: Excut Error!\r\n";

	return isSucc;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Rx 校准API
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//获取Rx校准
JC_STATUS JcGetOffsetRx(JC_RETURN_VALUE offset_val,
						JcInt8 byInternalBand, JcInt8 byDutPort,
						double freq_mhz){
	//获取当前频段显示字符
	std::string sband = __pobj->GetBandString(byInternalBand);
	//计算校准数据点
	offset_val = __pobj->offset.OffsetRx(sband.c_str(), byDutPort, freq_mhz);
	if (offset_val == JC_STATUS_ERROR) {

		__pobj->strErrorInfo = "RxOffset: Read error!\r\n";
		return JC_STATUS_ERROR_GET_RX_OFFSET;
	}
	else 
		return JC_STATUS_SUCCESS;
}

long JcGetOffsetRxNum(JcInt8 byInternalBand){
	//获取当前频段显示字符
	std::string sband = __pobj->GetBandString(byInternalBand);
	//获取Rx校准点
	double Rxfreq[256] = { 0 };
	int freq_num = __pobj->offset.FreqHeader(OFFSET_RX, sband.c_str(), Rxfreq, 256);
	return freq_num;
}

//自动校准接收Rx (校准前请确认连线)，（只使用JC_TX1来校准）
JC_STATUS JcSetOffsetRx(JcInt8 byInternalBand, JcInt8 byDutPort,
						double loss_db, Callback_Get_RX_Offset_Point pHandler) {

	//获取当前频段显示字符
	std::string sband = __pobj->GetBandString(byInternalBand);
	//获取Rx校准点
	double Rxfreq[256] = {0};
	int freq_num = __pobj->offset.FreqHeader(OFFSET_RX, sband.c_str(), Rxfreq, 256);
	double off[256] = {0};
	__pobj->ana->InstrRxOffsetSetting();

	std::string strLog = "start offset-rx-" +
		std::to_string(byInternalBand) + "-" +
		std::to_string(byDutPort) + "\r\n";

	//设置保护值
	JcSetSig(JC_CARRIER_TX1, Rxfreq[0] * 1000, OFFSET_PROTECT_RX);
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
		JcSetSig(JC_CARRIER_TX1, Rxfreq[i] * 1000, OFFSET_PROTECT_RX);
		Util::setSleep(200);
		//读取
		double v = JcGetAna(Rxfreq[i] * 1000, false);
		if (v == JC_STATUS_ERROR){
			//关闭功放
			fnSetTxOn(false, JC_CARRIER_TX1);
			__pobj->strErrorInfo = "Spectrum read error!\r\n";
			return JC_STATUS_ERROR_READ_SPECTRUM_FAIL;
		}
		//计算规则: 目标值（OFFSET_PROTECT_RX） = 实际值（v） + 校准值 （off） +差损（loss_db）
		off[i] = OFFSET_PROTECT_RX - v - loss_db;
		if (off[i] > 10 || off[i] < -10){
			//错误，关闭功放
			fnSetTxOn(false, JC_CARRIER_TX1);
			//__pobj->ana->InstrSetAvg(2);
			__pobj->strErrorInfo = "   RxOffset: No Find Power(-90)!\r\n";
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

	JC_STATUS s = __pobj->offset.Store_v2(OFFSET_RX, sband.c_str(), byDutPort, 0, 0, 0, off, freq_num);
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
	//传输模式
	JcInt8 byTempDutPort = byDutPort;
	if ((__pobj->isUseTransType == true) && (byDutPort == JC_DUTPORT_B))
		byTempDutPort = JC_DUTPORT_A;

	//获取当前频段显示字符
	std::string sband = __pobj->GetBandString(byInternalBand);
	//计算校准数据点
	offset_val = __pobj->offset.OffsetTx(sband.c_str(), byTempDutPort, coup, real_or_dsp, freq_mhz, tx_dbm);
	if (offset_val == JC_STATUS_ERROR){
		offset_val = 0;
		__pobj->strErrorInfo = "TxOffset: Read data error!\r\n";
		return JC_STATUS_ERROR;
	}
	else 
		return JC_STATUS_SUCCESS;
}

long JcGetOffsetTxNum(JcInt8 byInternalBand) {
	//获取当前频段显示字符
	std::string sband = __pobj->GetBandString(byInternalBand);
	//获取Tx校准频点
	double txfreq[256] = { 0 };
	int freq_num = __pobj->offset.FreqHeader(OFFSET_TX, sband.c_str(), txfreq, 256);
	return freq_num;
}

//自动校准发射Tx (校准前请确认连线)，(band:当前频段，dutport:当前测试端kou), 同时校准tx1,tx2
JC_STATUS JcSetOffsetTx(JcInt8 byInternalBand, JcInt8 byDutPort,
						double des_p_dbm, double loss_db,
						Callback_Get_TX_Offset_Point pHandler) {

	if (__pobj->ext_sen_index == 1){
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
	double txfreq[256] = { 0 };
	int freq_num = __pobj->offset.FreqHeader(OFFSET_TX, sband.c_str(), txfreq, 256);

	double off_real[256] = { 0 };
	double off_dsp[256] = { 0 };

	//Band转换开关参数
	int iswitch = byInternalBand * 2 + byDutPort;
	//----------------------------------------------------------------------------------------------
	//coup ==> JC_COUP_TX1 to JC_COUP_TX2
	for (int coup = 0; coup < 2; ++coup)
	{
#ifdef JC_OFFSET_TX_DEBUG
		std::cout << "\n";
		std::cout << "Start: " << coup << std::endl;
#endif
		//----------------------------------------------------------------------------------------------
		//切换开关
		JcBool isSwhConn = JcSetSwitch(iswitch, iswitch, iswitch, coup);
		if (isSwhConn == FALSE) {
			__pobj->strErrorInfo = "TxOffset: Switch-Coup Fail!\r\n";
			return JC_STATUS_ERROR;
		}
		//----------------------------------------------------------------------------------------------
		//开启功放,设置保护值
		std::shared_ptr<IfSignalSource> pow = coup == JC_COUP_TX1 ? __pobj->sig1 : __pobj->sig2;
		pow->InstrSetFreqPow(txfreq[0] * 1000, OFFSET_PROTECT_TX);
		pow->InstrOpenPow(true);
		//----------------------------------------------------------------------------------------------
		Util::setSleep(400);
		//开始 自动生成 TX1 校准数据
		for (int i = 0; i < freq_num; ++i) {
			//单点校准
			int s = JcSetOffsetTx_Single(off_real[i], off_dsp[i], coup, des_p_dbm, txfreq[i], loss_db);
			if (s) return s;
			//开始回调
			if(pHandler)
				pHandler(txfreq[i], off_real[i], off_dsp[i]);
#ifdef JC_OFFSET_TX_DEBUG
			std::cout << "Run: " << txfreq[i] << " MHz" << std::endl;
			std::cout << "Off = " << off_real[i] << " ; Sen= " << off_dsp[i] << std::endl;
#endif
		}
		//----------------------------------------------------------------------------------------------
		//关闭功放
		pow->InstrOpenPow(false);
		//----------------------------------------------------------------------------------------------
		//存储校准数据
		int s = __pobj->offset.Store_v2(OFFSET_TX, sband.c_str(), byDutPort, coup, JC_OFFSET_REAL, des_p_dbm, off_real, freq_num);
		if (s) {
			__pobj->strErrorInfo = "TxOffset: Tx1's data save error!\r\n";
			return JC_STATUS_ERROR;
		}
		s = __pobj->offset.Store_v2(OFFSET_TX, sband.c_str(), byDutPort, coup, JC_OFFSET_DSP, des_p_dbm, off_dsp, freq_num);
		if (s) {
			__pobj->strErrorInfo = "TxOffset: Tx2's data save error!\r\n";
			return JC_STATUS_ERROR;
		}
		//----------------------------------------------------------------------------------------------
#ifdef JC_OFFSET_TX_DEBUG
		std::cout << "Save Success!" << std::endl;
#endif
	}

	//还原频谱设置
	if (__pobj->ext_sen_index == 0)
		__pobj->ana->InstrPimSetting();
	
	return JC_STATUS_SUCCESS;
}

JC_STATUS JcSetOffsetTx_Single(JC_RETURN_VALUE resulte,
							   JC_RETURN_VALUE resulte_sen,
							   int coup, 
							   double des_p_dbm, 
							   double des_f_mhz, 
							   double loss_db) {
	std::shared_ptr<IfSignalSource> pow = coup == JC_COUP_TX1 ? __pobj->sig1 : __pobj->sig2;
	//设置保护值
	pow->InstrSetFreqPow(des_f_mhz * 1000, OFFSET_PROTECT_TX);
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

	double p_true = OFFSET_PROTECT_TX;
	resulte = 0;
	std::string strLog = "start offset-tx\r\n";

	for (int i = 0; i < 8; i++) {
		//设置
		pow->InstrSetFreqPow(des_f_mhz * 1000, p_true);
		Util::setSleep(100);
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
            if (v <= -50) {
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
		if (v <= -50) {
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

	Util::setSleep(500);
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

JC_STATUS JcSetOffsetVco(JcInt8 byInternalBand, JcInt8 byDutport, double val) {
	std::string sband = __pobj->GetBandString(byInternalBand);

	int s = __pobj->offset.Store_vco_single(sband.c_str(), byDutport, val);
	if (s){
		__pobj->strErrorInfo = "VCO_Offset: VCO's data save error!\r\n";
		return JC_STATUS_ERROR;
	}
	else
		return JC_STATUS_SUCCESS;
}

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

int JcGetIDN(unsigned long vi, OUT char* cIdn) {
	int iDeviceIDN = -1;
	unsigned char buf[1024] = { 0 };
	unsigned long retCount = 0;
	//int s = viQueryf(viSession, "*IDN?\n", "%#t", &retCount, buf);
	int s = viPrintf(vi, const_cast<char*>("*IDN?\n"));
	s = viRead(vi, buf, 1024, &retCount);
	
	if (retCount) {
		if (NULL != cIdn)
			memcpy(cIdn, buf, retCount);
		std::string strIdn((char*)buf);
		if      (Util::strFind(strIdn, "U2000A")  || Util::strFind(strIdn, "U2001A")  || Util::strFind(strIdn, "U2002A"))
			iDeviceIDN = INSTR_AG_U2000_SERIES;
		else if (Util::strFind(strIdn, "NRT"))
			iDeviceIDN = INSTR_RS_NRT_SERIES;
		else if (Util::strFind(strIdn, "NRPZ"))
			iDeviceIDN = INSTR_RS_NRPZ_SERIES;
		//信号源
		else if (Util::strFind(strIdn, "N5171A")  || Util::strFind(strIdn, "N5172A")  || 
				 Util::strFind(strIdn, "N5181A")  || Util::strFind(strIdn, "N5182A")  || 
				 Util::strFind(strIdn, "N5183A"))
			iDeviceIDN = INSTR_AG_MXG_SERIES;
		else if (Util::strFind(strIdn, "SMA") || Util::strFind(strIdn, "SMB") || 
				 Util::strFind(strIdn, "SMC") || Util::strFind(strIdn, "SMU"))
			iDeviceIDN = INSTR_RS_SM_SERIES;
		//频谱仪
		else if (Util::strFind(strIdn, "N9000A")  || Util::strFind(strIdn, "N9010A")  || 
				 Util::strFind(strIdn, "N9020A")  || Util::strFind(strIdn, "N9030A")  || 
				 Util::strFind(strIdn, "N9038A"))
			iDeviceIDN = INSTR_AG_MXA_SERIES;
		else if (Util::strFind(strIdn, "FSP")     || Util::strFind(strIdn, "FSU")     || Util::strFind(strIdn, "FSV"))
			iDeviceIDN = INSTR_RS_FS_SERIES;
		else
			Util::logged(L"JcGetIDN: We Cannot Support (%s)!", Util::utf8_to_wstring(strIdn).c_str());
	}
	return iDeviceIDN;
}

//废除
int JcGetSwtichEnable(int byInternalBandIndex){
	//if (byInternalBandIndex < 0 || byInternalBandIndex > 6)
	//	return 0;
	//return _switch_enable[byInternalBandIndex];
	return 0;
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
