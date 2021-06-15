#include <iostream>
#include <chrono> 
#include <ctime>
#include <sstream>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <time.h>

#include "defines.h"


int LCD = -1;
int SCROLLSPEED = SCROLLSPEED_DEF;
bool SCROLLSPEED_ISFAST = false;
int MENU = MENU_START;
int MARQUEE_SHIFT = 0;
int OUTDOOR_LAT = 0;
int OUTDOOR_LAT_R = 0;
int OUTDOOR_LAT_DEC = 0;
int OUTDOOR_LAT_DEC_R = 0;
int OUTDOOR_LON = 0;
int OUTDOOR_LON_R = 0;
int OUTDOOR_LON_DEC = 0;
int OUTDOOR_LON_DEC_R = 0;

/*!
* \brief Populates passed variables with the current system time.
* \param y Year
* \param m Month
* \param d Day
* \param h Hour
* \param min Minute
* \param s Second
*/
void defines::getCurrentTime(int* y, int* m, int* d, int* h, int* min, int* s) {
	//retrieving current system time, splitting into year, month, day, ...
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	time_t tt = std::chrono::system_clock::to_time_t(now);
	tm local_tm = *localtime(&tt); //filling struct tm (locally timed)
	*y = local_tm.tm_year + 1900; //tm counts years since 1900
	*m = local_tm.tm_mon + 1; //tm counts months 0-based
	*d = local_tm.tm_mday;
	*h = local_tm.tm_hour;
	*min = local_tm.tm_min;
	*s = local_tm.tm_sec;
}
