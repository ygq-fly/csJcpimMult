#ifndef _CLS_INSTRU2000A_HPP_
#define _CLS_INSTRU2000A_HPP_

#include "ClsVisa.hpp"
#include "IfSensor.hpp"

class ClsSenAgU2000A :public ClsVisa, Implements_ IfSensor
{
#define VERROR -10000
#define eNULL 0
public:
	ClsSenAgU2000A() 
		:_freq_now(eNULL),
		ClsVisa()
	{}

	~ClsSenAgU2000A() {}

public:
	bool InstrConnect(const char* c_addr){
		bool isconn= AgConnect(c_addr);
		if (isconn)
			InstrSenInit();
		return isconn;
	}

	void InstrSession(unsigned long viConnectedSession, const char* cIdn) {
		AgSession(viConnectedSession, cIdn);
		InstrSenInit();
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
	//init
	void InstrSenInit(){
		AgWrite("*RST\n");
		Util::setSleep(200);
		AgWrite("SYST:PRES DEF\n");
		AgWrite("FREQ 1000MHz\n");
		AgWrite("UNIT:POW dBm\n");
		//AgWrite("INIT:CONT ON\n");
	}

	//读取
	double InstrGetSesnor() {
		
		//开始读取
		char buf[128] = { 0 };
		long retCount = 0;
		double val = -10000;
		//retCount = AgWriteAndRead("FETC?\n", buf);
		retCount = AgWriteAndRead("FETC:POW:AC?\n", buf);
		
		//转换
		if (retCount)
			val = atof(buf);
		return val;
	}

	std::string InstrGetIdn(){
		return "";
	}

private:
	double _freq_now;
};

#endif