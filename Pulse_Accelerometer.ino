//Created by Glenn Gil David Tamayo
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <SparkFun_ADXL345.h>         // SparkFun ADXL345 Library
#include <EEPROM.h>
#include <Wire.h>

#define ADXL345_FIFOMODE_BYPASS       0x0
#define ADXL345_FIFOMODE_FIFO         0x5F
#define ADXL345_FIFOMODE_STREAM       0x9F
#define ADXL345_FIFOMODE_TRIGGER      0xDF

/***************** ARDUINO PIN CONFIGURATION ****************/
//const int adxlSCLPin = D1;
//const int adxlSDAPin = D2;
const int adxlPin = D0;
const int INT1Pin = D1;
/***************** ARDUINO PIN CONFIGURATION ****************/

/******************** CLASS CONSTRUCTORS ********************/
ADXL345 adxl = ADXL345(D0);
/******************** CLASS CONSTRUCTORS ********************/

/********************* GLOBAL VARIABLES *********************/
const char* ssid = "Pulse_GGT";
const char* password = "Accel_01";

volatile bool isDataReady = 0;
byte response;
char deviceNameChar[16];
struct adxl345Data {
  char node[16];
  int xOffset;
  int yOffset;
  int zOffset;
  float xGain;
  float yGain;
  float zGain;
};
int AccelMinX = 0;
int AccelMaxX = 0;
int AccelMinY = 0;
int AccelMaxY = 0;
int AccelMinZ = 0;
int AccelMaxZ = 0; 
int accX = 0;
int accY = 0;
int accZ = 0;
int xOffset = 0;
int yOffset = 0;
int zOffset = 0;
float xGain = 1;
float yGain = 1;
float zGain = 1;
int offsetX = 0;
int offsetY = 0;
int offsetZ = 0;

float gainX = 0;
float gainY = 0;
float gainZ = 0;

int xAdj = 0;
int yAdj = 0;
int zAdj = 0;

unsigned long previousMillis = 0;

//TEMP
int counter;
unsigned long previousMillisCounter = 0;
double loopRate = 0;
int readingCounter = 0;
double readingRate = 0;
//TEMP
/********************* GLOBAL VARIABLES *********************/

void setup(){
  Serial.begin(115200);
  //Wire.begin(adxlSDAPin, adxlSCLPin);
  pinMode(INT1Pin, INPUT);
  //BasicOTA();
  AdxlSetup(ADXL345_BW_1600, ADXL345_FIFOMODE_BYPASS);
  EEPROMSetup();
  readOffsetsFromEEPROM();
}

void loop(){
  //take measurements from Accelerometer unless user presses 'C'
  while (!Serial.available()){
    counter++;
    ArduinoOTA.handle();
    unsigned long currentMillis = millis();
    if ((isDataReady == 1) || (currentMillis - previousMillis > 20)) {
      int x,y,z;
      adjAccel(x, y, z);
      //adxlToSerial(xAdj, yAdj, zAdj);
      readingCounter++;
      previousMillis = currentMillis;
      isDataReady = 0;
    }
    if (currentMillis - previousMillisCounter > 1000) {
      Serial.print("Counter: ");
      Serial.print(counter);
      Serial.print(" .  Reading Counter: ");
      Serial.println(readingCounter);
      counter = 0;
      readingCounter = 0;
      previousMillisCounter = currentMillis;
    }
  }

  //read response if user presses any key. If it is 'C', then go to calibration mode
  while (Serial.available()) {
    response = Serial.read();     
    if((response == 67) || (response == 99)) {  //C || c
      calibrateADXL();  
    }               
  }
}
