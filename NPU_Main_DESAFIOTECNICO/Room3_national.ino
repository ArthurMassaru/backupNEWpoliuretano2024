void scan_edge_2()
{
    if (ent_m == 0) //entrance on the sides
    {
        float p=6.0;
        float nDiff=0.0;
        for(int i=0;i<6;i++){
            turn(3);
            update_angle(3);
            media_vt = getmnUltra(3, 5);
            media_vb = getmnUltra(4, 5);
            nDiff=media_vt-media_vb; //delta between the medias of bot and top
            if(media_vb>=0 && media_vt>=0 && nDiff>=-30){  // if is an edge
                Serial.println(nDiff);
                if(media_vb>=10.0)mTri[position]+=nDiff; //if it is a triangle
                else p--; // if it is a ball subtract one from the denominator of the medias
            }
            else{ //exit case
                //debug
                Serial.println("exit");
                
                mTri[position]=-1;//-1 is an edge
                i=7;//breaks out the scan loop
                if(position ==  0) exit_angle = 45;
                else if(position == 1) exit_angle = 135;
                else if(position == 2) 
                {
                    if(ent_s == -1)exit_angle = 225;
                    else exit_angle == 315;
                }
            }
            delay(75);
            nDiff=0.0;

            //debug
            Serial.println("mTril");
            Serial.println(mTri[position]);
        }
        if(mTri[position]!=-1){
            mTri[position]/=p;//get the media
        }

        //debug
        Serial.println("sim");
        Serial.println(mTri[position]);

        position++;//goes to the next edge

        //the lowest value will be the edge and -1 for exit
        if(position>=2){
        }

    } 
    else {
        Sinalize(1,1,1,1,1);
        walk(0,0);

    }

}

void turn_distance(int alfa) {
    if(alfa > 0) {
        left(600);
        delay((int)(alfa*17));
        freeze(25);
    } else {
        right(600);
        delay((int)(alfa*17));
        freeze(25);
    }
}

void national_room() {
    int tri_1, tri_2;

    delay(100);
    c_angle = 90;
    Serial.println(c_angle);
    scan_entrance();
    go_to_mid();

    turn_distance(30);

    scan_edge_2();
    while(digitalRead(23))walk(0, 0);
    

    turn_distance(90);
    scan_edge_2();
    while(digitalRead(23))walk(0, 0);
    walk(0,0);

    scan_edge_2();
    while(digitalRead(23))walk(0, 0);

    float edge = lower(mTri[0], mTri[1], mTri[2]);
    if (edge == mTri[0])
    {
        tri_1 = 135;
        if(ent_s == -1) tri_2 = 315;
        else tri_2 = 225;
        LEDcontrol(1,0,0);
    }
    else if (edge == mTri[1])
    {
        tri_1 = 45;
        if(ent_s == -1)tri_2 = 315;
        else tri_2 = 225;
        LEDcontrol(0,1,0);

    }
    else {
        tri_1 = 45;
        tri_2 = 135;
        LEDcontrol(0,0,1);

    }

}

float getmnUltra(int u, int den)//function used to get the media of an us sensor
{
   float m_ultra = 0.0;
   for(int i = 0; i < den; i++)
   {
      float aux = getUltra(u);
      if(aux != 0.0) m_ultra += aux;
      else m_ultra = -1;
      delay(3);
   }
   return m_ultra/den; 
}
