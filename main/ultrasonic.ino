double getUltra(int u) {
   long duration;
   double distance;

   //ultrasonic "1"
   if (u==1){
      digitalWrite(TRIG1, LOW);
      delayMicroseconds(2);

      //emits the ultrasonic sensor sonor wave
      digitalWrite(TRIG1, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG1, LOW);

      duration = pulseIn(ECHO1, HIGH, 25000); // captures the sonor wave emitted by the ultrasonic sensor. the last parameter is the max distance the sensor will get (_duration = _distance * 2 / 0,0343)
      distance = duration * 0.0343 / 2; // converts the distance in centimeters

      return distance;
   } 

   //ultrasonic "2"
   else if (u==2){
      digitalWrite(TRIG2, LOW);
      delayMicroseconds(2);

      //emits the ultrasonic sensor sonor wave
      digitalWrite(TRIG2, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG2, LOW);

      duration = pulseIn(ECHO2, HIGH, 25000); // captures the sonor wave emitted by the ultrasonic sensor. the last parameter is the max distance the sensor will get (_duration = _distance * 2 / 0,0343)
      distance = duration * 0.0343 / 2; // converts the distance in centimeters

      return distance;
   } 

   //ultrasonic "3"
   else if (u==3){
      digitalWrite(TRIG3, LOW);
      delayMicroseconds(2);

      //emits the ultrasonic sensor sonor wave
      digitalWrite(TRIG3, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG3, LOW);

      duration = pulseIn(ECHO3, HIGH, 25000); // captures the sonor wave emitted by the ultrasonic sensor. the last parameter is the max distance the sensor will get (_duration = _distance * 2 / 0,0343)
      distance = duration * 0.0343 / 2; // converts the distance in centimeters
      
      return distance;
   } 

   //ultrasonic "4"
   else{
      digitalWrite(TRIG4, LOW);
      delayMicroseconds(2);

      //emits the ultrasonic sensor sonor wave
      digitalWrite(TRIG4, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG4, LOW);

      duration = pulseIn(ECHO4, HIGH, 25000); // captures the sonor wave emitted by the ultrasonic sensor. the last parameter is the max distance the sensor will get (_duration = _distance * 2 / 0,0343)
      distance = duration * 0.0343 / 2; // converts the distance in centimeters  
      
      return distance;
   }
}

void dists(int d){
  //goes to right or left based on the value of d
  if(d == -1){
    walk(1000, -1000);
    delay(35);
  }
  else if(d == 1){
    walk(-1000, 1000);
    delay(35);
  }

  //stops
  walk(0, 0);
  delay(30);
  val_bot = 0;
  
  for(int i = 0; i < 15; i++){
    float med = 0;
    med = getUltra(4);
    
    //if ultrasonic value is too high or too low it sets as -1
    if(med == 0 || med >  105){
      i = 21;
      val_bot = -1;
    }
    else {
      val_bot += med; //sum of raw value
    }
  }

  //waiting for stability
  delay(10);
  val_top = 0;

  for(int i = 0; i < 15; i++){
    float med = 0;
    med = getUltra(3);

    //if ultrasonic value is too high or too low it sets as -1
    if(med == 0 || med > 105){
      i = 21;
      val_top = -1;
    }
    else {
      val_top += med; //sum of raw value
    }
  }

  if(val_bot!=-1)val_bot /= 15;
  if(val_top!=-1)val_top /= 15;

}

float getMaxLeftRight() {
   val_left = 0.0;
   val_right = 0.0;

   for (int i = 0; i < 20; i++) {
      float aux=getUltra(1);
      if(val_left<aux)val_left=aux;

      aux=getUltra(2);
      if(val_right<aux)val_right=aux;
   }
}

void distsLR() {
  //stops
  walk(0, 0);
  delay(30);
  val_left = 0.0;
  
  for(int i = 0; i < 15; i++){
    float med = 0;
    med = getUltra(1);
    
    //if ultrasonic value is too high or too low it sets as -1
    if(med == 0 || med >  120){
      i = 21;
      val_left = -1;
    }
    else {
      val_left += med; //sum of raw value
    }
  }

  //waiting for stability
  delay(10);
  val_right = 0.0;

  for(int i = 0; i < 15; i++){
    float med = 0;
    med = getUltra(3);

    //if ultrasonic value is too high or too low it sets as -1
    if(med == 0 || med > 120){
      i = 21;
      val_right = -1;
    }
    else {
      val_right += med; //sum of raw value
    }
  }

  if(val_right!=-1)val_right /= 15;
  if(val_left!=-1)val_left /= 15;
}

//if  detects obstacle with top sensor 5 times in a row, starts obstacle function
void getObstacle() {
   int dist = getUltra(3);
   if (dist < 5 && dist != 0)count_u++;
   else count_u = 0;

   if (count_u > 5)Obstacle('l');
}


void Obstacle(char c) {

   //timer for ultrasonic
   unsigned long flag_u = millis();

   //turns for right side
   if (c == 'r') {
      
      //Stop the robot and turn
      back(1000);
      delay(30);
      back(500);
      delay(100);
      freeze(30);
      turn(-90);

      //walk back
      walk(0, 0);
      walk(-500, 500);
      walk(500, 550);
      delay(500);

      //adjusts itself to be aligned perpendicular to the black line
      array_read();
      while(els<=BLACK && ers<=BLACK){
         array_read();
         walk(-600, -600);
      }
      if(ers<=BLACK){while(ers<=BLACK){array_read();walk(0, -600);}}
      if(els<=BLACK){while(els<=BLACK){array_read();walk(-600, 0);}}

      //reads the left ultrasonic value
      float dl = getUltra(1);

      //walks until it doesn't detects obstacle or for 5 seconds
      flag_u = millis();
      while(dl < 15 && dl != 0){
         walk(500, 550);
         dl = getUltra(1);
         if (millis() - flag_u > 5000)break;
      }
      
      //walks a little more for guarantee
      walk(500, 550);
      delay(1000);
      walk(0, 0);
      delay(70);

      //turns left and reads the value of left ultra
      turn(80);
      dl = getUltra(1);

      //walks until it detects obstacle or for 5 seconds
      flag_u = millis();
      while(dl > 8 || dl== 0) {
         walk(500, 550);
         dl = getUltra(1);
         if (millis() - flag_u > 5000)break;
      }
      
      //walks a little more for guarantee
      walk(500, 550);
      delay(500);
      walk(0, 0);

      //walks until it doesn't detect obstacle or for 5 seconds
      flag_u = millis();
      while(dl < 15 && dl != 0){
         walk(500, 550);
         dl = getUltra(1);
         if (millis() - flag_u > 5000)break;
      }
      
      //walks a little more for guarantee
      walk(500, 550);
      delay(1750);
      walk(0, 0);
      delay(70);

      //turns left and reads the value of left ultra
      turn(90);
      dl = getUltra(1);

      //walks until it detects obstacle or for 5 seconds
      flag_u = millis();
      while(dl > 15 || dl == 0) {
         walk(500, 550);
         dl = getUltra(1);
         array_read();
         if (millis() - flag_u > 5000)break;
      }

      //walks until it detects black
      flag_u = millis();
      array_read();
      while(els<=BLACK && ers<=BLACK){
         array_read();
         walk(600, 600);
      }
      if(ers<=BLACK){while(ers<=BLACK){array_read();walk(0, 600);}delay(200);}
      if(els<=BLACK){while(els<=BLACK){array_read();walk(600, 0);}delay(200);}
      
      //reads the left ultrasonic value
      dl = getUltra(1);

      //walks a little more for guarantee
      walk(500, 550);
      delay(500);

      //turns right
      turn(-70);
      while (ls < BLACK){
         right(600);
         array_read();
      }

      //does pid very slowly
      flag_u = millis();
      while(millis() - flag_u > 500) {
         array_read();
         PIDwalk(0.3);
      }

      //walks back what it has gone forward
      walk(-600, -600);
      delay((int)(dl/getDynaVelocity(600)*1000));
   }

   else {

      //Stop the robot and turn
      back(1000);
      delay(30);
      back(500);
      delay(100);
      freeze(30);
      turn(90);

      //walk back
      walk(0, 0);
      walk(-500, 500);
      walk(500, 550);
      delay(500);

      //adjusts itself to be aligned perpendicular to the black line
      array_read();
      while(els<=BLACK && ers<=BLACK){
         array_read();
         walk(-600, -600);
      }
      if(ers<=BLACK){while(ers<=BLACK){array_read();walk(0, -600);}delay(200);}
      if(els<=BLACK){while(els<=BLACK){array_read();walk(-600, 0);}delay(200);}

      //reads the right ultrasonic value
      float dr = getUltra(2);

      //walks until it doesn't detects obstacle or for 3 seconds
      flag_u = millis();
      while(dr < 15 && dr != 0){
         walk(500, 550);
         dr = getUltra(2);
         
         if (millis() - flag_u > 5000)break;
      }
      
      //walks a little more for guarantee
      walk(500, 550);
      delay(1000);
      walk(0, 0);
      delay(70);

      //turns right and reads the value of right ultra
      turn(-80);
      dr = getUltra(2);

      //walks until it detects obstacle or for 5 seconds
      flag_u = millis();
      while(dr > 8 || dr== 0) {
         walk(500, 550);
         dr = getUltra(2);
         if (millis() - flag_u > 5000)break;
      }
      
      //walks a little more for guarantee
      walk(500, 550);
      delay(500);
      walk(0, 0);

      //walks until it doesn't detect obstacle or for 5 seconds
      flag_u = millis();
      while(dr < 15 && dr != 0){
         walk(500, 550);
         dr = getUltra(2);
         if (millis() - flag_u > 5000)break;
      }
      
      //walks a little more for guarantee
      walk(500, 550);
      delay(1750);
      walk(0, 0);
      delay(70);

      //turns left and reads the value of left ultra
      turn(-90);
      dr = getUltra(2);

      //walks until it detects obstacle or for 5 seconds
      flag_u = millis();
      while(dr > 15 || dr == 0) {
         walk(500, 550);
         dr = getUltra(2);
         if (millis() - flag_u > 5000)break;
      }


      //walks until it detects black
      flag_u = millis();
      array_read();
      while(els<=BLACK && ers<=BLACK){
         array_read();
         walk(600, 600);
      }
      if(ers<=BLACK){while(ers<=BLACK){array_read();walk(0, 600);}}
      if(els<=BLACK){while(els<=BLACK){array_read();walk(600, 0);}}
      
      //reads the right ultrasonic value
      dr = getUltra(2);

      //walks a little more for guarantee
      walk(500, 550);
      delay(500);

      //turns left
      turn(70);
      while (rs < BLACK){
         left(600);
         array_read();
      }

      //does pid very slowly
      flag_u = millis();
      while(millis() - flag_u > 500) {
         array_read();
         PIDwalk(0.3);
      }

      //walks back what it has gone forward
      walk(-600, -600);
      delay((int)(dr/getDynaVelocity(600)*1000));
   }
 }

 float getmUltra(int u, int den)//function used to get the media of an us sensor
 {
  float m_ultra = 0.0;
  for(int i = 0; i < den; i++)
  {
    float aux = getUltra(u);
    if(aux != 0.0) m_ultra += aux;
    else m_ultra += 200;
  }
  return m_ultra/den; 
 }
