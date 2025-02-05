//function to look for green
void analyze_green() {

  //bool for detecting green on crossroad
  int leftgreen=0, rightgreen=0;


  //starts walking while looking for green
  walk(400, 400);

  //if it detects green once, keeps the information
  for (int i = 0; i < 10; i++) {
    readLED();
    if (i == 7){
      walk(0, 0);
      delay(10);
      readLED();
    }
    else walk(300, 300);

    //condition for green: green is the lowest value on the color read and red value is higher than blue
    if(gl < rl && gl < bl && rl > bl && higher(rl, gl, bl)-lower(rl, gl, bl)>=20){
      leftgreen=1;
    }
     
    if(gr < rr && gr < br && rr > br && higher(rr, gr, br)-lower(rr, gr, br)>=20){
      rightgreen=1;
    }
    
    delay(10);
  }
  

  //if there's green on both sides
  if (leftgreen == 1 && rightgreen == 1) {

    //ledcontrol for debug "there are 2 greens"
    LEDcontrol(1,0,0);

    //turn on the green led and turns until it's on a black line
    digitalWrite(LEDG, 1);
    turn(-140);
    while (ls < BLACK){
      right(600);
      array_read();
    }

    //stops
    walk(0, 0);
    digitalWrite(LEDG, 0);

    //turns off ledcontrol for debug "there are 2 greens"
    LEDcontrol(0,0,0);
  }

  //if there's green on left side
  else if (leftgreen == 1) {
    
    //ledcontrol for debug "there's a green"
    LEDcontrol(0,1,0);

    //turn on the green led and turns until it's on a black line
    digitalWrite(LEDG, 1);
    walk(600, 600);
    delay(200); 
    turn(70);
    while (rs < BLACK){
      left(700);
      array_read();
    }

    //stops
    walk(0, 0);
    digitalWrite(LEDG, 0);

    //turns off ledcontrol for debug "there's a green"
    LEDcontrol(0,0,0);
  }

  //if there's green on right side
  else if (rightgreen == 1) {
    //ledcontrol for debug "there's a green"
    LEDcontrol(0,1,0);

    //turn on the green led and turns until it's on a black line
    digitalWrite(LEDG, 1);
    walk(600, 600);
    delay(200);
    turn(-70);
    while (ls < BLACK){
      right(600);
      array_read();
    }

    //stops
    walk(0, 0);
    digitalWrite(LEDG, 0);

    //turns off ledcontrol for debug "there's a green"
    LEDcontrol(0,1,0);
  }
  
  //if there's no green
  else {
    //ledcontrol for debug "there's NOT a green"
    LEDcontrol(0,0,1);

    walk(-SWL, -SWR);
    delay(150);
    
    //does pid for a while
    unsigned long f = millis();
    while (millis() - f < 1500) {
      array_read();
      PIDwalk(0.6);
      delay(5);
    }

    //turns off ledcontrol for debug "there's NOTa green"
    LEDcontrol(0,0,1);
  }
  
}
