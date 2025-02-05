void go_to_dist (float final_d)// Function used to make the robot go to a distance
{
  int n_top = 0,n_bot = 0;
  
  do {
    walk(SWL,SWR);

    get_vbt();//Read top and bot ultrassonics

    //    Serial.print("nbot: ");
    //    Serial.print(n_bot);
    //    Serial.print(" n_top: ");
    //    Serial.println(n_top);


    if(vt < final_d && vt  != 0) n_top++;//if the top us value is lower than the distance wanted adds to the top counter
    
    else n_top = 0; // else resets the top count
    

    if(vb < 7.0 && vb > 0) n_bot++;//if the bot value is lower than 8.0cm adds to the bot counter
    
    else n_bot = 0;// else resets the top count
    

    if(n_bot >= 2) // if the bot counter gets higher or equals 2 it is a victim
    {
      freeze(500);//makes the robot stop for 500 milliseconds
      
      SwallowBalls();//catches the victim
    }
    
    delay(25);//default delay for reading values
  }
  
  while(n_top < 2);//do the above while the top counter is lower than 2
  
  freeze(100);//stop for 100 millisecons  
}



void walk_distance(float d)//function used to make the robot walk to a specific distance based on experiments
{
  int sign = abs(d) / d; //if sign is negative makes the robot walk back
  walk(SWL*sign,SWR*sign);
  delay((int)(125*abs(d)));
  freeze(100);
}



void scanner(int area_scanned,int apt,int dist_to_ball)//scanning function used to scan an area it receives the area to scan and apt(angles per turn)
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

    if ( vb < dist_to_ball && vb != 0) cont_b++;//if the bot us value is lower than 25.0cm adds to the ball counter
    else cont_b = 0;//else it resets the ball counter
    

    if( cont_b >= 2) //if the ball counter gets higher than 2 it is a victim
    {
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
  
  c_angle += area_scanned;  
  update_angle();
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


void update_angle()
{
  if(c_angle < 0)//if angle is negative 
  {
    c_angle += 360;//adds 360 
  }
  else if(c_angle > 360)//else if angle is higher than 360
  {
    c_angle -= 360;//subtracts 360
  }
}