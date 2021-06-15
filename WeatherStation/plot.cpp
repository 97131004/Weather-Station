#include <vector>
#include <string>
#include <tuple>
#include <utility>
#include <wiringPi.h>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <type_traits>
#include <memory>

#include "containers.h"
#include "defines.h"
#include "plot.h"

/*!
 * \brief Creates a pipe to the MIT. Continuously plots a number of dynamically added entries in a plot window.
 * \param window Plot window ID, only needed for multiple plot windows
 * \param srcVec Source vector, e.g. \link INDOOR_VEC \endlink or \link OUTDOOR_VEC \endlink
 * \param delay_ Time to wait to refresh the plot
 * \param title Plot window title
 */
template<typename T>
void plot::plotWindow(const int& window, std::vector<T>& srcVec, const int& delay_, const std::string& title) {
	
	if (!std::is_same<T, indoorEntry>::value && !std::is_same<T, outdoorEntry>::value) return;
	
	int x = 1;

	FILE *pipe = popen("MIT", "w");
	
	fprintf(pipe,"set term wxt %d noraise title '%s Temperature'\n", window, title.c_str()); 
	fprintf(pipe,"set grid\n");
	fprintf(pipe,"set xrange [-1:1]\n");
	fprintf(pipe,"set yrange [-1:1]\n");
	fprintf(pipe,"set xtics rotate by -45\n");
	fprintf(pipe,"set xlabel 'Date and time'\n");
	fprintf(pipe,"set ylabel 'Temperature in °C'\n");

	while(true) {
		
		if (!srcVec.empty()) {
			//copying x last lines from INDOOR_VEC/OUTDOOR_VEC into another vector
			std::vector<T> vec(srcVec.end() - std::min(srcVec.size(), (size_t)PLOT_N_ENTRIES), srcVec.end());
			
			if (!vec.empty()) {
				
				T *iVec = &vec.back(); //fetching last vector item to get its temp, so we can set a valid y-range
				float y = iVec->celsius;
				
				fprintf(pipe,"set xrange [%d:%d]\n", x, x + PLOT_N_ENTRIES);
				fprintf(pipe,"set yrange [%f:%f]\n", -2 + y, 2 + y);
				
				fprintf(pipe, "unset label\n");
				
				int x_ = x;
				for (size_t i = 0; i < vec.size(); i++) {
					fprintf(pipe, "set label '%s' at %d,%f center\n", vec[i].celsiusStr.c_str(), x_, vec[i].celsius + 0.1);
					x_++;
				}
				
				fprintf(pipe,"plot '-' using 1:3:xticlabels(2) with linespoints notitle\n");
				
				for (size_t i = 0; i < vec.size(); i++) {
					fprintf(pipe, "%d %s %f\n", x, (vec[i].dateStr + "," + vec[i].timeStr).c_str(), vec[i].celsius);
					x++;
				}
				
				fprintf(pipe,"e\n"); //e is a maker to close input that will be sent via pipe
				fflush(pipe); //flush the pipe to update the plot
			}
		}

		delay(delay_); //wait some time
	}

	fclose(pipe);
}

/*!
* \brief Thread that starts plotting the \link INDOOR_VEC \endlink.
*/
PI_THREAD(plotIndoor) {
	
	plot::plotWindow<indoorEntry>(0, INDOOR_VEC, DHT22_SEND_INTERVAL, "Indoor");
	return (int*)-1; //returning error if program reaches this line, also bypasses compiler warnings
}

/*!
* \brief Thread that starts plotting the \link OUTDOOR_VEC \endlink.
*/
PI_THREAD(plotOutdoor) {
	
	//using dht22 interval, to handle outdoor_vec sync, (otherwise we'd have to wait 5min when outofsync)
	plot::plotWindow<outdoorEntry>(1, OUTDOOR_VEC, DHT22_SEND_INTERVAL, "Outdoor");
	return (int*)-1; //returning error if program reaches this line, also bypasses compiler warnings
}

/*!
* \brief Creating plot threads.
*/
int plot::create() {
	return piThreadCreate(plotIndoor) || piThreadCreate(plotOutdoor);
}
