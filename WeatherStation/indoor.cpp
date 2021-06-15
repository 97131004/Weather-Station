#include <wiringPi.h>
#include <fstream>
#include <iostream>
#include <ios>
#include <sstream>
#include <string>
#include <cstdlib>
#include <dht22.h>

#include "defines.h"
#include "containers.h"
#include "indoor.h"
#include "indoorEntry.h"


/*!
 * \brief Loading .csv file from \link IO_PATH_INDOOR \endlink using the line callback \link indoor::parseLineIndoor \endlink.
 */
void indoor::csvLoad() {
	containers::csvLoadAll(IO_PATH_INDOOR, indoor::parseLineIndoor);
}

/*!
* \brief Callback on a read line from the .csv file. Parsing, creating \link indoorEntry \endlink and filling \link INDOOR_VEC \endlink.
* \param ss Line as a std::stringstream*
*/
void indoor::parseLineIndoor(std::stringstream* ss) {
	
	std::string cell;
	int cellIncr = 0; //cell incr
	indoorEntry e; //declaring entry obj
	
	while(*ss) { //while not eol
	
		if (!std::getline(*ss, cell, ',')) break; //get cell

		switch(cellIncr) {
			case 0: e.day = std::stoi(cell); break;
			case 1: e.month = std::stoi(cell); break;
			case 2: e.year = std::stoi(cell); break;
			case 3: e.hour = std::stoi(cell); break;
			case 4: e.minute = std::stoi(cell); break;
			case 5: e.second = std::stoi(cell); break;
			case 6: e.celsius = std::atof(cell.c_str()); break;
			case 7: e.fahrenheit = std::atof(cell.c_str()); break;
			case 8: e.humidity = std::atof(cell.c_str()); break;
		}
		
		cellIncr++; //next cell
	}
	
	e.update();
	INDOOR_VEC.push_back(e);
}

/*!
* \brief Format and append \link indoorEntry \endlink to a .csv file. 
* \param ie Entry to append
*/
void indoor::saveEntry(const indoorEntry* ie) {
	
	std::stringstream line;
	line << ie->day << "," << ie->month << "," << ie->year << ",";
	line << ie->hour << "," << ie->minute << "," << ie->second << ",";
	line << ie->celsius << "," << ie->fahrenheit << "," << ie->humidity << "\n";
	
	containers::csvAppendLine(IO_PATH_INDOOR, line.str());
}

/*!
* \brief Thread that reads DHT22 sensor data, creates an \link indoorEntry \endlink, fills \link INDOOR_VEC \endlink and appends it to a .csv file.
*/
PI_THREAD(indoorGet) {
	
	float celsius, humidity;
	
	while(true) {
		
		//reading from dht22 sensor
		if (dht22::read_dht22_dat(PIN_DHT22, humidity, celsius) == 1) { //if checksum correct
			
			indoorEntry ie(&celsius, &humidity);
			ie.update();
			INDOOR_VEC.push_back(ie);
			indoor::saveEntry(&ie);
		}
		
		delay(DHT22_SEND_INTERVAL);
	}
}

/*!
* \brief Creating indoor thread.
*/
int indoor::create() {
	return piThreadCreate(indoorGet);
}
