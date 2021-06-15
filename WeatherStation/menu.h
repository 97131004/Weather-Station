#include <string>

enum class displayMode;

/*!
 * \brief Class that handles the contents of the menu.
 * \copyright MIT License
 * \author notpavlov
 */
class menu {
public:
	static int create();
	static void displayS(const displayMode& pos, const std::string& str);
	static void applyFastScroll(int& incrVal, const int& min, const int& max);
	static std::string marqueeS(const std::string& text, const int& limit);
};

