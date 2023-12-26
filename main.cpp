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


void setup()
{
    pinMode(redled,OUTPUT);
    pinMode(greenled,OUTPUT);
    pinMode(buzzer,OUTPUT);
    pinMode(sensor,INPUT);
    Serial.begin(9600);
    lcd.begin(16,2);
    bootstrapESP8266();
}

void loop()
{
    int analogValue= analogRead(sensor);
    Serial.print(analogValue);
  
  // Construct HTTP call.
  String httpPacket_1 = "GET " + CHANNEL_FEED_URI_1 + String(analogValue) + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n";
  int packetLength_1 = httpPacket_1.length();
  
  // Send the data to the server. Also send the packet length.
  Serial.print("AT+CIPSEND=");
  Serial.println(packetLength_1);
  delay(10);
  
  Serial.print(httpPacket_1);
  delay(10);
  
  
  if (!Serial.find("SEND OK\r\n")) return;
  
if(analogValue>sensorThresh)
{
  
digitalWrite(redled,HIGH);
digitalWrite(greenled,LOW);
tone(buzzer,1000,10000);
lcd.clear();
lcd.setCursor(0,1);
lcd.print("ALERT");
delay(1000);
lcd.clear();
lcd.setCursor(0,1);
lcd.print("EVACUATE");
delay(1000);
}
else
{
digitalWrite(greenled,HIGH);
digitalWrite(redled,LOW);
noTone(buzzer);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("SAFE");
delay(1000);
lcd.clear();
lcd.setCursor(0,1);
lcd.print("ALL CLEAR");
delay(1000);
}
  delay(500);
}

// WIP.
void close() {
  // Gracefully close the connection.
  Serial.print("AT+CIPCLOSE");
  
  // Else if fails, close anyway.
  if (!Serial.find("OK")) return;
}