//if the lowest value is blue and the highest value is green and blue > 600 detects room3 at the silver line
void DetectRoom() {
  readLED();
  int deltal = higher(rl, gl, bl) - lower(rl, gl, bl);
  int deltar = higher(rr, gr, br) - lower(rr, gr, br);
  //if ((rl > 850 && rr > 850 && gl > 850 && gr > 850 && bl > 850 && br > 850 && deltar > 12 && deltal > 12) || (els < 50 && ls < 50 && rs < 50 && ers < 50 && ms < 600))detec++;
  if(els < 250 && ls < 250 && ms < 500 && rs < 250 && ers < 250)detec++;
  else {
    if(detec>=5)detec-=10;
    else detec=0;
  }
  Serial.println(detec);

  if(detec >= 25 && ram <= 1)
  {
    walk_distance(-7.0);
    //Troom();
    Nroom();
  }
}

void NPU_DetectRoom(){
  array_read();//reads the  value of the array sensor
  if(els < 200 && ls < 200 && rs < 200 && ers < 200 && ms < 200)s++;//if the robot got its sensors in white sums 2 do s
  else {Serial.println("REF");array_print();s--;}//else subtracts 1 from s
  if(s<0)s=0;//if s is lower than 0 then s is than 0
  if(s>40){//room 3
    // while (els < 400 && ls < 400 && rs < 400 && ers < 400 && ms < 400) {
    //   walk(-SWL, -SWR);
    // } 
    freeze(10);
    walk_distance(-20.0);
    New_Room();
  }
}

void touch(int lim_p) 
{
  unsigned long flag_p = millis();
  while (digitalRead(TOUCH1) || digitalRead(TOUCH2) )// Until the two sensors touch the wall
  {
    Serial.print(digitalRead(TOUCH1));
    Serial.println(digitalRead(TOUCH2));
    walk(-1023,-1023); // while the back buttons dont touch the wall make the robot go back
    if(!digitalRead(TOUCH1))walk(0, -1023); // while the left back button doesn´t touch the wall walk with the left motor 
    if(!digitalRead(TOUCH2))walk(-1023, 0); // while the right back button doesn´t touch the wall walk with the right motor
    delay(15);
    if(millis() - flag_p > lim_p)break;
  }
  freeze(100);
}

bool lu=0, ru=0;
float ml, mr;
//Function for adjust the robot on the wall
void adjust_on_wall() {

  if (lu) {
    turn(90);
    walk(500, 480);
    delay(100);

    touch(2000);
    lu=1;
  } else if (ru) {
    turn(-90);
    walk(500, 480);
    delay(100);

    touch(2000);
    ru=1;

  } else {

    ml=val_left, mr=val_right;

    if (mr < ml || ml == -1) {
      turn(90);
      walk(500, 480);
      delay(100);

      touch(2000);
      walk(500, 480);
      delay(100);

      ru=1;
    } else {
      turn(-90);
      walk(500, 480);
      delay(100);

      touch(2000);
      walk(500, 480);
      delay(100);

      lu=1;
    } 
  }
}

void go_to_middle() {
  //enter the room
  float vertical_size=0.0;
  walk(0, 0);
  delay(500);
  walk(800, 770);
  delay(1000);
  walk(0, 0);
  delay(50);
  
  val_left = 0.0;
  val_right = 0.0;
  // Walks a little and sets both ultrasonics values in 0.0

  //measures the room
  for (int i = 0; i < 5; i++) {
    float aux_l = getUltra(1);
    float aux_r = getUltra(2);
    
    if (aux_l != 0.0 && aux_l < 100.0)val_left+= aux_l;
    else val_left += 200;

    if (aux_r != 0.0 && aux_r < 100.0)val_right+= aux_r;
    else val_right += 200;
    
    walk(500, 480);
    delay(100);
  }
  val_left /= 5.0;
  val_right /= 5.0;
  // Gets the average value of the left and right ultrassonics values
  dists(0);
  if(val_top != -1)vertical_size=val_top;

  
  // makes the robot adjust in the closer wall
  if (val_left < 20.0 || val_right < 20.0) {

    //gets the first top distance of the room
    dists(0);
    float d1 = val_top;
    distsLR();

    adjust_on_wall();

    //gets the distance to the wall
    dists(0);

    //room horizontal size
    float aux = val_top;
    _size = aux;
    
    //walks a little
    walk(1000, 1000);
    delay(100);
    // gets all the measuraments of the room while walks to the middle of it
    flag=millis();
    walk(800, 770);
    float auxl=0.0, auxr=0.0, time30=2000;
    while(millis()-flag<=time30+350){
      walk(800, 770);
      distsLR();
      if(val_left!=-1)auxl=val_left;
      if(val_right!=-1)auxr=val_right;
    }
    walk(0, 0);
    float length=0.0;

    if((auxl+auxr+10)>90 || auxr==0 || auxl==0)length=120.0; 
    else length=90;
    // sets the length of the room by using the two ultrassonic sensors value

    adjust_on_wall();
    

    walk(1000, 1000);
    delay(100);

    flag=millis();
    walk(800, 770);
    auxl=0.0, auxr=0.0;

    float mid_time=0;

    if(length==90)mid_time=2000;
    else mid_time=3500;
    // sets the time for the robot to go mid based o the dimensions of room 3 

    while(millis()-flag<=mid_time){
      distsLR();
      walk(800, 770);
      if(val_left!=-1)auxl=val_left;
      if(val_right!=-1)auxr=val_right;
    }
    walk(0, 0);
    float width=0.0;

    if((auxl+auxr+10)>90 || auxl==0 || auxr==0)width=120.0;
    else width=90;
    // sets the witdh of the room by using the two ultrassonic sensors value
    if (lu==1)turn(90); 
    else turn(-90);
    // analizes where the robot is facing and makes it turn to the right side
    
    if(width==120){
      Serial.println(auxl);
      Serial.println(auxr);
      if(auxl<auxr && auxl != 0){
        walk(800, 770);
        delay((time30+400)/2);
        turn(90);
      }
      // gets the robot distance to mid and makes it walk to it
      else{
        walk(-800, -800); 
        delay((time30+400)/2);
      }
      // go to mid
     
    }
    walk(0, 0);



    /*
    
    //walks until it goes to the middle of the room or until it detects "exit"
    while (aux >= (((val_top + 20.0)/ 2.0) - 10.0) && aux !=-1) {
      aux=0.0;

      //med of 3 values
      for(int i = 0; i < 3; i++){
        float med = 0.0;
        med = getUltra(3);
        
        //if ultrasonic value is too high or too low it sets as -1
        if(med == 0.0 || med > 110.0){
          i = 21;
          aux = -1;
        }
        else {
          aux += med; //sum of raw value
        }
      }
      if (aux != -1)aux/=3;

      //walks while
      walk(500, 480);
    }

    //stop
    walk(0, 0);
    delay(50);


    //if it didn't detect exit
    if (aux != -1) {

      //search for exit on the middle of the perpendicular direction to the robot
      if (lu==1) {

        //for 1 second
        unsigned long f = millis();
        while (millis() - f < 1000) {
          walk(500, 480);
          for(int i = 0; i < 3; i++){
            float med = 0.0;
            med = getUltra(1);
            
            //if ultrasonic value is too high or too low it sets as -1
            if(med == 0.0 || med > 120.0){
              i = 21;
              aux = -1;
            }
            else {
              aux += med; //sum of raw value
            }
          }
          if (aux != -1)aux /= 3;
        }

        //walks back a little more so the touch sensors will work
        if (aux == -1) {
          walk(-800, -800);
          delay(millis() - f + 700);
          adjust_on_wall();
        } else {
          walk(-800, -800);
          delay(millis() - f);
          adjust_on_wall();
        }

      } 

      //other sensor
      else {

        //for 1 second
        unsigned long f = millis();
        while (millis() - f < 1000) {
          walk(500, 480);
          for(int i = 0; i < 3; i++){
            float med = 0.0;
            med = getUltra(3);
            
            //if ultrasonic value is too high or too low it sets as -1
            if(med == 0.0 || med > 110){
              i = 21;
              aux = -1;
            }
            else {
              aux += med; //sum of raw value
            }
          }
          if (aux != -1)aux /= 3;
        }

        //walks back a little more so the touch sensors will work
        if (aux == -1) {
          walk(-500, -500);
          delay(millis() - f + 700);
          adjust_on_wall();
        } else {
          walk(-500, -500);
          delay(millis() - f);
          adjust_on_wall();
        }
      }
    }
    
    //if it detects exit adjusts on the wall far from the entrance
    else {
      walk(800, 770);
      delay(2000);
      adjust_on_wall();
    }
    
    //walks a little
    walk(1000, 1000);
    delay(100);

    //if there's no exit on the first top read, that's the size of the room
    if (d1 != -1) {
      walk(800, 770);
      delay((1000*(d1/2))/14);

      //stop
      walk(0, 0);
      delay(50);
    } 
    
    else {
      //goes to the middle
      aux = 10000.0;
      while (aux >= (((val_top + 20.0)/ 2.0) - 10.0) && aux !=-1) {
        aux=0.0;

        for(int i = 0; i < 3; i++){
          float med = 0.0;
          med = getUltra(3);
          
          //if ultrasonic value is too high or too low it sets as -1
          if(med == 0.0 || med > 120.0){
            i = 21;
            aux = -1;
          }
          else {
            aux += med; //sum of raw value
          }
        }
        if (aux != -1)aux/=3;
        
        walk(500, 480);
      }
      
      //stop
      walk(0, 0);
      delay(50);
    }

    //the horizontal size of the room is the sum of left, right and robot length if the first distance was -1 (0)
    distsLR();
    if (size == -1)size = (int)(val_left + val_right) + 12;


    //turn to the opposite side of turn on adjust on wall (valid for all rooms)
    if (lu==1)turn(90);
    else turn(-90);
    
    //while it's not on the middle
    dists(0);
    aux = val_top;
    if (aux <= (((float)size/2.0) - 3.0)) {
      while (aux <= (((float)size/2.0) - 3.0) && aux !=-1) {
        aux=0.0;
        for(int i = 0; i < 3; i++){
          float med = 0.0;
          med = getUltra(3);
          
          //if ultrasonic value is too high or too low it sets as -1
          if(med == 0.0 || med > 110.0){
            i = 21;
            aux = -1;
          }
          else {
            aux += med; //sum of raw value
          }
        }
        if (aux != -1)aux/=3;
        walk(-500, -500);
      }
    
    } else {
      
      while (aux >= (((float)size/2.0) - 3.0) && aux !=-1) {
        aux=0.0;
        for(int i = 0; i < 3; i++){
          float med = 0.0;
          med = getUltra(3);
          
          //if ultrasonic value is too high or too low it sets as -1
          if(med == 0.0 || med > 110.0){
            i = 21;
            aux = -1;
          }
          else {
            aux += med; //sum of raw value
          }
        }
        if (aux != -1)aux/=3;
        walk(500, 480);
      }

    }
    if (lu==1)turn(30);
    else turn(-30);*/
  }
  
  //if the values of left or right are around 30, the entrance was in the middle
  else {
    //if the exit isn't on the middle
    dists(0);
    Serial.println(val_top);
    if (vertical_size != -1 ) {
      
            
      //imossible value to do while once
      val_top = 10000.0;
      
      //walks until it goes to the middle of the room or until it detects "exit"
      while (vertical_size/2  <= val_top) {
        val_top=0.0;

        //med of 3 values
        for(int i = 0; i < 3; i++){
          float med = 0.0;
          med = getUltra(3);
          
          //if ultrasonic value is too high or too low it sets as -1
          if(med == 0.0 || med > 110.0){
            i = 21;
            val_top = -1;
          }
          else {
            val_top += med; //sum of raw value
          }
        }
        if (val_top != -1)val_top/=3;

        //walks while
        walk(500, 480);
      }

    } 
    //if there's exit in the middle 
    else {
      
      //walks a little to the right to measure vertical size
      turn(-90);
      walk(800, 770);
      delay(1500);
      turn(90);

      //gets the size of the room
      dists(0);
      vertical_size = val_top;
      _size = val_left + val_right + 12;

      //imossible value to do while once
      val_top = 10000.0;
      
      //walks until it goes to the middle of the room or until it detects "exit"
      while (vertical_size/2  <= val_top && val_top != -1) {
        val_top=0.0;

        //med of 3 values
        for(int i = 0; i < 3; i++){
          float med = 0.0;
          med = getUltra(3);
          
          //if ultrasonic value is too high or too low it sets as -1
          if(med == 0.0 || med > 110.0){
            i = 21;
            val_top = -1;
          }
          else {
            val_top += med; //sum of raw value
          }
        }
        if (val_top != -1)val_top/=3;

        //walks while
        walk(500, 480);
      }

      //turns right
      turn(-90);
      
      //while it's not on the middle
      dists(0);
      val_top = 0.0;
      while (val_top <= (((float)_size/2.0) - 3.0)) {
        val_top=0.0;
        for(int i = 0; i < 3; i++){
          float med = 0.0;
          med = getUltra(3);
          
          //if ultrasonic value is too high or too low it sets as -1
          if(med == 0.0 || med > 110.0){
            i = 21;
            val_top = -1;
          }
          else {
            val_top += med; //sum of raw value
          }
        }
        if (val_top != -1)val_top/=3;
        walk(-500, -500);
      }

      //goes to the right position  
      turn(135);

    }

  }

  
  walk(0, 0);
  delay(50);
  Room3();
  

}
