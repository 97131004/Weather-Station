/*!
 * 
 * \author notpavlov
 * \version 1.0
 * \copyright MIT License 
 * \mainpage Documentation
 * 
 * \section compile_sec Compilation
 * Compile the program using:<br>
 * g++ -o WeatherStation *.cpp -l wiringPi -l wiringPiDev -l curl -l jsoncpp
 * 
 * \section execution_sec Execution
 * Execute the progam using:<br>
 * ./WeatherStation
 * 
 */

#include <iostream>
#include <wiringPi.h>
#include <lcd.h>

#include "defines.h"
#include "navigation.h"
#include "menu.h"
#include "indoor.h"
#include "outdoor.h"
#include "timezone.h"
#include "containers.h"
#include "plot.h"


/*!
 * \brief Initializing LCD, timezones, loading .cfg and .csv files, creating all threads.
 */
int init()
{
	//init gpio
	if (wiringPiSetupGpio() == -1) return ERR_GPIO;
	
	//init lcd display
    LCD = lcdInit(2, 16, 4, PIN_LCD_RS, PIN_LCD_EN, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7, 0, 0, 0, 0);
    
    if (LCD == -1) return ERR_LCD;

	lcdClear(LCD);
	lcdPosition(LCD, 0, 0);
	//lcdPuts(LCD, "Loading..");
    
    //loading timezones
    if (timezone::init() < 0) return ERR_TIMEZONE;
    
    //loading .cfg and .csv data
    containers::cfgLoad();
    indoor::csvLoad();
    outdoor::csvLoad();
    
    //creating all threads
	if (navigation::create() != 0 || menu::create() != 0 || indoor::create() || outdoor::create() != 0 || plot::create() != 0) return ERR_THREAD;
	
	lcdCursorBlink(LCD, false);
	
	return 0;
}

/*!
 * \brief Main method, runs \link init() \endlink and handles its errors.
 */
int main() {
	
	int err = init();
	
    switch(err) {
		case ERR_GPIO: std::cout << "GPIO initialization failed." << std::endl; break;
		case ERR_LCD: std::cout << "LCD initialization failed." << std::endl; break;
		case ERR_THREAD: std::cout << "THREAD initialization failed." << std::endl; break;
		case ERR_TIMEZONE: std::cout << "TIMEZONE initialization failed." << std::endl; break;
	}
	
	if (err < 0) {
		lcdClear(LCD);
		lcdPosition(LCD, 0, 0);
		lcdPuts(LCD, "Error.");
		return -1;
	}
	
	std::cout << "[CTRL + C TO EXIT]" << std::endl;
	
	while(true) { };
    
    return 0;
}
