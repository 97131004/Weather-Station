#include <vector>
#include <string>

/*!
 * \brief Class that retrieves and sets the system's timezone.
 * \copyright MIT License
 * \author 97131004
 */
class timezone {
public:
	static void timezoneGet(std::string& timezone);
	static void timezoneSet(const std::string& timezone);
	static void timezoneListGet(std::vector<std::string>* vec);
	static int init();
};
