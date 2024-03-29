//Incluimos librerías externas
//--------------------------------------------------------
#include <DHT.h>
#include <AsyncUDP.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <vector>
//--------------------------------------------------------


//Incluimos cabeceras de nuestras clases
//--------------------------------------------------------
#include "SoundSensor.h"
#include "TemperatureSensor.h"
#include "LedLight.h"
#include "ESP32Abstract.h"
//--------------------------------------------------------

//Realizamos nuestras definiciones que no ocupan espacio, acciones de compilador
//--------------------------------------------------------------------------------------
#define TEMPERATUREPIN 17
#define SOUNDPIN 34
#define LEDPIN 2
#define udpPort 6230
//#define ssid "TP-LINK_6B36"
//#define password "89776513"
// #define ssid "Redmi_Alex"
// #define password "abcde4567"
#define ssid "darkasa"
#define password "0Spoilerspls"
//--------------------------------------------------------------------------------------


LedLight* ledLight;
SoundSensor* soundSensor;
TemperatureSensor* temperatureSensor;
ESP32Abstract* esp32Abstract;

void setup(){
  Serial.begin(115200);
  // Inicializamos las instancias dentro de setup
  esp32Abstract = ESP32Abstract::getInstance(ssid, password, udpPort, TEMPERATUREPIN, SOUNDPIN, LEDPIN);
}


void loop() {
  esp32Abstract->listenForMessages();
  delay(4000);
  
}

