/*
Project:  BME280 Sensor data send over MQTT with a ESP8266 / NodeMCU with Deep-Sleep
Author:   Timm Buchholz / Kay Siemer
Date:     Last update 30.04.2021
Version:  V1.0
Arduino IDE V1.8.13
Required libraries (Tools -> manage libraries)
 - ESP8266WiFi
 - PubSubClient by Nick O'Leary V2.7.0  
 - Wire
 - Adafruit_Sensor
 - Adafruit_BME280
Required Board (Tools -> Board -> Boards Manager...)
 - Board: esp8266 by ESP8266 Community V2.6.3
SCL         D1 / GPIO5
SDA         D2 / GPIO4
*/

// ******************* Declarations *******************
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h> 
   
#define wifi_ssid "IOT"
#define wifi_password "digitalisatoren"

#define mqtt_server "192.168.1.1"
#define mqtt_user ""         
#define mqtt_password ""     

#define humidity_topic "dhbw/team5/value6"
#define temperature_topic "dhbw/team5/value5"
#define pressure_topic "dhbw/team5/value7"
    
#define durationSleep  30   // Sekunden
   
Adafruit_BME280 bme; // I2C  
WiFiClient espClient;  
PubSubClient client(espClient);  
   
float temp = 0.0;  
float hum = 0.0;  
float pressure = 0.0;  
bool status;  

// ******************* Setup *******************    
 void setup() {  
  Serial.begin(115200);   

  //initialize BME280
  status = bme.begin(0x76, &Wire);   // I2C address is either 0x76 or 0x77 // // Verbindung zum Sensor (BME280) über I2C herstellen
  if (!status) {  
     Serial.println("BME280 Sensor nicht gefunden, überprüfe die Verkabelung, Adresse, oder die Sensor ID!");  
     while (1);  
  } 
   
  setup_wifi();                   // WLAN-Verbindung herstellen 
  client.setServer(mqtt_server, 8883);  
  if (!client.connected()) {  
    reconnect();                  // MQTT-Kommunikation aufbauen
  }  
  
  client.loop();  

  // Get data from sensor  // Temperatur- & Luftfeuchtigkeitswert, sowie Luftdruck aus Sensor auslesen
  temp=bme.readTemperature();   
  hum=bme.readHumidity();  
  pressure=(bme.readPressure()/100.0F); 

  // // Temperatur- & Luftfeuchtigkeitswert, sowie Luftdruck per MQTT versenden
  Serial.print(F("Temperature = ")); 
  Serial.print( String(temp).c_str()); 
  Serial.println("C");
  client.publish(temperature_topic, String(temp).c_str(), false); 
   
  Serial.print(F("Pressure = ")); 
  Serial.print( String(pressure).c_str()); 
  Serial.println("hPa");
  client.publish(pressure_topic, String(pressure).c_str(), false);  
  
  Serial.print(F("Humidity = ")); 
  Serial.print(String(hum).c_str()); 
  Serial.println("%"); 
  client.publish(humidity_topic, String(hum).c_str(), false);
  
  delay(1000);  //Zeit zum übertrage von den MQTT Daten

  //Entering Deep Sleep Mode
  Serial.println("ESP01 geht in deep sleep");  
  ESP.deepSleep(durationSleep * 1e6);         // Schlafmodus des ESP aktivieren
 }  

// ******************* Loop *******************  
 void loop() {  
 }  

// ******************* Functions *******************   
 void setup_wifi() {  
  delay(10);  
  // We start by connecting to a WiFi network  
  Serial.println();  
  Serial.print("Connecting to ");  
  Serial.println(wifi_ssid);  
  int _try = 0;
  WiFi.begin(wifi_ssid, wifi_password);  
  while (WiFi.status() != WL_CONNECTED) {  
   Serial.print("."); 
   delay(500);  
   _try++;      

   // If WiFi-Connection fails 10 times, go to deep-sleep
   if ( _try >= 10 ) {
        Serial.println("Kann nicht mit WiFi verbunden werden, gehe in deep-sleep");
        ESP.deepSleep(durationSleep * 1e6);
    } 
  }

  // Successful connection to WiFi network  
  Serial.println("");  
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");  
  Serial.println(WiFi.localIP());  
 }  
   
 void reconnect() {   // MQTT Connection
  
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Create a random client ID
    String clientId = "ESP8266-BME280";
    
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");
      
    } 
    else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
      }
   }
}
