void go_to_mid_120()
{
  if(roomL == 120)
  {
    if(ent_m == 0)// left/right entrance
    {
      turn(ent_s*90);//turn 90 deegres right / left
      touch(3000);//touch function

      walk_distance(32.0);//goes to next tile that is garanted to not be an exit 
      turn(ent_s*-90);//turns
      touch(3000);//adjusts on wall
      media_vt = getmUltra(3,15);//gets the media of the top US
      if(media_vt > 3000.0)//exit
      {
        turn(ent_s*90);//turns back
        walk_distance(R_MID_90);//goes to mid on the x axis
        turn(ent_s*-90);//turns to face the y axis
        touch(2000);//adjusts on wall
        go_to_dist(R_MID_90);//goes to mid on y axis
      }
      else//theres no exit so its safe to continue the default path
      {
        walk_distance(R_MID_90);//goes to mid on the y axis
        turn(ent_s*90);//turns to face the x axis
        freeze(25);
        media_vt = getmUltra(3,15);//gets the media of top US
        if(media_vt > 3000.0)//exit
        {
          turn(ent_s*-90);//goes back to the previous position
          touch(8000);//goes back to the previous position
          turn(ent_s*90);//makes the robot face the x axis
          go_to_dist(R_MID_120);//goes to mid on the y axis
        }
        else//theres no exit so its safe to continue the default path 
        {
          walk_distance(18.0);//goes to mid on the y axis
        }
      }
      
    }


    else //mid entrance
    {
      media_vl = getmUltra(1,15);//gets the media of the left US
      delay(25);//delay for getting the information
      media_vr = getmUltra(2,15);//gets the media of the right US
      //set ent_s based on the closest wall to the robot
      if(media_vl > 90.0 && media_vl < 100.0) ent_s = 1;// if the left media is between 90 and 100, entrance side is right
      else if(media_vr > 90.0 && media_vr < 100.0) ent_s = -1;//if the right media is between 90 and 100, entrance side is left
      else ent_s = -1; //else, entrance side is left

      media_vt = getmUltra(3,15);//gets the media of the top US
      if(media_vt > 3000.0)
      {
        turn(ent_s*90);//makes the robot face the x axis
        walk_distance(R_MID_120);//goes to mid on the y axis
        turn(ent_s*-90);//turn -90
        touch(2000);//adjusts on wall
        go_to_dist(R_MID_90);//goes to mid on the y axis
      }
      else
      {
        walk_distance(R_MID_90);//goes to mid on the y axis
        turn(ent_s*90);//makes the robot face the x axis
        media_vt = getmUltra(3,15);//gets the media of the top US
        if(media_vt > 3000.0)//if the top media is more than 120
        {
          turn(ent_s*-90);//turn -90
          walk_distance(-(R_MID_90));//goes back on the y axis
          turn(ent_s*90);//makes the robot face the x axis
          touch(2000);//ajusts on wall
          walk_distance(R_MID_120);//goes to mid on the y axis
          turn(ent_s*-90);//go to -90
          go_to_dist(R_MID_90);//goes to mid on the y axis
        }
        else
        {
          walk_distancewb(16.0);//goes to mid on the y axis
        }
      }
    }
    
  }


  else
  {
    
    if(ent_m == 0)
    {
      turn(ent_s*90);
      touch(5000);//adjusts on wall
      walk_distancewb(R_MID_90);//goes to mid x axis
      turn(ent_s*-90);//turns to face the y axis
      touch(5000);
      walk_distancewb(R_MID_120);//goes to mid on the y axis
    }

    else //mid entrance
    {
      walk_distancewb(R_MID_120 - 10.0);//goes to mid in y axis 
    }
  }
}



void New_Room()
{
  
  scan_entrance();// scan the entrance
  
  if (roomF == 120)
  {
    LEDcontrol(0,1,0);
    delay(3000);
    LEDcontrol(0, 0, 0);
    delay(50);
  }
  else 
  {
    LEDcontrol(1, 0, 0);
    delay(3000);
    LEDcontrol(0, 0, 0);
    delay(50);
  }

  if (roomL == 120)
  {
    LEDcontrol(0,1,0);
    delay(3000);
    LEDcontrol(0, 0, 0);
    delay(50);
  }
  else 
  {
    LEDcontrol(1, 0, 0);
    delay(3000);
    LEDcontrol(0, 0, 0);
    delay(50);
  }

  // if(roomF == 120 && roomL == 90 &&  ent_s == -1)while(1);
  bool z = 0;
  if(roomL == 90 && roomF == 90) {
    if(special == 0)go_to_mid();// goes to mid on the 90x90 room

    //rescue the victims 
    scan_turn(360,26.0);
    if(balls == 0)
    {
      turn(3);
    }
    else if(balls == 2)
    {
      turn(-3);
    }
    else
    {
      turn(-7);
    }

    //starts triangle
    t = getmUltra(3, 15);// gets a media of top US
    if (t < 80.0) { // if there´s a wall 
      walk_distance(30.0);//walks until you almost touched id
      turn(180);//turns to align
      touch(2000);// aligns
      walk_distance(5.0);//walks a little just to get out of the wall
    } 
    
    else { //if there´s an exit on the top US the robot scans starting on the left wall
      turn(-90);
      walk_distance(30.0);
      touch(2000);
      walk_distance(5.0);
    }
  
    turn(-90);//turns 90 deegres counterclock wise 
    tri_90();
    media_sp=getmsharp(10);//gets the media of sharp sensor
    Serial.print("SHARP: ");
    Serial.println(media_sp);
    if(media_sp >= 200.0) //if there´s a wall adjusts
    {
      turn(90);//turns to adjust on wall
      touch(2000);//adjusts on wall
      walk_distance(8.0);//walks to get out of the wall
      turn(-90);//turns to stay parallel to the wall    
      
    }
    else //if there's no wall on the right
    {
      turn(-90);//turn 90 
      check_exit();//check to see if that was an exit
      //if that was the entrance
      if(room_clear==0){
        freeze(100);//the robot is now in its initial position
        walk_distance(3.0);//walks to restart the cycle
        turn(-90);//turns 90  
        
        if (side[l] == 0) {//if the robot isn´t in the same side as a triangle 
          walk_distance(R_MID_90-10.0);//walks to adjust on both walls
          turn(90);//turns do adjust on wall
          touch(3000);//adjusts on wall
          walk_distance(3.0);//walks to get out of the wall
          media_sp=getmsharp(5);
          Serial.print("SHARP: ");
          Serial.println(media_sp);
          if(media_sp <= 200){
            walk_distance(7.0);
            turn(-90);
            check_exit();
            room_clear=1;
          }
          
        } 
        else {
          go_to_distnb(30.0);
          turn(45);//turns 45 degrees to stay parallel to the triangle
          walk_distance(30.0);//the triangle
          turn(45);//turns to stay parallel to the wall
          walk_distance(10.0);//walks to restart the cycle
          l = l < 3 ? ++l : l=0;
        }
      
      }
      if (room_clear == 0) { //if the robot didn´t detect exit
        media_sp=getmsharp(10);//gets the media of the sharp sensor
        Serial.print("SHARP: ");
        Serial.println(media_sp);
        if(media_sp <= 230.0)//if there´s an exit
        {
          walk_distance(7.0);//walks a little
          turn(-90);//turns to face the exit
          check_exit();//checs exit
          room_clear=1;
        }
        else//that isn´t the exit so we can adjust on that wall
        {
          turn(90);//turns to make the back of the robot face the wall
          touch(3000);//adjusts on the wall
          walk_distance(7.0);//walks a little
          turn(-90);//turns back
        }
      }  
    }
    //scan for exit
    Serial.println(l);
    for(int i=0;i<=3;i++){
      Serial.println(side[i]);
    } 
    //after the triangle search: array_read();
    // #endif

    //AFTER the triangle search
    //walk and look for wall at both sides to adjust
    // bool z =0;
    //cycle searching for exit
    while(room_clear == 0)
    {
      //gets the side media and looks for an exit. If there's none, keep going and don't consider it (z = 1) 
      walk_distance(5.0);//walks just a little to start the cycle
      media_sp=getmsharp(10);//gets the media of the sharp sensor
      Serial.print("SHARP: ");
      Serial.println(media_sp);
      if (media_sp < 230) {//if that is an "exit"
        
        walk_distance(-2.0);//walks back to check the exit
        turn(-90);//turns to face the "exit"
        //walks with a limit of time to see the exit
        
        check_exit();
        if(room_clear==0){
          turn(-90);//turns 90
          walk_distance(7.0);
        } else {
          break;
        }
      }
      //search exit with bottom sensor. If there's one, checks.
      walk_distance(7.0);//walks a little to get the media of top US

      if(side[l])//if the side of the robot is a triangle  side
      {
        go_to_distnb(30.0);//go to the start of the triangle
        turn(45);//turns 45 degrees to stay parallel to the triangle
        walk_distance(20.0);//the triangle
        turn(45);//turns to stay parallel to the wall
        walk_distance(10.0);//walks to restart the cycle
      }

      else//if there´s an "exit" on the top US
      {
        walk_distance(10.0);//walks a little to see the wall in front of the robot
        media_vt = getmUltra(3,10);//gets the media of top US
        Serial.print("TOP: ");
        Serial.println(media_vt);
        if(media_vt >= 50.0)//if that is an "exit"
        {
          walk_distance(10.0);//walks a little
          walk(0,0);
          tone(BUZZER, 155, 5);//debug
          turn(90);//turns to adjust on the adjacent wall of the exit
          touch(5000);//adjusts the robot
          walk_distance(7.0);//walks just so it can get out of the wall
          turn(-90);//turns to face the "exit"
          check_exit();//checks the exit
          if(room_clear==0){//if that isn´t an exit
            walk_distance(3.0);//walks to restart the cycle
            turn(-90);//turns 90  
            walk_distance(R_MID_90 - 10.0);//go to the next wall
            turn(90);//turns do adjust on wall
            touch(3000);//adjusts on wall
            walk_distance(3.0);//walks to get out of the wall
            turn(-90);//turns to continue to follow the wall
          } else {
            break;
          }
        }

        else //no exit/on the right
        {
          go_to_distnb(7.0);//the robot get close to the guaranted wall
          media_sp=getmsharp(10);//gets the right US media
          Serial.print("SHARP: ");
          Serial.println(media_sp);
          if (media_sp <= 230.0)//confirms that there´s an "exit" on the right side
          {
            turn(-180);//turns back
            touch(3000);// adjust on wall
            walk_distance(7.0);//walks to the exit
            turn(90);//turs to face the exit
            check_exit();//checks the "exit"
            freeze(500);
            if(room_clear==0){
              turn(90);//turns do adjust on wall
              touch(3000);//adjusts on wall
              walk_distance(3.0);//walks to get out of the wall
              turn(-90);//turns to continue to follow the wall
              walk_distance(R_MID_90 - 5.0);//walks to start a new cycle
            } else {
              break;
            }
          }
          else//else there´s no "exit" on the right
          {
            turn(90);//turns to continue the cycle
            touch(5000);
            walk_distance(R_MID_90 - 5.0);//walks to restart the cycle
          }
        }
        freeze(25);
      }
      if(l < 3)l++;
      else l=0;
    }
  }
  else
  { // 120*90
    // #ifndef SAIDA
    if(special == 0)go_to_mid_120();
    tone(BUZZER,440,1000);
    scan_turn(360,25.0);
    if(balls == 0)
    {
      turn(3);
    }
    else if(balls == 2)
    {
      turn(-1);
    }
    else
    {
      turn(-5);
    }
    bool sigma=true;
    while(sigma)
    {
      //media_vr = getmUltra(3, 30);
      media_vt = getmUltra(3, 30);
      Serial.println(media_vt);
      if(media_vt < 37.0)//the robot is facing the 120 room dimension with top us // troca por media_vr junto com o valor
      {
        //indo para de 120 *** 129 no sensor da direita
        //turn(-90);
        walk_distance(R_MID_90-10);
        turn(90);
        //turn(90);
        walk_distance(9.0);
        media_sp = getmsharp(10);
        if(media_sp <= 200.0)//exit on the right
        {
          walk_distance(-14.0);
          turn(90);
          touch(1500);
          walk_distance(7.0);
          turn(-90);
          walk_distance(10.0);
        }
        else//wall on the right
        {
          turn(90);
          touch(1500);
          walk_distance(7.0);
          turn(-90);
          walk_distance(-6.0);
        }
        sigma=false;
        l_120=2;
        break;
      }
      else if(media_vt  >= 37.0 && media_vt <= 70.0)//the robot is facing the 90 room dimension with top us trocar mpor media_vr
      {
        //(indo pra de 90) *** direita acusando sala de 90
        //turn(-90);
        walk_distance(20.0);
        tone(BUZZER,440,1000);
        float aux_1 = 0.0 ,max_1 = 0.1,aux_2 = 0.0,max_2=0.1, aux_3 = 0.0, max_3 = 0.0;
        for(int i = 0; i < 5; i++)
        {
          aux_1 = getUltra(3);
          if (aux_1 != 0.0 && max_1 != -1.0){if (aux_1 > max_1)max_1 = aux_1;}
          else max_1 = -1.0;
        }
        turn(-3);
        for(int i = 0; i < 5; i++)
        {
          aux_2 = getUltra(3);
          if (aux_2 != 0.0 && max_2 != -1.0){if (aux_2 > max_2)max_2 = aux_2;}
          else max_2 = -1.0;
        }
        turn(6);
        for(int i = 0; i < 5; i++)
        {
          aux_3 = getUltra(3);
          if (aux_3 != 0.0 && max_3 != -1.0){if (aux_3 > max_3)max_3 = aux_3;}
          else max_3 = -1.0;
        }
        turn(-3);
        float max_f = 0.0;
        max_f = higher(max_1, max_2, max_3);
        if(max_f <=50.0 ){
          walk_distance(R_MID_90 - 10.0);
          turn(180);
          touch(6000);
          walk_distance(7.0);
          turn(-90);
          l_120=1;
          sigma=false;
          break;
        }
        else{
          walk_distance(-15.0);
        }
      }
      turn(-90);
    }
    //triangle search
    tri_120();
    if(l_120 % 2 == 0) walk_distance(-15.0);
    media_sp=getmsharp(10);//gets the media of sharp sensor
    if(media_sp <= 230)//exit
    {
      turn(-90);
      check_exit();
      turn(-90);
      if(room_clear == 0)//no exit
      {
        if(l_120 % 2 == 0)
        {
          walk_distance(30.0);
          turn(90);
          touch(4000);
          walk_distance(7.0);
          turn(-90);
        }
        else
        {
          walk_distance(2.0);
          if(side[l])//if the side of the robot is a triangle  side
          {
            go_to_distnb(30.0);//go to the start of the triangle
            turn(45);//turns 45 degrees to stay parallel to the triangle
            walk_distance(20.0);//the triangle
            turn(45);//turns to stay parallel to the wall
            walk_distance(15.0);//exit the triangle
            walk_distance(30.0);//go to the starting tile
            l = l < 3 ? ++l : l=0;
          }
          else
          {
            walk_distance(25.0);//walks a little to see the wall in front of the robot
            turn(90);
            touch(3000);
            walk_distance(7.0);
            turn(-90);
            media_vt = getmUltra(3,10);//gets the media of top US
            Serial.print("TOP: ");
            Serial.println(media_vt);
            if(media_vt >= 50.0)//if that is an "exit"
            {
              check_exit();//checks the exit
              if(room_clear==0){//if that isn´t an exit
                turn(-90);//turns 90  
                walk_distance(R_MID_90 - 10.0);//go to the next wall
                l = l < 3 ? ++l : l=0;
                turn(90);//turns do adjust on wall
                touch(3000);//adjusts on wall
                walk_distance(7.0);//walks to get out of the wall
                turn(-90);//turns to continue to follow the wall
                walk_distance(30.0);//go to the starting tile
              } 
            } 
            else
            {
              turn(90);  
              walk_distance(R_MID_90 - 10.0);//go to the next wall
              walk_distance(30.0);//go to the starting tile
              l = l < 3 ? ++l : l=0;
            }
          }
          l_120 ++;
        }
      }
    }

    else 
    {
      if(l_120 % 2 == 0)
      {
        walk_distance(30.0);
      }
    }

    while(room_clear == 0)
    {
      if(l_120 % 2 == 0)//if the robot is on 120 side
      {
        LEDcontrol(0,1,0);
        delay(1000);
        LEDcontrol(0,0,0);
        media_sp = getmsharp(10);//gets the media of sharp sensor
        if(media_sp <= 230)//if there´s an exit on the right
        {
          turn(-90);//turns to face the exit
          check_exit();//check exit
          if(room_clear == 0)
          {
            turn(-90);//turns to stay parallel to the waçç
          }
          else break;//just break out of while
        }

        if(side[l] == 1)//if the robot is on the same side of the triangle
        {
          go_to_distnb(25.0);//go to the start of the triangle
          turn(45);//turns 45 degrees to stay parallel to the triangle
          walk_distance(20.0);//the triangle
          turn(45);//turns to stay parallel to the wall
          walk_distance(10.0);//walks to restart the cycle
        }
        else//if the robot isn´t on the same side of the triangle
        {
          walk_distance(10.0);
          media_vt = getmUltra(3,15);
          if(media_vt >= 50.0)//exit on the front
          {
            walk_distance(10.0);//walks a little
            walk(0,0);
            tone(BUZZER, 155, 5);//debug
            turn(90);//turns to adjust on the adjacent wall of the exit
            touch(5000);//adjusts the robot
            walk_distance(7.0);//walks just so it can get out of the wall
            turn(-90);//turns to face the "exit"
            check_exit();//checks the exitis
            if(room_clear==0){//if that isn´t an exit
              walk_distance(3.0);//walks to restart the cycle
              turn(-90);//turns 90  
              walk_distance(R_MID_90 - 10.0);//go to the next wall
              turn(90);//turns do adjust on wall
              touch(3000);//adjusts on wall
              walk_distance(7.0);//walks to get out of the wall
              turn(-90);//turns to continue to follow the wall
	          }
            else
            {
              break;
            }
          }
          else//no exit on the front
          {
            go_to_distnb(7.0);//the robot get close to the guaranted wall
            media_sp=getmsharp(10);//gets the right US media
            Serial.print("SHARP: ");
            Serial.println(media_sp);
            if (media_sp <= 230.0)//confirms that there´s an "exit" on the right side
            {
              turn(-180);//turns back
              touch(3000);// adjust on wall
              walk_distance(7.0);//walks to the exit
              turn(90);//turs to face the exit
              check_exit();//checks the "exit"
              freeze(500);
              if(room_clear==0){
                turn(90);//turns do adjust on wall
                touch(3000);//adjusts on wall
                walk_distance(7.0);//walks to get out of the wall
                turn(-90);//turns to continue to follow the wall
                walk_distance(R_MID_90 - 10.0);//walks to start a new cycle
              } 
              else
              {
                break;
              }  
            }
            else//else there´s no "exit" on the right
            {
              turn(90);//turns to continue the cycle
              touch(5000);
              walk_distance(R_MID_90 - 5.0);//walks to restart the cycle
            }
          }
        }
      }

      else //the robot is on 90 side
      {
        LEDcontrol(1,1,1);
        delay(1000);
        LEDcontrol(0,0,0);
        media_sp = getmsharp(10);//gets the media of sharp sensor
        if(media_sp <= 230)//if there´s an exit on the right side
        {
          turn(-90);//turns to face the exit
          check_exit();//checks exit     
          if(room_clear == 0)//if no exit
          {
            turn(-90);//turns to stay parallel to the wall
          }       
          else break;//breaks to get out of the loop
        }
        if(side[l] == 1)//triangle
        {
          go_to_distnb(30.0);//go to the start of the triangle
          turn(45);//turns 45 degrees to stay parallel to the triangle
          walk_distance(20.0);//the triangle
          turn(45);//turns to stay parallel to the wall
          walk_distance(10.0);//walks to restart the cycle 
          media_sp = getmsharp(10);//gets the media of sharp sensor
          if(media_sp >= 230)//wall on the right
          {
            turn(90);//turns 
            touch(3000);//adjusts on wall
            walk_distance(7.0);//walks to get out of the wall
            turn(-90);//turns to stay parallel to the wall
            walk_distance(30.0);//walks to next tile
          }
          else //exit on the right
          {
            turn(-90);//turns to face the exit
            check_exit();//checks exit
            if(room_clear == 0)//if that was an entrance
            {
              walk_distance(2.0);//walks a little
              turn(-90.0);//turns to stay parallel to the wall
              walk_distance(20.0);//walks to next tile
              turn(90);//turns to adjust on the wall
              touch(3000);//adjusts on wall
              walk_distance(7.0);//get out of the wall
              turn(-90);//turns to stay parallel to the wall
            }
            else break;
          }
        }
        else // no triangle
        {
          walk_distance(10.0);
          media_vt = getmUltra(3,15);
          if(media_vt >= 50.0)//front exit
          {
            walk_distance(20.0);//walks to face the exit
            turn(90);//turns to adjust on wall
            touch(3000);//adjusts on wall
            walk_distance(7.0);//gets out of the wall
            turn(-90);//turns to stay parallel to the wall
            check_exit();//checks exit
            if(room_clear == 0)//if that was an entrance
            {
              walk_distance(2.0); //walks a little
              turn(-90); //turns
              walk_distance(20.0); //walks to the next tile
              turn(90); //turns
              touch(3000); //touches the wall
              walk_distance(7.0); //walks to get off the wall
              turn(-90); //turns to stay parallel to the wall
              walk_distance(30.0); //walks to the next tile
            }
          }
          else //there's no front exit
          {
            go_to_distnb(7.0); //gets close to the wall
            media_sp = getmsharp(10); //gets the sharp value
            if(media_sp >= 230) //if there's no exit
            {
              turn(90); //turns
              touch(3000); //touches the wall
              walk_distance(7.0); //get off the wall
              walk_distance(R_MID_90 - 5.0); //walks to the next tile
              media_sp = getmsharp(10); //gets the sharp value

              if(media_sp >= 230) //if there's no exit
              {
                turn(90); //turns
                touch(3000); //touches the wall
                walk_distance(7.0); //walks to get off the wall
                turn(-90); //turns
                walk_distance(30.0); //walks to the next tile
              }

              else //there's an exit
              {
                turn(-90); //turns to face the exit
                check_exit(); //checks the exit
                if(room_clear == 0) //if that was an entrance
                {
                  turn(-90); //turns
                  walk_distance(30.0); //walks to the next tile
                  turn(90); //turns
                  touch(3000); //touches the wall
                  walk_distance(7.0); //walks to get off the wall
                  turn(-90); //turns
                }
                else //ther's an exit 
                {
                  break;
                }
              }
            }
            else //there's an exit on the side
            {
              turn(180); //turns to face the exit
              touch(3000); //touches the wall
              walk_distance(7.0); //walks to get off the wall
              turn(90); //turns
              check_exit(); //checks the exit
              if (room_clear == 0) //if there's no exit
              {
                walk_distance(30.0); //goes to the next tile
                media_sp = getmsharp(10); //gets the sharp value
                if (media_sp <= 230) //if there's an exit
                {
                  //as it just got an entrance, this is definitely an exit
                  turn(-90); //faces the exit
                  walk_distance(10.0); //walks to the exit
                  room_clear = 1; //end of room3
                  break; //break
                }
                else 
                {
                  turn(90); //turns
                  touch(3000); //touches the wall
                  walk_distance(7.0); //gets off the wall
                  turn(-90); //turns
                  walk_distance(30.0); //goes to the next tile
                }
              }
            }
          }
        }
      }
      l_120++; //goes to the next side, not a 120
      l = l < 3 ? ++l : l=0;
    }

    tone(BUZZER, 155, 10000);
  }
}

  /*walk_distance(3.0);
  array_read();
  if(NOSIB()==0){
    turn(30);
    walk_distance(5.0);
    turn(-30);
    array_read();
    if(NOSIB()==0){
      turn(30);
      walk_distance(-5.0);
      turn(-30);
      turn(-30);
      walk_distance(5.0);
      turn(30);
    }
  }*/


void check_exit()
{
  int cont_bl = 0;//counter for black line
  float walking_back = 0.0;
  unsigned long timer = millis();//gets a timer
  while(cont_bl < 5 && (millis() - timer <= 2000))//while the black line counter is lower than five and the robot is scanning for less than 3 seconds
  {
    walk(SWL,SWR);//walks
    array_read();//reads the array
    if(NOSIB() >= 3) cont_bl++;//if the robot is in a black line 
    else cont_bl = 0; //resets the black line counter
  } 
  
  walking_back = (float)(millis() - timer);
  if(cont_bl > 3) {     //if the robot has seen the black line 4 times
    tone(BUZZER, 440, 1000);
    room_clear = 1;
    //fazer o robo sair na linha talvez
  }
  else 
  {
    turn(180);
    walk(SWL, SWR);
    delay(walking_back);
    freeze(25);
  }
  walk(0,0);
  freeze(25);
}

void go_to_distnb (float final_d)// Function used to make the robot go to a distance
{
  int n_top = 0;
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
    delay(25);//default delay for reading values
  } while(n_top < 2);//do the above while the top counter is lower than 2
  freeze(100);//stop for 100 millisecons  
  Sinalize(1,3,1,3,20);
}

void tri_90()
{
  while(triangles < 2) { // search for triangles until you have deposited the victms
    walk_distance(2.0);
    walk_distance(7.0);//walks until it gets close to the triangle

    turn(45);//turns to stay parallel to the triangle 
    walk_distance(15.0);//walks and scan the edge for a triangle
    int isTri = 0;

    for (int i = 0; i < 10; i++) {
      isTri += analogRead(0);
    }
    isTri = (int)(isTri/10);
    // Serial.println(isTri);
    //media of 10 values of the sharp sensor

    if (isTri > 350) {//if it is a triangle
      side[l]=true;
      
      turn(-90);//turns to face the triangle
      walk_distance(10.0);//walks to touch the triangle

      if(tc != 0) //if we know the color of one triangle
      {
        if (tc == 1) //if the  other triangle is green, this is red
        {
          deposit(0);
        }
        else //if the  other triangle is red, this is green
        { 
          deposit(1);
        }   
      }
      else {
        tc = detectTri(); //detects the color of the triangle |1 = green| / |2 =  red|
        if (tc == 1) {// if the triangle is green
          LEDcontrol(0, 1, 0);//green led
          delay(1000);
          LEDcontrol(0, 0, 0);
          deposit(1);//deposit the alives victims
        } else {//if the triangle is red
          LEDcontrol(1, 0, 0);//red led
          delay(1000);
          LEDcontrol(0, 0, 0);
          deposit(0);//deposit the dead victims
        }
      }

      triangles++;// it already detected a triangle
      walk_distance(10.0); //walk
      turn(45); //turn
      walk_distance(10.0);
    } 
    
    else 
    {// if there´s not a triangle
      walk_distance(10.0);
      turn(45);//turns to continue the scan
      delay(5);
      walk_distance(10.0);
    }
    t = getmUltra(3,15); //last media
    if(l >= 3)l = 0;
    else l++;
  } 
}

void tri_120() {
 while(triangles < 2) { // search for triangles until you have deposited the victms
    if(l_120 % 2 == 1){ //parede 90
      LEDcontrol(1,1,1);
      walk_distance(2.0);
      walk_distance(7.0);//walks until it gets close to the triangle
      LEDcontrol(0,0,0);
    }
    else{ // parede 120
      LEDcontrol(0,1,0);
      walk_distance(2.0);
      walk_distance(30.0);//walks until it gets close to the triangle
      LEDcontrol(0,0,0);

    }

    turn(50);//turns to stay parallel to the triangle 
    walk_distance(12.0);//walks and scan the edge for a triangle
    int isTri = 0;

    for (int i = 0; i < 10; i++) {  
      isTri += analogRead(0);
    }
    isTri = (int)(isTri/10);
    // Serial.println(isTri);
    //media of 10 values of the sharp sensor

    if (isTri > 370 && !side[l]) {//if it is a triangle
 
      side[l]=true;
      
      turn(-90);//turns to face the triangle
      walk_distance(10.0);//walks to touch the triangle

      if(tc != 0) //if we know the color of one triangle
      {
        if (tc == 1) //if the  other triangle is green, this is red
        {
          deposit(0);
        }
        else //if the  other triangle is red, this is green
        {
          deposit(1);
        }   
      }
      else {
        tc = detectTri(); //detects the color of the triangle |1 = green| / |2 =  red|
        if (tc == 1) {// if the triangle is green
          LEDcontrol(0, 1, 0);//green led
          delay(1000);
          LEDcontrol(0, 0, 0);
          deposit(1);//deposit the alives victims
        } else {//if the triangle is red
          LEDcontrol(1, 0, 0);//red led
          delay(1000);
          LEDcontrol(0, 0, 0);
          deposit(0);//deposit the dead victims
        }
      }

      triangles++;// it already detected a triangle
      walk_distance(15.0); //walk
      turn(50); //turn
      freeze(25);
      walk_distance(11.0);
    } 
    
    else 
    {// if there´s not a triangle
      walk_distance(11.0);
      turn(45);//turns to continue the scan
      freeze(25);
      if(l_120%2==0)walk_distance(17.0);
      else walk_distance(16.0);
    }
    t = getmUltra(3,15); //last media
    if(l >= 3)l = 0;
    else l++;
    l_120++;
    freeze(10);
    media_sp=getmsharp(10);
    if(media_sp>=230 && triangles < 2){
      turn(90);
      touch(3000);
      walk_distance(9.0);
      turn(-90);
    } 
  }
}

void walk_distancewb(float d)
{
	int vitima = 0;
  int sign = abs(d) / d; //if sign is negative makes the robot walk back
  unsigned long start_wdb = millis();
	unsigned long tempo_i = 0;
	while ( millis() - start_wdb < 125*abs(d) + tempo_i )
	{
		walk(SWR*sign,SWL*sign);
		get_vbt();
		if(vb <= 5.0 && vb != 0.0)vitima++;
		else vitima = 0;
		if (vitima > 1)
		{	
      delay(50);
			unsigned long d_interno = millis();
      freeze(50);   
			SwallowBalls();
			d_interno = millis() - d_interno;
			tempo_i += d_interno;
			vitima = 0;
		}
	}
	
 	freeze(100);
}

void front_scan()
{
  walk_distancewb(40.0);
  freeze(100);
  float auxt = 0.0;
  for(int i = 0; i < 15; i++)
  {
    auxt = getUltra(3);
    if (auxt != 0.0 && max_top != -1.0){if (auxt > max_top)max_top = auxt;}  
    else max_top = -1.0;
    delay(10);
  }
  max_top += 40.0;
  walk_distance(-40.0);
}

void front_scan_2()
{
  walk_distancewb(30.0);
  freeze(100);
  float auxlat = 0.0;
  turn(-3*ent_s);
  for(int i = 0; i < 15; i++)
  {
    auxlat = getUltra(3);
    if (auxlat != 0.0 && maxlat != -1.0){if (auxlat > maxlat)maxlat = auxlat;}  
    else maxlat = -1.0;
    delay(10);
  }
  maxlat += 30.0;
  turn(3*ent_s);
  Serial.println(maxlat);
  delay(3000);
  walk_distance(-30.0);
}