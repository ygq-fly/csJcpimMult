#ifndef _CLS_INSTRRSNRT_HPP_
#define _CLS_INSTRRSNRT_HPP_

#include "../ClsVisa.hpp"
#include "../IfSensor.hpp"

class ClsSenRsNrt :public ClsVisa, Implements_ IfSensor
{
#define VERROR -10000
#define eNULL 0
public:
	ClsSenRsNrt()
		:_freq_now(eNULL),
		_isAver(false),
		ClsVisa()
	{}

	~ClsSenRsNrt() {}

public:
	bool InstrConnect(const char* c_addr){
		bool isconn = AgConnect(c_addr);
		if (isconn)
			ReadyToSet();
		return isconn;
	}

	void InstrSession(unsigned long viConnectedSession, const char* cIdn) {
		AgSession(viConnectedSession, cIdn);
		ReadyToSet();
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
	//读取
	double InstrGetSesnor() {
		//开始读取
		//TRIG; *WAI; :SENS1:DATA? 'POW:FORW:AVER'
		unsigned char buf[256] = {0};
		unsigned long retCount = 0;
		double val = -10000;
		//_viStatus = viPrintf(_viSession, "TRIG\n");
		AgWrite("TRIG\n");

		while (1)
		{
			_esr = 0;
			viQueryf(_viSession, (char*)"*OPC?\n", (char*)"%ld", &_esr);
			if (_esr & 1) break;
			Util::setSleep(300);
		}

		if (_isAver)
			//_viStatus = viPrintf(_viSession, "SENS1:DATA? 'POW:FORW:AVER'\n");
			AgWrite("SENS1:DATA? 'POW:FORW:AVER'\n");
		else
			//_viStatus = viPrintf(_viSession, "SENS1:DATA? 'POW:FORW:PEP'\n");
			AgWrite("SENS1:DATA? 'POW:FORW:PEP'\n");
		_viStatus = viRead(_viSession, buf, 256, &retCount);
		//转换
		if (retCount)
			val = atof((char*)buf);
		return val;
	}

	void InstrSetModeAver() {
		//SENS1:FUNC:ON 'POW:FORW:AVER'
		AgWrite("SENS1:FUNC:ON 'POW:FORW:AVER'\n");
		_isAver = true;
	}

	void InstrSetModePep() {
		//SENS1:FUNC:ON 'POW:FORW:PEP'
		AgWrite("SENS1:FUNC:ON 'POW:FORW:PEP'\n");
		_isAver = false;
	}

	std::string InstrGetIdn(){
		return "nrt";
	}

private:
	void ReadyToSet() {
		bool isconn = false;
		isconn = AgWrite("UNIT:POW:REFL SWR\n");
		isconn = AgWrite("UNIT:POW DBM\n");
		isconn = AgWrite("SENS1:FUNC:CONC OFF\n");

		InstrSetModePep();
	}

private:
	double _freq_now;
	bool _isAver;
};

#endif