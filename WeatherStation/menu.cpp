#include <iostream>
#include <chrono> 
#include <ctime>
#include <iomanip>
#include <string>
#include <ostream>
#include <iostream>
#include <wiringPi.h>
#include <lcd.h>
#include <sstream>

#include "defines.h"
#include "containers.h"
#include "indoorEntry.h"
#include "outdoorEntry.h"
#include "dht22.h"
#include "menu.h"

/*!
 * \brief LCD display positions. Corners.
 */
enum class displayMode {topLeft = 0, bottomLeft = 1, topRight = 2, bottomRight = 3};

/*!
 * \brief Display a string on the LCD display at a certain position.
 * \param pos Position on display
 * \param str String to display
 */
void menu::displayS(const displayMode& pos, const std::string& str) {
	
	switch(pos) {
		case displayMode::topLeft:
			lcdPosition(LCD, 0, 0);
		break;
		case displayMode::bottomLeft:
			lcdPosition(LCD, 0, 1);
		break;
		case displayMode::topRight:
			lcdPosition(LCD, 16 - str.length(), 0);
		break;
		case displayMode::bottomRight:
			lcdPosition(LCD, 16 - str.length(), 1);
		break;
	}
	
	lcdPuts(LCD, str.c_str());
}

/*!
 * \brief Apply fast scroll effect, that scrolls faster through the list when holding \link PIN_BT2 \endlink or \link PIN_BT3 \endlink.
 * \param incrVal Menu iterator or integer variable
 * \param min Minimal value
 * \param max Maximal value
 */
void menu::applyFastScroll(int& incrVal, const int& min, const int& max) {
	if (SCROLLSPEED_ISFAST) { //only when fastscrolling
		if (digitalRead(PIN_BT2) == HIGH) {
			containers::setIncr(incrVal, min, max, false);
		}
		else if (digitalRead(PIN_BT3) == HIGH) {
			containers::setIncr(incrVal, min, max, true);
		}
	}
	
}

/*!
* \brief Scrolls through characters in a string and parses it, so it fits within a certain width. Creates a marquee effect. Uses \link MARQUEE_SHIFT \endlink.
* \param text Full-length string
* \param limit Maximal width to fit the string into
* \return Copy of a newly created string
*/
std::string menu::marqueeS(const std::string& text, const int& limit) {
	if ((int)text.length() > limit) {
		std::string temp = text;
		if (MARQUEE_SHIFT <= (int)text.length() - limit) {
			temp.erase(0, MARQUEE_SHIFT);
			if (!SCROLLSPEED_ISFAST) MARQUEE_SHIFT++; //only shift when not fastscrolling
		}
		else {
			MARQUEE_SHIFT = 0;
		}
		return temp.substr(0, limit);
	}
	else return text;
}

/*!
* \brief Thread that handles the contents of the menu, depending on the mode set in the \link navigation.cpp \endlink.
*/
PI_THREAD(menuWork) {
    
    std::chrono::time_point<std::chrono::system_clock> currentTime;
	std::time_t currentTime_time_t;
    
    int toggleStep = 0;
    bool toggle = true;
    
    while(true) {
		
		lcdClear(LCD);
		
		//toggler
		if (toggleStep++ > 9 && !SCROLLSPEED_ISFAST) { //each x steps, toggle variable
			toggleStep = 0;
			toggle = !toggle;
		};

		
		if (MENU == MENU_START) {
			
			//display date and time
			currentTime = std::chrono::system_clock::now();
			currentTime_time_t = std::chrono::system_clock::to_time_t(currentTime);
			
			std::stringstream ssDate;
			ssDate << std::put_time(std::localtime(&currentTime_time_t), "%d.%m.%y");
			menu::displayS(displayMode::topLeft, ssDate.str());
			
			std::stringstream ssTime;
			ssTime << std::put_time(std::localtime(&currentTime_time_t), "%H:%M:%S");
			menu::displayS(displayMode::bottomLeft, ssTime.str());
			
			
			if (!INDOOR_VEC.empty()) {
				//display temp and hum
				menu::displayS(displayMode::topRight, toggle ? INDOOR_VEC.back().celsiusStr : INDOOR_VEC.back().fahrenheitStr);
				menu::displayS(displayMode::bottomRight, INDOOR_VEC.back().humidityStr);
			}
			
		}
		
		else if (MENU == MENU_START_TIMEZONE) {
			
			//display timezone
			menu::displayS(displayMode::topLeft, "Set timezone:");
			menu::displayS(displayMode::bottomLeft, menu::marqueeS(TIMEZONES_VEC[TIMEZONES_VEC_INCR], 16));
			menu::applyFastScroll(TIMEZONES_VEC_INCR, 0, TIMEZONES_VEC_INCR_MAX());
		}
		
		else if (MENU == MENU_INDOOR) {
			
			menu::displayS(displayMode::topLeft, "Indoor:");
			
			if (!INDOOR_VEC.empty()) {
				//display last indoor entry
				auto* entry = &INDOOR_VEC.back();
				
				menu::displayS(displayMode::bottomLeft, entry->status);
				menu::displayS(displayMode::topRight, toggle ? entry->celsiusStr : entry->fahrenheitStr);
				menu::displayS(displayMode::bottomRight, entry->humidityStr);
				
			}
			else menu::displayS(displayMode::bottomLeft, "No entries");
		}
		else if (MENU == MENU_INDOOR_HISTORY) {

			if (!INDOOR_VEC.empty()) {
				
				//display entry that has the index of the indoor menu iterator
				auto* entry = &INDOOR_VEC[INDOOR_VEC_INCR];
				
				if (toggle) {
					menu::displayS(displayMode::topLeft, "#" + std::to_string(INDOOR_VEC_INCR + 1));
				}
				else {
					menu::displayS(displayMode::topLeft, (entry->dateStr).substr(0, (entry->dateStr).length() - 3));
					menu::displayS(displayMode::topRight, entry->timeStr);
				}

				menu::displayS(displayMode::bottomLeft, menu::marqueeS(entry->celsiusStr + " " + entry->fahrenheitStr + " " + entry->humidityStr + " " + entry->status, 16));
				menu::applyFastScroll(INDOOR_VEC_INCR, 0, INDOOR_VEC_INCR_MAX());
			}
			else menu::displayS(displayMode::topLeft, "<No entries>");
		}
		else if (MENU == MENU_OUTDOOR) {
			
			menu::displayS(displayMode::topLeft, "Outdoor:");
			
			if (!OUTDOOR_VEC.empty()) {
				//display last outdoor entry
				auto* entry = &OUTDOOR_VEC.back();
				
				menu::displayS(displayMode::bottomLeft, menu::marqueeS(entry->status, 9));
				menu::displayS(displayMode::topRight, toggle ? entry->celsiusStr : entry->fahrenheitStr);
				menu::displayS(displayMode::bottomRight, entry->humidityStr);
			}
			else menu::displayS(displayMode::bottomLeft, "No entries");
		}
		else if (MENU == MENU_OUTDOOR_SUB_H) {
			
			menu::displayS(displayMode::topLeft, "Outdoor:");
			menu::displayS(displayMode::bottomLeft, "Show history");
		}
		else if (MENU == MENU_OUTDOOR_SUB_C) {
			
			lcdCursorBlink(LCD, false);
			menu::displayS(displayMode::topLeft, "Outdoor:");
			menu::displayS(displayMode::bottomLeft, "Set location");
		}
		else if (MENU == MENU_OUTDOOR_SUB_HISTORY) {

			if (!OUTDOOR_VEC.empty()) {
				
				//display entry that has the index of the outdoor menu iterator
				auto* entry = &OUTDOOR_VEC[OUTDOOR_VEC_INCR];
				
				if (toggle) {
					menu::displayS(displayMode::topLeft, "#" + std::to_string(OUTDOOR_VEC_INCR + 1));
				}
				else {
					menu::displayS(displayMode::topLeft, (entry->dateStr).substr(0, (entry->dateStr).length() - 3));
					menu::displayS(displayMode::topRight, entry->timeStr);
				}
				
				menu::displayS(displayMode::bottomLeft, menu::marqueeS(entry->celsiusStr + " " + entry->fahrenheitStr + " " + entry->humidityStr + " "+ entry->pressureStr + " "+ entry->windspeedStr + " " + entry->status, 16));
				menu::applyFastScroll(OUTDOOR_VEC_INCR, 0, OUTDOOR_VEC_INCR_MAX());
			}
			else menu::displayS(displayMode::topLeft, "<No entries>");
		}
		else if (MENU == MENU_OUTDOOR_SUB_COORDS_LAT || MENU == MENU_OUTDOOR_SUB_COORDS_LAT_DEC ||
				 MENU == MENU_OUTDOOR_SUB_COORDS_LON || MENU == MENU_OUTDOOR_SUB_COORDS_LON_DEC) {
			
			//whether we are in latitude or longitude mode
			bool isLat = MENU == MENU_OUTDOOR_SUB_COORDS_LAT || MENU == MENU_OUTDOOR_SUB_COORDS_LAT_DEC;
			
			menu::displayS(displayMode::topLeft, isLat ? "Set latitude:" : "Set longitude:");
			
			//display integer part
			std::stringstream ssInt;
			ssInt << std::right << std::setw(3 + !isLat) << std::setfill('0') << std::internal;
			ssInt << (isLat ? OUTDOOR_LAT : OUTDOOR_LON) << ".";
			menu::displayS(displayMode::bottomLeft, ssInt.str());
			
			//display decimal part
			std::stringstream ssDec;
			ssDec << std::setw(2) << std::setfill('0') << std::right << (isLat ? OUTDOOR_LAT_DEC : OUTDOOR_LON_DEC);
			lcdPuts(LCD, ssDec.str().c_str());
			
			//applying fast scroll
			if (MENU == MENU_OUTDOOR_SUB_COORDS_LAT || MENU == MENU_OUTDOOR_SUB_COORDS_LON) {
				lcdPosition(LCD, 2 + !isLat, 1);
				menu::applyFastScroll(isLat ? OUTDOOR_LAT : OUTDOOR_LON , isLat ? -90 : -180, isLat ? 90 : 180);
			}
			else if (MENU == MENU_OUTDOOR_SUB_COORDS_LAT_DEC || MENU == MENU_OUTDOOR_SUB_COORDS_LON_DEC) {
				lcdPosition(LCD, 5 + !isLat, 1);
				menu::applyFastScroll(isLat ? OUTDOOR_LAT_DEC : OUTDOOR_LON_DEC, 0, 99);
			}
			
			lcdCursorBlink(LCD, true);
		}
		
		
		delay(SCROLLSPEED); //delay
	}
}

/*!
* \brief Creating menu thread.
*/
int menu::create() {
	return piThreadCreate(menuWork);
}
