#include <sstream>
#include <iomanip>
#include "indoorEntry.h"

/*!
 * \brief Setting temperature and humidity.
 * \param celsius_ Celsius
 * \param humidity_ Humidity
 */
indoorEntry::indoorEntry(const float* celsius_, const float* humidity_) {
		
	defines::getCurrentTime(&year, &month, &day, &hour, &minute, &second);
	celsius = *celsius_;
	fahrenheit = (*celsius_ * 1.8) + 32;
	humidity = *humidity_;
}

/*!
 * \brief Updating status and string representations of date, time, temperature, humidity. Usually called after constructor.
 */
void indoorEntry::update() {
	
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
	
	if (celsius < 18) {
		status = "Cold";
	}
	else if (celsius >= 18 && celsius <=28)  {
		status = "Warm";
	}
	else if (celsius > 28) {
		status = "Hot";
	}
}
