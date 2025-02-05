int NOSIW()
{
  int n = 0; // Number of reflection sensors in the black using the int varianble n that a counter that starts by 0 
  
  if(els < WHITE) n++;
  if(ls < WHITE) n++;
  if(rs < WHITE) n++;
  if(ers < WHITE) n++;
  
  return n;
}
