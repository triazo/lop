int rpiLop_add(int a, int b) {
    while (b!=0) {
        int carry = (a & b);
        a = a ^ b;
        b = carry << 1;
    }
    return a;
}
int rpiLop_subtract(int a, int b)
{
  b = -b;
  while (b!=0)
    {
      int carry = (a & b);
      a = a ^ b;
      b = carry << 1;
    }
  return a;

}
int rpiLop_multiply(int a, int b)
{
  int total = 0;
  for(;b;total+=rpiLop_add(total,a) ,b--);
  return total;
}
