/*
Project:  RFID Sensor data send over MQTT with a ESP8266 / NodeMCU with Deep-Sleep
Author:   Timm Buchholz / Kay Siemer
Date:     Last update 30.04.2021
Version:  V1.0
Arduino IDE V1.8.13
Required libraries (Tools -> manage libraries)
 - SPI
 - MFRC522
Required Board (Tools -> Board -> Boards Manager...)
 - Board: esp8266 by ESP8266 Community V2.6.3
*/

// ******************* Declarations *******************
#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
#define SS_PIN D8
#define RST_PIN D3
#define wifi_ssid "IOT"
#define wifi_password "digitalisatoren"
#define mqtt_server "192.168.1.1"
#define mqtt_port 8883
#define mqtt_user ""         
#define mqtt_password ""     
#define RFID_topic "dhbw/team5/value2"

const int ledPin = D4;
String lastString;
String myString ;
byte checkString[10];
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;

#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

// ******************* Setup *******************
void setup() {
  SPI.begin();       // SPI-Bussystem des ESP initialisieren       
  rfid.PCD_Init();
  
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  Serial.println("Boot RFID-Reader...");
  pinMode(ledPin, OUTPUT);

  setup_wifi();               // WLAN-Verbindung herstellen  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

// ******************* Loop *******************
void loop() {

  if (!client.connected()) {
    reconnect();              // MQTT-Kommunikation (wieder-) herstellen
  }
  client.loop();

  digitalWrite(ledPin,LOW);
  handleRFID();               // Wenn Karte gehalten & Seriennummer ausgelesen, dann versende Seriennummer per MQTT
  
}

// ******************* Functions *******************
void setup_wifi() { // WiFi Connection
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {      // MQTT Connection
  
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-RFID";
    // Attempt to connect
    if (client.connect(clientId.c_str(),mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe(RFID_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void handleRFID() {
if (!rfid.PICC_IsNewCardPresent()) return;
if (!rfid.PICC_ReadCardSerial()) return;

client.publish(RFID_topic, String(printHex(rfid.uid.uidByte, rfid.uid.size)).c_str(), false);

Serial.println(printHex(rfid.uid.uidByte, rfid.uid.size));
 
digitalWrite(ledPin,HIGH);
delay(500);
digitalWrite(ledPin,LOW);
 
rfid.PICC_HaltA();
rfid.PCD_StopCrypto1();
}
 
String printHex(byte *buffer, byte bufferSize) {
String id = "";
for (byte i = 0; i < bufferSize; i++) {
id += buffer[i] < 0x10 ? "0" : "";
id += String(buffer[i], HEX);
}
return id;
}
