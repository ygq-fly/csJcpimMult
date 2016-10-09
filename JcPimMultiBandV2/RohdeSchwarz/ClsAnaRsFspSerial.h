/**
 * @file ClaAnaRsFspSerial.h
 * @brief
 *
 *
 * @author 
 * @note
 * @copyright Jointcom
 * @date 2015.4.10
 * @version v0.1 alpha
 */

#ifndef CLS_ANA_RS_FSP_SERIAL_H
#define CLS_ANA_RS_FSP_SERIAL_H


//#include "Instrument.hpp"
//#include "SignalAnalyzer.hpp"

#include "../IfAnalyzer.hpp"
#include "../ClsVisa.hpp"


class ClsAnaRsFspSerial : public IfAnalyzer, public ClsVisa
{
public:
#define PRESET_PARAMETER_TOTAL      3
	enum preset_parameter
	{
		preset_no_set = -1,
		preset_default = 0,
		preset_mensuration = 1,
		preset_calibration = 2
	};

//    SignalAnalyzer sa;
public:
	bool InstrConnect(const char* c_addr);
	void InstrSession(unsigned long viConnectedSession, const char* cIdn);
	bool InstrWrite(const char* c_cmd);
	long InstrWriteAndRead(const char* c_cmd, char* rbuf);
	bool InstrConnStatus() const;
	void InstrClose();
	
public:
	virtual void InstrInit();
	virtual double InstrGetAnalyzer(double freq_khz, bool isMax);
	virtual void InstrSetAvg(const int& avg_time);
	virtual void InstrClosgAvg();
	virtual void InstrSetOffset(const double& pow_db);
	//virtual void InstrSetAttRef(const int& att, const int& reflevel) = 0;
	//virtual void InstrSetRbwSpan(const int& rbw_hz, const int& span_hz) = 0;
	virtual void InstrSetAtt(const int& att);
	virtual void InstrSetRef(const int& reflevel);
	virtual void InstrSetRbw(const double& rbw_hz);
	virtual void InstrSetVbw(const double& vbw_hz);
	virtual void InstrSetSpan(const double& span_hz);
	virtual void InstrSetCenterFreq(const double& freq_khz);
	virtual void InstrSetPreamp(bool isOn);

	virtual void InstrPimSetting();
	virtual void InstrVcoSetting();
	virtual void InstrTxOffsetSetting();
	virtual void InstrRxOffsetSetting();
	virtual void InstrSetSweepTime(int count_ms);
	void InstrSetModeAtt(int pim_att, int offset_att);

public:
	ClsAnaRsFspSerial(bool isPreamp) 
		: pp_(preset_parameter::preset_no_set)
		, _isNeedPreamp(isPreamp)
	{}
    //重置仪表的参数 
    void Preset(enum preset_parameter pp);
private:
    bool CommonSet(char const *command, ...);
	enum preset_parameter pp_;
	bool _isNeedPreamp;
	int m_pim_att;
	int m_offset_att;
};




#endif




