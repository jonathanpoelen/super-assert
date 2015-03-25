#include "sassert.hpp"

int main()
{
  int  a = 6;
  int  b = 1;
  int  c = 0;
  int  d = 9;
  sassert_message(a * b && c < d, "unsatisfactory because of operator precedence");
  struct A{ bool operator == (A const &){return true;}} aa;
  sassert_message(aa == aa, "no printable");
  sassert(""=="");
  
  sassert(a * b + c + 'a' < d );
}
