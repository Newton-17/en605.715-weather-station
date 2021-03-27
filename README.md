# ESP8266 Weather Station

## Description
The purpose of this library is to contain the set of code for the ESP8266 for interfacing with a DHT11 temperature sensor. It also contains a webserver on ESP8266 for sharing the csv temperature data between applications.

## Files
### src/temp_sensor.ino
* Reads Temperature & Humidity via DHT11
* Saves time, temperature, humidity to CSV formatted char array
* Connects to wifi to utilize NTP for time
* Spins up a webserver for sharing the information accessible via <i>http://\<ip>/</i>
* Connects and Initializes the Display

### plot_csv.ipynb
* Jupyter Notebook for connecting to the sensor
* Parses CSV
* Plots via Matplotlib 2 charts humidity vs time and temperature vs time