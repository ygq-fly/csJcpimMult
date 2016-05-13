#ifndef _CLS_JCSWITCH_H_
#define _CLS_JCSWITCH_H_

//#include "Switch/com_io_ctl.h"
#include "JcMatrixSwitch.h"
#include "../IfSwitch.hpp"

class ClsJcSwitch: Implements_ IfSwitch
{
public:
	ClsJcSwitch();
	~ClsJcSwitch();

public:
	//开关初始化
	bool SwitchInit(int switch_work_type, int switch_conn_type, std::string path);
	//开关执行切换
	bool SwitchExcut(const int& iSwitchTx1, const int& iSwitchTx2, 
		const int& iSwitchPim, const int& iSwitchDet, bool reset = false);
	//释放资源
	void SwitchClose();
	//获取信息
	std::string SwitchGetError();
	//获取设备信息
	void SwitchGetInfo(std::string& sInfo, int iIndex);

private:
	bool _isConn;
	//开关工作类型
	int _switch_work_type;
	//开关连接类型
	int _switch_conn_type;
	std::string _error_info;
};
#endif