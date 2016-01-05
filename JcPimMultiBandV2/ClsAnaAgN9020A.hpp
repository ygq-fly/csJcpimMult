#ifndef _CLS_AGN9020A_HPP_
#define _CLS_AGN9020A_HPP_

#include "ClsVisa.hpp"
#include "IfAnalyzer.hpp"

#define eNULL 0

class ClsAnaAgN9020A : public ClsVisa, Implements_ IfAnalyzer
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
		ClsVisa()
	{}

	~ClsAnaAgN9020A() {

	}

public:
	bool InstrConnect(const char* c_addr){
		bool isconn = AgConnect(c_addr);
		//���ӳɹ�����ʼ��ʼ��
		if (isconn)
			InstrInit();
		return isconn;
	}

	void InstrSession(unsigned long viConnectedSession, const char* cIdn) {
		AgSession(viConnectedSession, cIdn);
		//���ӳɹ�����ʼ��ʼ��
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

	//��ʼ��ʼ��
	void InstrInit() {
		AgWrite("*RST\n");
		//SetParam(eParam::SPAN, 500);
		//SetParam(eParam::CENTER, 637 * 1000);
		//SetParam(eParam::ATT, 0);
		//SetParam(eParam::REFLEVEL, -60);
		//SetParam(eParam::RBW, 10);
		//SetParam(eParam::VBW, 10);
		//SetParam(eParam::MARK_POS, eNULL);
		////��ʼԤ��
		//AgWrite(":POW:GAIN ON\n");
		//AgWrite(":POW:GAIN:BAND LOW\n");
		//InstrSetAvg(0);
		////InstrClosgAvg();
		InstrPimSetting();
		SetParam(eParam::MARK_POS, eNULL);
		if (GetSpeIndex() == 0)
			AgWrite("MARK1:PEAK\n");
	}

	//��ʼ��ȡ
	double InstrGetAnalyzer(double freq_khz, bool isMax) {
		double result_val = 0;
		if (freq_khz != _freq_now){
			InstrSetCenterFreq(freq_khz);
			_freq_now = freq_khz;
		}	

		//���״̬
		_isCmdSucc = AgWrite("*CLS\n");
		//ִ��ɨ��
		_isCmdSucc = AgWrite("INIT:IMM\n");
		//��ʼ�ȴ�
		if (AgWait() == false)
			return -10000;
	
		//��ȡ
		result_val = ReadMarkY(isMax);
		return result_val;
	}

	//����ƽ������
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

	//����offset
	void InstrSetOffset(const double& pow_db) {
		std::string cmd = "DISP:WIND:TRAC:Y:RLEV:OFFS " + std::to_string(pow_db) + "\n";
		AgWrite(cmd.c_str());
	}

	//����att����
	void InstrSetAtt(const int& att) {
		SetParam(eParam::ATT, att);		
	}

	//����ref����
	void InstrSetRef(const int& reflevel) {
		SetParam(eParam::REFLEVEL, reflevel);
	}

	//����rbw����
	void InstrSetRbw(const double& rbw_hz) {
		SetParam(eParam::RBW, rbw_hz);
	}

	//����vbw����
	void InstrSetVbw(const double& vbw_hz) {
		SetParam(eParam::VBW, vbw_hz);
	}

	//����span����
	void InstrSetSpan(const double& span_hz) {
		SetParam(eParam::SPAN, span_hz);
	}

	//��������Ƶ��
	void InstrSetCenterFreq(const double& freq_khz) {
		SetParam(eParam::CENTER, freq_khz);
		SetParam(eParam::MARK_X, freq_khz);
	}

	//��������
	void InstrPimSetting() {
		InstrSetOffset(0);
		//��ʼԤ��
		AgWrite(":POW:GAIN ON\n");
		AgWrite(":POW:GAIN:BAND LOW\n");
		InstrSetRef(-60);
		InstrClosgAvg();
		InstrSetAtt(0);

		if (GetSpeIndex() == 1) {
			InstrSetRbw(10);
			InstrSetVbw(10);
			InstrSetSpan(500);
		}
		else {
			InstrSetRbw(1000);
			InstrSetVbw(1000);
			InstrSetSpan(10 * 1000);
		}
	}
	
	void InstrVcoSetting() {
		//OFFSET����
		InstrSetOffset(0);
		//��ʼԤ��
		AgWrite(":POW:GAIN ON\n");
		AgWrite(":POW:GAIN:BAND LOW\n");

		if (GetSpeIndex() == 1) {
		//2015-5-5/vco: 15khz-100hz-100hz
		InstrSetRbw(100);
		InstrSetVbw(100);
		InstrSetSpan(15 * 1000);
		//InstrSetRbw(10 * 1000);
		//InstrSetVbw(10 * 1000);
		//InstrSetSpan(400 * 1000);
		}
		else {
			InstrSetRbw(1000);
			InstrSetVbw(1000);
			InstrSetSpan(10 * 1000);
		}
	}
	void InstrTxOffsetSetting() {
		//OFFSET����
		InstrSetOffset(0);
		//�ر�Ԥ��
		AgWrite(":POW:GAIN OFF\n");
		InstrSetRef(10);
		InstrClosgAvg();
		InstrSetAtt(40);

		if (GetSpeIndex() == 1) {
			InstrSetRbw(100);
			InstrSetVbw(100);
			InstrSetSpan(1000);
		} else {
			InstrSetRbw(1000);
			InstrSetVbw(1000);
			InstrSetSpan(10*1000);
		}
	}
	void InstrRxOffsetSetting() {
		InstrPimSetting();
	}

private:
	//��ȡMARK
	double ReadMarkY(bool isMax) {
		if (isMax) SetParam(eParam::MARK_MAX, eNULL);

		//��ʼ��ȡ
		char buf[128] = {0};
		long retCount = 0;
		double val = -10000;
		retCount = AgWriteAndRead("CALC:MARK1:Y?\n", buf);
		//ת��
		if (retCount)
			val = atof(buf);
		return val;
	}

	//����
	bool SetParam(const eParam& param, const double& val) {
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

	int GetSpeIndex() {
		if (Util::strFind(_strIDN, "JCSPE"))
			return 0;
		else
			return 1;
	}
	
private:
	bool _isCmdSucc;
	double _freq_now;
	
};

#endif

