#include <LiquidCrystal.h>

const int HTTP_PORT = 80; 
const String THINGSPEAK_API_KEY = "OYGQ2KBT50KDOML2"; // From our ThingSpeak settings.
const String CHANNEL_FEED_URI_1 = "/update?api_key=" + THINGSPEAK_API_KEY + "&field1="; // Write channel URI

//Ininitilzation
LiquidCrystal lcd(5,6,8,9,10,11);

// Initialize ESP8266 for internet connectivity.
String ssid = "Simulator Wifi"; // Tinkercad uses this SSID
String password = ""; // Password is not needed.
String host = "api.thingspeak.com";

//Sensor pins
int redled=3;
int greenled=2;
int buzzer=4;
int sensor= A0;
int sensorThresh=400;

/*
 * Function:  bootstrapESP8266 
 * --------------------
 * Setup ESP2866 by starting a serial connection.
 * Then use the simulator SSID and password to connect to the WiFi.
 * Establish a TCP connection with ThingSpeak over HTTP.
 * This function fast-fails if any of the above steps does not result in success.
 */
int bootstrapESP8266(void) {
  // Initialize ESP2866 and serial connection.
  Serial.begin(115200);
  Serial.println("AT");
  delay(10);
    
  if (!Serial.find("OK")) return 1;  
  
  // Connect to the internet (router) using SSID and Password.
  // If successful, returns an OK message.
  Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  delay(10);
  
  if (!Serial.find("OK")) return 2;

  // Connects to the host server (ThingSpeak) as a TCP Client.
  Serial.println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + HTTP_PORT);
}