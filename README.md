# Weather Station

Weather Station made with Raspberry Pi for indoor and outdoor tempereature measurements. Outdoor temperature is measured via the Internet using a Open Weather Map API. Indoor temperature is measured using a DHT22 sensor. An additional LCD display visualizes the date, time and other weather data for a certain location using an Internet connection. The tool visualizes all measurements in multiple temperature-time diagrams on a monitor connected to the Raspberry. The tool also logs all measurements into a .csv file on the Raspberry.

![rasp1](https://github.com/notpavlov/Weather-Station/blob/main/rasp1.jpg?raw=true)

![rasp2](https://github.com/notpavlov/Weather-Station/blob/main/rasp2.jpg?raw=true)

## Specifications and structure

- Raspberry Pi 3 Model B+
- DHT22 sensor for indoor temperature recordings (using dht22-Lib)
- LCD display (Standard HD44780) for visualization of the date, time, weather data
- 4 pull-down buttons for menu navigation (with wiringPi)
- Visualization of temeparature-time diagrams (with MIT) on monitor via HDMI to the Raspberry

**Fritzing sketch of the Hardware**
![fritzing](https://github.com/notpavlov/Weather-Station/blob/main/fritzing.png?raw=true)

## Installation

Compile the program using:
```
g++ -o WeatherStation *.cpp -l wiringPi -l wiringPiDev -l curl -l jsoncpp
```

## Execution

Execute the progam using the command line:

```
./WeatherStation
```

## License

MIT