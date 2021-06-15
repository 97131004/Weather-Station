#include <string>

/*!
 * \brief Class that continuously plots a number of dynamically added entries on Temperature-Time-Diagrams.
 * \copyright MIT License
 * \author notpavlov
 */
class plot {
public:
	template<typename T>
	static void plotWindow(const int& window, std::vector<T>& srcVec, const int& delay_, const std::string& title);
	static int create();
};
