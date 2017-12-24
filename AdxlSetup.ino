/*
#include <SparkFun_ADXL345.h>         // SparkFun ADXL345 Library
*/

/*
#define ADXL345_FIFOMODE_BYPASS       0x0
#define ADXL345_FIFOMODE_FIFO         0x5F
#define ADXL345_FIFOMODE_STREAM       0x9F
#define ADXL345_FIFOMODE_TRIGGER      0xDF
*/


void AdxlSetup(byte rate, byte FIFOMode){

  adxl.writeTo(ADXL345_BW_RATE, rate);

  adxl.writeTo(ADXL345_FIFO_CTL, FIFOMode);

  adxl.setRangeSetting(2);           // Give the range settings
                                      // Accepted values are 2g, 4g, 8g or 16g
                                      // Higher Values = Wider Measurement Range
                                      // Lower Values = Greater Sensitivity

  adxl.setSpiBit(0);                  // Configure the device to be in 4 wire SPI mode when set to '0' or 3 wire SPI mode when set to 1
                                      // Default: Set to 1
                                      // SPI pins on the ATMega328: 11, 12 and 13 as reference in SPI Library 
  
  

  attachInterrupt(digitalPinToInterrupt(INT1Pin), ADXL_ISR, RISING);   // Attach Interrupt

  adxl.writeTo(ADXL345_INT_ENABLE, 0b10000000);
  delay(1000);
  adxl.writeTo(ADXL345_POWER_CTL, 0x8); //MEASURE MODE  
    
}

void adjAccel(int x, int y, int z) {
  adxl.readAccel(&x, &y, &z);
  xAdj = (float)(x - xOffset) / xGain;
  yAdj = (float)(y - yOffset) / yGain;
  zAdj = (float)(z - zOffset) / zGain;  
}

void adxlToSerial(int x, int y, int z) {
  // Output Results to Serial
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print(", ");
  Serial.println(z);   
}

void ADXL_ISR() {
  byte interrupts = adxl.getInterruptSource();
  
  if(adxl.triggered(interrupts, ADXL345_DATA_READY)){
    isDataReady = 1;
  } 
  
}

