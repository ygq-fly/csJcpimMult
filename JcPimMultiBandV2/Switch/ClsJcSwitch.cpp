#include "ClsJcSwitch.h"
#include "../MyUtil/JcCommonAPI.h"
using namespace std;

ClsJcSwitch::ClsJcSwitch()
	: _isConn(false)
	, _switch_work_type(ID_HUAWEI)
	, _switch_conn_type(COMM_TYPE_TCP)
{}
ClsJcSwitch::~ClsJcSwitch() {}

//���س�ʼ��
bool ClsJcSwitch::SwitchInit(int switch_work_type, int switch_conn_type) {
	_isConn = false;
	//���ù���ģʽ
	if (switch_work_type == MODE_POI)
		_switch_work_type = ID_POI;
	else
		_switch_work_type = ID_HUAWEI;
	//��������ģʽ
	_switch_conn_type = COMM_TYPE_TCP;
	//��������
	int b = MartrixSwitchInit(NULL, "JcPimMultiBandV2.dll", _switch_work_type, _switch_conn_type);
	if (b == -1){
		_error_info = "SwitchInit: Init Error!";
		Util::logged(L"SwitchInit: ��ʼ���쳣");
	}
	else if (b == -2) {
		_error_info = "SwitchInit: Load-Map Error";
		Util::logged(L"SwitchInit: �����ļ�������");
	}
	else if (b == -3) {
		_error_info = "SwitchInit: Object Error";
		Util::logged(L"SwitchInit: ���󲻴���");
	}
	else if (b == -4) {
		_error_info = "SwitchInit: Tunnel-Index Error";
		Util::logged(L"SwitchInit: ͨ����Ų�����");
	}
	else if (b == 0)
		_isConn = true;

	return _isConn;
}

//����ִ���л�
bool ClsJcSwitch::SwitchExcut(const int& iSwitchTx1, const int& iSwitchTx2,
							  const int& iSwitchPim, const int& iSwitchDet) {
	bool ret = true;
	//�������ؾ���
	int b = MartrixSwitchBoxExcute(iSwitchTx1, iSwitchTx2, iSwitchPim, iSwitchDet);
	//���ز���ִ��ʧ��
	if (b == -1) 
	{
		ret = false;
		_error_info = "SwitchExcut: switch set error";
		//Util::logged(L"SwitchExcut: ���ز���ִ��ʧ��");
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

//�ͷ���Դ
void ClsJcSwitch::SwitchClose() {
	MartrixSwitchDispose();
}

std::string ClsJcSwitch::SwitchGetError(){
	return _error_info;
}

void ClsJcSwitch::SwitchGetInfo(std::string& sInfo, int iIndex) {
	sInfo = "Switch";
}