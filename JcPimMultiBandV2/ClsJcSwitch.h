#ifndef _CLS_JCSWITCH_H_
#define _CLS_JCSWITCH_H_

#include "Switch/com_io_ctl.h"
#include "IfSwitch.hpp"

class ClsJcSwitch: Implements_ IfSwitch
{
public:
	ClsJcSwitch();
	~ClsJcSwitch();

public:
	//开关初始化
	bool SwitchInit();
	//设置开关Enable
	void SwitchSetEnable(int iIndex, bool isEnable);
	//获取信息
	std::string SwitchGetInfo();
	//获取设备连接信息
	void SwitchGetInfo(std::string& sInfo, int iIndex);
	//开始连接
	bool SwitchConnect();
	//开关执行切换
	bool SwitchExcut(const int& iSwitchTx1, const int& iSwitchTx2, const int& iSwitchPim, const int& iSwitchDet);
	//释放资源
	void SwitchClose();

private:
	bool CheckStatus();

private:
	bool _isConn;
	ns_com_io_ctl::com_io_ctl _cic;
	std::map<std::string, ns_com_io_ctl::com_io_ctl::stHostControl> _hosts;

	std::vector<std::string> _moduleList;
	std::vector<std::string> _nltx1;
	std::vector<std::string> _nltx2;
	std::vector<std::string> _nlpim;
	std::vector<std::string> _nldet;

	std::string _info;
};
#endif