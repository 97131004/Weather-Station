#include <sstream>
#include <iomanip>
#include "outdoorEntry.h"

/*!
 * \brief Creates a new \link outdoorEntry \endlink. Sets its date and time to the current system's ones.
 */
outdoorEntry::outdoorEntry() {
	defines::getCurrentTime(&year, &month, &day, &hour, &minute, &second);
}

/*!
 * \brief Updating status and string representations of date, time, temperature, humidity, pressure, wind speed. Usually called after constructor.
 */
void outdoorEntry::update() {
	
	fahrenheit = (celsius * 1.8) + 32;
	
	std::stringstream ssDate;
	ssDate << std::setw(2) << std::setfill('0') << day << ".";
	ssDate << std::setw(2) << std::setfill('0') << month << ".";
	std::string sYear = std::to_string(year);
	ssDate << sYear.substr(sYear.length() - 2);
	dateStr = ssDate.str();
	
	std::stringstream ssTime;
	ssTime << std::setw(2) << std::setfill('0') << hour << ":";
	ssTime << std::setw(2) << std::setfill('0') << minute << ":";
	ssTime << std::setw(2) << std::setfill('0') << second;
	timeStr = ssTime.str();
	
	std::stringstream ssCelsius;
	ssCelsius << std::fixed << std::setprecision(1) << celsius << "C";
	celsiusStr = ssCelsius.str();
	
	std::stringstream ssFahrenheit;
	ssFahrenheit << std::fixed << std::setprecision(1) << fahrenheit << "F";
	fahrenheitStr = ssFahrenheit.str();
	
	std::stringstream ssHumidity;
	ssHumidity << std::fixed << std::setprecision(1) << humidity << "%";
	humidityStr = ssHumidity.str();
	
	std::stringstream ssPressure;
	ssPressure << std::fixed << std::setprecision(1) << pressure << "hPa";
	pressureStr = ssPressure.str();
	
	std::stringstream ssWindspeed;
	ssWindspeed << std::fixed << std::setprecision(1) << windspeed << "m/s";
	windspeedStr = ssWindspeed.str();
}
