void EEPROMSetup() {
 EEPROM.begin(512); 
}

void readOffsetsFromEEPROM(){
  adxl345Data getAccelData;
  EEPROM.get(0, getAccelData);
  Serial.println("Successfully written to EEPROM:");
  Serial.println(getAccelData.node);
  Serial.println(getAccelData.xOffset);
  Serial.println(getAccelData.yOffset);
  Serial.println(getAccelData.zOffset);
  Serial.println(getAccelData.xGain);
  Serial.println(getAccelData.yGain);
  Serial.println(getAccelData.zGain);
}

void writeOffsetsToEEPROM() {
  adxl345Data accelData;
  strcpy(accelData.node, deviceNameChar);
  accelData.xOffset = offsetX;
  accelData.yOffset = offsetY;
  accelData.zOffset = offsetZ;
  accelData.xGain = gainX;
  accelData.yGain = gainY;
  accelData.zGain = gainZ;

  EEPROM.put(0, accelData);  
}
