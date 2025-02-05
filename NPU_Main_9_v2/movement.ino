//Walk function
void walk(int left, int right) {
  int l, r;

  //limits the input to 1023
  if (left > 1023)left=1023;
  if (left < -1023)left=-1023;
  if (right > 1023)right=1023;
  if (right < -1023)right=1023;

  //dynamixel works from 0-1023 clock wise and 1024-2047 counterclock wise. This code makes both side work as "positive to 1023" = CW and "negative to 1023" CCW.
  //note it's reversed for the other side as each motor is pointing a direction on the robot
  if (left > 0) {
    l = left;
  } else l = 1024 + abs(left);
  
  if (right > 0) {
    r = right + 1024; //right is slightly faster, so needs a small correction for the straight walk
  } else r = abs(right);

  //aplies the function
  dxl.setGoalVelocity(DXL_ID, l, UNIT_RAW);
  dxl2.setGoalVelocity(DXL_ID2, r, UNIT_RAW);
}

// get dynamixel linear velocity based on angular velocity
float getDynaVelocity(int a) {
  float R = 26.0; 
  float rpm = (114.0 * a) / 102.3;
  float v = (2*PI*R*rpm)/1;

  return v;
}


//Walk back function
void back(int vel) {
  walk(-vel, -vel);
}


//turn right function
void right(int vel)  {
  walk(vel, -vel);
}


//turn left function
void left(int vel) {
  walk(-vel, vel);
}


//Stop for "del" moment
void freeze(int del) {
  walk(0,0); 
  delay(del);
}


//positive values = turn left
void turn(int alfa) {
  //sensor addres and value of Z axis of gyroscope
  int16_t GyZ=0;

  double total_alfa = 0.0, dimensional_error = 1.08; //error calculated by the difference in degree 

  //timer variables
  unsigned long previoustimer = millis();
  double elapsedtime;
  
  //while it didn't walk to the said angle "alfa"
  while(abs(total_alfa) <= abs(alfa)){

    //the elapsed time
    elapsedtime = (double)(millis() - previoustimer) / 1000.0; //as the scale is degrees/seconds, we divided the time in millis by 1000
    previoustimer = millis();
    
    //transmission on wire library
    Wire.beginTransmission(0x68);
    Wire.write(0x47); //0x47 (GYRO_ZOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(0x68, 2, true); //each axis value is stored in 2 registers
    
    //put the value received on the variable
    GyZ = (Wire.read()<<8|Wire.read()); //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
    GyZ -=  GYZ_ERROR; //small correction

    // if (abs(GyZ) < 350)GyZ = 0;
    
    total_alfa += (double)GyZ * dimensional_error * elapsedtime / 131.0; //scale in degrees/seconds. For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet

    //debug
    //Serial.println(GyZ);
    
    //turns to left when positive, right when negative
    if (alfa < 0)walk(SWL, -SWR);
    if (alfa > 0)walk(-SWL, SWR);

    //debug
    Serial.println(total_alfa);
  }
  //stops
  freeze(50);
}


//need to send walk before
void Simple_turn(int alfa) 
{
  //sensor addres and value of Z axis of gyroscope
  int16_t GyZ=0;
  float total_alfa = 0.0, dimensional_error = 1.0357; //error calculated by the difference in degree 
  //timer variables
  unsigned long previoustimer = millis();
  float elapsedtime;
  //while it didn't walk to the said angle "alfa"
  while( abs(total_alfa) <= abs(alfa) )
  {
    //the elapsed time
    elapsedtime = (float)(millis() - previoustimer) / 1000.0; //as the scale is degrees/seconds, we divided the time in millis by 1000
    previoustimer = millis();
    //transmission on wire library
    Wire.beginTransmission(0x68);
    Wire.write(0x47); //0x47 (GYRO_ZOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(0x68, 2, true); //each axis value is stored in 2 registers
    //put the value received on the variable
    GyZ = (Wire.read()<<8|Wire.read()); //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
    GyZ -=  GYZ_ERROR; //small correction
    total_alfa += (float)GyZ * dimensional_error * elapsedtime / 131.0; //scale in degrees/seconds. For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
    //debug
    //Serial.println(GyZ);
    //debug
    //Serial.println(total_alfa);
  }
}



//uses mercury sensor to detect inclination
void DetectInclination() {
  bool getup=0;
  if (digitalRead(MERC))ram++;
  else ram = 0;
  if (ram >= 3){
    freeze(50);
    set_servo(1, 180);
    ram=5;
    Serial.println("abaixei");
    }
  while (ram >= 3)  {
    Serial.println(ram);
    // while(1)freeze(25);
    array_read();
    PIDwalk(0.6);
    getup=1;
    if (digitalRead(MERC) && ram < 10)ram++;
    else if (!digitalRead(MERC))ram--;
    delay(25);
  }

  if (getup){freeze(50);set_servo(1, 10);getup=0;}
}


//detect accelerometer
float DetectAccel() {
  //sensor addres and value of Y axis of accelerometer
  int16_t AcY=0;
  int16_t AcZ=0;

  //transmission on wire library
  Wire.beginTransmission(0x68);
  Wire.write(0x3D); //0x3D (ACCEL_YOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 4, true); //each axis value is stored in 2 registers

  AcY = (Wire.read() << 8 | Wire.read()); // Y-axis value
  AcZ = (Wire.read() << 8 | Wire.read()); // Z-axis value

  // Serial.print("Y: ");
  // Serial.println(AcY);
  // Serial.print("Z: ");
  // Serial.println(AcZ);

  //vector product of gravity and robot acceleration
  float teta = atan((float)AcY / (float)AcZ);

  //conversion to degrees
  float teta_ang = teta*180/PI;
  accel_f += (teta_ang - accel_f)*KF;
  return teta_ang;
}


//calculate error on robot resting
void CalculateErrorGyro() {
  float elapsedtime, GyroErrorZ = 0.0;
  int16_t GyroZ = 0;

  //read the value at the Z axis 200 times
  for(int i = 1 ; i <= 10000 ; i++){

    //reading the values
    Wire.beginTransmission(0x68);
    Wire.write(0x47);
    Wire.endTransmission(false);
    Wire.requestFrom(0x68, 2, true);
    GyroZ = (Wire.read() << 8 | Wire.read());

    // Sum all readings
    GyroErrorZ += ((float)GyroZ - GyroErrorZ)/(float)i;

    delay(2);
  }

  Serial.println(GyroErrorZ);
}

//function used to make the robot go back and forth. Receives the amount of times to go back and and forth and del is the delay between going foward and back
void back_forth(int times, int del)
{
  for(int i = 0; i < times; i++)//for used to count how many times the robot went back and forth
  {
    walk(SWL,SWR);//walk foward
    delay(del);//delay "del"
    walk(-SWL,-SWR);//walk back
    delay(del);//delay "del"
  }

  freeze(25);
}
