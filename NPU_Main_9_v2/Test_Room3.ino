void Troom()
{
  delay(100);
  c_angle = 90;
  Serial.println(c_angle);
  scan_entrance();
  go_to_mid();
  freeze(1000);



  scan_turn(90,26.0);
  c_angle = 90;
  go_to_angle(45);
  c_angle = 45;
  scan_edge();
  freeze(100);
  

  scan_turn(25,26.0);
  scan_turn(30,26.0);
  scan_turn(60,26.0);
  scan_turn(15,26.0);
  go_to_angle(135);
  c_angle = 135;
  scan_edge();
  freeze(100);
  
  scan_turn(25,26.0);
  scan_turn(30,26.0);
  scan_turn(60,26.0);
  scan_turn(15,26.0);
  go_to_angle(225);
  c_angle = 225;
  if(triangles <= 1 && entrance_angle != 225)scan_edge();
  freeze(100);
   

  scan_turn(25,26.0);
  scan_turn(30,26.0);
  scan_turn(60,26.0);
  scan_turn(15,26.0);
  go_to_angle(300);
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



void deposit(bool a)  { //rescue "a" (alive) or dead victims
  int prevc_angle = c_angle; //save the previous angle

  //if the victim is alive
  if (a==1) {
    //go to the green triangle
    
    //turns to  and touches the triangle
    walk(-SWL, -SWR);
    delay(500);
    turn(180);
    touch(5000);

    //deposit the ball
    set_servo(3, DEPR);
    back_forth(15,100);
    delay(300);
    Servo_ipos();

    walk_distance(10.0);
    turn(-90);
    avs=0;
  } 
  
  else {
    //turns and touches the triangle
    walk(-SWL, -SWR);
    delay(500);
    turn(180);
    touch(5000);

    //deposit the ball
    set_servo(3, DEPL);
    back_forth(15,100);
    delay(1500);
    Servo_ipos();//gets the media of the bot us
    
    walk_distance(10.0);
    turn(-90);
  }
}



void scan_edge()//function used to scan edges
{
  media_vb = getmUltra(4,20);
  Serial.println("MEDIA_VB:");
  Serial.println(media_vb);
  if(media_vb <=40.0 && media_vb > 0.0)//triangle condition
  {
    Sinalize(1,10,1,10,10);

    go_to_tri();//go to the triangle
    tri = 1;

    if (tri_green == 0 && tri_red == 0) {
    
      if (tri != 0) {
        triangles++;
        int tc = detectTri(); //detects the color of the triangle |1 = green| / |2 =  red|
        if (tc == 1) //if the triangle is green
        {
          LEDcontrol(0,1,0);
          delay(300);
          LEDcontrol(0,0,0);
          tri_green = c_angle; //the angle of the green triangle is the current angle of the robot
          if(avs > 0)//if the robot has an alive victim in the separator
          {
            turn(180);
            update_angle(180);
            touch(1000);
            set_servo(3,DEPR);
            back_forth(15,100);
            freeze(500);
            walk_distance(vb*0.92);
            go_to_angle(tri_green);
            avs = 0;
            //deposit and return to the previous angle
          }
          else walk_distance(-vb*0.92);//else the robot goes back to its initial position
        } 
        else if (tc == 2)//else if the triangle is red
        {
          LEDcontrol(1,0,0);
          delay(300);
          LEDcontrol(0,0,0);
          tri_red = c_angle; //the angle of the green triangle is the current angle of the robot
          if(dvs > 0)
          {
            turn(180);
            update_angle(180);
            touch(1000);
            set_servo(3,DEPL);
            back_forth(15,100);
            freeze(500);
            walk_distance(vb*0.92);
            go_to_angle(tri_red);
            dvs = 0;
          }
          else walk_distance(-vb*0.92);
        }

        else walk_distance(-vb*0.92);
      }
      else walk_distance(-vb*0.92);

      Servo_ipos();
    
      freeze(50);//stops for 250 milliseconds

      //walks back to the centre
    } 
    
    else {
      tri = 1;
      if (tri != 0) {
        if (tri_green != 0)tri_red = c_angle;
        else tri_green = c_angle;
        triangles++;

        walk_distance(-vb*0.95);
      } 
      
      else walk_distance(-vb*0.92);      
    }

    // delay(1000);
    // Serial.println("c_angle :");
    // Serial.println(c_angle);
    // delay(1000);
    // Serial.println("tri_green :");
    // Serial.println(tri_green);
    // delay(1000);
    // Serial.println("tri_green :");
    // Serial.println(tri_green);
   
    
  } 
  
  /*else if (media_vb > 0.0 && media_vb < 26.0) {
    turn(10);
    update_angle(10);
    walk_distance(media_vb*0.95- 3.0);
    SwallowBalls();
    walk_distance(-(media_vb*0.9 - 3.0));
  }*/
  
  else LEDcontrol(1,1,1);//no triangle edge uses LEDs for debug
}


void go_to_tri()
{
  float med;
  get_vbt();
  med = vb;
  walk_distance(vb*0.95);
}

int front_touch(int lim_f)
{
  tri = 0;
  unsigned long flag_f = millis();
  while (!digitalRead(FTOUCH))// Until the two sensors touch the wall
  {
      Serial.print(digitalRead(FTOUCH));
      walk(SWL,SWR); // while the back buttons dont touch the wall make the robot go back
     if(digitalRead(FTOUCH))
     {
      tri = 1;
      return tri;
     }
     if(millis() - flag_f > lim_f)
     {
      return tri;
     }
  }
  freeze(100);
}

//uses the color sensor for rescue. 1 for green, 2 for red and 0 is wall
int detectTri() {
  LEDRescue(0, 0, 0);

  int green=0, red=0;
  
  //sets the initial value of red green and blue as values impossible to be obtained as a read
  int minr = 10000;
  int ming = 10000;
  int minb = 10000;
  int maxr = 1;
  int maxg = 1;
  int maxb = 1;

  //read 100 times
  for (int i = 0; i < 100; i++) {
    readLEDRescue(); 

    //the lowest and the highest values are kept
    if(minr > red_rescue && red_rescue>0)minr=red_rescue;
    if(ming > green_rescue && green_rescue>0)ming=green_rescue;
    if(minb > blue_rescue && blue_rescue>0)minb=blue_rescue;
    if(maxr < red_rescue)maxr=red_rescue;
    if(maxg < green_rescue)maxg=green_rescue;
    if(maxb < blue_rescue)maxb=blue_rescue;

    //if red is the lowest value it's red
    if (red_rescue < green_rescue && red_rescue < blue_rescue && red_rescue < 900) {
      red++;
    }
    //if the maximum and minimum value are really close to each other it's green
    else {
      green++;
    }
  }
  cont_tri=0;
  //if there are more green than red 
  if (green > red)return 1;
  else if (red>green) return 2;
  else return 0;
}

// Function to print all variables of variaton of ultrassonic values
void print_all ()
{
  get_vbt();
  
  prev_vb = vb;
  prev_vt = vt;
  
  get_vbt();
  
  delta_vb = prev_vb - vb; 
  delta_vt = prev_vt - vt;
  
  Serial.print("vb: ");
  Serial.print(vb);
  Serial.print(" prev_vb: ");
  Serial.print(prev_vb);
  
  Serial.print(" vt: ");
  Serial.print(vt);
  Serial.print(" prev_vt: ");
  Serial.print(prev_vt);
  
  Serial.print(" delta_vb: ");
  Serial.print(delta_vb);
  Serial.print(" delta_vt:");
  Serial.print(delta_vt);
  
  Serial.print(" delta_mbt: ");
  Serial.println(delta_mbt);
  
  delay(750);
}



void print_vbt()
{
  get_vbt();
  
  prev_vb = vb;
  prev_vt = vt;
  
  get_vbt();
  
  delta_vb = prev_vb - vb; 
  delta_vt = prev_vt - vt;
  
  Serial.print("vb: ");
  Serial.print(vb);
  Serial.print(" prev_vb: ");
  Serial.print(prev_vb);
  Serial.print(" delta_vb: ");
  Serial.print(delta_vb);
  
  Serial.print(" vt: ");
  Serial.print(vt);
  Serial.print(" prev_vt: ");
  Serial.print(prev_vt);
  Serial.print(" delta_vt:");
  Serial.println(delta_vt);
  
  delay(500);
}



void get_vbt()//function used to get bot and top us
{
  vb = getUltra(4);
  vt = getUltra(3);
}



void scan_exit()
{

  //reads the distance of the ultrasonic sensors
  delay(15);
  media_vl = getmUltra(1,10);
  delay(100);
  media_vr = getmUltra(2,10);
  delay(100);
  media_vt = getmUltra(3,10); 
  delay(100);
  Serial.println("media_vl");
  Serial.println(media_vl);
  delay(100);
  Serial.println("media_vr");
  Serial.println(media_vr);
  delay(100);
  Serial.println("media_vt");
  Serial.println(media_vt);
  
  //if the media is too high it detects exit
  if (media_vr > 150.0)
  {
    exit_angle = 0;
    Sinalize(0,0,1,3,30);
  }
  else if(media_vl > 150.0)
  {
    exit_angle = 180;
    Sinalize(1,3,1,3,30);
  }
  else if(media_vt > 150.0)
  {
    exit_angle = 90;
    Sinalize(1,3,0,0,30);
  }
  else { 
    //45 and 135
    if( (tri_green == 45 && tri_red == 135) || (tri_green == 135 && tri_red == 45) )//if the two triangles are in 45 and 135 deegres
    {
      if(ent_m == 0)//if the entrance is in the corners
      {
        if(ent_s == -1)exit_angle = 315;//if the entrance is in the left corner the exit_angle is 315 deegres
        else exit_angle = 225; //if the entrance is in the right corner the exit_angle is 225 deegres
      }
      else //mid entrance
      {
        walk_distance(-R_MID_90);
        turn(90);
        media_vt == getmUltra(3,10);
        //*********
        if(media_vt >= 100.0)
        {
          do
          {
            array_read();
            walk(SWL,SWR);
          }
          while(NOSIB() >= 2);
          walk(0, 0);
          room_clear = 1;
        }
        else
        {
          turn(-180);
          do
          {
            array_read();
            walk(SWL,SWR);
          }
          while(NOSIB() >= 2);
          walk(0, 0);
          room_clear = 1;
        }
    }
   }
    //45 and 225 && 45 and 315 and entrance from corner
    else if( ent_m == 0 && ( (tri_green == 45 && tri_red == 225) || (tri_green == 225 && tri_red == 45) || (tri_green == 45 && tri_red == 315) || (tri_green == 315 && tri_red == 45) ) )
    {
      exit_angle = 135; 
    }
    
    //45 and 225 mid variation
    else if( ent_m == 1 && (tri_green == 45 && tri_red == 225) || (tri_green == 225 && tri_red == 45) )//*********
    {
      walk_distance(-R_MID_90);
      turn(-90);
      media_vt = getmUltra(3,10);
      if(media_vt >= 100.0)
      {
        do
        {
          array_read();
          walk(SWL,SWR);
        }
        while(NOSIB() >= 2);
        walk(0, 0);
        room_clear = 1;
      }
      else
      {
        turn(90);
        walk_distance(R_MID_90);
        exit_angle = 135;
      }
    }
    
    //45 and 315 mid variation
    else if( ent_m == 1 && (tri_green == 45 && tri_red == 315) || (tri_green == 315 && tri_red == 45) )//*********
    {
      walk_distance(-R_MID_90);
      turn(90);
      media_vt = getmUltra(3,10);
      if(media_vt >= 100.0)
      {
        do
        {
          array_read();
          walk(SWL,SWR);
        }
        while(NOSIB() >= 2);
        walk(0, 0);
        room_clear = 1;
      }
      else
      {
        turn(-90);
        walk_distance(R_MID_90);
        exit_angle = 135;
      }
    }

    //135 and 225 
    else if( ent_m == 0 && (tri_green == 135 && tri_red == 225) || (tri_green == 225 && tri_red == 135) )exit_angle = 45;
    //135 and 225 mid variation
    else if(ent_m == 1 && (tri_green == 135 && tri_red == 225) || (tri_green == 225 && tri_red == 135) )//*********
    {
      walk_distance(-R_MID_90);
      turn(90);
      media_vt = getmUltra(3,10);
      if(media_vt >= 100.0)
      {
        do
        {
          array_read();
          walk(SWL,SWR);
        }
        while(NOSIB() >= 2);
        walk(0, 0);
        room_clear = 1;
      }
      else
      {
        turn(90);
        walk_distance(R_MID_90);
        exit_angle = 45;
      }
    }
    //135 and 315
    else if( ent_m == 0 && (tri_green == 135 && tri_red == 315) || (tri_green == 315 && tri_red == 135) ) exit_angle = 45;
    //135 and 315 mid variation
    else if( ent_m == 1 && (tri_green == 135 && tri_red == 315) || (tri_green == 315 && tri_red == 135) ) //**********
    {
      walk_distance(-R_MID_90);
      turn(90);
      media_vt = getmUltra(3,10);
      if(media_vt >= 100.0)
      {
        do
        {
          array_read();
          walk(SWL,SWR);
        }
        while(NOSIB() >= 2);
        walk(0, 0);
        room_clear = 1;
      }
      else
      {
        turn(-90);
        walk_distance(R_MID_90);
        exit_angle = 45;
      }
    }
    //225 and 315
    else
    {
      walk_distance(R_MID_90);
      turn(90);
      media_vt = getmUltra(3,10);
      if(media_vt >= 100.0)
      {
        do
        {
          array_read();
          walk(SWL,SWR);
        }
        while(NOSIB() >= 2);
        walk(0, 0);
        room_clear = 1;
      }
      else 
      {
        turn(-180);
        do
        {
          array_read();
          walk(SWL,SWR);
        }
        while(NOSIB() >= 2);
        walk(0, 0);
        room_clear = 1;
      }
    }
  }
}


void confirm_exit()
{

  if(exit_angle == 45 || exit_angle == 225)
  {
    if(exit_angle == 45) go_to_angle(90);
    else go_to_angle(270);
    media_vt = getmUltra(3,10); // Top US
    delay(25);
    if(media_vt <= 20.0)
    {
      turn(-90);

      freeze(10);
      walk_distance(5.0);
      do
      {
        array_read();
        walk(SWL,SWR);
      }while(NOSIB() < 2);
      freeze(100);
      walk(0, 0);
    }
    else
    {
      freeze(10);
      walk_distance(5.0);
      do
      { 
        array_read();
        walk(SWL,SWR);
      }while(NOSIB() < 2);
      walk(0, 0);
    }
  }
  else
  {
    if(exit_angle == 135)go_to_angle(90);
    else go_to_angle(270);
    media_vt = getmUltra(3,10);
    delay(25);
    if(media_vt <= 20.0)
    {
      turn(90);
      
      freeze(10);
      walk_distance(10.0);
      do
      {
        array_read();
        walk(SWL,SWR);
      }while(NOSIB() < 2);
      walk(0, 0);
    }
    else
    {
      freeze(10);
      walk_distance(10.0);
      do
      {
        array_read();
        walk(SWL,SWR);
      }while(NOSIB() < 2);
      walk(0, 0);
    }
  }
}


void scan_entrance() //function used to scan entrance
{  


  media_vl = getmUltra(1,15);//get the media for top us
  delay(25);//default delay for getting information

  //previous code
  // media_vr = getmUltra(2,15);//get the media for right us
  // delay(25);//default delay for getting information
  // media_vt = getmUltra(3,15);//get the media for left us
  // delay(25);//default delay for getting information

  media_sp = getmsharp(10);

  float mL=0.1, mT=0.1, mR=0.1; //minimum values


  // if(media_vl < 28.0)//if the robot is closer to the left wall
  // {
  //   ent_s = -1;//used for turns, when the entrance is in left ent_s = -1 
  //   Sinalize(1,5,0,0,20);
  // } 
  

  // else if(media_vr < 28.0)//if the robot is closer to the right wall
  // {
  //   ent_s = 1;//used for turns, when the entrance is in right ent_s = 1 
  //   Sinalize(0,0,1,5,20);
  // }
  // else //mid entrance
  // {
  //   ent_s=1;
  //   ent_m=1;
  //   Sinalize(1,5,1,5,20);
  // }

  if (media_sp > 300)
  {
    ent_s = 1;//used for turns, when the entrance is in right ent_s = 1 
    Sinalize(0,0,1,5,20);
  }

  else
  {
    turn(185);
    media_sp=getmsharp(10);
    if (media_sp > 350)
    {
      ent_s = -1;//used for turns, when the entrance is in left ent_s = -1 
      Sinalize(1,5,0,0,20);
      turn(180);
    }
    else //mid entrance
    {
      ent_s=1;
      ent_m=1;
      turn(185);
      //Sinalize(1,5,1,5,20);
    }
  }

  
  //scan fot possible victims:
  if(ent_m == 0)
  {
    turn(ent_s*90);
    touch(8000);
    walk_distance(15.0);
    turn(ent_s*-90);
    media_vb = getmUltra(4,15);
    Serial.println("M inicial");
    Serial.println(media_vb);
    if(media_vb < 40.0)
    {
      walk_distancewb(media_vb - 1.0);
      SwallowBalls();
      walk_distance(-media_vb + 1.0);
    }
    // if(ent_s == 1)//right
    // {
    //   media_vl = getmUltra(1,15);
    //   if(media_vl <= 40.0)
    //   {
    //     turn(90);
    //     walk_distancewb(media_vl);
    //     SwallowBalls();
    //     walk_distance(-media_vl);
    //     turn(-90);
    //   }
    // }
    // else//left
    // {
    //   media_vr = getmUltra(1,15);
    //   if(media_vr <= 40.0)
    //   {
    //     turn(-90);
    //     walk_distancewb(media_vr);
    //     SwallowBalls();
    //     walk_distance(-media_vr);
    //     turn(90);
    //   }
    // }
  }
  else
  {
    media_vb = getmUltra(4,15);
    Serial.println("M inicial");
    Serial.println(media_vt);
    if(media_vb < 40.0)
    {
      walk_distancewb(media_vb - 1.0);
      SwallowBalls();
      freeze(50);
      delay(50);
      walk_distance(-media_vb + 1.0);
    }
  }

  freeze(100);  
  for (int i = 0; i < 15; i++) {
    //auxiliar values
    float auxr, auxl;
  
    auxl=getUltra(1);
    // Serial.print("1: ");Serial.println(auxl);
    delay(25);
  
    auxr=getUltra(2);
    // Serial.print("2: ");Serial.println(auxr);
    delay(25);
  
    //if the minimum value is lower than the recent, replaces
    if (auxl != 0.0 && mL != -1.0){if (auxl > mL)mL = auxl;}
    else mL = -1.0;
    if (auxr != 0.0 && mR != -1.0){if (auxr > mR)mR = auxr;}
    else mR = -1.0;
  
  //   //debug
  //   // Serial.println();
  //   // Serial.print("1: ");Serial.println(mL);
  //   // Serial.print("2: ");Serial.println(mR);
  //   // Serial.print("3: ");Serial.println(mT);
  //   // Serial.println();
  //   // Serial.println();
  }
  freeze(50);//stops the robot for 50 milliseconds  
  // Serial.println();
  // Serial.print("mL: ");Serial.print(mL);
  // Serial.print(" mR: ");Serial.println(mR);
  // Serial.println();
  
  
  front_scan();
  if(ent_m == 0)
  {
    turn(90*ent_s);
    front_scan_2();
    turn(-90*ent_s);
  }
  //if entrance is in the middle
  if (ent_m == 1) {
    tone(BUZZER,440,1000);
    LEDcontrol(1,0,0);
    delay(500);
    LEDcontrol(0,0,0);
    //"exit" of the room of 120 by the middle on left
    if (mL > 70.0 || mL == -1.0) {
      //search fot exit in the middle. If the value is still too high it's a 120cm room on the side.
      walk_distance(30.0);
      float mL2=0.1;
      float auxl2 = getUltra(1);
      for (int i = 0; i < 15; i++) {
        auxl2 = getUltra(1);
        if (auxl2 != 0.0 && mL2 != -1.0){if (auxl2 > mL2)mL2 = auxl2;}
        else mL2 = -1.0;
        
      }
      if (mL2 > 70.0 || mL2 == -1.0)
      {
        roomL = 120;roomF=90;
        walk_distance(-30.0);
        return;
      }
      else roomL = 90;
      walk_distance(-30.0);
    }
    else {
      roomL = 90; //side is 90cm
    }

    //"exit" of the room of 120 by the middle on right
    if (mR > 70.0 || mR == -1.0) {
      //search fot exit in the middle. If the value is still too high it's a 120cm room on the side.
      walk_distance(30.0);
      float mR2=0.1;
      float auxr2 = getUltra(2);
      for (int i = 0; i < 15; i++) {
        auxr2 = getUltra(2);
        if (auxr2 != 0.0 && mR2 != -1.0){if (auxr2 > mR2)mR2 = auxr2;}
        else mR2 = -1.0;
      }
      if (mR2 > 70.0 || mR2 == -1.0)
      {
        roomL = 120;roomF=90;
        walk_distance(-30.0);
        return;
      }
      else roomL = 90;
      walk_distance(-30.0);
    }
    else {
      roomL = 90; //side is 90cm
    }

    //"exit" of the room of 120 by the middle on top sensor
    if ((max_top > 70.0 || max_top == -1.0) && roomF == 0) {
      //search fot exit in the middle. If the value is still too high it's a 120cm room on the side.
      LEDcontrol(0,0,1);
      delay(2000);
      LEDcontrol(0,0,0);
      walk_distancewb(30.0);//walks to tte next tile
      turn(-90*ent_s);//turns to face the adjacent tile
      walk_distancewb(15.0);//goes to the adjacent tile
      media_vt = getmUltra(3,15);
      if(media_vt <= 25.0)
      {
        walk_distance(10.0);
        turn(180);
        touch(5000);
        walk_distance(20.0);
        turn(-90*ent_s);
      }
      else
      {
        turn(90*ent_s);//turns to face the front wall
      }
      float mT2=0.1;
      float auxt2 = getUltra(3);
      for (int i = 0; i < 15; i++) {//for used to get the maximum value of the top US
        auxt2 = getUltra(3);
        if (auxt2 != 0.0 && mT2 != -1.0)
        {
          if (auxt2 > mT2)mT2 = auxt2;
        }
        else mT2 = -1.0;
      }
      if (mT2 > 40.0 || mT2 == -1.0)//if the max valueof top US is a 120 or exit
      {
        roomF = 120;
        roomL = 90;
        tone(BUZZER, 440, 1000);
        LEDcontrol(0,0,1);
        delay(1000);
        LEDcontrol(0,0,0);

        turn(-90*ent_s);
        walk_distance(-15.0);
        turn(90*ent_s);
        walk_distance(-30.0);
        return;
      }
      else roomF = 90;
      //goes back
      turn(-90*ent_s);
      walk_distance(-15.0);
      turn(90*ent_s);
      walk_distance(-30.0);
    }
    else {
      roomF = 90; //front is 90cm
    }
  }

  //if entrance is on the right
  else if (ent_s == 1 && ent_m == 0) {
    //"exit" of the room of 120 by the middle on left
    if (maxlat == -1.0) {
      freeze(20);
      walk_distance(30.0);
      turn(90);
      front_scan();
      if (max_top > 70.0 || max_top == -1.0)
      {
        roomL = 120;
        roomF=90;
        turn(-90);
        walk_distance(-30.0);
        return;
      }
      else roomL = 90;
      turn(-90);
      walk_distance(-30.0);
    }

    else if (maxlat > 70.0) 
    {
      roomL = 120;
      roomF=90;
      return;
    }
    else {
      roomL = 90; //side is 90cm
    }

  }
  
  //if entrance is on the left
  else {
    //"exit" of the room of 120 by the middle on left
    if (maxlat == -1.0) {
      freeze(20);
      walk_distance(30.0);
      turn(-90);
      front_scan();
      if (max_top > 70.0 || max_top == -1.0)
      {
        roomL = 120;roomF=90;
        turn(90);
        walk_distance(-30.0);
        return;
      }
      else roomL = 90;
      turn(90);
      walk_distance(-30.0);
    }

    else if (maxlat > 70.0) 
    {
      roomL = 120;
      roomF=90;
      return;
    }
    else {
      roomL = 90; //side is 90cm
    }
  }
  
  //if entrance is on either side
  if ((max_top == -1.0) && roomF == 0 && ent_m == 0) {
    //"exit" of the room of 120 by the middle on top
    turn(ent_s*90);
    touch(5000);
    walk_distance(7.0);
    walk_distancewb(R_MID_90 - 10.0);
    freeze(50);
    turn(ent_s*-90);
    touch(5000);
    freeze(50);
    walk_distance(7.0);
    freeze(50);
    float mT2=0.1;
    float auxt2 = getUltra(3);
    for (int i = 0; i < 15; i++) 
    {
      auxt2 = getUltra(3);
      if (auxt2 != 0.0 && mT2 != -1.0){if (auxt2 > mT2)mT2 = auxt2;}
      else mT2 = -1.0;
      delay(10);
    }
    Serial.println("MT2:");
    Serial.println(mT2);
    if (mT2 > 70.0 || mT2 == -1.0)
    {
      walk_distancewb(R_MID_120 - 10.0);
      roomF = 120;
      roomL = 90;
      special = 1;
      LEDcontrol(1, 1, 1);
      delay(25);
      return;
    }
    else 
    {
      walk_distancewb(R_MID_90 - 3.0);
      roomF = 90;
      special = 1;
      LEDcontrol(0, 0, 1);
      delay(25);
    }
  }

  else if (max_top > 70.0) 
  {
    roomF = 120;
    roomL = 90;
    return;
  }
  else {
    roomF = 90; //front is 90cm
  }   

}


void go_to_mid() {
  
  if(ent_m == 0)// left/right entrance
  {
    turn(ent_s*90);//turn 90 deegres right / left
    Serial.println(c_angle);
    touch(5000);//touch function
  
    media_vt = getmUltra(3,15);//gets a media of the top US just for confirmation purposes 
  
    if(media_vt >= 100.0) //if the media is higher than 100.0 cm the robot is facing an exit
    {
      turn(ent_s*-90);

      //make the robot go to inital and updtades its angle    
      walk_distancewb(R_MID_90 - 1.0);
      turn(ent_s*90);//turns left/right
      
      touch(5000);//touch function
    
      walk_distancewb(R_MID_90 - 5.0);
      
    }
    
    else
    {
      // go_to_dist(R_MID_90);//go to the middle of the room
      walk_distancewb(R_MID_90 - 5.0);
      turn(ent_s*-90);//turn left / right
      touch(5000);
      
      // Serial.println(c_angle);

      // walk_distance(15.0);
      freeze(500);

      media_vt = getmUltra(3,15);
      if(media_vt >= 100.0)
      {
        walk_distance(-10.0);
        turn(ent_s*90);//turn left / right
        walk_distance(-(R_MID_90 - 1.0)); //walks to get in the begin of the room
        turn(ent_s*-90);
        // go_to_dist(R_MID_90);
        walk_distance(R_MID_90 - 1.0);
        turn(ent_s*90);
        touch(5000);
        // go_to_dist(R_MID_90);
        walk_distance(R_MID_90 - 1.0);
        
      }
      else  walk_distancewb(R_MID_90 - 1.0); //go_to_dist(R_MID_90);      
    }
  }
  
  else //entrance in the middle
  {
    media_vt = getmUltra(3, 25);
    if (media_vt >= 100.0)
    {
      //walk_distance
      walk_distancewb(R_MID_90);
    }
    else  
    {
      walk_distance(30.0);
      go_to_dist(R_MID_90);//if the exit isnt in the middle just go to the middle
    }
  }
  
}


void go_to_exit()
{

  LEDcontrol(0, 0, 0);
  go_to_angle(exit_angle);
  walk_distance(38.0);
  
  if(exit_angle == 90) //if exit angle == 90
  {
    do
    {
      array_read();
      walk(SWL,SWR);
    } while (NOSIB() < 3);
    room_clear = 1;
    walk(0, 0);
  }
}

