//value for dead victim
#define BLACK_BALL 930


// makes the robot turn in max speed to prevent the ball from getting out of the claw
void swoosh() {
  walk(1000, -1000);
  delay(100);
  walk(-1000, 1000);
  delay(100);
  walk(0,0);
}


void SwallowBalls() {
  
  int ball = 1023; //analogRead
  bool aux=0, alive=0, dead=0; //aux to do a single "swoosh2", alive or dead is the victim conditio
  Servo_ipos();
  
  //goes to the right position with both servo and catches the victim
  set_servo(1,180);
  set_servo(2,100);
  delay(10);
  
  set_servo(1, 150);
  set_servo(2, 120);
  delay(500);
  set_servo(1,120);
  int claw_TCRT = 1023, auxclaw = 0,lock = 0;
  for (int i = 0; i < 50; i++) 
  {
    if(i % 2 == 0 && i <= 30)
    {
      set_servo(1, 150);
      set_servo(1, 120);
    }
    auxclaw = analogRead(A1);
    if(auxclaw < claw_TCRT)
    {
      claw_TCRT = auxclaw;
    }
  }
  //starts to lift te ball and stops t the middle
  set_servo(1,100);
  set_servo(2, 60);
  set_servo(1, 40);

  set_servo(2, 10);
    
  // makes the ball go a little further from the reflectance sensor to read the victim color more pecisely
//  for (int i=0; i < 100; i++) {
//      int auxb=analogRead(A1);      
//      //reads the lowest value of the victim color
//      if (auxb < BLACK_BALL)ball=auxb;
  Serial.println(claw_TCRT);
      //reads the victim color
  if (claw_TCRT < BLACK_BALL) {
    set_servo(3,OPENR); //separator
    alive=true;
    avs++; ////another alive victim secured
  } 
    else 
   {
     set_servo(3,OPENL); //separator
     dead=true;
     dvs++;//another dead victim secured
   }

  
  //finishes going up
  set_servo(1, 0);
  delay(50);
  //pushes the ball inside the separator
  for (int a=0; a<5; a++) {
    for(int i = 10; i >= 140; i++){s2.write(i); delay(10);}
    delay(100);
    for(int i = 140; i >= 10; i--) {s2.write(i); delay(1);}
  }
  delay(50);
  //everything goes back to its original position
  Servo_ipos();
  balls++;//another victim secured
}

//back to the middle after rescue a ball
void BackMiddle(int d1, int t, int d2) {
  //first delay reversed
  walk(-350, -350);
  delay(d1);
  walk(0, 0);
  
  //first turn reversed
  turn(t);
  delay(10);

  //second delay reversed
  walk(-350, -350);
  delay(d2);
  walk(0, 0);

  //resets variables
  dels1=0;
  turns=0;
  dels2=0;
}



//go to the victim, rescue it and goes back
void GotoBalls(){

  //turns on the red led
  digitalWrite(LEDR, 1);


  float med=0; //ultrasonic read
  // float prev_value=0; //ultrasonic preview value
  
  //sets the initial value as values impossible to be obtained as a read
  float min=100000;

  float vtopb=0.0;

  //walks forward for an empiric value of time
  walk(400, 378);
  delay((int)((1000*(dball/2))/3.6));
  walk(0, 0);
  dels2 += (int)((1000*(dball/2)) / 3.6);
  
  
  //10 reads of ultrasonic
  med = 0.0;
  for(int i=0; i<=10; i++){
    med += getUltra(4);
  }
  med/=10;

  //get the distance for rescue
  if (med <= 5.0){
    walk(400,378);
    delay(300);
    walk(0, 0);
    SwallowBalls();
    walk(-400, -400);
    delay(300);
    walk(0, 0);
  }
  else {
    //turns to the left and search for ball
    int aux=0;
    int contmed=5;
    while(contmed>=5 && aux<15){
      turn(1);
      aux++;
      med = 0.0;
      contmed=0;
      for(int i=0; i<=8; i++){
        med = getUltra(4);
        if(med>=5.0)contmed++;
        if(med==0.0)contmed=-100;
      }
      vtopb = getUltra(3);
      
    }

    //10 reads of ultrasonic
    turn(2);


    //get the distance for rescue
    if (med <= 5.0){
      walk(400,378);
      delay(300);
      walk(0, 0);
      SwallowBalls();
      walk(-400, -400);
      delay(300);
      walk(0, 0);
    }
    //turns to the right and search for ball
    else {
      turn(-aux);
      int contmed=5;
      aux=0;
      while(contmed>=5 && aux<15){
        turn(-1);
        aux++;
        med = 0.0;
        contmed=0;
        for(int i=0; i<=8; i++){
          med = getUltra(4);
          if(med>=5)contmed++;
          if(med==0.0)contmed=-100;
        }
    
        vtopb = getUltra(3);
      
      }

      turn(-2);
      //get the distance for rescue
      if (med <= 5.0){
        walk(400,378);
        delay(300);
        walk(0, 0);
        SwallowBalls();
        walk(-400, -400);
        delay(300);
        walk(0, 0);
        turn(aux);
      }
      else {
        walk(400,378);
        delay(600);
        walk(0, 0);
        SwallowBalls();
        walk(-400, -400);
        delay(600);
        walk(0, 0); 
        turn(aux);
      }
      
    }
  
  }

}

void Deposit_alive()
{
  int auxi=0;
  if(ctri == 1 || ctri == 10) 
  {
    for(int i = 0; i <= 6; i += 2)
    {
      if(map_room[i] == 3) 
      {

        avs = false;
        auxi=i;
        for(int h=0;h<mapper-i;h++){
          if(h==0){
            for(int j=0;j<20-ex;j++){
              turn(-1);
            }
          }
          else{
            for(int j=0;j<20;j++){
              turn(-1);
            }
          }
        }
          walk(0, 0);
          flag=millis();
          float med=100000;
          while(med>=6 && color_detected==false){
            med=getUltra(4);
            walk(450, 420);
          }
          walk(450, 450);
          delay(750);
          flag=(millis()-flag-350);
          walk(0, 0);
          int p=0;
          for(int k = 0; k < 180; k++)
          {
            s1.write(p);
            s2.write(k);
            delay(1);
            if(p<45)p++;
          }
          walk(-450, -450);
          delay(flag);
          walk(0, 0);
          for(int h=0;h<mapper-auxi;h++){
            if(h==0){
              for(int j=0;j<20-ex;j++){
                turn(1);
              }
            }
            else{
              for(int j=0;j<20;j++){
                turn(1);
              }
            }
          }
      }
    }
  }
}
