void Room3() {
  Ro  = true; //protocol bool
  
  int ex=0;
  int ball_cont=0;
  int cont_media=0;
  int b=1;

  while(Ro){
    if(ctri==1 || ctri==0 || ctri==-1)prev_ctri=ctri;
    //debug
    // Serial.println(val_top);
    // Serial.println(val_bot);
    // Serial.println();
    saida=0;
    config=1;
    if (edge_state==1 && !config && b>0) {
      if (saida) {
        map_corner[0] = 0;
        map_corner[1] = 1;
        map_corner[2] = 1;
        config=true;
        edge_confirm=-1;
        
        
        turn(45);
        float d = getUltra(3);
        while(d > 7.0){walk(400,400);d=getUltra(3);}
        walk(0, 0);
        turn(-90);
        array_read();
        while (NOSIB() < 2){walk(600, 680);array_read();}
        while(1)walk(0, 0);
        b=0;
      } else {
        triangle++;
        walk(0, 0);
        //while (digitalRead(23)){walk(0, 0);digitalWrite(LEDR, LOW);digitalWrite(LEDG, LOW);}
        if(triangle>=1)triangle=1;
        map_corner[0] = 1;
      }
    }
    else if (edge_state==2 && !config && b>0) {
      if (saida) {
        map_corner[0] = 1;
        map_corner[1] = 0;
        map_corner[2] = 1;
        config = true;
        edge_confirm=-1;


        turn(45);
        float d = getUltra(3);
        while(d > 7.0){walk(400,400);d=getUltra(3);}
        walk(0, 0);
        turn(-90);
        array_read();
        while (NOSIB() < 2){walk(600, 680);array_read();}
        while(1)walk(0, 0);
        b=0;
      } else {
        triangle++;
        if(triangle>=2)triangle=2;
        //while (digitalRead(23)){walk(0, 0);digitalWrite(LEDR, LOW);digitalWrite(LEDG, LOW);}
        if (triangle == 2 && b>0) {
          map_corner[0] = 1;
          map_corner[1] = 1;
          map_corner[2] = 0;
          config = true;
          edge_confirm=-1;
          
          turn(135);
          float d = getUltra(3);
          while(d > 7.0){walk(400,400);d=getUltra(3);}
          walk(0, 0);
          turn(-90);
          array_read();
          while (NOSIB() < 2){walk(600, 680);array_read();}
          while(1)walk(0, 0);
          b=0;
        }
        map_corner[1] = 1;
      }
      
    }
    //debug for when it's the third wall and it didn't detect a triangle. The side with the lowest media have the triangle
    else if(edge_state==3 && !config && b>0){
      edge_state=100;

      if (saida) {
        map_corner[0] = 1;
        map_corner[1] = 1;
        map_corner[2] = 0;

        turn(45);
        float d = getUltra(3);
        while(d > 7.0){walk(400,400);d=getUltra(3);}
        walk(0, 0);
        turn(-90);
        d = getUltra(3);
        if (d != 0) {
          while(d > 7.0){walk(400,400);d=getUltra(3);}
          turn(90);
        } else {
          array_read();
          while (NOSIB() < 2){walk(600, 680);array_read();}
          while(1)walk(0, 0);
        }
      }
      else {
        walk(0, 0);
        map_corner[2] = 1;
      }

      config = true;
      edge_confirm=-1;
      walk(0, 0);
      b=0;
    }
      prev_detec_top_2=prev_detec_top;
      prev_detec_top=val_top;

    //gets the media of top and bottom ultra. If value is too high or too low sets as -1
    dists(1);

    if (cont_edge>=3){edge_state++;saida=true;}
    
    if(ex>=20){
      ex=0;
      if(mapper%2 == 1)map_room[mapper]=1;
      else map_room[mapper]=-2;
      mapper++;
      LEDcontrol(1, 0, 0);
      delay(2000);
      LEDcontrol(0, 0, 0);
    }

    if(mapper==8){
      while(1);
    }    
    ex++;
    //only if value_top gets a fair distance value
    if(val_top!=-1 && val_bot!=-1 && val_bot-val_top<15){

      if(avs == true)Deposit_alive();

      cont++;
      cont_tri++;
      cont_canto++;
      cont_cont++;
      if (cont_cont==2){cont_cont=0;cont_edge=0;}

      //gets the media and declares the values as previous to the next
      digitalWrite(24, LOW);
      if(prev_detec_top_2==0 && prev_detec_top!=0)prev_detec_top_2=prev_detec_top;
      if(prev_detec_top==0)prev_detec_top=val_top;
      if(prev_detec_bot==0)prev_detec_bot=val_bot;

      if(ex>=5){
        if(cont_media>4)cont_media=0;
        media_movel_top[cont_media]=val_top;
        cont_media++;
        val_media_movel_top=0;
        for(int i=0;i<5;i++){
          val_media_movel_top+=media_movel_top[i];
        }
        val_media_movel_top/=5;
      }
      else media_movel_top[ex]=val_top;

      if(edge_confirm>=0)edge_confirm++;

      edge_confirm=-1;

      //if the previous value of top sensor is greather than the one before and the actual one and it in't on the third wall
      if(prev_detec_top>prev_detec_top_2 && prev_detec_top>val_top && prev_detec_top!=0 && prev_detec_top_2!=0 && cont>9 && prev_detec_top>val_media_movel_top && edge_state<3 && !config && 1==0){
        //goes back a little
        walk(1000, -1000);
        delay(35);
        walk(0, 0);

        //reset some values
        cont=2; //it has gone back twice
        prev_detec_top=0;
        prev_detec_bot=0;

        //gets the previous value media
        for(int i=0;i<100;i++){
          float med=0;
          med = getUltra(3);
          if(med==0 || med>100){i=101;prev_detec_top=-1;i--;}
          else {
            prev_detec_top+=med;
          }
        }
        if(prev_detec_top!=-1)prev_detec_top/=100;

        //goes back again
        walk(1000, -1000);
        delay(250);
        walk(0, 0);

        //reset some values
        prev_detec_top_2=0;

        //gets the previous of the previous value media
        for(int i=0;i<100;i++){
          float med=0;
          med = getUltra(3);
          if(med==0 || med>100){i=101;prev_detec_top_2=-1;i--;}
          else {
            prev_detec_top_2+=med;
          }
        }
        if(prev_detec_top_2!=-1)prev_detec_top_2/=100;

        //returns and turn again
        walk(-1000, 1000);
        delay(250);
        walk(0, 0);
        delay(20);
        walk(-1000, 1000);
        delay(300);
        walk(0, 0);

        //reset some values
        val_top=0;

        //gets the actual value media
        for(int i=0;i<100;i++){
          float med=0;
          med = getUltra(3);
          if(med==0 || med>100){i=101;val_top=-1;i--;}
          else {
            val_top+=med;
          }
        }
        if(val_top!=-1)val_top/=100;

        //if the previous value is still greater than the one before and the recent one it detects a victim
        if(val_top!=-1 && prev_detec_top!=-1 && prev_detec_top_2!=-1 && prev_detec_top+2.5>val_top && prev_detec_top+2.5>prev_detec_top_2){
          
          //resets the turned times and sets as NOT a corner
          cont=-4;
          edge_confirm=-1;
          edge_state++;

          //it will detect triangle while search for victims
          walk(1000, -1000);
          delay(350);
          walk(0, 0);
          digitalWrite(24, HIGH);
          digitalWrite(22, HIGH);
          delay(1000);
          digitalWrite(24, LOW);
          digitalWrite(22, LOW);
          delay(1000);

          //media of difference between top and bottom
          float diff_media=0;
          diff_media+=(val_top-val_bot);
          
          for(int i=0;i<5;i++){ //get triangle media
            bool edge=false;

            //refresh ultra values
            val_bot=getUltra(4);
            prev_detec_bot=val_bot;
            dists(1);
            walk(-600, 600);
            delay(50);
            walk(0, 0);
            diff=val_top-val_bot;

            //math to analyze the difference in degrees.
            float variacao_bot=(prev_detec_bot-val_bot)/0.025; //~2*PI/360 "mathematic great approximation"
            bool direct=0;
            if(val_bot>=52)edge=true; //detects edge. 45(~room diagonal) + sqrt(2)*30/2(triangle to corner maximum distance)
            if(variacao_bot<0){variacao_bot*=-1;if(prev_diff<7)variacao_bot=variacao_bot+(prev_diff*1.3);direct=1;}
            else if(diff<7)variacao_bot=variacao_bot+(diff*1.3); 

            //at this condition it detects a victim
            if((variacao_bot>150 && prev_detec_bot!=0 && val_bot!=0 && !edge) || val_bot<=15){
              int diff2=prev_detec_bot-val_bot;
              if(diff2<0)diff2*=-1;

              // //debug
              // for(int i=0;i<diff2;i++){
              //   digitalWrite(22, HIGH);
              //   delay(500);
              //   digitalWrite(22, LOW);
              //   delay(500);
              // }

              // //buzzer
              // digitalWrite(8, HIGH);
              // delay(500);
              // digitalWrite(8, LOW);
              
              if(direct){
                while(digitalRead(23)){
                  digitalWrite(24, HIGH);
                }
                GotoBalls();
                BackMiddle(dels1, turns, dels2);
                dists(0);
              }
              else {
                while(digitalRead(23)){
                  digitalWrite(24, HIGH);
                }
                GotoBalls();
                if(color_detected==true && a==0){
                  map_corner[(edge_state-1)] = detectTri();
                  a = map_corner[(edge_state-1)];
                }
                BackMiddle(dels1, turns, dels2);
                dists(0);
              }
            }
            if(val_top==-1){
              i=6;
              cont_edge++;
            }
            diff_media+=(val_top-val_bot);
          }

          //debug for triangle search
          digitalWrite(24, HIGH);
          delay(1000);
          digitalWrite(24, LOW);
          delay(1000);
          
          // //turns
          walk(800, -800);
          delay(110);
          walk(0, 0);

          // //get triangle media
          for(int i=0;i<5;i++){
            bool edge=false;

          //   //refresh ultra values
            val_bot=getUltra(4);
            prev_detec_bot=val_bot;
            dists(-1);
            walk(600, -600);
            delay(50);
            walk(0, 0);
            diff=val_top-val_bot;

          //   //math to analyze the difference in degrees.
            float variacao_bot=(prev_detec_bot-val_bot)/0.025;
            bool direct=0;
            if(val_bot>=52)edge=true;
            if(variacao_bot<0){variacao_bot*=-1;if(prev_diff<7)variacao_bot=variacao_bot+(prev_diff*1.3);direct=1;}
            else if(diff<7)variacao_bot=variacao_bot+(diff*1.3);

          //   //at this condition it detects a victim
            if((variacao_bot>150 && prev_detec_bot!=0 && val_bot!=0 && !edge && i>1) || val_bot<=15){
              int diff2=prev_detec_bot-val_bot;
              if(diff2<0)diff2*=-1;

          //     //debug
              // for(int i=0;i<diff2;i++){
              //   digitalWrite(22, HIGH);
              //   delay(500);
              //   digitalWrite(22, LOW);
              //   delay(500);
              // }

              // digitalWrite(8, HIGH);
              // delay(500);
              // digitalWrite(8, LOW);
              
              if(direct){
                while(digitalRead(23)){
                  digitalWrite(24, HIGH);
                }
                GotoBalls();
                BackMiddle(dels1, turns, dels2);
                dists(0);
              }
              else {
                while(digitalRead(23)){
                  digitalWrite(24, HIGH);
                }
                GotoBalls();
                if(color_detected==true && a==0){
                  map_corner[(edge_state-1)] = detectTri();
                  a = map_corner[(edge_state-1)];
                }
                BackMiddle(dels1, turns, dels2);
                dists(0);
              }
            }
            if(val_top==-1){
              i=6;
              cont_edge++;
            }
            diff_media+=(val_top-val_bot);
          }

          digitalWrite(24, HIGH);
          delay(1000);
          digitalWrite(24, LOW);
          delay(1000);

          // //turns
          walk(-1000, 1000);
          delay(50);
          walk(0, 0);
          flag=millis();
          float med=100000;
          while(med>=6 && color_detected==false){
            med=getUltra(4);
            walk(450, 420);
          }
          walk(450, 450);
          delay(350);
          flag=(millis()-flag);
          walk(0, 0);
          int auxc = detectTri();
          if(a==0 && auxc!=0 && map_corner[edge_state-1]!=-1){
            map_corner[(edge_state-1)] = auxc;
            a = map_corner[(edge_state-1)];
          }
          else map_corner[edge_state-1]=1;
          if(auxc==1){
            digitalWrite(22, HIGH);
            if (victims > 0) {
              turn(180);
              while(digitalRead(TOUCH1) && digitalRead(TOUCH2)){walk(-300, -300);}
              walk(0, 0);
              for(int i=80;i<150;i++) {
                s3.write(i);
                delay(2);
              }
              delay(100);
              for (int i=150;i>80;i--){s3.write(i);}
              victims=0;
              walk(300, 300);
              delay(300);
              turn(180);
            }
            delay(100);
            digitalWrite(22, LOW);
          }
          else{
            if (victims > 0) {
              digitalWrite(24, HIGH);
              turn(180);
              while(digitalRead(TOUCH1) && digitalRead(TOUCH2)){walk(-400, -400);}
              walk(0, 0);
              for(int i=80;i<0;i++) {
                s3.write(i);
                delay(2);
              }
              delay(100);
              for (int i=0;i<80;i++){s3.write(i);}
              victims=0;
              walk(300, 300);
              delay(300);
              turn(180);
            }

            digitalWrite(24, LOW);
          }
          walk(-450, -420);
          delay(flag);
          walk(0, 0);

          walk(-1000, 1000);
          delay(50);
          walk(0, 0);

          //refresh or reset some values
          diff_media/=11;
          prev_detec_top=0;
          prev_detec_top_2=0;
          media_cantos[(edge_state-1)]=diff_media;
          
        }
        else{
          //turns and reset some values
          walk(1000, -1000);
          delay(400);
          walk(0, 0);
          prev_detec_top=0;
          prev_detec_top_2=0;
        }
      }
      else{
        //previous of previous value and declare actual value as previous one. Declares difference between top and bottom
        diff=val_top-val_bot;
        float prev_variacao_top=variacao_top;

        //math. The direct value is set as true when the actual bottom value is greater than the previous one
        float variacao_bot=(prev_detec_bot-val_bot)/0.025; //~2*PI/360
        if(prev_detec_top!=0 && val_top!=0)variacao_top=(val_top-prev_detec_top)/0.025;
        if(variacao_top<0)variacao_top*=-1;
        if(diff>2.0 && variacao_top<180 && cont>1 && variacao_bot<130){
          if(diff<5.0)sum_diff+=diff;
          else sum_diff+=5.0;
        }
        else if(diff<0)sum_diff=0;
        else if(sum_diff>3)sum_diff-=3;
        else sum_diff=0;
        bool direct=0;
        if(ctri < -1) ctri = prev_ctri;

        if(variacao_bot<0){variacao_bot*=-1;if(prev_diff>=1.5)variacao_bot=variacao_bot+(100/prev_diff);direct=1;}
        else if(diff>=1.5){variacao_bot=variacao_bot+(100/diff);if(val_top>40)variacao_bot-=val_top*1.3;}
        Serial.println(val_bot);
        Serial.println(val_top);
        Serial.println(variacao_top);
        Serial.println(diff);
        Serial.println();
        // Serial.println(cont_tri);
        // Serial.println(mapper);
        // Serial.println(ctri);
        Serial.println(avs);
        Serial.println(" ");
        if(variacao_top<180 && variacao_top>=27 && st_tri==0 && cont>3 && diff>2 && cont_tri>15){
          st_tri=1;
          flag_ex=ex;

        }
        else if(ex-flag_ex<10 && st_tri==1 && cont>3 && cont_tri>15){
          if(variacao_top<150 && variacao_top>=27 && ex-flag_ex>1){st_confirm=1;}
        }
        else st_tri=0;

        if((cont>10 && variacao_top>=120 && variacao_bot>120 && (diff<-3 || prev_diff<-3)) || (variacao_top > ((val_top*185)/39) && cont>1 && cont_canto>18)){
          cont=-3;
          cont_canto=0;
          digitalWrite(22, HIGH);   
          delay(2000);
          digitalWrite(22, LOW);
          prev_detec_top=0;
          prev_detec_top_2=0;
          prev_detec_bot=0;   
          st_tri=0;
          if(mapper%2==0 && map_room[mapper]==-1)map_room[mapper]=-2;
        }
        else if((sum_diff>(34*19)/val_top || st_confirm==1) && cont>3 && diff<10 && cont_tri>20 && (diff>2.5 || prev_diff>2.5) && val_top>27 && (map_room[mapper]==-1  || map_room[mapper]==-2)){
          st_confirm=0;
          sum_diff=0;
          cont=0;
          st_tri=0;
          digitalWrite(22, HIGH);
          digitalWrite(24, HIGH);
          delay(3000);
          digitalWrite(22, LOW);
          digitalWrite(24, LOW);

          if(ctri==0){

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
            int auxc = detectTri();
            dists(0);
            if(auxc==1 && val_top<27){
              digitalWrite(22, HIGH);
              delay(1000);
              digitalWrite(22, LOW);
              cont_tri=0;
              ctri=1;
              if(mapper%2 == 1){
                map_room[mapper]=1;
                mapper++;
                map_room[mapper]=3;
                mapper++;
                ex=-10;
              }
              else{
                map_room[mapper]=2;
                mapper++;
                ex=-10;
              }
            }
            else if(auxc==2 && val_top<27){
              digitalWrite(24, HIGH);
              delay(1000);
              digitalWrite(24, LOW);
              if(avs){
                int p=0;
                for(int k = 0; k < 180; k++)
                {
                  s1.write(p);
                  s2.write(k);
                  delay(1);
                  if(p<45)p++;
                }
              }
              cont_tri=0;
              ctri=-1;
              if(mapper%2 == 1){
                map_room[mapper]=1;
                mapper++;
                map_room[mapper]=4;
                mapper++;
                ex=-10;
              }
              else{
                map_room[mapper]=4;
                mapper++;
                ex=-10;
              }
            }
            else{
              GotoBalls();
              BackMiddle(dels1, turns, dels2);
              turn(-8);
            }
            walk(-450, -450);
            delay(flag);
            walk(0, 0);
            turn(-3);
        }
        else if(ctri==1){
          cont_tri=0;
          map_room[mapper]=4;
          mapper++;
          ex=-10;
          digitalWrite(24, HIGH);
          delay(1000);
          digitalWrite(24, LOW);
        }
        else if(ctri==-1){
          cont_tri=0;
          map_room[mapper]=3;
          mapper++;
          ex=-10;
          digitalWrite(22, HIGH);
          delay(1000);
          digitalWrite(22, LOW);
        }
      }

        //if it doesn't meet the conditions just sets the previous values
        else if(((variacao_bot>val_bot*4.4 || variacao_bot>prev_detec_bot*4.4) && variacao_top<200 && prev_variacao_top<200 && prev_detec_bot!=0 && val_bot!=0 && cont>2 && edge_confirm==-1 && ((diff>-1.0 && prev_diff>=10) || (diff>=1.5 && prev_diff>-0.5 && prev_diff<diff))) || edge_confirm==5){
          //if it didn't clean this side
          dball=0.0;
          if(val_bot<prev_detec_bot)dball=val_bot;
          else dball=prev_detec_bot;
          cont=0;
          if(diff>prev_diff)sum_diff-=diff;
          else sum_diff-=prev_diff;
          if(edge_confirm==-1){
            //turns
            walk(-1000, 1000);
            delay(80);
            walk(0, 0);
            edge_confirm=-1;

            //buzzer
            digitalWrite(8, HIGH);
            delay(500);
            digitalWrite(8, LOW);
            
            if(direct){
              dists(0);
              float maxb=-1.0;
              int aux=0;
              while(maxb>=dball+3 && aux<4){
                for(int i=0;i<15;i++){
                  float med=getUltra(4);
                  if(maxb<med && med!=0)maxb=med;
                }
                aux++;
                turn(3);
              }
              if(aux==4){
                for(int i=0;i<2;i++){
                  turn(-3);
                }
              }
              walk(0, 0);
              while(digitalRead(23)){
                digitalWrite(LEDG, LOW);
                digitalWrite(LEDR, LOW);
                walk(0, 0);
              }
                digitalWrite(24, HIGH);
                GotoBalls();
                BackMiddle(dels1, turns, dels2);
                turn(-8);
            }
            else {
              dists(0);
              float maxb=-1.0;
              int aux=0;
              while(val_bot>=dball+3 && aux<4){
                for(int i=0;i<15;i++){
                  float med=getUltra(4);
                  if(maxb<med && med!=0)maxb=med;
                }
                turn(-1);
                aux++;
              }
              if(aux==4){
                for(int i=0;i<2;i++){
                  turn(1);
                }
              }
              walk(0, 0);
              while(digitalRead(23))walk(0, 0);
                digitalWrite(24, HIGH);
                GotoBalls();
                BackMiddle(dels1, turns, dels2);
                turn(-8);
            }
            prev_detec_bot=0;
            prev_diff=0;
          }
          else edge_confirm=0; //reset
        }
        else prev_detec_bot=val_bot;
        prev_diff=diff;
      }
    }
    else {
      cont_edge++;
      //detects as triangle if top value detects too high or too low value and bottom value is fair
      digitalWrite(24, HIGH);
      cont=0;
      if(val_bot!=-1){
        digitalWrite(22, HIGH);
      }
      else digitalWrite(22, LOW);

      //returns the previous top values to 0 to reset
      prev_detec_top=0;
      prev_detec_top_2=0;
      if(!exit_state){
        if(ex<5){
          map_room[mapper-1]=2;
        }
        else{
          map_room[mapper]=2;
          mapper++;
          exit_state=true;
          ex=0;
        }
      }
    }
  }
  delay(20);
}

// ALL PREVIEWS TESTS OF ROOM 3

/*int aux=val_top/10;
      if(aux>0){
        for(int i=0;i<aux;i++){
        digitalWrite(24, HIGH);
        delay(500);
        digitalWrite(24, LOW);
        delay(500);
      }
        digitalWrite(8, HIGH);
        delay(1000);
        digitalWrite(8, LOW);
        aux=val_top-(aux*10);
        for(int i=0;i<aux;i++){
        digitalWrite(24, HIGH);
        delay(500);
        digitalWrite(24, LOW);
        delay(500);
        }
      }
      else{
        digitalWrite(8, HIGH);
        delay(1000);
        digitalWrite(8, LOW);
        aux=val_top;
        for(int i=0;i<aux;i++){
        digitalWrite(24, HIGH);
        delay(500);
        digitalWrite(24, LOW);
        delay(500);
        }
      }
      delay(1000);
      
      aux=val_bot/10;
      if(aux>0){
        for(int i=0;i<aux;i++){
        digitalWrite(22, HIGH);
        delay(500);
        digitalWrite(22, LOW);
        delay(500);
      }
        digitalWrite(8, HIGH);
        delay(1000);
        digitalWrite(8, LOW);
        aux=val_bot-(aux*10);
        for(int i=0;i<aux;i++){
        digitalWrite(22, HIGH);
        delay(500);
        digitalWrite(22, LOW);
        delay(500);
        }
      }
      else{
        digitalWrite(8, HIGH);
        delay(1000);
        digitalWrite(8, LOW);
        aux=val_bot;
        for(int i=0;i<aux;i++){
        digitalWrite(22, HIGH);
        delay(500);
        digitalWrite(22, LOW);
        delay(500);
        }
      }*/
      /*if(val_bot<val_top-2){
        digitalWrite(22, HIGH);
        delay(500);
        digitalWrite(22, LOW);
        delay(500);
        int aux=3;
        while(val_bot<val_top-aux){
          digitalWrite(22, HIGH);
          delay(500);
          digitalWrite(22, LOW);
          delay(500);
          aux++;
        }
      }
      else digitalWrite(22, LOW);*/



/*dists(0);
            int back_array[100];
            int back_cont=0;
            while(val_bot>8 && direct){
              walk(-800, -800);
              val_bot=getUltra(4);
              /*if(diff<2 || prev_detec_bot<val_bot){
                int counter=0;
                while((diff<=2 || prev_detec_bot<val_bot) && counter<=4){
                  walk(800, -800);
                  delay(50);
                  back_array[back_cont]=-1;back_cont++;
                  counter++;
                  prev_detec_bot=val_bot;
                  dists(0);
                  diff=val_top-val_bot;
                }
                if(counter==5)direct=false;
              }
            }
            dists(0);
            diff=val_top-val_bot;
            if(direct && diff>3){
              while(digitalRead(23)){
                digitalWrite(24, HIGH);
              }
              for(int i=back_cont-1;i>=0;i--){
                if(back_array[i]==1){walk(800, 800);delay(50);walk(0, 0);delay(100);}
                else if(back_array[i]==-1){walk(-800, 800);delay(50);walk(0, 0);delay(100);}
              }
              walk(800, -800);
              delay(150);
              walk(0, 0);
            }
            else{
              for(int i=back_cont-1;i>=0;i--){
                if(back_array[i]==1){walk(800, 800);delay(50);walk(0, 0);delay(100);}
                else {walk(-800, 800);delay(75);walk(0, 0);delay(100);}
              }
              walk(800, -800);
              delay(150);
              walk(0, 0);
            }*/



            /*dists(0);
            int back_array[100];
            int back_cont=0;
            while(val_bot>8 && !direct){
              walk(-800, -800);
              val_bot=getUltra(4);
              
              /*if(diff<2 || prev_detec_bot<val_bot){
                int counter=0;
                while((diff<2 || prev_detec_bot<val_bot) && counter<=4){
                  walk(-800, 800);
                  delay(50);
                  back_array[back_cont]=-1;back_cont++;
                  counter++;
                  prev_detec_bot=val_bot;
                  dists(0);
                  diff=val_top-val_bot;
                }
                if(counter==5)direct=true;
              }
            }
            dists(0);
            diff=val_top-val_bot;
            if(!direct && diff>3){
              while(digitalRead(23)){
                digitalWrite(24, HIGH);
              }
              for(int i=back_cont-1;i>=0;i--){
                if(back_array[i]==1){walk(800, 800);delay(50);walk(0, 0);delay(100);}
                else {walk(800, -800);delay(50);walk(0, 0);delay(100);}
              }
              walk(800, -800);
              delay(150);
              walk(0, 0);
            }
            else{
              for(int i=back_cont-1;i>=0;i--){
                if(back_array[i]==1){walk(-800, -800);delay(50);walk(0, 0);delay(100);}
                else {walk(800, -800);delay(50);walk(0, 0);delay(100);}
              }
              walk(800, -800);
              delay(150);
              walk(0, 0);
            }
*/
