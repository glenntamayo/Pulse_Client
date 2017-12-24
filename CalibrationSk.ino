void calibrateADXL() {
  Serial.println("Send: ");
  Serial.println("'C' to calibrate,");
  Serial.println("'M' to measure,");
  while (!Serial.available()){}       // Waiting for character to be sent to Serial
  while (Serial.available()) {
    response = Serial.read();  
    if((response == 67) || (response == 99)) {  //C || c
      while ((response == 67) || (response == 99) || (response == 78) || (response == 110)){ //C || c || N || n
        calibrateStart();
        Serial.println("'N' for next position; OR 'Y' to end calibration and write values.");    
        while (!Serial.available()){}       // Waiting for character to be sent to Serial
        while (Serial.available()) {
          response = Serial.read();  
          if((response == 89) || (response == 121)) { //Y || y
            calibrateEnd();
          }
        }
      }
    }    
  }
}  

void calibrateStart() {
   // Get the Accelerometer Readings
  int x,y,z;                          // init variables hold results
  adxl.readAccel(&x, &y, &z);         // Read the accelerometer values and store in variables x,y,z

  if(x < AccelMinX) AccelMinX = x;
  if(x > AccelMaxX) AccelMaxX = x;

  if(y < AccelMinY) AccelMinY = y;
  if(y > AccelMaxY) AccelMaxY = y;

  if(z < AccelMinZ) AccelMinZ = z;
  if(z > AccelMaxZ) AccelMaxZ = z;

  Serial.print("Accel Minimums: "); Serial.print(AccelMinX); Serial.print("  ");Serial.print(AccelMinY); Serial.print("  "); Serial.print(AccelMinZ); Serial.println();
  Serial.print("Accel Maximums: "); Serial.print(AccelMaxX); Serial.print("  ");Serial.print(AccelMaxY); Serial.print("  "); Serial.print(AccelMaxZ); Serial.println();
  
  /* Note: Must perform offset and gain calculations prior to seeing updated results
  /  Refer to SparkFun ADXL345 Hook Up Guide: https://learn.sparkfun.com/tutorials/adxl345-hookup-guide
  /  offsetAxis = 0.5 * (Acel+1g + Accel-1g)
  /  gainAxis = 0.5 * ((Acel+1g - Accel-1g)/1g) */

  offsetX = 0.5 * (AccelMaxX + AccelMinX);
  offsetY = 0.5 * (AccelMaxY + AccelMinY);
  offsetZ = 0.5 * (AccelMaxZ + AccelMinZ);
  Serial.print("Offsets: "); Serial.print(offsetX); Serial.print("  "); Serial.print(offsetY); Serial.print("  ");  Serial.println(offsetZ);
  gainX = 0.5 * (AccelMaxX - AccelMinX) / 256;
  gainY = 0.5 * (AccelMaxY - AccelMinY) / 256;
  gainZ = 0.5 * (AccelMaxZ - AccelMinZ) / 256;
  Serial.print("Gains: "); Serial.print(gainX); Serial.print("  "); Serial.print(gainY); Serial.print("  ");  Serial.println(gainZ);;
  Serial.println();
}

void calibrateEnd() {
  Serial.print("Enter device name: ");
  while (!Serial.available()){}       // Waiting for character to be sent to Serial
  char character;
  String deviceName = "";
  delay(500);
  while(Serial.available()) {
    character = Serial.read();
    deviceName.concat(character);
  }
  if (deviceName != "") {
    Serial.println(deviceName);
  }
  deviceName.toCharArray(deviceNameChar, deviceName.length()+1);
  writeOffsetsToEEPROM();  
  delay(1000);
  readOffsetsFromEEPROM();
}
