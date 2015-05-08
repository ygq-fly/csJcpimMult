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
	hinst = LoadLibraryW(L"libJcPimMultiBandV2.dll");

	if (NULL == hinst){
		std::cout << "LoadLibrary Error!" << std::endl;
		return;
	}

	pTest test = (pTest)GetProcAddress(hinst, "gettestval");
    ptestcb testcb = (ptestcb)GetProcAddress(hinst, "testcb");

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
	pJcGetSig_ExtRefStatus jcGetSig_ExtRefStatus = (pJcGetSig_ExtRefStatus)GetProcAddress(hinst, "JcGetSig_ExtRefStatus");

	pGetError getError = (pGetError)GetProcAddress(hinst, "JcGetError");
	pGetOffsetRx getOffsetRx = (pGetOffsetRx)GetProcAddress(hinst, "JcGetOffsetRx");
	pGetOffsetRxNum getOffsetRxNum = (pGetOffsetRxNum)GetProcAddress(hinst, "JcGetOffsetRxNum");
	pGetOffsetTx getOffsetTx = (pGetOffsetTx)GetProcAddress(hinst, "JcGetOffsetTx");
	pGetOffsetTxNum getOffsetTxNum = (pGetOffsetTxNum)GetProcAddress(hinst, "JcGetOffsetTxNum");
	pGetOffsetVco getOffsetVco = (pGetOffsetVco)GetProcAddress(hinst, "JcGetOffsetVco");
	pSetOffsetVco setOffsetVco = (pSetOffsetVco)GetProcAddress(hinst, "JcSetOffsetVco");

	pGetDllVersion getDllVersion = (pGetDllVersion)GetProcAddress(hinst, "JcGetDllVersion");

	int a = test(3, 5);
    printf("a = %d\n", a);
    testcb([](double offset_freq, double Offset_val){
        printf("freq = %lf ; val = %lf\n", offset_freq, Offset_val);
    });

	int v1, v2, v3, v4;
	getDllVersion(v1, v2, v3, v4);
    printf("%d, %d, %d, %d\n", v1,v2,v3,v4);
	
	//TCPIP0::192.168.1.3::5025::SOCKET
	std::string addr_sig1 = "TCPIP0::192.168.1.3::5025::SOCKET";
	//TCPIP0::192.168.1.4::5025::SOCKET
	std::string addr_sig2 = "TCPIP0::192.168.1.4::5025::SOCKET";
	//std::string addr_ana = "TCPIP0::192.168.1.5::inst0::INSTR";
	//GPIB0::20::INSTR
	std::string addr_ana = "GPIB0::20::INSTR";
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

	//���
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


