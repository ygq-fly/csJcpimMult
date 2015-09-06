// TestDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TestDemo.h"

void Test_dll();

//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char* argv[])
{
	std::cout << "Welcome!" << std::endl;

	for (int i = 0; i < 1; i++) {
		printf("==================No.%d=================\n", i);
		Test_dll();
	}

#ifdef _MSC_VER
	getchar();
#endif
	return 0;
}

void Test_dll(){
	HINSTANCE hinst;
#ifdef _MSC_VER
	hinst = LoadLibraryW(L"JcPimMultiBandV2.dll");
#else
	hinst = LoadLibraryW(L"libJcPimMultiBandV2-d.dll");
#endif

	if (NULL == hinst){
		std::cout << "LoadLibrary Error!" << std::endl;
		return;
	}

	pTest test = (pTest)GetProcAddress(hinst, "gettestval");
    ptestcb testcb = (ptestcb)GetProcAddress(hinst, "testcb");

	pSetInit setInit = (pSetInit)GetProcAddress(hinst, "fnSetInit");
	pHwSetIsExtBand hwSetIsExtBand = (pHwSetIsExtBand)GetProcAddress(hinst, "HwSetIsExtBand");
	pSetMeasBand setMeasBand = (pSetMeasBand)GetProcAddress(hinst, "fnSetMeasBand");
	pSetExit setExit = (pSetExit)GetProcAddress(hinst, "fnSetExit");
	pSetImAvg setImAvg = (pSetImAvg)GetProcAddress(hinst, "fnSetImAvg");
	pSetDutPort setDutPort = (pSetDutPort)GetProcAddress(hinst, "fnSetDutPort");
	pSetImOrder setImOrder = (pSetImOrder)GetProcAddress(hinst, "fnSetImOrder");
	pCheckReceiveChannel checkReceiveChannel = (pCheckReceiveChannel)GetProcAddress(hinst, "fnCheckReceiveChannel");
	pCheckTwoSignalROSC checkTwoSignalROSC = (pCheckTwoSignalROSC)GetProcAddress(hinst, "fnCheckTwoSignalROSC");
	pSetTxPower setTxPower = (pSetTxPower)GetProcAddress(hinst, "fnSetTxPower");
	pSetTxFreqs setTxFreqs = (pSetTxFreqs)GetProcAddress(hinst, "fnSetTxFreqs");
	pHwSetTxFreqs hwSetTxFreqs = (pHwSetTxFreqs)GetProcAddress(hinst, "HwSetTxFreqs");
	pSetTxOn setTxOn = (pSetTxOn)GetProcAddress(hinst, "fnSetTxOn");
	pGetImResult getImResult = (pGetImResult)GetProcAddress(hinst, "fnGetImResult");
	pGetSpectrumType getSpectrumType = (pGetSpectrumType)GetProcAddress(hinst, "fnGetSpectrumType");

	pHwSetCoup hwSetCoup = (pHwSetCoup)GetProcAddress(hinst, "HwSetCoup");
	pJcGetAna jcGetAna = (pJcGetAna)GetProcAddress(hinst, "JcGetAna");
	pJcSetSig jcSetSig = (pJcSetSig)GetProcAddress(hinst, "JcSetSig");
	pJcGetSen jcGetSen = (pJcGetSen)GetProcAddress(hinst, "JcGetSen");
	pJcSetSwitch jcSetSwitch = (pJcSetSwitch)GetProcAddress(hinst, "JcSetSwitch");
	pJcGetSig_ExtRefStatus jcGetSig_ExtRefStatus = (pJcGetSig_ExtRefStatus)GetProcAddress(hinst, "JcGetSig_ExtRefStatus");

	pGetError getError = (pGetError)GetProcAddress(hinst, "JcGetError");
	pGetOffsetRx getOffsetRx = (pGetOffsetRx)GetProcAddress(hinst, "JcGetOffsetRx");
	pGetOffsetRxNum getOffsetRxNum = (pGetOffsetRxNum)GetProcAddress(hinst, "JcGetOffsetRxNum");
	pJcGetOffsetTx jcGetOffsetTx = (pJcGetOffsetTx)GetProcAddress(hinst, "JcGetOffsetTx");
	pJcGetOffsetTxNum jcGetOffsetTxNum = (pJcGetOffsetTxNum)GetProcAddress(hinst, "JcGetOffsetTxNum");
	pJcSetOffsetTx jcSetOffsetTx = (pJcSetOffsetTx)GetProcAddress(hinst, "JcSetOffsetTx");
	pGetOffsetVco getOffsetVco = (pGetOffsetVco)GetProcAddress(hinst, "JcGetOffsetVco");
	pSetOffsetVco setOffsetVco = (pSetOffsetVco)GetProcAddress(hinst, "JcSetOffsetVco");

	pGetDllVersion getDllVersion = (pGetDllVersion)GetProcAddress(hinst, "JcGetDllVersion");
	pJcSetOffsetTxIncremental jcSetOffsetTxIncremental = (pJcSetOffsetTxIncremental)GetProcAddress(hinst, "JcSetOffsetTxIncremental");
	int a = test(3, 5);
    printf("a = %d\n", a);
    testcb([](double offset_freq, double Offset_val){
        printf("freq = %lf ; val = %lf\n", offset_freq, Offset_val);
    });

	int v1, v2, v3, v4;
	getDllVersion(v1, v2, v3, v4);
    printf("%d, %d, %d, %d\n", v1,v2,v3,v4);
	
	//TCPIP0::192.168.1.3::5025::SOCKET
	std::string addr_sig1 = "GPIB0::19::INSTR";

	//TCPIP0::192.168.1.4::5025::SOCKET
	std::string addr_sig2 = "0";

	//GPIB0::20::INSTR
	//std::string addr_ana = "GPIB0::20::INSTR";
	std::string addr_ana = "0";

	//USB::0x0aad::0x000c::102838
	//USB0::0x0957::0x2B18::MY51020008::0::INSTR
	//USB0::0x0957::0x2B18::MY51050018::0::INSTR
	std::string addr_sen = "0";
	std::string addr_swh = "0";
	std::string addr = addr_sig1 + "," + addr_sig2 + "," + addr_ana + "," + addr_sen + "," + addr_swh;
	bool isCont = true;
	int s = setInit("0,0,0,0,0");
	//int s = setInit(const_cast<char*>(addr.c_str()));
	if (s == 0 && isCont == true) {
		std::cout << "init success!" << std::endl;
	}
	else {
		char msg[512] = { 0 };
		getError(msg, 512);
		std::cout << msg << std::endl;
	}
	
	//jcSetSwitch(0, 0, 0, 1);

	hwSetIsExtBand(FALSE);
	double tx_freq, rx_freq;
	if (setMeasBand(8) < 0) {
		tx_freq = 729;
		rx_freq = 698;
		printf("This is HW_BAND!\n");
	}
	else {
		tx_freq = 926;
		rx_freq = 880;
		printf("This is POI_BAND!\n");
	}


	//setDutPort(0);
	//setTxPower(43, 43, 0.5, 0.5);
	//hwSetTxFreqs(1942, 1990, "mhz");
	//double pim_freq, pim_val;
	//getImResult(pim_freq, pim_val, "mhz");
	
	//jcSetOffsetTx(8, 0, 43, 42.5, NULL);
	//���
	std::cout << std::endl;
	for (int i = 0; i < 7; i++) {
		long n = getOffsetRxNum(i);
		std::cout << i << "-Rx-Num: " << n << std::endl;
		n = jcGetOffsetTxNum(i);
		std::cout << i << "-Tx-Num: " << n << std::endl;
	}
	double val = 0;
	s = jcSetOffsetTxIncremental(0, JC_DUTPORT_A, JC_COUP_TX1, 0, 0);
	s = jcGetOffsetTx(val, 0, JC_DUTPORT_A, JC_COUP_TX2, JC_OFFSET_REAL, tx_freq, 43);
	std::cout << "tx_offset_real = " << val << std::endl;
	s = jcGetOffsetTx(val, 0, JC_DUTPORT_A, JC_COUP_TX2, JC_OFFSET_DSP, tx_freq, 43);
	std::cout << "tx_offset_dsp = " << val << std::endl;
	s = getOffsetRx(val, 0, JC_DUTPORT_A, rx_freq);
	std::cout << "rx_offset = " << val << std::endl;
	s = getOffsetVco(val, 0, JC_DUTPORT_A);
	std::cout << "vco_offset = " << val << std::endl;

	s = setExit();
	std::cout << "Exit!" << std::endl;

	FreeLibrary(hinst);
}


