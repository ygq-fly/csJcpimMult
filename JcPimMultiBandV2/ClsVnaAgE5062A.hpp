#ifndef _CLS_AGE5062A_HPP_
#define _CLS_AGE5062A_HPP_

//#include <Windows.h>
#include "ClsVisa.hpp"
#include "IfVna.hpp"

class ClsVnaAgE5062A : public ClsVisa, Implements_ IfVna
{
public:
	enum  Sparam
	{
		S_PAR11,
		S_PAR21,
		S_PAR12,
		S_PAR22
	};
public:
	ClsVnaAgE5062A() 
		:_isCmdSucc(false),
		ClsVisa()
	{}

	~ClsVnaAgE5062A(){}

public:
	bool InstrConnect(const char* c_addr){
		return AgConnect(c_addr);
	}

	void InstrSession(unsigned long viConnectedSession, const char* cIdn) {
		AgSession(viConnectedSession, cIdn);
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
	void SetMeasType(const char* inputSparam) {
		std::string stemp;
		stemp = "CALC1:PAR1:DEF " + std::string(inputSparam) + "\n";
		_isCmdSucc = AgWrite(stemp.c_str());
	}

	bool SetFreqBand(double f_start_mhz, double f_stop_mhz) {
		f_start_mhz = f_start_mhz * 1000 *1000;
		f_stop_mhz = f_stop_mhz * 1000 *1000;

		std::string stemp;
		//need c++11 support!
		stemp = "SENS1:FREQ:STAR " + std::to_string(f_start_mhz) + "\n";
		_isCmdSucc = AgWrite(stemp.c_str());

		//need c++11 support!
		stemp = "SENS1:FREQ:STOP " + std::to_string(f_stop_mhz) + "\n";
		_isCmdSucc = AgWrite(stemp.c_str());

		return _isCmdSucc;
	}

	bool SetIFBand(int iBw, int iPoints) {
		std::string stemp;
		//need c++11 support!
		stemp = "SENS1:BWID " + std::to_string(iBw) + "\n";
		_isCmdSucc = AgWrite(stemp.c_str());

		//need c++11 support!
		stemp = "SENS1:SWE:POIN " + std::to_string(iPoints) + "\n";
		_isCmdSucc = AgWrite(stemp.c_str());

		return _isCmdSucc;
	}

	bool SetMarkMax() {
		_isCmdSucc = AgWrite("CALC1:MARK1:ACT\n");
		_isCmdSucc &= AgWrite("CALC1:MARK1:FUNC:TYPE MAX\n");
		_isCmdSucc &= AgWrite("CALC1:MARK1:FUNC:EXEC\n");

		return _isCmdSucc;
	}

	bool SetMarkMin() {
		_isCmdSucc = AgWrite("CALC1:MARK2:ACT\n");
		_isCmdSucc &= AgWrite("CALC1:MARK2:FUNC:TYPE MIN\n");
		_isCmdSucc &= AgWrite("CALC1:MARK2:FUNC:EXEC\n");

		return _isCmdSucc;
	}

	bool SetSingleSweep() {
		_isCmdSucc = AgWrite("INIT1:CONT ON\n");
		_isCmdSucc &= AgWrite("TRIG:SOUR BUS\n");

		return _isCmdSucc;
	}

	double GetSingleSweepMax() {
		//清空状态
		_isCmdSucc = AgWrite("*CLS\n");
		//执行扫描
		_isCmdSucc = AgWrite("TRIG:SING\n");
		//开始等待
		AgWait();
		return GetMarkY();
	}
	
private:
	double GetMarkY() {
		//开始读取
		char buf[128] = { 0 };
		long retCount = 0;
		double val = -10000;
		retCount = AgWriteAndRead("CALC1:MARK1:Y?\n", buf);
		//转换
		if (retCount)
			val = atof(buf);
		return val;
	}

private:
	bool _isCmdSucc;
};

#endif
