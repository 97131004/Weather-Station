#include <vector>
#include <string>
#include <sstream>

#include "indoorEntry.h"
#include "outdoorEntry.h"

extern std::vector<std::string> TIMEZONES_VEC;
extern int TIMEZONES_VEC_INCR;
extern int TIMEZONES_VEC_INCR_MAX();
extern int TIMEZONES_VEC_INCR_PREV;

extern std::vector<indoorEntry> INDOOR_VEC;
extern int INDOOR_VEC_INCR;
extern int INDOOR_VEC_INCR_MAX();

extern std::vector<outdoorEntry> OUTDOOR_VEC;
extern int OUTDOOR_VEC_INCR;
extern int OUTDOOR_VEC_INCR_MAX();

/*!
 * \brief Storage class of all container types: \link TIMEZONES_VEC \endlink, \link INDOOR_VEC \endlink, \link OUTDOOR_VEC \endlink. Also handles the I/O of all containers.
 * \copyright MIT License
 * \author 97131004
 */
class containers {
private:
	static void cfgParseLine(std::stringstream* ss);
public:
	static void setIncr(int& incrVal, const int& min, const int& max, const bool& incr);
	static void csvAppendLine(const std::string& path, const std::string& line);
	static void csvWriteNew(const std::string& path, const std::string& line);
	static void csvLoadAll(const std::string& path, void(*parseLineEvent)(std::stringstream*));
	static void cfgLoad();
	static void cfgSave();
};
