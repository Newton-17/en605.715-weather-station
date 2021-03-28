#include <Adafruit_Sensor.h>
#include "DHTesp.h"
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SSD1306Wire.h>

// DHT Pin & Object
#define DHTPIN 5              // Digital pin connected to the DHT sensor
#define DHTTYPE DHTesp::DHT11 // DHT 11
DHTesp dht;

// Print Interval & Interrupt Interval
const long interval = 5000;
const long interruptInterval = 3125000;
unsigned long previousMillis = 0; // will store last time DHT was updated

// Interrupt Flag for Measuring
bool interruptFlag = 0;
unsigned long interruptMillis = 0;

// current time, temperature & humidity, updated in loop()
unsigned long now = 0;
float t = 0.0;
float h = 0.0;

// CSV String
char csvData[20480] = "timestamp,temp,humidity\n";

// SSID Info for NTP & Webserver
const char* ssid = "NuggetsInParadise";
const char* password = "<PASSWORD>";

// Time Info
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// Wifi Server on Local IP:80
AsyncWebServer server(80);

// Display Variable
SSD1306Wire display(0x3c, 0, 2);
String defaultDisplayString = "Welcome to the \n Weather Station";

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

// Handles updating time and returning epoch
unsigned long getTime() {
  timeClient.update();
  unsigned long now = timeClient.getEpochTime();
  return now;
}

// Interrupt Function
void ICACHE_RAM_ATTR onTimerISR()
{
  char tmp [22];
  // Save the Data in CSV Format

  sprintf(tmp, "%i,%.2f,%.2f\n", now, t, h);

  strcat(csvData, tmp);
  interruptFlag = 1;
}

void writeTempHumidity(){
      char tempBuffer[50];
      display.clear();
      sprintf(tempBuffer, "Temperature: %.2f\nHumdity: %.2f", t, h);
      display.drawString(0, 0, tempBuffer);
      display.display();
}

void setup()
{
  // Start Serial Monitor
  Serial.begin(74880);

  // Connect & Print Wifi Info
  initWiFi();
  timeClient.begin();

  // Setup Get Server Request
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", csvData);
    });
  server.begin();

  // Init DHT Setup w/ Pin & DHT TYPE
  dht.setup(DHTPIN, DHTTYPE);

  // Create Initial Interrupt for Saving Data
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP);
  timer1_write(interruptInterval);

  // Initialize Display & Set Default Font
  display.init();
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, defaultDisplayString);
  display.display();

  // Initial Intro Printout
  Serial.println();
  Serial.println("Welcome to Temp Reader.");
  Serial.println("To Get Past Values Input: VALUES");
  Serial.println("To Exit input: STOP");
}

void loop()
{
  if (Serial.available() > 0)
  {
    // read the incoming byte:
    String incoming_string = Serial.readString();

    // Add Check for Exit Code
    char incoming_string_buf[incoming_string.length() + 1];
    incoming_string.toCharArray(incoming_string_buf, incoming_string.length() + 1);
    if (strcmp(incoming_string_buf, "STOP") == 0)
    {
      Serial.println("Exiting...");
      exit(0);
    }
    else  if (strcmp(incoming_string_buf, "VALUES") == 0)
    {
      timer1_disable();
      Serial.println("VALUES...");
      Serial.print(csvData);
      timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP);
    }
  }
  unsigned long currentMillis = millis();

  if (interruptFlag)
  {
    Serial.print("Interupt Timing: ");
    Serial.println(currentMillis - interruptMillis);
    interruptMillis = currentMillis;
    interruptFlag = 0;
  }

  if (currentMillis - previousMillis >= interval) {
    timer1_disable();
    // Reset Millis for spacing
    previousMillis = currentMillis;

    // Set Current Read Time
    now = getTime();

    // Get Current Temperature
    float newT = dht.getTemperature();
    if (! isnan(newT)) {
      t = dht.toFahrenheit(newT);
    }
    // Get Current Humidity
    float newH = dht.getHumidity();
    // if humidity read failed, don't change h value 
    if (! isnan(newH)) {
      h = newH;
      writeTempHumidity();
    }
    timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP);
  }
}