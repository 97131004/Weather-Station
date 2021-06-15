#ifndef OUTDOORENTRY_
#define OUTDOORENTRY_

#include <string>
#include "defines.h"

/*!
 * \brief Class that includes all outdoor data (date and time, temperature, humidity, pressure, wind speed) and string representations of those.
 * \copyright MIT License
 * \author notpavlov
 */
class outdoorEntry {
public:

	int year, month, day, hour, minute, second, pressure;
	float celsius, fahrenheit, humidity, windspeed;
	std::string status, celsiusStr, fahrenheitStr, humidityStr, dateStr, timeStr, pressureStr, windspeedStr;
	
	outdoorEntry();
	
	void update();
};

#endif
