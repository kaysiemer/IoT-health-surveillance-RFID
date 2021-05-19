/*
Project:  MH-Z19 Sensor data send over MQTT with a ESP8266 / NodeMCU with Deep-Sleep
Author:   Timm Buchholz / Kay Siemer
Date:     Created 29.04.2021
Version:  V1.0
Arduino IDE V1.8.13
Required libraries (Tools -> manage libraries)
 - ESP8266WiFi
 - PubSubClient by Nick O'Leary V2.7.0  
 - Wire
Required Board (Tools -> Board -> Boards Manager...)
 - Board: esp8266 by ESP8266 Community V2.6.3
*/

// ******************* Declarations *******************
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "MHZ19.h"

const int rx_pin = D1; //Serial rx pin no
const int tx_pin = D2; //Serial tx pin no
MHZ19 *mhz19_uart = new MHZ19(rx_pin,tx_pin);

#define outPin D3 
   
#define wifi_ssid "IOT"
#define wifi_password "digitalisatoren"

#define mqtt_server "192.168.1.1"
#define mqtt_port 8883
#define mqtt_user ""         
#define mqtt_password ""     

#define CO2_topic "dhbw/team5/value1"
    
#define durationSleep  30   // Einheit: Sekunden
   
WiFiClient espClient;  
PubSubClient client(espClient);  
   
float CO2 = 0.0;    
bool status;  

// ******************* Setup *******************
void setup() {
  Serial.begin(115200);                     
  setup_wifi();   // WLAN-Verbindung herstellen  
  client.setServer(mqtt_server, mqtt_port);
    
  if (!client.connected()) {  // If client isn't connected, establish MQTT communication
    reconnect();              // MQTT-Kommunikation aufbauen
  }  

  pinMode(outPin, OUTPUT);
  digitalWrite(outPin, HIGH);   // Transistor durchsteuern
  client.loop();              // ?

  // Sensor initialisation
  Serial.println("Initializing");
  mhz19_uart->begin(rx_pin, tx_pin);  // Verbindung zum CO2-Sensor über UART herstellen
  mhz19_uart->setAutoCalibration(true); // CO2-Sensor Autokalibrierung aktivieren
  delay(3000); // Issue #14
  Serial.print("MH-Z19 now warming up...  status:");
  Serial.println(mhz19_uart->getStatus());
  delay(1000);
  Serial.println("Leaving Setup");

  // Get data from sensor
  measurement_t m = mhz19_uart->getMeasurement();   // CO2-Wert aus Sensor auslesen

  // MQTT Communication
  client.publish(CO2_topic, String(m.co2_ppm).c_str(), false);   // CO2-Wert per MQTT versenden
  delay(1000);  // Zeit zur Übertragung der MQTT Daten

  digitalWrite(outPin, LOW);   // Transistor sperren

  // Entering Deep Sleep Mode
  Serial.println("Deep Sleep Mode CO2-Sensor");  
  ESP.deepSleep(durationSleep * 1e6);                 // Schlafmodus des ESP aktivieren
 }  

// ******************* Loop *******************
void loop() {  
 }  

// ******************* Functions *******************
void setup_wifi() {  // WiFi Connection
  delay(10);
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
        Serial.println("WiFi-Connection unsuccessful, going to deep-sleep mode");
        ESP.deepSleep(durationSleep * 1e6);
    } 
  }

  // Successful connection to WiFi network 
  Serial.println("");  
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");  
  Serial.println(WiFi.localIP());  
 }  

void reconnect() {    // MQTT Connection
  
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Client ID
    String clientId = "ESP8266-CO2-Sensor";
    
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
