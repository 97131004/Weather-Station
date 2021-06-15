#ifndef INDOORENTRY_
#define INDOORENTRY_

#include <string>
#include "defines.h"

/*!
 * \brief Class that includes all indoor data (date and time, temperature, humidity) and string representations of those.
 * \copyright MIT License
 * \author notpavlov
 */
class indoorEntry {
public:
	
	int year, month, day, hour, minute, second;
	float celsius, fahrenheit, humidity;
	std::string status, celsiusStr, fahrenheitStr, humidityStr, dateStr, timeStr;
	
	indoorEntry() { }
	
	indoorEntry(const float* celsius_, const float* humidity_);
	
	void update();
};

#endif
