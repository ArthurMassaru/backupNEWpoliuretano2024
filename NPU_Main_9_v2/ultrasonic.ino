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
   int dist = getUltra(4);
   if (dist < 9.0 && dist != 0.0)count_u++;
   else count_u = 0;
   if (els > BLACK || ers > BLACK)count_u=0;

   if (count_u > 7) {
      walk_distance(-2.0);
      Obstacle('r');
   }
}



void Obstacle(char c) {

   //timer for ultrasonic
   unsigned long flag_u = millis();
   int obs = 0;
   bool confirm=0;
   int counter_ref = 0;

   //if the right side is greater, it turns right
   vl = getmnUltra(1, 15);
   vr = getmnUltra(2, 15);
   if (vr >= vl)obs=1;
   else obs = -1;

   //side 
   float dl;
   
   
   //Stop the robot and turn
   back(1000);
   delay(30);
   freeze(30);
   turn(-90*obs);


   //walk back
   walk(0, 0);
   walk(SWL, SWR);
   delay(500);

   //adjusts itself to be aligned perpendicular to the black line
   array_read();
   flag = millis();
   while(els<=BLACK && ers<=BLACK && millis()-flag<800){
      array_read();
      walk(-600, -600);
   }
   if(ers<=BLACK && millis()-flag<800){while(ers<=BLACK && millis()-flag<800){array_read();walk(0, -600);}}
   if(els<=BLACK && millis()-flag<800){while(els<=BLACK && millis()-flag<800){array_read();walk(-600, 0);}}

   //reads the side ultrasonic value
   dl = getUltra(obs==1 ? 1 : 2);

   //walks until it doesn't detects obstacle or for 5 seconds
   flag_u = millis();
   walk(SWL, SWR);
   while(dl < 25.0 && dl != 0){
      delay(5);
      dl = getUltra(obs==1 ? 1 : 2);
      if (millis() - flag_u > 5000)break;
   }
   
   //walks a little more for guarantee
   walk(SWL, SWR);
   delay(1300);
   walk(0, 0);
   delay(70);

   //turns left and reads the value of left ultra
   turn(85*obs);

 
   //REFERENCIA










   //updates side value
   dl = getUltra(obs==1 ? 1 : 2);

   //walks until it detects obstacle or for 5 seconds
   walk(SWL, SWR);
   flag_u = millis();
   while(dl > 20.0 || dl== 0.0) {
      array_read();
      delay(5);
      dl = getUltra(obs==1 ? 1 : 2);

      if (millis() - flag_u > 5000)break;
      
      if (NOSIB() >= 2)
      {
         counter_ref+=5;
      }

      if (counter_ref > 5){
         do
         {
            array_read();
            walk(SWL,SWR);
            delay(5);
         }
         while(NOSIB() >= 1);   
         //turns right
         turn(-90*obs);

         //walks back what it has gone forward
         walk(-600, -600);
         delay(200);
         //does pid very slowly
         flag_u = millis();
         while(millis() - flag_u < 1000) {
            array_read();
            PIDwalk(0.3);
         }
         walk(0,0);
         freeze(25);
         return;
      }
   }
   counter_ref = 0;
   
   //walks a little more for guarantee
   walk(SWL, SWR);
   
   flag_u = millis();
   while (millis() - flag_u < 1250) {
      array_read();
      delay(5);
      if (NOSIB() >= 2)
      {
         do
         {
            array_read();
            walk(SWL,SWR);
            delay(5);
         }
         while(NOSIB() >= 1);
         //turns right
         turn(-90*obs);

         //walks back what it has gone forward
         walk(-600, -600);
         delay(200);

         //does pid very slowly
         flag_u = millis();
         while(millis() - flag_u < 1000) {
            array_read();
            PIDwalk(0.3);
         }


         freeze(25);
         return;
      }
   }
   freeze(50);
   
   //walks a little more for guarantee
   walk(SWL, SWR);
   flag_u = millis();
   while (millis() - flag_u < 500) {
      array_read();
      delay(5);
      if (NOSIB() >= 2)
      {
         do
         {
            array_read();
            walk(SWL,SWR);
            delay(5);
         }
         while(NOSIB() >= 1);
         //turns right
         turn(-90*obs);

         //walks back what it has gone forward
         walk(-600, -600);
         delay(200);
         //does pid very slowly
         flag_u = millis();
         while(millis() - flag_u < 1000) {
            array_read();
            PIDwalk(0.3);
         }


         freeze(25);
         return;
      }
   }
   walk(0, 0);

   //walks until it doesn't detect obstacle or for 5 seconds
   flag_u = millis();
   walk(SWL, SWR);
   counter_ref = 0;


   //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA


   //walks a little more for guarantee
   walk(SWL, SWR);
   flag_u = millis();
   while (millis() - flag_u < 1250) {
      array_read();
      delay(5);
      if (NOSIB() >= 2)
      {
         do
         {
            array_read();
            walk(SWL,SWR);
            delay(5);
         }
         while(NOSIB() >= 1);
         //turns right
         turn(-90*obs);

         //walks back what it has gone forward
         walk(-600, -600);
         delay(200);
         //does pid very slowly
         flag_u = millis();
         while(millis() - flag_u < 1000) {
            array_read();
            PIDwalk(0.3);
         }


         walk(0,0);
         freeze(25);
         return;
      }
   }
   walk(SWL, SWR);
   delay(500);

   //turns left and reads the value of left ultra
   turn(85*obs);
   freeze(25);


   //updates side value
   dl = getUltra(obs==1 ? 1 : 2);

   //walks until it detects obstacle or for 5 seconds
   walk(SWL, SWR);
   flag_u = millis();
   while(dl > 20.0 || dl== 0.0) {
      array_read();
      delay(5);
      dl = getUltra(obs==1 ? 1 : 2);

      if (millis() - flag_u > 5000)break;
      
      if (NOSIB() >= 2)
      {
         counter_ref+=5;
      }

      if (counter_ref > 5){
         do
         {
            array_read();
            walk(SWL,SWR);
            delay(5);
         }
         while(NOSIB() >= 1);   
         //turns right
         turn(-90*obs);

         //walks back what it has gone forward
         walk(-600, -600);
         delay(200);
         //does pid very slowly
         flag_u = millis();
         while(millis() - flag_u < 1000) {
            array_read();
            PIDwalk(0.3);
         }
         walk(0,0);
         freeze(25);
         return;
      }
   }
   counter_ref = 0;


   //walks a little more for guarantee
   walk(SWL, SWR);
   
   flag_u = millis();
   while (millis() - flag_u < 1250) {
      array_read();
      delay(5);
      if (NOSIB() >= 2)
      {
         do
         {
            array_read();
            walk(SWL,SWR);
            delay(5);
         }
         while(NOSIB() >= 1);
         //turns right
         turn(-90*obs);

         //walks back what it has gone forward
         walk(-600, -600);
         delay(200);

         //does pid very slowly
         flag_u = millis();
         while(millis() - flag_u < 1000) {
            array_read();
            PIDwalk(0.3);
         }


         freeze(25);
         return;
      }
   }
   freeze(50);
   
   //walks a little more for guarantee
   walk(SWL, SWR);
   flag_u = millis();
   while (millis() - flag_u < 500) {
      array_read();
      delay(5);
      if (NOSIB() >= 2)
      {
         do
         {
            array_read();
            walk(SWL,SWR);
            delay(5);
         }
         while(NOSIB() >= 1);
         //turns right
         turn(-90*obs);

         //walks back what it has gone forward
         walk(-600, -600);
         delay(200);
         //does pid very slowly
         flag_u = millis();
         while(millis() - flag_u < 1000) {
            array_read();
            PIDwalk(0.3);
         }


         freeze(25);
         return;
      }
   }
   walk(0, 0);

   //walks until it doesn't detect obstacle or for 5 seconds
   flag_u = millis();
   walk(SWL, SWR);
   counter_ref = 0;


   //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA


   //walks a little more for guarantee
   walk(SWL, SWR);
   flag_u = millis();
   while (millis() - flag_u < 1250) {
      array_read();
      delay(5);
      if (NOSIB() >= 2)
      {
         do
         {
            array_read();
            walk(SWL,SWR);
            delay(5);
         }
         while(NOSIB() >= 1);
         //turns right
         turn(-90*obs);

         //walks back what it has gone forward
         walk(-600, -600);
         delay(500);
         //does pid very slowly
         flag_u = millis();
         while(millis() - flag_u < 1000) {
            array_read();
            PIDwalk(0.3);
         }


         walk(0,0);
         freeze(25);
         return;
      }
   }
   walk(SWL, SWR);
   delay(500);

   //turns left and reads the value of left ultra
   turn(85*obs);
   freeze(25);

   array_read();
   while (counter_ref < 5) {
      walk(SWL, SWR);
      if (NOSIB() >= 2)counter_ref+=5;
      array_read();
   }

   while(NOSIB() >= 1) {
      array_read();
      walk(SWL,SWR);
   }
   
   //turns right
   turn(-85*obs);

   //does pid very slowly
   flag_u = millis();
   while(millis() - flag_u < 1000) {
      array_read();
      PIDwalk(0.3);
   }

   //walks back what it has gone forward
   walk(-600, -600);
   delay(500);
   
   walk(0,0);
   freeze(25);
}

float getmUltra(int u, int den)//function used to get the media of an us sensor
{
   float m_ultra = 0.0;
   for(int i = 0; i < den; i++)
   {
      float aux = getUltra(u);
      if(aux != 0.0) m_ultra += aux;
      else m_ultra += 200;
      delay(3);
   }
   return m_ultra/den; 
}
