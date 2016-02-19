//信号源类
//支持安捷伦N5180系列
//
//by San,2015

#ifndef _CLS_AGN5181A_HPP_
#define _CLS_AGN5181A_HPP_

#include "ClsVisa.hpp"
#include "IfSignalSource.hpp"

class ClsSigAgN5181A : public ClsVisa, Implements_ IfSignalSource
{
#define AG_POW_OPEN true
#define AG_POW_CLOSE false
public:
	ClsSigAgN5181A()
		:_bOpen(false),
		_isCmdSucc(false),
		_freq_now(0),
		_pow_now(0),
		ClsVisa()
	{}

	~ClsSigAgN5181A() {}

public:
	bool InstrConnect(const char* c_addr){
		//return AgConnect(c_addr);
		bool isconn = AgConnect(c_addr);
		if (isconn) {
			AgWrite("*RST\n");
			InstrOpenMod(false);
		}
		return isconn;
	}

	void InstrSession(unsigned long viConnectedSession, const char* cIdn) {
		AgSession(viConnectedSession, cIdn);
		AgWrite("*RST\n");
		InstrOpenMod(false);
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
	//设置频率
	bool InstrSetFreq(double freq_khz) {
		//set freq
		if (freq_khz > _maxFreq_khz && freq_khz < _minFreq_khz)
			return false;

		//if (freq_khz != _freq_now) {
			//need c++11 support!
			std::string stemp = "FREQ " + std::to_string(freq_khz) + "KHz\n";
			_isCmdSucc = AgWrite(stemp.c_str());
			if (_isCmdSucc) _freq_now = freq_khz;
		//}
		return _isCmdSucc;
	}

	bool InstrSetPow(double pow_dbm) {
		//set pow
		if (pow_dbm >_maxPow_dbm && pow_dbm < _minPow_dbm)
			return false;

		//if (pow_dbm != _pow_now) {
			//need c++11 support!
			std::string stemp = "POW " + std::to_string(pow_dbm) + "dBm\n";
			_isCmdSucc = AgWrite(stemp.c_str());
			if (_isCmdSucc) _pow_now = pow_dbm;
		//}
		return _isCmdSucc;
	}

	//设置频率功率
	bool InstrSetFreqPow(double freq_khz, double pow_dbm) {
		//_isCmdSucc = InstrSetFreq(freq_khz);
		//_isCmdSucc &= InstrSetPow(pow_dbm);
		_isCmdSucc = InstrSetPow(pow_dbm);
		_isCmdSucc &= InstrSetFreq(freq_khz);
		return _isCmdSucc;
	}

	bool InstrOpenMod(bool bOpen) {
		if (bOpen)
			_isCmdSucc = AgWrite("OUTP:MOD:STAT ON\n");
		else
			_isCmdSucc = AgWrite("OUTP:MOD:STAT OFF\n");

		return _isCmdSucc;
	}

	//功放开关
	bool InstrOpenPow(bool bOpen) {
		if (bOpen)
			_isCmdSucc = AgWrite("OUTP:STAT ON\n");
		else
			_isCmdSucc = AgWrite("OUTP:STAT OFF\n");

		if (_isCmdSucc) _bOpen = bOpen;

		return _isCmdSucc;
	}

	//功放状态
	bool InstrPowStatus() const {
		return _bOpen;
	}

	bool InstrGetReferenceStatus() {
		char buf[128] = { 0 };
		long ret = 0;
		bool reslute = false;
		ret = AgWriteAndRead(":ROSC:SOUR?\n", buf);
		if (ret > 0) {
			std::string temp(buf);
			if (temp == "EXT\n")
				reslute = true;
			else
				reslute = false;
		}

		return reslute;
	}

private:
	bool _bOpen;
	bool _isCmdSucc;
	double _freq_now;
	double _pow_now;

	const double _maxFreq_khz = 6000 * 1000;
	const double _minFreq_khz = 100;
	const double _maxPow_dbm = 17;
	const double _minPow_dbm = -110;

};

#endif