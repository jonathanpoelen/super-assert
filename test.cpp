#include "sassert.hpp"

int main()
{
  int  a = 6;
  int  b = 1;
  int  c = 0;
  int  d = 9;
  sassert("message",
          a * b + c + 'a' < d );
  sassert("unsatisfactory because of operator precedence",
          a * b && c < d );
  struct A{ operator bool (){return true;}} aa;
  sassert("no printable", aa);
  sassert("s==s", ""=="");
}
