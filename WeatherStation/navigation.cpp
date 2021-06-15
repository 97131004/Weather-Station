#include "defines.h"
#include <wiringPi.h>
#include <lcd.h>
#include <iostream>

#include "containers.h"
#include "navigation.h"
#include "timezone.h"

/*! \brief Storing current button states for all 4 buttons. */
int navigation::buttonState[4];
/*! \brief Storing previous button states for all 4 buttons. */
int navigation::buttonStatePrev[4];

/*!
 * \brief Rising edge detection (0 => 1) for pull-down buttons. Binding buttons to callbacks. Callback will be executed a single time per button press.
 * \param index Index of button that has to be processed
 * \param pin GPIO-Pin of button that has to be processed
 * \param buttonEvent Callback on button pressed
 */
void navigation::buttonRisingEdge(const int& index, const int& pin, void(*buttonEvent)()) { //edge detection: 0 => 1
	buttonState[index] = digitalRead(pin);
	if (buttonState[index] != buttonStatePrev[index]) {
		if (buttonState[index] == HIGH) { //reading button
			(*buttonEvent)(); //calling function through pointer with 0 arguments
			delay(BT_BOUNCE_DELAY); //avoiding bouncing
		}
	}
	buttonStatePrev[index] = buttonState[index];
}

/*!
* \brief Thread that handles button presses and holds.
*/
PI_THREAD(buttonGet) {
	int buttonHoldIncr = 0;
	
    while(true) {
		//binding button presses to callbacks
		navigation::buttonRisingEdge(0, PIN_BT1, navigation::button1Pressed);
		navigation::buttonRisingEdge(1, PIN_BT2, navigation::button2Pressed);
		navigation::buttonRisingEdge(2, PIN_BT3, navigation::button3Pressed);
		navigation::buttonRisingEdge(3, PIN_BT4, navigation::button4Pressed);
		
		//binding button holds to fast scroll:
		//increasing scrollspeed (and with that the refresh rate) on bt2/bt3 hold:
		if (MENU != MENU_START && (digitalRead(PIN_BT2) == HIGH || digitalRead(PIN_BT3) == HIGH)) {
			buttonHoldIncr++;
		}
		else {
			buttonHoldIncr = 0;
			SCROLLSPEED = SCROLLSPEED_DEF;
			SCROLLSPEED_ISFAST = false;
		}
		
		if (buttonHoldIncr > SCROLLSPEED_FAST_BTHOLD) {
			MARQUEE_SHIFT = 0;
			SCROLLSPEED = SCROLLSPEED_FAST;
			SCROLLSPEED_ISFAST = true;
			buttonHoldIncr = SCROLLSPEED_FAST_BTHOLD;
		}
		
		delay(1); //each ms
	}
}

/*!
* \brief Creating button retrieval thread.
*/
int navigation::create() {
	return piThreadCreate(buttonGet);
}

/*!
* \brief Callback for \link PIN_BT1 \endlink.
*/
void navigation::button1Pressed() {
	MARQUEE_SHIFT = 0;
	
	switch(MENU)  {
		case MENU_START_TIMEZONE: MENU = MENU_START; break;
		case MENU_INDOOR_HISTORY: MENU = MENU_INDOOR; break;
		case MENU_OUTDOOR_SUB_C:
		case MENU_OUTDOOR_SUB_H:
			MENU = MENU_OUTDOOR;
		break;
		case MENU_OUTDOOR_SUB_HISTORY: MENU = MENU_OUTDOOR_SUB_H; break;
		case MENU_OUTDOOR_SUB_COORDS_LAT:
		case MENU_OUTDOOR_SUB_COORDS_LAT_DEC:
		case MENU_OUTDOOR_SUB_COORDS_LON:
		case MENU_OUTDOOR_SUB_COORDS_LON_DEC:
			MENU = MENU_OUTDOOR_SUB_C;
			lcdCursorBlink(LCD, false);
		break;
	}
}

/*!
* \brief Callback for \link PIN_BT2 \endlink.
*/
void navigation::button2Pressed() {
	MARQUEE_SHIFT = 0;

	switch(MENU)  {
		case MENU_START_TIMEZONE:
			containers::setIncr(TIMEZONES_VEC_INCR, 0, TIMEZONES_VEC_INCR_MAX(), false);
		break;
		case MENU_INDOOR: MENU = MENU_START; break;
		case MENU_INDOOR_HISTORY:
			containers::setIncr(INDOOR_VEC_INCR, 0, INDOOR_VEC_INCR_MAX(), false);
		break;
		case MENU_OUTDOOR: MENU = MENU_INDOOR; break;
		case MENU_OUTDOOR_SUB_C: MENU = MENU_OUTDOOR_SUB_H; break;
		case MENU_OUTDOOR_SUB_HISTORY:
			containers::setIncr(OUTDOOR_VEC_INCR, 0, OUTDOOR_VEC_INCR_MAX(), false);
		break;
		case MENU_OUTDOOR_SUB_COORDS_LAT:
			containers::setIncr(OUTDOOR_LAT, -90, 90, false);
		break;
		case MENU_OUTDOOR_SUB_COORDS_LAT_DEC:
			containers::setIncr(OUTDOOR_LAT_DEC, 0, 99, false);
		break;
		case MENU_OUTDOOR_SUB_COORDS_LON:
			containers::setIncr(OUTDOOR_LON, -180, 180, false);
		break;
		case MENU_OUTDOOR_SUB_COORDS_LON_DEC:
			containers::setIncr(OUTDOOR_LON_DEC, 0, 99, false);
		break;
	}
}

/*!
* \brief Callback for \link PIN_BT3 \endlink.
*/
void navigation::button3Pressed() {
	MARQUEE_SHIFT = 0;

	switch(MENU)  {
		case MENU_START: MENU = MENU_INDOOR; break;
		case MENU_START_TIMEZONE:
			containers::setIncr(TIMEZONES_VEC_INCR, 0, TIMEZONES_VEC_INCR_MAX(), true);
		break;
		case MENU_INDOOR_HISTORY:
			containers::setIncr(INDOOR_VEC_INCR, 0, INDOOR_VEC_INCR_MAX(), true);
		break;
		case MENU_INDOOR: MENU = MENU_OUTDOOR; break;
		case MENU_OUTDOOR_SUB_H: MENU = MENU_OUTDOOR_SUB_C; break;
		case MENU_OUTDOOR_SUB_HISTORY:
			containers::setIncr(OUTDOOR_VEC_INCR, 0, OUTDOOR_VEC_INCR_MAX(), true);
		break;
		case MENU_OUTDOOR_SUB_COORDS_LAT:
			containers::setIncr(OUTDOOR_LAT, -90, 90, true);
		break;
		case MENU_OUTDOOR_SUB_COORDS_LAT_DEC:
			containers::setIncr(OUTDOOR_LAT_DEC, 0, 99, true);
		break;
		case MENU_OUTDOOR_SUB_COORDS_LON:
			containers::setIncr(OUTDOOR_LON, -180, 180, true);
		break;
		case MENU_OUTDOOR_SUB_COORDS_LON_DEC:
			containers::setIncr(OUTDOOR_LON_DEC, 0, 99, true);
		break;
	}
}

/*!
* \brief Callback for \link PIN_BT4 \endlink.
*/
void navigation::button4Pressed() {
	MARQUEE_SHIFT = 0;
	
	switch(MENU)  {
		case MENU_START:
			TIMEZONES_VEC_INCR = TIMEZONES_VEC_INCR_PREV;
			MENU = MENU_START_TIMEZONE;
		break;
		case MENU_START_TIMEZONE:
			timezone::timezoneSet(TIMEZONES_VEC[TIMEZONES_VEC_INCR]);
			TIMEZONES_VEC_INCR_PREV = TIMEZONES_VEC_INCR;
			MENU = MENU_START;
		break;
		case MENU_INDOOR:
			INDOOR_VEC_INCR = INDOOR_VEC_INCR_MAX();
			MENU = MENU_INDOOR_HISTORY;
		break; 
		case MENU_OUTDOOR: MENU = MENU_OUTDOOR_SUB_H; break;
		case MENU_OUTDOOR_SUB_H:
			OUTDOOR_VEC_INCR = OUTDOOR_VEC_INCR_MAX();
			MENU = MENU_OUTDOOR_SUB_HISTORY;
		break;
		case MENU_OUTDOOR_SUB_C:
			//restoring lat, lon to their restore values
			OUTDOOR_LAT = OUTDOOR_LAT_R;
			OUTDOOR_LAT_DEC = OUTDOOR_LAT_DEC_R;
			OUTDOOR_LON = OUTDOOR_LON_R;
			OUTDOOR_LON_DEC = OUTDOOR_LON_DEC_R;
			MENU = MENU_OUTDOOR_SUB_COORDS_LAT;
		break;
		case MENU_OUTDOOR_SUB_COORDS_LAT: MENU = MENU_OUTDOOR_SUB_COORDS_LAT_DEC; break;
		case MENU_OUTDOOR_SUB_COORDS_LAT_DEC: MENU = MENU_OUTDOOR_SUB_COORDS_LON; break;
		case MENU_OUTDOOR_SUB_COORDS_LON: MENU = MENU_OUTDOOR_SUB_COORDS_LON_DEC; break;
		case MENU_OUTDOOR_SUB_COORDS_LON_DEC:
			//copying temp values to restore values, and save them to cfg
			OUTDOOR_LAT_R = OUTDOOR_LAT;
			OUTDOOR_LAT_DEC_R = OUTDOOR_LAT_DEC;
			OUTDOOR_LON_R = OUTDOOR_LON;
			OUTDOOR_LON_DEC_R = OUTDOOR_LON_DEC;
			containers::cfgSave();
			lcdCursorBlink(LCD, false);
			MENU = MENU_OUTDOOR_SUB_C;
		break;
	}
}
