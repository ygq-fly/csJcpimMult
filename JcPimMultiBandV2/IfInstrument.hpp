#ifndef _IF_INSTRUMENT_HPP_
#define _IF_INSTRUMENT_HPP_

#include "Interface.h"

Interface_ IfInstrument
{
public:
	virtual ~IfInstrument() {};
	virtual bool InstrConnect(const char* c_addr) = 0;
	virtual void InstrSession(unsigned long viConnectedSession, const char* cIdn) = 0;
	virtual bool InstrWrite(const char* c_cmd) = 0;
	virtual long InstrWriteAndRead(const char* c_cmd, char* rbuf) = 0;
	virtual bool InstrConnStatus() const = 0;
	virtual void InstrClose() = 0;
};

#endif