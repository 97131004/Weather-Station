#include <iostream>
#include <iterator>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <algorithm>
#include <sstream>
#include <stdlib.h>
#include <fstream>

#include "containers.h"
#include "timezone.h"

/*!
 * \brief Initializes the timezone class. Sets timezone's menu iterator \link TIMEZONES_VEC_INCR \endlink to the current system's timezone.
 */
int timezone::init() {
	timezoneListGet(&TIMEZONES_VEC);
	if (TIMEZONES_VEC.empty()) return -1;
	
	std::string tz;
	timezoneGet(tz);
	//searching for index of the current system's timezone in a vector of all system's timezones
	auto iter = std::find(TIMEZONES_VEC.begin(), TIMEZONES_VEC.end(), tz);
	if (iter != TIMEZONES_VEC.end()) //if not reached end of vector 
	{
		//set incr to found index
		TIMEZONES_VEC_INCR = std::distance(TIMEZONES_VEC.begin(), iter);
		TIMEZONES_VEC_INCR_PREV = TIMEZONES_VEC_INCR;
	}
	else return -1; //else: nothing found
	
	return 0;
}

/*!
 * \brief Reads all system's timezones by piping timedatectl and stores them in a vector.
 * \param vec Vector to be filled, e.g. \link TIMEZONES_VEC \endlink.
 */
void timezone::timezoneListGet(std::vector<std::string>* vec) {
    std::array<char, 128> buffer;
    std::string str;
    std::shared_ptr<FILE> pipe(popen("timedatectl list-timezones", "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) { //while not eof
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr) {//until reached \n or eof, writing in chunks to our 128-length-buffer
			str = buffer.data();
			//puts all the \n at the end of the string, returns an iterator
			//erases all the \n's at the end
			str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
            (*vec).push_back(str);
		}
    }
}

/*!
 * \brief Reads the current system's timezone.
 * \param timezone String to write the current system's timezone to
 */
void timezone::timezoneGet(std::string& timezone) {
    std::ifstream ifs("/etc/timezone");
	timezone.assign((std::istreambuf_iterator<char>(ifs)),
					(std::istreambuf_iterator<char>()));
    timezone.erase(std::remove(timezone.begin(), timezone.end(), '\n'), timezone.end());
}

/*!
 * \brief Sets the current system's timezone.
 * \param timezone Timezone as a string
 */
void timezone::timezoneSet(const std::string& timezone) {
	system(("sudo timedatectl set-timezone " + timezone).c_str());
}
