#ifndef _IF_SENSOR_HPP_
#define _IF_SENSOR_HPP_

#include "IfInstrument.hpp"

Interface_ IfSensor: FInterface_ IfInstrument
{
public:
	virtual ~IfSensor() {};
	virtual double InstrGetSesnor() = 0;
	virtual std::string InstrGetIdn() = 0;
};

#endif