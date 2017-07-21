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

template<class T>
Interface_ IfSignalSource2
{
public:
	IfSignalSource2() {}
	virtual ~IfSignalSource2() {}

	bool InstrSetFreq(double freq_khz) {
		return getInstance()->T::InstrSetFreq();
	}

	bool InstrSetPow(double pow_dbm) {
		return getInstance()->T::InstrSetPow();
	}

	bool InstrSetFreqPow(double freq_khz, double pow_dbm) {
		return getInstance()->T::InstrSetFreqPow();
	}

	bool InstrOpenPow(bool bOpen) {
		return getInstance()->T::InstrOpenPow();
	}

	bool InstrPowStatus() const {
		return getInstance()->T::InstrPowStatus();
	}

	bool InstrGetReferenceStatus() {
		return getInstance()->T::InstrGetReferenceStatus();
	}


protected:
	inline T* getInstance() {
		return static_cast<T *>(this);
	}

	inline T* getInstance() const {
		return static_cast<const T *>(this);
	}
};

#endif