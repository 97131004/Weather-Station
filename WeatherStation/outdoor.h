#include <string>
#include "outdoorEntry.h"

/*!
 * \brief Class that continuously retrieves and parses weather data from the OpenWeatherMap-API, fills the \link OUTDOOR_VEC \endlink with a new \link outdoorEntry \endlink and appends it to a .csv file.
 * \copyright MIT License
 * \author notpavlov
 */
class outdoor {
private:
	static void parseLineOutdoor(std::stringstream* ss);
	static size_t writeHTTPresponse(char *contents, size_t size, size_t nmemb, void *userp);
public:
	static int create();
	static void saveEntry(const outdoorEntry* oe);
	static void csvLoad();
	static int sendHTTPRequest(const std::string& url, std::string& response);
};
