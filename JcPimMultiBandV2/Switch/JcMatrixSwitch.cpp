// MatrixSwitch.cpp : Defines the exported functions for the DLL application.
//

#include "../stdafx.h"
#include "com_io_ctl.h"
#include "JcMatrixSwitch.h"

using namespace std;
using namespace ns_com_io_ctl;

com_io_ctl *__cic = nullptr;
int __switchType = ID_HUAWEI;

int Lameda1 = []{
	
	return 1;
}();

int Lameda2 = []{

	return 1;
}();

int MartrixSwitchInit(int handle,char *dllName, int swType,int comType)
{
	int result = MATRIX_SWITCH_OK;
	com_io_ctl *cic;

	__switchType = swType;

	try
	{
		cic = new com_io_ctl();
		//选择通信方式
		cic->__conType = comType;
		//导入宿主动态库名称
		if (dllName != NULL)
		{
			cic->__dllHostName.assign(dllName);
		}

		char cfgPath[512];
		string scfgPath = cic->GetRunPath();
		scfgPath = scfgPath.substr(0, scfgPath.rfind('\\'));
		strcpy_s(cfgPath, sizeof(cfgPath), scfgPath.c_str());

		//加载配置
		if (!cic->LoadMap(swType,cfgPath))
		{
			result = MATRIX_SWITCH_ERROR_INI_FILE_NOEXIST;
		}		

		__cic = cic;

	}
	catch (exception ex)
	{
		return MATRIX_SWITCH_ERROR_INIT_EXCEPTION;
	}

	return result;
}

int MartrixSwitchOpenAccpetMask(char*hosts[], int size)
{
	if (__cic == nullptr)
	{
		return MATRIX_SWITCH_ERROR_OBJECT_NULL;
	}

	__cic->SetAccpectHosts(hosts,size);

	return MATRIX_SWITCH_OK;
}

int MartrixSwitchExcute(int addr, int swId, int swIdx)
{
	int result = MATRIX_SWITCH_OK;
	com_io_ctl *cic = __cic;

	if (cic == nullptr)
	{
		return MATRIX_SWITCH_ERROR_OBJECT_NULL;
	}

	cic->Clear();
	cic->AddSwitchActionList(addr,swId,swIdx);

	result = cic->Excute() ? MATRIX_SWITCH_OK : MATRIX_SWITCH_EXCUTE_FAILED;

	return result;
}

int MartrixSwitchBoxQClear(int addr)
{
	int result = MATRIX_SWITCH_OK;
	com_io_ctl *cic = __cic;

	if (cic == nullptr)
	{
		return MATRIX_SWITCH_ERROR_OBJECT_NULL;
	}

	cic->Clear();

	return result;
}

int MartrixSwitchBoxQAdd(int addr, int swId, int swIdx)
{
	int result = MATRIX_SWITCH_OK;
	com_io_ctl *cic = __cic;

	if (cic == nullptr)
	{
		return MATRIX_SWITCH_ERROR_OBJECT_NULL;
	}

	cic->AddSwitchActionList(addr, swId, swIdx);

	return result;
}

int MartrixSwitchBoxQExcute(int addr)
{
	int result = MATRIX_SWITCH_OK;
	com_io_ctl *cic = __cic;

	if (cic == nullptr)
	{
		return MATRIX_SWITCH_ERROR_OBJECT_NULL;
	}

	result = cic->Excute() ? MATRIX_SWITCH_OK : MATRIX_SWITCH_EXCUTE_FAILED;

	return result;
}

int MartrixSwitchBoxExcuteAD(int tx1, int tx2, int pim, int det)
{
	int result = MATRIX_SWITCH_OK;
	com_io_ctl *cic = __cic;

	if (cic == nullptr)
	{
		return MATRIX_SWITCH_ERROR_OBJECT_NULL;
	}

	//获取所有通道字符串名
	vector<string> nltx1 = cic->GetTx1NameList();
	vector<string> nltx2 = cic->GetTx2NameList();
	vector<string> nlpim = cic->GetPimNameList();
	vector<string> nldet = cic->GetDetNameList();

	cic->Clear();

	if (tx1 > ID_CHAN_IGNORE && tx1 < (int)nltx1.size())
		cic->SelChanTx1(nltx1[tx1]);
	else if (tx1 >= (int)nltx1.size() || tx1 < ID_CHAN_IGNORE)
		result = MATRIX_CHAN_IDX_INVAID_1;

	if (tx2 > ID_CHAN_IGNORE && tx2 < (int)nltx2.size())
		cic->SelChanTx2(nltx2[tx2]);
	else if (tx2 >= (int)nltx2.size() || tx2 < ID_CHAN_IGNORE)
		result = MATRIX_CHAN_IDX_INVAID_2;

	if (pim > ID_CHAN_IGNORE && pim <  (int)nlpim.size())
		cic->SelChanPim(nlpim[pim]);
	else if (pim >= (int)nlpim.size() || pim < ID_CHAN_IGNORE)
		result = MATRIX_CHAN_IDX_INVAID_3;

	if (det > ID_CHAN_IGNORE && det <  (int)nldet.size())
		cic->SelChanDet(nldet[det]);
	else if (det >= (int)nldet.size() || det < ID_CHAN_IGNORE)
		result = MATRIX_CHAN_IDX_INVAID_4;

	//通道非法的结果不处理...
	//if (result != MATRIX_SWITCH_OK)
	//	return result;

	result |= cic->Excute() ? MATRIX_SWITCH_OK : MATRIX_SWITCH_EXCUTE_FAILED;

	//if (__switchType == ID_POI)
	//{
	//	if (tx1 == 8 || tx2 == 10 || pim == 8)
	//		result = MATRIX_SWITCH_ERROR_CHAN_NO_EXIST;
	//}

	return result;
}

int MartrixSwitchBoxExcute(int tx1, int tx2, int pim, int det)
{
	if (__cic == nullptr)
	{
		return MATRIX_SWITCH_ERROR_OBJECT_NULL;
	}

	__cic->isZeroFrame = false;

	return MartrixSwitchBoxExcuteAD(tx1, tx2, pim, det);
}

int MartrixSwitchBoxReset(int tx1, int tx2, int pim, int det)
{
	if (__cic == nullptr)
	{
		return MATRIX_SWITCH_ERROR_OBJECT_NULL;
	}
	
	__cic->isZeroFrame = true;

	return MartrixSwitchBoxExcuteAD(tx1,tx2,pim,det);	
}

int MartrixSwitchDispose()
{
	if (__cic != nullptr)
	{
		__cic->DisConnect();

		delete __cic;
	}

	return 0;
}