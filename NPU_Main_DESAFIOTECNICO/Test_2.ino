void go_to_dist (float final_d)// Function used to make the robot go to a distance
{
  int n_top = 0,n_bot = 0;
  do
  {
    walk(SWL,SWR);
    get_vbt();//Read top and bot ultrassonics
    //    Serial.print("nbot: ");
    //    Serial.print(n_bot);
    //    Serial.print(" n_top: ");
    //    Serial.println(n_top);
    Serial.println(vt);
    if(vt < final_d && vt  != 0) n_top++;//if the top us value is lower than the distance wanted adds to the top counter
    else n_top = 0; // else resets the top count
    
    if(vb < 6.0 && vb > 0) n_bot++;//if the bot value is lower than 8.0cm adds to the bot counter
    else n_bot = 0;// else resets the top count
    
    if(n_bot >= 2) // if the bot counter gets higher or equals 2 it is a victim
    {
      freeze(500);//makes the robot stop for 500 milliseconds
      SwallowBalls();
    }
    
    delay(25);//default delay for reading values
  } while(n_top < 2);//do the above while the top counter is lower than 2
  freeze(100);//stop for 100 millisecons  
  Sinalize(1,3,1,3,20);
}



void walk_distance(float d)//function used to make the robot walk to a specific distance based on experiments
{
  int sign = abs(d) / d; //if sign is negative makes the robot walk back
  walk(540*sign,500*sign);
  delay((int)(125*abs(d)));
  freeze(100);
}



void scanner(int area_scanned,int apt,float dist_to_ball)//scanning function used to scan an area it receives the area to scan and apt(angles per turn)
{
  
  int cont_b = 0; //ball counter
  
  for(int i = 0; i < area_scanned; i+=apt)
  {
    //     get_vbt(); 
    //     prev_vb = vb;
    //     prev_vt = vt;
    //     get_vbt();
    //     delta_vb = prev_vb - vb;
    //     delta_vt = prev_vt - vt;

    vb = getUltra(4); // Read the bottom US 

    if ( vb < dist_to_ball && vb > 0.0) cont_b++;//if the bot us value is lower than 25.0cm adds to the ball counter
    else cont_b = 0;//else it resets the ball counter
    


    if( cont_b > 1) //if the ball counter gets higher or equal 2 it is a victim
    {
      Serial.println("VB:");
      Serial.println(vb);
      freeze(100);//stop for 100 millisecons  
      LEDcontrol(0,0,1);//uses array LED for debug

      float b_dist = 0.95*vb - 5.0; //go to balls
      
      walk_distance( b_dist );
      SwallowBalls();//catches the victim
      walk_distance(-b_dist);
      LEDcontrol(0,0,0);//turns off LED
    }

    walk(-SWL, SWR);
    Simple_turn(apt);//turns apt

  }
   
  update_angle(area_scanned);
  freeze(50);
  
}




void go_to_angle(int f_angle)//function used to make the robot go to an specific angle starting on its current angle
{
  int delta_angle = f_angle - c_angle;//get angle variation (delta_angle)
  
  if(delta_angle < -180)//if angle variation if lower than -180
  { 
    walk(-SWL, SWR);
    Simple_turn(delta_angle + 360);//adds 360 to the angle
    freeze(50);
  }

  else if(delta_angle > 180)//if angle variation if higher than 180
  {
    walk(SWL, -SWR);
    Simple_turn(360 - delta_angle);//subtracts 360 to the angle
  }

  else turn(delta_angle);//just turn the angle variation
  
  c_angle = f_angle;//updates final angle
  freeze(25);
}


void update_angle(int ang)
{
  c_angle+=ang;
  if(c_angle < 0)//if angle is negative 
  {
    c_angle += 360;//adds 360 
  }
  else if(c_angle > 360)//else if angle is higher than 360
  {
    c_angle -= 360;//subtracts 360
  }
}

void scan_turn(int alfa, float dist_to_ball) 
{
  //sensor addres and value of Z axis of gyroscope
  int16_t GyZ=0;

  float total_alfa = 0.0, dimensional_error = 1.0357; //error calculated by the difference in degree 

  //timer variables
  unsigned long previoustimer = millis();
  float elapsedtime;
  int cont_b = 0; //ball counter

  //while it didn't walk to the said angle "alfa"
  if (alfa < 0)walk(SWL, -SWR);
  if (alfa > 0)walk(-SWL, SWR);
  
  while(abs(total_alfa) <= abs(alfa))
  {
    //the elapsed time
    elapsedtime = (float)(millis() - previoustimer) / 1000.0; //as the scale is degrees/seconds, we divided the time in millis by 1000
    previoustimer = millis(); 
    //transmission on wire library
    Wire.beginTransmission(0x68);
    Wire.write(0x47); //0x47 (GYRO_ZOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(0x68, 2, true); //each axis value is stored in 2 registers
    total_alfa += (float)GyZ * dimensional_error * elapsedtime / 131.0; //scale in degrees/seconds. For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
    //put the value received on the variable
    GyZ = (Wire.read()<<8|Wire.read()); //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
    GyZ -=  GYZ_ERROR; //small correction

    vb = getUltra(4); // Read the bottom US

    // Serial.print("bottom distance: ");
    // Serial.println(vb);
    
    if ( vb < dist_to_ball && vb > 0.0) cont_b++;//if the bot us value is lower than 25.0cm adds to the ball counter
    else cont_b = 0;//else it resets the ball counter

    // Serial.print("count: ");
    // Serial.println(cont_b);

    if( cont_b >= 12 ) //if the ball counter gets higher or equal 2 it is a victim
    {
      float b_dist = vb; //go to balls
      freeze(100);//stop for 100 millisecons  
      LEDcontrol(0,0,1);//uses array LED for debug
      Serial.println("VB:");
      Serial.println(vb);
      if(b_dist <= 4.0)
      {
        SwallowBalls();
      }
      else if(b_dist <= 7.0 && b_dist >= 4.0)//first detection radius
      {
        LEDcontrol(0,0,1);
        delay(500);
        LEDcontrol(0,0,0);
        turn(1);
        freeze(100);
        total_alfa += 1.0*total_alfa/abs(total_alfa);
        walk_distance(b_dist*0.95 - 1.0);
        freeze(500);
        SwallowBalls();//catches the victim
        freeze(10);
        walk_distance(-(b_dist*0.95 - 1.0));
        LEDcontrol(0,0,0);//turns off LED
      }
      else if(b_dist >= 7.0 && b_dist <= 14.0)
      {
        LEDcontrol(0,1,0);
        delay(500);
        LEDcontrol(0,0,0);
        turn(3);
        freeze(100);
        total_alfa += 3.0*total_alfa/abs(total_alfa);
        walk_distance(b_dist*0.95 - 3.0);
        freeze(500);
        SwallowBalls();//catches the victim
        freeze(10);
        walk_distance(-(b_dist*0.95 - 3.0));
        LEDcontrol(0,0,0);//turns off LED
      }
      else if(b_dist >= 14.0 && b_dist <= 21.0)
      {
        
        LEDcontrol(1,0,0);
        delay(500);
        LEDcontrol(0,0,0);
        turn(5);
        freeze(100);
        total_alfa += 5.0*total_alfa/abs(total_alfa);
        walk_distance(b_dist*0.95 - 3.0);
        freeze(500);
        SwallowBalls();//catches the victim
        freeze(10);
        walk_distance(-(b_dist*0.95 - 3.0));
        LEDcontrol(0,0,0);//turns off LED
      }
      else
      {
        LEDcontrol(1,1,1);
        delay(500);
        LEDcontrol(0,0,0);
        turn(8);
        freeze(100);
        total_alfa += 8.0*total_alfa/abs(total_alfa);
        walk_distance(b_dist*0.95 - 4.0);
        freeze(500);
        SwallowBalls();//catches the victim
        freeze(10);
        walk_distance(-(b_dist*0.95 - 4.0));
        LEDcontrol(0,0,0);//turns off LED
      }

      // precaution=1;
      freeze(10);
      previoustimer = millis();
      cont_b=0;
    }
    
    if (alfa < 0)walk(SWL, -SWR);
    if (alfa > 0)walk(-SWL, SWR);
    delayMicroseconds(800);   
  }

  //stops
  freeze(25);
  update_angle(alfa);
}

void Nroom()
{
  delay(100);
  c_angle = 90;
  Serial.println(c_angle);
  scan_entrance();
  go_to_mid();
  freeze(500);
  scan_turn(360,26.0);


  go_to_angle(90);
  c_angle = 90;
  touch(10000);
  walk_distance(R_MID_90);

  
  go_to_angle(45);
  scan_edge();
  go_to_angle(130);
  c_angle = 135;
  scan_edge();
  go_to_angle(225);
  c_angle = 225;
  if(triangles <= 1 && entrance_angle != 225)scan_edge();
  go_to_angle(310);
  c_angle = 315;
  if(triangles <= 1 && entrance_angle != 315)scan_edge();
  go_to_angle(90);
  c_angle = 90;
  
  if(balls == 0)
  {
    Sinalize(0,0,1,5,20);
  }
  else
  {
    if (avs > 0){
      deposit(1);
    }
    if (dvs > 0){
      deposit(0);
    }
  }
  
  if(exit_angle == 1023)
  {
    Sinalize(1,5,0,0,20);
    scan_exit();
    go_to_exit();
    if(room_clear == 0)confirm_exit();
  }
  else {
    go_to_exit();
    confirm_exit();
  }

}
