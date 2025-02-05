void go_to_mid_120()
{
  if(ent_m == 0)// left/right entrance
  {
    turn(ent_s*90);//turn 90 deegres right / left
    update_angle(ent_s*90); //current angle after the turn
    touch(2000);//touch function

    media_vt = getmUltra(3,15);
    if(media_vt >= 120.0)
    {
      go_to_angle(90);  //make the robot go to inital and updtades its angle
      go_to_dist(R_MID_90);
      turn(ent_s*90);//turns left/right
      update_angle(ent_s*90);
      touch(2000);//touch function
      go_to_dist(R_MID_120);
    }
    else
    {
      go_to_dist(R_MID_120);
      turn(ent_s*-90);
      media_vt = getmUltra(3,15);
      if(media_vt >= 120.0)
      {
        turn(ent_s*90);
        walk_distance(-(R_MID_120 - 1.0));
        go_to_angle(90);
        go_to_dist(R_MID_90);
        turn(ent_s*90);
        go_to_dist(R_MID_120);
      }
      else
      {
        touch(2000);
        go_to_dist(R_MID_90);
      }
    }
  }
}

void New_Room()
{
  scan_entrance();
  if(rooms == 120)go_to_mid_120();
  else LEDcontrol(1,1,1);
  while(1);
  
}
