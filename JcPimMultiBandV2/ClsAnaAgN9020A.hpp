#ifndef _CLS_AGN9020A_HPP_
#define _CLS_AGN9020A_HPP_

#include "ClsInstrAgilent.hpp"
#include "IfAnalyzer.hpp"

#define eNULL 0

class ClsAnaAgN9020A: public ClsInstrAgilent, Implements_ IfAnalyzer
{
public:
	enum eParam
	{
		SPAN,
		CENTER,
		REFLEVEL,
		ATT,
		RBW,
		VBW,
		MARK_X,
		MARK_MAX,
		MARK_POS
	};

public:
	ClsAnaAgN9020A()
		:_isCmdSucc(false),
		_freq_now(eNULL),
		ClsInstrAgilent()
	{}

	~ClsAnaAgN9020A() {

	}

public:
	bool InstrConnect(const char* c_addr){
		bool isconn = AgConnect(c_addr);
		//连接成功即开始初始化
		if (isconn)
			InstrInit();
		return isconn;
	}

	void InstrSession(unsigned long viConnectedSession) {
		AgSession(viConnectedSession);
		//连接成功即开始初始化
		InstrInit();
	}

	bool InstrWrite(const char* c_cmd) {
		return AgWrite(c_cmd);
	}

	long InstrWriteAndRead(const char* c_cmd, char* rbuf) {
		return AgWriteAndRead(c_cmd, rbuf);
	}

	bool InstrConnStatus() const {
		return AgConnStatus();
	}

	void InstrClose() {
		AgClose();
	}

public:

	//开始初始化
	void InstrInit() {
		AgWrite("*RST\n");

		SetParam(eParam::SPAN, 500);
		SetParam(eParam::CENTER, 637 * 1000);
		SetParam(eParam::ATT, 0);
		SetParam(eParam::REFLEVEL, -60);
		SetParam(eParam::RBW, 10);
		SetParam(eParam::VBW, 10);
		SetParam(eParam::MARK_POS, eNULL);

		InstrSetAvg(0);
		//InstrClosgAvg();
	}

	//开始读取
	double InstrGetAnalyzer(double freq_khz, bool isMax) {
		double result_val = 0;
		if (freq_khz != _freq_now){
			SetCenterFreq(freq_khz);
			_freq_now = freq_khz;
		}	

		//清空状态
		_isCmdSucc = AgWrite("*CLS\n");
		//执行扫描
		_isCmdSucc = AgWrite("INIT:IMM\n");
		//开始等待
		if (AgWait() == false){

			SetCenterFreq(freq_khz);
			_isCmdSucc = AgWrite("*CLS\n");
			_isCmdSucc = AgWrite("INIT:IMM\n");
			if (AgWait() == false)
				return -10000;
		}
		//读取
		result_val = ReadMarkY(isMax);
		return result_val;
	}

	//设置平均次数
	void InstrSetAvg(const int& avg_time) {
		AgWrite("AVER:TYPE RMS\n");
		if (avg_time > 0){
			std::string cmd = "AVER:COUN " + std::to_string(avg_time) + "\n";
			AgWrite(cmd.c_str());
			//AgWrite("AVER:STAT ON\n");
		}
		else {
			AgWrite("AVER:STAT OFF\n");
		}
	}

	void InstrClosgAvg() {
		AgWrite("AVER:STAT OFF\n");
	}

	//设置offset
	void InstrSetOffset(const double& pow_dbm) {
		std::string cmd = "DISP:WIND:TRAC:Y:RLEV:OFFS " + std::to_string(pow_dbm) + "\n";
		AgWrite(cmd.c_str());
	}

	//设置att参数
	void InstrSetAtt(const int& att) {
		SetParam(eParam::ATT, att);		
	}

	//设置ref参数
	void InstrSetRef(const int& reflevel) {
		SetParam(eParam::REFLEVEL, reflevel);
	}

	//设置rbw参数
	void InstrSetRbw(const double& rbw_hz) {
		SetParam(eParam::RBW, rbw_hz);
	}

	//设置vbw参数
	void InstrSetVbw(const double& vbw_hz) {
		SetParam(eParam::VBW, vbw_hz);
	}

	//设置span参数
	void InstrSetSpan(const double& span_hz) {
		SetParam(eParam::SPAN, span_hz);
	}

	//设置中心频率
	void SetCenterFreq(const double& freq_khz) {
		SetParam(eParam::CENTER, freq_khz);
		SetParam(eParam::MARK_X, freq_khz);
	}

private:
	//读取MARK
	double ReadMarkY(bool isMax) {
		if (isMax) SetParam(eParam::MARK_MAX, eNULL);

		//开始读取
		char buf[128] = {0};
		long retCount = 0;
		double val = -10000;
		retCount = AgWriteAndRead("CALC:MARK1:Y?\n", buf);
		//转换
		if (retCount)
			val = atof(buf);
		return val;
	}

	//设置
	ViStatus SetParam(const eParam& param, const double& val) {
		std::stringstream sstemp;	
		switch (param)
		{
		case SPAN:
			sstemp << "FREQ:SPAN " << val << " Hz\n"; break;	
		case RBW:
			sstemp << "BAND " << val << " Hz\n"; break;
		case VBW:
			sstemp << "BAND:VID " << val << " Hz\n"; break;
		case CENTER:
			sstemp << "FREQ:CENT " << val << " kHz\n"; break;	
		case MARK_X:
			sstemp << "CALC:MARK1:X " << val << " kHz\n"; break;
		case REFLEVEL:
			sstemp << "DISPlay:WINDow:TRACe:Y:RLEVel " << val << " dBm\n"; break;		
		case ATT:
			sstemp << "POW:ATT " << val << " dB\n"; break;		
		case MARK_MAX:
			sstemp << "CALC:MARK1:MAX" << "\n"; break;
		case MARK_POS:
			sstemp << "CALC:MARK1:MODE POS" << "\n"; break;
		default:
			break;
		}
		
		std::string str = sstemp.str();
		_isCmdSucc = AgWrite(str.c_str());

		return _isCmdSucc;
	}
	
private:
	bool _isCmdSucc;
	double _freq_now;
};

#endif

