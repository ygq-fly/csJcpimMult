#ifndef _IF_SIGNALSOURCE_HPP_
#define _IF_SIGNALSOURCE_HPP_

#include "IfInstrument.hpp"

Interface_ IfSignalSource: FInterface_ IfInstrument
{
public:
	virtual ~IfSignalSource() {};
	virtual bool InstrSetFreq(double freq_khz) = 0;
	virtual bool InstrSetPow(double pow_dbm) = 0;
	virtual bool InstrSetFreqPow(double freq_khz, double pow_dbm) = 0;
	virtual bool InstrOpenPow(bool bOpen) = 0;
	virtual bool InstrPowStatus() const = 0;
	virtual bool InstrGetReferenceStatus() = 0;
};

#endif