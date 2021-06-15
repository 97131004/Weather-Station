#include <fstream>
#include <exception>
#include <iostream>
#include "containers.h"

/*! \brief Vector of all system's timezones. */
std::vector<std::string> TIMEZONES_VEC;
/*! \brief Menu iterator for the \link TIMEZONES_VEC \endlink. */
int TIMEZONES_VEC_INCR = 0;
/*! \return Number of timezones in \link TIMEZONES_VEC \endlink, 0-based. */
int TIMEZONES_VEC_INCR_MAX() { return TIMEZONES_VEC.size() - 1; };
/*! \brief Stores previous value of \link TIMEZONES_VEC_INCR \endlink to reset the timezone when leaving the timezone settings. */
int TIMEZONES_VEC_INCR_PREV = 0;

/*! \brief Vector of \link indoorEntry \endlink. All indoor entries. */
std::vector<indoorEntry> INDOOR_VEC;
/*! \brief Menu iterator for the \link INDOOR_VEC \endlink. */
int INDOOR_VEC_INCR = 0;
/*! \return Number of entries in \link INDOOR_VEC \endlink, 0-based. */
int INDOOR_VEC_INCR_MAX() { return INDOOR_VEC.size() - 1; };

/*! \brief Vector of \link outdoorEntry \endlink. All outdoor entries. */
std::vector<outdoorEntry> OUTDOOR_VEC;
/*! \brief Menu iterator for the \link OUTDOOR_VEC \endlink. */
int OUTDOOR_VEC_INCR = 0;
/*! \return Number of entries in \link OUTDOOR_VEC \endlink, 0-based. */
int OUTDOOR_VEC_INCR_MAX() { return OUTDOOR_VEC.size() - 1; };

/*!
 * \brief Increases or decreases a menu iterator or an integer variable within its borders.
 * \param incrVal Menu iterator or integer variable
 * \param min Minimal value
 * \param max Maximal value
 * \param incr True to increment, False to decrement
 */
void containers::setIncr(int& incrVal, const int& min, const int& max, const bool& incr) {
	if (!incr) {
		if (incrVal-1 < min) incrVal = min; else incrVal--;
	}
	else {
		if (incrVal+1 > max) incrVal = max; else incrVal++;
	}
}

/*!
 * \brief Writes a new .csv file. Replaces existing file.
 * \param path Path to file
 * \param line Line to append
 */
void containers::csvWriteNew(const std::string& path, const std::string& line) {
	std::ofstream clear;
	clear.open(path, std::ofstream::out | std::ofstream::trunc);
	clear.close();
	containers::csvAppendLine(path, line);
}

/*!
 * \brief Appends a line to a .csv file.
 * \param path Path to file
 * \param line Line to append
 */
void containers::csvAppendLine(const std::string& path, const std::string& line) {
	std::ofstream file;
	file.open(path, std::ios_base::out | std::ios_base::app); //write and append
	file << line;
	file.close();
}

/*!
* \brief Loads all lines from a .csv file.
* \param path Path to file
* \param parseLineEvent Callback on each line, passes the line as a std::stringstream*
*/
void containers::csvLoadAll(const std::string& path, void(*parseLineEvent)(std::stringstream*)) {
	
	std::ifstream fileExists(path);
	if (fileExists.good()) { //if file exists
		std::ifstream file(path, std::ios_base::in);

		std::string line, cell;

		while(file) { //while not eof
			if (!std::getline(file, line)) break; //get line
			
			std::stringstream ssLine(line);
			
			try { //in case we are converting to wrong types, or parsing errors
				(*parseLineEvent)(&ssLine); //function pointer where the line will be extracted
			}
			catch(std::exception& e) {
				std::cout << "[ERROR ON CSV PARSE IN CONTAINERS.CPP]"  << std::endl;
			}
		}
	}
}

/*!
 * \brief Loading config (.cfg) file from \link IO_PATH_CONFIG \endlink using the line callback \link containers::cfgParseLine \endlink.
 */
void containers::cfgLoad() {
	containers::csvLoadAll(IO_PATH_CONFIG, containers::cfgParseLine);
}

/*!
* \brief Callback on a read line from the config (.cfg) file. Loading latitude and longitude.
* \param ss Line as a std::stringstream*
*/
void containers::cfgParseLine(std::stringstream* ss) {
	
	std::string cell;
	int cellIncr = 0; //cell incr
	outdoorEntry e; //declaring entry obj
	
	while(*ss) { //while not eol
	
		if (!std::getline(*ss, cell, ',')) break; //get cell

		switch(cellIncr) {
			case 0: OUTDOOR_LAT = std::stoi(cell); OUTDOOR_LAT_R = OUTDOOR_LAT; break;
			case 1: OUTDOOR_LAT_DEC = std::stoi(cell); OUTDOOR_LAT_DEC_R = OUTDOOR_LAT_DEC; break;
			case 2: OUTDOOR_LON = std::stoi(cell); OUTDOOR_LON_R = OUTDOOR_LON; break;
			case 3: OUTDOOR_LON_DEC = std::stoi(cell); OUTDOOR_LON_DEC_R = OUTDOOR_LON_DEC; break;
		}
		
		cellIncr++; //next cell
	}
}

/*!
* \brief Saving the config (.cfg) file. Saving latitude and longitude.
*/
void containers::cfgSave() {
	
	std::stringstream out;
	out << OUTDOOR_LAT_R << "," << OUTDOOR_LAT_DEC_R << ",";
	out << OUTDOOR_LON_R << "," << OUTDOOR_LON_DEC_R;
	
	containers::csvWriteNew(IO_PATH_CONFIG, out.str());
}
