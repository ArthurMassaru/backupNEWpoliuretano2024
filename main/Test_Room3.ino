void Troom()
{
  delay(100);

  scan_entrance();
  go_to_mid();

  scanner(60,3, 25);
  c_angle=75;
  go_to_angle(45);
  scan_edge();
  

  scanner(90,3, 25);
  c_angle=160;
  go_to_angle(135);
  scan_edge();
  
  scanner(90,3, 25);
  c_angle=250;
  go_to_angle(225);
  freeze(3000);
  scan_edge();

  scanner(90, 3, 25);
  c_angle=330;
  go_to_angle(315);
  freeze(3000);
  scan_edge();


  go_to_angle(90);
  freeze(1000);

  if (avs > 0){
    deposit(1);
  }
  deposit(0);

  scan_exit();
  go_to_exit();

}


void deposit(bool a)  { //rescue "a" (alive) or dead victims
  int prevc_angle = c_angle; //save the previous angle

  //if the victim is alive
  if (a==1) {
    //go to the green triangle
    go_to_angle(tri_green);
    go_to_tri();

    //if there are balls in claw, deposit it
    if (balls_in_claw > 0){set_servo(2, 0);delay(1500);Servo_ipos();}
    
    //turns and touches the triangle
    walk(-SWL, -SWR);
    delay(500);
    turn(180);
    c_angle += 180;
    update_angle();
    touch(6000);

    //deposit the ball
    set_servo(3, 105);
    delay(1500);
    Servo_ipos();

    //goes back
    walk_distance(vb);
    go_to_angle(prevc_angle);
    avs=0;
  } 
  
  else {
    //turns and touches the triangle
    go_to_angle(tri_red);
    go_to_tri();
    walk(-SWL, -SWR);
    delay(500);
    turn(180);
    c_angle += 180;
    update_angle();
    touch(6000);

    //deposit the ball
    set_servo(3, 105);
    delay(1500);
    Servo_ipos();

    //goes back
    walk_distance(vb);
    go_to_angle(prevc_angle);
  }
  c_angle = prevc_angle; //updtade the current angle of the robot
}



void scan_edge()//function used to scan edges
{
  freeze(250);//stops for 250 milliseconds

  media_vb = getmUltra(4,20);//gets the media of the bot us
  
  if(media_vb >= 26.0 && media_vb <=40.0 && media_vb != 0)//triangle condition
  {
    if (tri_green == 0 && tri_red == 0) {
      go_to_tri();//go to the triangle
      int tc = detectTri();
    
      if (tc == 1)tri_green = c_angle;
      else if (tc == 2)tri_red = c_angle;
    
      walk_distance(-vb);//walks back to the centre
    } 
    
    else {
      if (tri_green != 0)tri_red = c_angle;
      else tri_green = c_angle;
    }
    
  } 
  
  else if (media_vb > 0.0 && media_vb < 25.0) {
    walk_distance(media_vb - 5.0);
    SwallowBalls();
    walk_distance(-media_vb + 5.0);
    
    if(media_vb >= 26.0 && media_vb <=40.0 && media_vb != 0)//triangle condition
    {
      go_to_tri();//go to the triangle
      int tc = detectTri();
      if (tc == 1)tri_green = c_angle;
      else tri_red = c_angle;
      walk_distance(-vb);//walks back to the centre
    }

    //else LEDcontrol(1,1,1);//no triangle edge uses LEDs for debug
  }
  
  else LEDcontrol(1,1,1);//no triangle edge uses LEDs for debug
}


void go_to_tri()
{
  float med;
  get_vbt();
  med = vb;
  walk_distance(vb);
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

    //if red is the lowest value and the difference between blue and red is higher than 40 (empiric) it's red
    if (red_rescue < green_rescue && red_rescue < blue_rescue) {
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
  media_vl = getmUltra(1,10);
  media_vr = getmUltra(2,10);
  media_vt = getmUltra(3, 10); 

  //if the media is too high it detects exit
  if (media_vr > 60.0)
  {
    exit_angle = 360;
    Sinalize(1,3,0,0,100);
  }
  else if (media_vl > 60.0)
  {
    exit_angle = 180;
    Sinalize(1,3,1,3,100);
  }
  else if (media_vt > 60.0)
  {
    exit_angle = 90;
    Sinalize(0,0,1,3,100);
  }

  else {    
    unsigned long tim = millis();
    int aux=0;
    walk(-300, -300);
    
    while(millis() - tim < 1000) {
      aux += millis() - tim;
      
      //reads the distance of the ultrasonic sensors
      media_vl = getmUltra(1,5);
      media_vr = getmUltra(2,5);
      media_vt = getmUltra(3, 5); 

      //if the media is too high it detects exit
      if (media_vr > 60.0)
      {
        exit_angle = 360;
        break;
      }
      else if (media_vl > 60.0)
      {
        exit_angle = 180;
        break;
      }
      else if (media_vt > 60.0)
      {
        exit_angle = 90;
        break;
      }

    }

    freeze(100);
    walk(300, 300);
    delay(aux);
    walk(0, 0);
    
    tim = millis();
    aux = 0;

    walk(300, 300);
    while(millis() - tim < 1000) {
      aux += millis() - tim;
      
      //reads the distance of the ultrasonic sensors
      media_vl = getmUltra(1,5);
      media_vr = getmUltra(2,5);
      media_vt = getmUltra(3, 5); 

      //if the media is too high it detects exit
      if (media_vr > 60.0)
      {
        exit_angle = 360;
        break;
      }
      else if (media_vl > 60.0)
      {
        exit_angle = 180;
        break;
      }
      else if (media_vt > 60.0)
      {
        exit_angle = 90;
        break;
      }

    }    
    freeze(100);
    walk(-300, -300);
    delay(aux);
    walk(0, 0);
  }
  
}

void scan_entrance()//function used to scan everything
{
  freeze(50);//stops the robot for 50 milliseconds

  media_vl = getmUltra(1,15);//get the media for top us
  delay(25);//default delay for getting information

  media_vr = getmUltra(2,15);//get the media for right us
  delay(25);//default delay for getting information

  media_vt = getmUltra(3,15);//get the media for left us
  delay(25);//default delay for getting information

  if(media_vl < 15.0)//if the robot is closer to the left wall
  {
    entrance_angle = 225;//the entrance angle is 225 degrees
    c_angle = 225;//uptading the current angle of the robot
    ent_s = -1;//used for turns, when the entrance is in left ent_s = -1 
    Sinalize(1,5,0,0,200);
  } 
  

  else if(media_vr < 15.0)//if the robot is closer to the right wall
  {
    entrance_angle = 315;////the entrance angle is 315 degrees
    c_angle = 315;//uptading the current angle of the robot
    ent_s = 1;//used for turns, when the entrance is in right ent_s = 1 
    Sinalize(0,0,1,5,200);
  }

  else //mid entrance
  {
    entrance_angle = 270;//the entrance angle is 270 degrees
    c_angle = 270;//uptading the current angle of the robot
    ent_s=1;
    ent_m=1;
    Sinalize(1,5,1,5,200);
  }

  //if there's an exit on the left
  if (media_vl >= 100.0) {
    exit_angle = 225; //exitange on the left
    Sinalize(1,5,0,0,50);
  }

  //if there's an exit on the right
  else if (media_vr >= 100.0) {
    exit_angle = 315; //exit angle on the right
    Sinalize(0, 0, 1, 5, 50);
  } 
  
  //if there's an exit on the middle
  else if (media_vt >= 100.0) {
    if (entrance_angle==270)exit_angle = 90; //if entrance is on the middle

    else if (entrance_angle==225)exit_angle = 135; //if entrance is on the left
    
    else exit_angle = 45; //if entrance is on the right

    Sinalize(1, 5, 1, 5, 50);
    Serial.println(exit_angle);
  }
}


void go_to_mid() {

  if(ent_m == 0)// left/right entrance
  {
    turn(ent_s*90);//turn 90 deegres right / left
    c_angle -= ent_s*90; //current angle after the turn
    touch(2000);//touch function
    
    if (exit_angle != c_angle) { //if there's no exit, it can follow the wall

      media_vt = getmUltra(3, 25);
      
      if (media_vt < 80.0)go_to_dist(R_MID_90);
      else walk_distance(R_MID_90);//go to middle of the room
      
      turn(ent_s*-90);//turn 90 degrees left / right
      
      c_angle = 90; //current angle after the turn
      touch(2000); //touch function
      
      media_vt = getmUltra(3, 25);
      
      //if there's no exit angle
      if (exit_angle == 1023){
        //gets the exit angle if the exit is on the middle
        if (media_vt >= 100.0)exit_angle=90;
      }
      
      if (exit_angle != c_angle) { //if there's no exit, it can follow the wall
        if (media_vt < 80.0)go_to_dist(R_MID_90);
        else walk_distance(R_MID_90);//go to middle of the room
  
        turn(-90);
      }
      else {
        walk_distance(R_MID_90);//go to middle of the room
        turn(-90);
      }
      
    } 

    else {
      walk_distance(R_MID_90);//go to middle of the room
      turn(ent_s*-90);//turn 90 degrees left / right

      c_angle = 90; //current angle after the turn
      touch(2000); //touch function
      
      //if there's no exit angle
      if (exit_angle == 1023){
        //gets the exit angle if the exit is on the middle
        media_vt = getmUltra(3, 25);
        if (media_vt >= 100.0)exit_angle=90;
      }
      
      
      if (exit_angle != c_angle) { //if there's no exit, it can follow the wall
        if (media_vt < 80.0)go_to_dist(R_MID_90);
        else walk_distance(R_MID_90);//go to middle of the room
  
        turn(-90);
      }

      else {
        walk_distance(R_MID_90);//go to middle of the room
        turn(-90);
      }
    }
  }
  
  else //entrance in the middle
  {
    c_angle = 90; //current angle after the turn
    
    media_vt = getmUltra(3, 25);

    //if there's no exit angle
    if (exit_angle == 1023){
      //gets the exit angle if the exit is on the middle
      if (media_vt >= 100.0)exit_angle=90;
    }
    
    if (exit_angle != c_angle) { //if there's no exit, it can follow the wall
      //go_to_dist(R_MID_90);//go to middle of the room
      if (media_vt < 80.0)go_to_dist(R_MID_90);
      else walk_distance(R_MID_90);
      turn(-90);
    }
    else {
      walk_distance(R_MID_90);//go to middle of the room
      turn(-90);
    }
  }
  
  c_angle = 0;//current angle of the robot is 90 deegres
}


void go_to_exit()
{
  if(exit_angle == 360)
  {
    go_to_angle(360);
    walk_distance(35.0);
  }
  else if(exit_angle == 180)
  {
    go_to_angle(180);
    walk_distance(35.0);
  }
   else if(exit_angle == 90)
  {
    go_to_angle(90);
    walk_distance(35.0);
  }
}