

#include "ClsSigTekTsg4000.h"

//#include <windows.h>
//
//#include <iostream>
//#include <string>


bool ClsSigTekTsg4000::InstrConnect(const char* c_addr){
    //return AgConnect(c_addr);
    bool isconn = AgConnect(c_addr);
	if (isconn) {
		AgWrite("*RST\n");
		InstrOpenPow(false);
	}
    return isconn;
}

void ClsSigTekTsg4000::InstrSession(unsigned long viConnectedSession, const char* cIdn) {
	AgSession(viConnectedSession, cIdn);

	AgWrite("*RST\n");
	InstrOpenPow(false);
}

bool ClsSigTekTsg4000::InstrWrite(const char* c_cmd) {
    return AgWrite(c_cmd);
}

long ClsSigTekTsg4000::InstrWriteAndRead(const char* c_cmd, char* rbuf) {
    return AgWriteAndRead(c_cmd, rbuf);
}

bool ClsSigTekTsg4000::InstrConnStatus() const {
    return AgConnStatus();
}

void ClsSigTekTsg4000::InstrClose() {
    AgClose();
}

bool ClsSigTekTsg4000::InstrSetFreq(double freq_khz) {
    //set freq
    if (freq_khz > _maxFreq_khz && freq_khz < _minFreq_khz)
        return false;

    if (freq_khz != _freq_now) {
		char ctemp[128] = { 0 };
		sprintf_s(ctemp, "FREQ %lf KHz\n", freq_khz);
        _isCmdSucc = AgWrite(ctemp);
        if (_isCmdSucc) _freq_now = freq_khz;
    }
    return _isCmdSucc;
}

bool ClsSigTekTsg4000::InstrSetPow(double pow_dbm) {
    //set pow
    if (pow_dbm >_maxPow_dbm && pow_dbm < _minPow_dbm)
        return false;

    if (pow_dbm != _pow_now) {
		char ctemp[128] = { 0 };
		sprintf_s(ctemp, "AMPR %lf dBm\n", pow_dbm);
        _isCmdSucc = AgWrite(ctemp);
        if (_isCmdSucc) _pow_now = pow_dbm;
    }
    return _isCmdSucc;
}

bool ClsSigTekTsg4000::InstrSetFreqPow(double freq_khz, double pow_dbm) {
    _isCmdSucc = InstrSetFreq(freq_khz);
    _isCmdSucc &= InstrSetPow(pow_dbm);
    return _isCmdSucc;
}

bool ClsSigTekTsg4000::InstrOpenPow(bool bOpen)
{
    if (bOpen)
        _isCmdSucc = AgWrite("ENBR 1\n");
    else
        _isCmdSucc = AgWrite("ENBR 0\n");

    if (_isCmdSucc) _bOpen = bOpen;

    return _isCmdSucc;
}

bool ClsSigTekTsg4000::InstrPowStatus() const {
    return _bOpen;
}

bool ClsSigTekTsg4000::InstrGetReferenceStatus() {

	char buf[16] = { 0 };
	if (AgWriteAndRead("TIMB?\n", buf) < 0) {
		return false;
	}
	std::string temp(buf);
	int rs = atoi(buf);

	return (rs == 3) ;
}

bool ClsSigTekTsg4000::InstrInit()
{
    
    return InstrGetReferenceStatus();
}



