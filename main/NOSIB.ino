int NOSIB()
{
  int n = 0; // Number of reflection sensors in the black using the int varianble n as a counter that starts at 0 
  
  if(els > BLACK) n++;
  if(ls > BLACK) n++;
  if(rs > BLACK) n++;
  if(ers > BLACK) n++;
  
  return n;
}
