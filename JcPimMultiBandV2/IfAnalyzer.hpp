#ifndef _IF_ANALYZER_HPP_
#define _IF_ANALYZER_HPP_

#include "IfInstrument.hpp"

Interface_ IfAnalyzer : FInterface_ IfInstrument
{
public:
	virtual ~IfAnalyzer() {};
	virtual void InstrInit() = 0;
	virtual double InstrGetAnalyzer(double freq_khz, bool isMax) = 0;
	virtual void InstrSetAvg(const int& avg_time) = 0;
	virtual void InstrClosgAvg() = 0;
	virtual void InstrSetOffset(const double& pow_db) = 0;
	//virtual void InstrSetAttRef(const int& att, const int& reflevel) = 0;
	//virtual void InstrSetRbwSpan(const int& rbw_hz, const int& span_hz) = 0;
	virtual void InstrSetAtt(const int& att) = 0;
	virtual void InstrSetRef(const int& reflevel) = 0;
	virtual void InstrSetRbw(const double& rbw_hz) = 0;
	virtual void InstrSetVbw(const double& vbw_hz) = 0;
	virtual void InstrSetSpan(const double& span_hz) = 0;
	virtual void InstrSetCenterFreq(const double& freq_khz) = 0;

	virtual void InstrPimSetting() = 0;
	virtual void InstrVcoSetting() = 0;
	virtual void InstrTxOffsetSetting() = 0;
	virtual void InstrRxOffsetSetting() = 0;
	virtual void InstrSetModeAtt(int pim_att, int offset_att) = 0;
};

#endif