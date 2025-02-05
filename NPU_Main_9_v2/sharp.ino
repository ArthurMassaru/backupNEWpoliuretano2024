int getmsharp(int j){
  int auxs=0;
  for (int i = 0; i < j; i++) 
  {
    auxs += analogRead(A0);
    delay(5);//delay for information
  }
  auxs = (int)(auxs/j);//gets the media of the right sensor that SHOULD be facing a wall
  return auxs;
}
