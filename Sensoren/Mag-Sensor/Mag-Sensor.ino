/*
Project:  Magnetic Window sensor data send over MQTT with a ESP8266 / NodeMCU with Deep-Sleep
Author:   Timm Buchholz / Kay Siemer
Date:     Last Update 30.04.2021
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
   
#define wifi_ssid "IOT"
#define wifi_password "digitalisatoren"

#define mqtt_server "192.168.1.1"
#define mqtt_port 8883
#define mqtt_user ""         
#define mqtt_password ""

#define Mag_topic "dhbw/team5/value4"
    
#define durationSleep  30   // Einheit: Sekunden

#define inPin D1           // Festlegung des Input-Pins
   
WiFiClient espClient;  
PubSubClient client(espClient);  
   
int value = 0;
bool mag_state = 0;    
bool status;  

// ******************* Setup *******************
void setup() {
  pinMode(inPin, INPUT_PULLUP);     // Anschlusspin als Input mit PullUp-Widerstand definieren
  
  Serial.begin(115200);                     
  setup_wifi();                     // WLAN-Verbindung herstellen
  client.setServer(mqtt_server, mqtt_port);
    
  if (!client.connected()) {  // If client isn't connected, establish MQTT communication
    reconnect();                    // MQTT-Kommunikation aufbauen
  }  
  
  client.loop();                    // ?

  // Get data from sensor
  mag_state = 0;
  mag_state = !(digitalRead(inPin));  // Fensterstellung digital auslesen

  // MQTT Communication
  client.publish(Mag_topic, String(mag_state).c_str(), false);     // Fensterstellung per MQTT versenden
  Serial.println("Window position:"); 
  Serial.println(mag_state);  // Fensterstellung ausgeben
  delay(1000);  // Zeit zur Übertragung der MQTT Daten

  // Entering Deep Sleep Mode
  Serial.println("Deep Sleep Mode CO2-Sensor");  
  ESP.deepSleep(durationSleep * 1e6);                             // Schlafmodus des ESP aktivieren
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
    String clientId = "ESP8266-Mag-Sensor";
    
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
