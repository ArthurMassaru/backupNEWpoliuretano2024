void set_servo (int n, int f_pos)
{
  // get which servo it got to move
  if(n == 1)
  {
    for(int i = s1.read(); i < f_pos; i++)
    {
      s1.write(i);
      delay(3);
    }
    for(int i = s1.read(); i > f_pos; i--)
    {
      s1.write(i);
      delay(3);
    }
    s1.write(f_pos);
  }
  else if(n == 2)
  {
    for(int i = s2.read(); i < f_pos; i++)
    {
      s2.write(i);
      delay(3);
    }
    for(int i = s2.read(); i > f_pos; i--)
    {
      s2.write(i);
      delay(3);
    }
    s2.write(f_pos);
  }
  else
  {
    for(int i = s3.read(); i < f_pos; i++)
    {
      s3.write(i);
      delay(3);
    }
    for(int i = s3.read(); i > f_pos; i--)
    {
      s3.write(i);
      delay(3);
    }
    s3.write(f_pos);
  }
}

void Servo_ipos()
{
  set_servo(1,10);
  set_servo(2,180);
  set_servo(3,80);
}
