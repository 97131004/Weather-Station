#ifndef DEFINES_
#define DEFINES_

//lcd pins
#define PIN_LCD_RS 26
#define PIN_LCD_EN 19
#define PIN_LCD_D4 13
#define PIN_LCD_D5 6
#define PIN_LCD_D6 20
#define PIN_LCD_D7 21

//button pins
/*! \brief GPIO-Pin of the leftmost button. Button function: Back, Cancel. */
#define PIN_BT1 18
/*! \brief GPIO-Pin of the middle-left button. Button function: Go Up, Decrement entry. */
#define PIN_BT2 1
/*! \brief GPIO-Pin of the middle-right button. Button function: Go Down, Increment entry. */
#define PIN_BT3 24
/*! \brief GPIO-Pin of the rightmost button. Button function: OK, Save. */
#define PIN_BT4 25

/*! \brief Delay after button edge detected. Avoids bounces. */
#define BT_BOUNCE_DELAY 125

//dht22 pin
#define PIN_DHT22 5

/*! \brief Send interval to the DHT22 sensor. Indoor. */
#define DHT22_SEND_INTERVAL 2000

/*! \brief Send interval to the Open Weather Map API. Outdoor. */
#define OWM_SEND_INTERVAL 300000 //300sec = 5min

//error flags
#define ERR_GPIO -1
#define ERR_LCD -2
#define ERR_THREAD -3
#define ERR_TIMEZONE -4

//menu flags
#define MENU_START 0
#define MENU_START_TIMEZONE -1
#define MENU_INDOOR 1
#define MENU_INDOOR_HISTORY 10
#define MENU_OUTDOOR 2
#define MENU_OUTDOOR_SUB_H 20 //submenu history
#define MENU_OUTDOOR_SUB_C 21 //submenu coords
#define MENU_OUTDOOR_SUB_HISTORY 200
#define MENU_OUTDOOR_SUB_COORDS_LAT 201
#define MENU_OUTDOOR_SUB_COORDS_LAT_DEC 202
#define MENU_OUTDOOR_SUB_COORDS_LON 203
#define MENU_OUTDOOR_SUB_COORDS_LON_DEC 204

//global vars
/*! \brief LCD Handle. */
extern int LCD;
/*! \brief Default scroll speed value in lists. */
#define SCROLLSPEED_DEF 300
/*! \brief Default fast scroll speed value in lists. */
#define SCROLLSPEED_FAST 50
/*! \brief Delay before fast scroll in lists will be activated. */
#define SCROLLSPEED_FAST_BTHOLD 500
/*! \brief Scroll speed in lists, default is \link SCROLLSPEED_DEF \endlink. */
extern int SCROLLSPEED;
/*! \brief Flag, whether we are currently fastscrolling. */
extern bool SCROLLSPEED_ISFAST;
/*! \brief Menu initialization. Default is \link MENU_START \endlink. */
extern int MENU;
/*! \brief Marquee position shift value. */
extern int MARQUEE_SHIFT;
/*! \brief Outdoor latitude. Integer part. */
extern int OUTDOOR_LAT;
/*! \brief Outdoor latitude. Integer part. Restore value. Used to restore to its previous value when leaving the coordinate settings. */
extern int OUTDOOR_LAT_R;
/*! \brief Outdoor latitude. Decimal part. */
extern int OUTDOOR_LAT_DEC;
/*! \brief Outdoor latitude. Decimal part. Restore value. Used to restore to its previous value when leaving the coordinate settings. */
extern int OUTDOOR_LAT_DEC_R;
/*! \brief Outdoor longitude. Integer part. */
extern int OUTDOOR_LON;
/*! \brief Outdoor longitude. Integer part. Restore value. Used to restore to its previous value when leaving the coordinate settings. */
extern int OUTDOOR_LON_R;
/*! \brief Outdoor longitude. Decimal part. */
extern int OUTDOOR_LON_DEC;
/*! \brief Outdoor longitude. Decimal part. Restore value. Used to restore to its previous value when leaving the coordinate settings. */
extern int OUTDOOR_LON_DEC_R;

//data i/o paths
/*! \brief Path to the config (.cfg) file. */
#define IO_PATH_CONFIG "config.cfg"
/*! \brief Path to the .csv file for indoor entries. */
#define IO_PATH_INDOOR "indoor.csv"
/*! \brief Path to the .csv file for outdoor entries. */
#define IO_PATH_OUTDOOR "outdoor.csv"

//plot
/*! \brief Number of entries to plot in the graph. */
#define PLOT_N_ENTRIES 7

#include <string>

/*!
 * \brief Helper class with defines, global variables and commonly used functions.
 * \copyright MIT License
 * \author 97131004
 */
class defines {
public:
	static void getCurrentTime(int* y, int* m, int* d, int* h, int* min, int* s);
};

#endif
