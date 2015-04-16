

#include "ClsSigRsSMxSerial.h"

#include <windows.h>

#include <iostream>
#include <string>


bool ClsSigRsSMxSerial::InstrConnect(const char* c_addr){
    //return AgConnect(c_addr);
    bool isconn = AgConnect(c_addr);
    if (isconn)
        AgWrite("*RST\n");
    return isconn;
}

void ClsSigRsSMxSerial::InstrSession(unsigned long viConnectedSession) {
    AgSession(viConnectedSession);
    AgWrite("*RST\n");
}

bool ClsSigRsSMxSerial::InstrWrite(const char* c_cmd) {
    return AgWrite(c_cmd);
}

long ClsSigRsSMxSerial::InstrWriteAndRead(const char* c_cmd, char* rbuf) {
    return AgWriteAndRead(c_cmd, rbuf);
}

bool ClsSigRsSMxSerial::InstrConnStatus() const {
    return AgConnStatus();
}

void ClsSigRsSMxSerial::InstrClose() {
    AgClose();
}

bool ClsSigRsSMxSerial::InstrSetFreq(double freq_khz) {
    //set freq
    if (freq_khz > _maxFreq_khz && freq_khz < _minFreq_khz)
        return false;

    if (freq_khz != _freq_now) {
        //need c++11 support!
        std::string stemp = ":SOUR" + std::to_string(_sourTunnel) + ":FREQ " + std::to_string(freq_khz) + "KHz\n";
        _isCmdSucc = AgWrite(stemp.c_str());
        if (_isCmdSucc) _freq_now = freq_khz;
    }
    return _isCmdSucc;
}

bool ClsSigRsSMxSerial::InstrSetPow(double pow_dbm) {
    //set pow
    if (pow_dbm >_maxPow_dbm && pow_dbm < _minPow_dbm)
        return false;

    if (pow_dbm != _pow_now) {
        //need c++11 support!
        std::string stemp = ":SOUR" + std::to_string(_sourTunnel) + ":POW " + std::to_string(pow_dbm) + "dBm\n";
        _isCmdSucc = AgWrite(stemp.c_str());
        if (_isCmdSucc) _pow_now = pow_dbm;
    }
    return _isCmdSucc;
}

bool ClsSigRsSMxSerial::InstrSetFreqPow(double freq_khz, double pow_dbm) {
    _isCmdSucc = InstrSetFreq(freq_khz);
    _isCmdSucc &= InstrSetPow(pow_dbm);
    return _isCmdSucc;
}

bool ClsSigRsSMxSerial::InstrOpenPow(bool bOpen)
{
    std::string statON = std::string(":SOUR") + std::to_string(_sourTunnel) + ":OUTP:STAT ON\n";
    std::string statOFF;



    if (bOpen)
        _isCmdSucc = AgWrite("OUTP:STAT ON\n");
    else
        _isCmdSucc = AgWrite("OUTP:STAT OFF\n");

    if (_isCmdSucc) _bOpen = bOpen;

    return _isCmdSucc;
}

bool ClsSigRsSMxSerial::InstrPowStatus() const {
    return _bOpen;
}

bool ClsSigRsSMxSerial::InstrGetReferenceStatus() {

    long ret = 0;
    bool reslute = false;
    
    ret = AgWrite("*CLS\n");
    ret = AgWrite("ROSC:SOUR INT\n");
    Sleep (2000);
    ret = AgWrite("ROSC:SOUR EXT\n");
    Sleep (2000);
    
    char buf[1024] = { 0 };    
    ret = AgWriteAndRead("SYST:ERR?\n", buf);
    
    if (ret <= 0)
        return reslute;

    std::string temp(buf);
    
    if (temp.find("reference") == std::string::npos)
        reslute = true;
    else
        reslute = false;

    return reslute;
}

bool ClsSigRsSMxSerial::InstrInit()
{
    
    return InstrGetReferenceStatus();
}



