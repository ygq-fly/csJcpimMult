#include "ClsJcSwitch.h"
#include "MyUtil/JcCommonAPI.h"
using namespace std;

ClsJcSwitch::ClsJcSwitch()
	: _isConn(false)
	, _switch_work_type(ID_HUAWEI)
	, _switch_conn_type(COMM_TYPE_TCP)
{}
ClsJcSwitch::~ClsJcSwitch() {}

//开关初始化
bool ClsJcSwitch::SwitchInit(int switch_work_type, int switch_conn_type) {
	_isConn = false;
	//设置工作模式
	if (switch_work_type == MODE_POI)
		_switch_work_type = ID_POI;
	else
		_switch_work_type = ID_HUAWEI;
	//设置连接模式
	_switch_conn_type = COMM_TYPE_TCP;
	//加载配置
	int b = MartrixSwitchInit(NULL, "JcPimMultiBandV2.dll", _switch_work_type, _switch_conn_type);
	if (b == -1){
		_error_info = "SwitchInit: Init Error!";
		Util::logged(L"SwitchInit: 初始化异常");
	}
	else if (b == -2) {
		_error_info = "SwitchInit: Load-Map Error";
		Util::logged(L"SwitchInit: 配置文件不存在");
	}
	else if (b == -3) {
		_error_info = "SwitchInit: Object Error";
		Util::logged(L"SwitchInit: 对象不存在");
	}
	else if (b == -4) {
		_error_info = "SwitchInit: Tunnel-Index Error";
		Util::logged(L"SwitchInit: 通道序号不存在");
	}
	else if (b == 0)
		_isConn = true;

	return _isConn;
}

//开关执行切换
bool ClsJcSwitch::SwitchExcut(const int& iSwitchTx1, const int& iSwitchTx2,
							  const int& iSwitchPim, const int& iSwitchDet) {
	bool ret = true;
	//操作开关矩阵
	int b = MartrixSwitchBoxExcute(iSwitchTx1, iSwitchTx2, iSwitchPim, iSwitchDet);
	//开关操作执行失败
	if (b == -1) 
	{
		ret = false;
		_error_info = "SwitchExcut: switch set error";
		//Util::logged(L"SwitchExcut: 开关操作执行失败");
	}
	else if (b == -10001)
	{
		//channel(1) set error!
	}
	else if (b == -10002)
	{
		//channel(2) set error!
	}
	else if (b == -10003)
	{
		//channel(3) set error!
	}
	else if (b == -10004)
	{
		//channel(4) set error!
	}
	return ret;
}

//释放资源
void ClsJcSwitch::SwitchClose() {
	MartrixSwitchDispose();
}

std::string ClsJcSwitch::SwitchGetError(){
	return _error_info;
}

void ClsJcSwitch::SwitchGetInfo(std::string& sInfo, int iIndex) {
	sInfo = "Switch";
}
