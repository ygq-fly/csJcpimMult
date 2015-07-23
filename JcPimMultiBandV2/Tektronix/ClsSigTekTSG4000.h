
#ifndef CLS_SIG_TEK_TSG4000_H
#define CLS_SIG_TEK_TSG4000_H

#include "../ClsInstrAgilent.hpp"
#include "../IfSignalSource.hpp"


class ClsSigTekTsg4000 : public ClsInstrAgilent, Implements_ IfSignalSource
{
#define AG_POW_OPEN true
#define AG_POW_CLOSE false
public:
	ClsSigTekTsg4000()
        :_bOpen(false),
        _isCmdSucc(false),
        _freq_now(0),
        _pow_now(0),
        _sourTunnel(1),
        ClsInstrAgilent()
    {}

	~ClsSigTekTsg4000() {}

public:
    bool InstrConnect(const char* c_addr);

	void InstrSession(unsigned long viConnectedSession, const char* cIdn);

    bool InstrWrite(const char* c_cmd);

    long InstrWriteAndRead(const char* c_cmd, char* rbuf);

    bool InstrConnStatus() const;

    void InstrClose();

public:
    bool InstrSetFreq(double freq_khz);

    bool InstrSetPow(double pow_dbm);

    //����Ƶ�ʹ���
    bool InstrSetFreqPow(double freq_khz, double pow_dbm);

    //���ſ���
    bool InstrOpenPow(bool bOpen);

    //����״̬
    bool InstrPowStatus() const;

    //���ͬ�����Ƿ����/����
    bool InstrGetReferenceStatus();

    //
    bool InstrInit();

private:
    bool _bOpen;
    bool _isCmdSucc;
    double _freq_now;
    double _pow_now;
    int   _sourTunnel;                 //���ͨ�� 1 = A; 2 = B

    const double _maxFreq_khz = 6000 * 1000;
    const double _minFreq_khz = 100;
    const double _maxPow_dbm = 17;
    const double _minPow_dbm = -110;

};



#endif

