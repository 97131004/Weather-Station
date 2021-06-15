#include "indoorEntry.h"

/*!
 * \brief Class that continuously retrieves DHT22 sensor data, fills the \link INDOOR_VEC \endlink with a new \link indoorEntry \endlink and appends it to a .csv file.
 * \copyright MIT License
 * \author notpavlov
 */
class indoor {
public:
	static int create();
	static void saveEntry(const indoorEntry* ie);
	static void parseLineIndoor(std::stringstream* ss);
	static void csvLoad();
};
