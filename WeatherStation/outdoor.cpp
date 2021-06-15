#include <wiringPi.h>
#include <fstream>
#include <iostream>
#include <ios>
#include <sstream>
#include <cstdlib>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>
#include <algorithm>
#include <exception>
#include <curl/curl.h>
#include <stdio.h>

#include "defines.h"
#include "containers.h"
#include "outdoor.h"
#include "outdoorEntry.h"
#include "dht22.h"

/*!
 * \brief Loading .csv file from \link IO_PATH_OUTDOOR \endlink using the line callback \link outdoor::parseLineIndoor \endlink.
 */
void outdoor::csvLoad() {
	containers::csvLoadAll(IO_PATH_OUTDOOR, outdoor::parseLineOutdoor);
}

/*!
 * \brief Callback on a read line from the .csv file. Parsing, creating \link outdoorEntry \endlink and filling \link OUTDOOR_VEC \endlink.
 * \param ss Line as a std::stringstream*
 */
void outdoor::parseLineOutdoor(std::stringstream* ss) {
	
	std::string cell;
	int cellIncr = 0; //cell incr
	outdoorEntry e; //declaring entry obj
	
	while(*ss) { //while not eol
	
		if (!std::getline(*ss, cell, ',')) break; //get cell

		switch(cellIncr) {
			case 0: e.day = std::stoi(cell); break;
			case 1: e.month = std::stoi(cell); break;
			case 2: e.year = std::stoi(cell); break;
			case 3: e.hour = std::stoi(cell); break;
			case 4: e.minute = std::stoi(cell); break;
			case 5: e.second = std::stoi(cell); break;
			case 6: e.celsius = std::atof(cell.c_str()); break;
			case 7: e.fahrenheit = std::atof(cell.c_str()); break;
			case 8: e.humidity = std::atof(cell.c_str()); break;
			case 9: e.pressure = std::stoi(cell); break;
			case 10: e.windspeed = std::atof(cell.c_str()); break;
			case 11: e.status = cell; break;
		}
		
		cellIncr++; //next cell
	}
	
	e.update();
	OUTDOOR_VEC.push_back(e);
}

/*!
 * \brief Format and append \link outdoorEntry \endlink to a .csv file. 
 * \param ie Entry to append
 */
void outdoor::saveEntry(const outdoorEntry* oe) {
	
	std::stringstream line;
	line << oe->day << "," << oe->month << "," << oe->year << ",";
	line << oe->hour << "," << oe->minute << "," << oe->second << ",";
	line << oe->celsius << "," << oe->fahrenheit << "," << oe->humidity << ",";
	line << oe->pressure << "," << oe->windspeed << ",";
	line << oe->status << "\n";
	
	containers::csvAppendLine(IO_PATH_OUTDOOR, line.str());
}

/*!
 * \brief Writes response string to a user-defined string pointer passed in \link outdoor::sendHTTPRequest \endlink.
 * 
 * See https://curl.haxx.se/libcurl/c/CURLOPT_WRITEFUNCTION.html for more details.
 */
size_t outdoor::writeHTTPresponse(char *contents, size_t size, size_t nmemb, void *userp) {
	// http://www.cplusplus.com/reference/string/string/append/
    ((std::string*)userp)->append(contents, size * nmemb);
    return size * nmemb;
}

/*!
 * \brief Sends a GET-Request to a certain URL using cURL. Runs synchronously. Stops execution of the thread until response received. Calls \link outdoor::writeHTTPresponse \endlink to write response.
 * \param url URL to send the request to
 * \param reponse String pointer, where the reponse string will be written to
 * 
 * See https://curl.haxx.se/libcurl/c/CURLOPT_WRITEDATA.html for more details.
 */
int outdoor::sendHTTPRequest(const std::string& url, std::string& response) {

	//init libcurl
	curl_global_init(CURL_GLOBAL_ALL);
	
    CURL* curl = curl_easy_init();
    
    int exitcode = 0;
    
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //optional debug info
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, outdoor::writeHTTPresponse); //callback to write response string
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

		if (curl_easy_perform(curl) != CURLE_OK) { //will be called sync
			exitcode = -1;
		}
		curl_easy_cleanup(curl);
	}
	else exitcode = -2;
	
    return exitcode;
}

/*!
 * \brief Thread that sends a GET-Request to the OpenWeatherMap-API, parses its JSON-Response, creates an \link outdoorEntry \endlink, fills \link OUTDOOR_VEC \endlink and appends it to a .csv file.
 */
PI_THREAD(outdoorGet) {
	
	while(true) {
		
		outdoorEntry oe;
		
		//send get-request
		
		std::string url, response;
		
		url = "http://api.openweathermap.org/data/2.5/weather?id=524901&APPID=***";
		url += "&lat=" + std::to_string(OUTDOOR_LAT_R) + "." + std::to_string(OUTDOOR_LAT_DEC_R);
		url += "&lon=" + std::to_string(OUTDOOR_LON_R) + "." + std::to_string(OUTDOOR_LON_DEC_R);
		url += "&units=metric";

		outdoor::sendHTTPRequest(url, response);
		
		//parse json response
		
		Json::Value root;
		Json::Reader reader;
		
		if (reader.parse(response.c_str(), root))
		{	
			try { //try-catch, because not all of the keys are always present
				oe.celsius = std::atof((root["main"]["temp"].asString()).c_str());
				oe.humidity = std::atof((root["main"]["humidity"].asString()).c_str());
				oe.pressure = std::stoi(root["main"]["pressure"].asString());
				oe.windspeed = std::atof((root["wind"]["speed"].asString()).c_str());
				
				Json::Value weather = root["weather"];
				std::string status = weather[0]["description"].asString();
				status[0] = toupper(status[0]); //capital first letter
				oe.status = status;
				
				oe.update();
				OUTDOOR_VEC.push_back(oe);
				outdoor::saveEntry(&oe);
			}
			catch(...) {
				std::cout << "[ERROR ON JSON PARSE IN OUTDOOR.CPP]" << std::endl;
			}
		}
		
		delay(OWM_SEND_INTERVAL);
	}
}

/*!
 * \brief Creating outdoor thread.
 */
int outdoor::create() {
	return piThreadCreate(outdoorGet);
}
