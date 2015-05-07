#include "ClsJcSwitch.h"
using namespace std;

ClsJcSwitch::ClsJcSwitch()
	:_isConn(false)
{}
ClsJcSwitch::~ClsJcSwitch() {}

//开关初始化
bool ClsJcSwitch::SwitchInit() {
	//加载配置
	bool isConfig = _cic.LoadMap();
	
	if (!isConfig){
		_info = "switch: loadMap error\r\n";
		return false;
	}
	//获取头信息
	_moduleList = _cic.GetModuleList();
	_nltx1 = _cic.GetTx1NameList();
	_nltx2 = _cic.GetTx2NameList();
	_nlpim = _cic.GetPimNameList();
	_nldet = _cic.GetDetNameList();

	//获取设备连接信息
	_cic.GetHostsCtrl(_hosts);
	_info = "";

	return true;
}

std::string ClsJcSwitch::SwitchGetInfo(){
	return _info;
}

void ClsJcSwitch::SwitchGetInfo(std::string& sInfo, int iIndex) {
	sInfo = _moduleList[iIndex];
}

void ClsJcSwitch::SwitchSetEnable(int iIndex, bool isEnable) {
	_hosts[_moduleList[iIndex]].enable = isEnable;
}

bool ClsJcSwitch::SwitchConnect() {
	_cic.SetHostsCtrl(_hosts, true, true);
	//连接所有模块
	_isConn = _cic.Connect();
	_cic.GetHostsCtrl(_hosts);
	for (size_t i = 0; i < _moduleList.size(); i++) {
		if (_hosts[_moduleList[i]].state)
			_info += (_moduleList[i] + ":Connected\r\n");
		else {
			_info += (_moduleList[i] + ":Not Connected\r\n");
			//to do
		}
	}
	return _isConn;
}

//开关执行切换
bool ClsJcSwitch::SwitchExcut(const int& iSwitchTx1, const int& iSwitchTx2,
							  const int& iSwitchPim, const int& iSwitchDet) {
	//操作开关矩阵
	_cic.Clear();
	_cic.SelChanTx1(_nltx1[iSwitchTx1]);
	_cic.SelChanTx2(_nltx2[iSwitchTx2]);
	_cic.SelChanPim(_nlpim[iSwitchPim]);
	_cic.SelChanDet(_nldet[iSwitchDet]);
	return _cic.Excute();
}

//释放资源
void ClsJcSwitch::SwitchClose() {
	_cic.DisConnect();
}
