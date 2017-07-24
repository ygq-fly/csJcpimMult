#ifndef _IF_JCSWITCH_HPP_
#define _IF_JCSWITCH_HPP_

#include "IfInstrument.hpp"
#include "stdafx.h"

enum
{
	MODE_HUAWEI = 0,
	MODE_TRANSMISSION = 1,
	MODE_POI = 2,
	MODE_NEWPOI = 3,
	MODE_NEWHUAWEI = 4,
	MODE_DPX = 5
};

Interface_ IfSwitch
{
public:
	virtual ~IfSwitch() {};
	virtual bool SwitchInit(int switch_work_type, int switch_conn_type, std::string path) = 0;
	virtual bool SwitchExcut(const int& iSwitchTx1, const int& iSwitchTx2, 
		const int& iSwitchPim, const int& iSwitchDet, bool reset = false) = 0;
	virtual void SwitchClose() = 0;
	virtual std::string SwitchGetError() = 0;
	virtual void SwitchGetInfo(std::string& sInfo, int iIndex) = 0;
	//virtual void SwitchSetEnable(int iIndex, bool isEnable) = 0;
	//virtual bool SwitchConnect() = 0;
};

#endif