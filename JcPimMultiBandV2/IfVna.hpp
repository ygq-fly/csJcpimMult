#ifndef _IF_VNA_HPP_
#define _IF_VNA_HPP_

#include "IfInstrument.hpp"

Interface_ IfVna: FInterface_ IfInstrument
{
public:
	virtual ~IfVna() {};
	virtual void SetMeasType(const char* inputSparam) = 0;
	virtual bool SetFreqBand(double f_start_mhz, double f_stop_mhz) = 0;
	virtual bool SetIFBand(int iBw, int iPoints) = 0;
	virtual bool SetMarkMax() = 0;
	virtual bool SetSingleSweep() = 0;
	virtual double GetSingleSweepMax() = 0;
};

#endif