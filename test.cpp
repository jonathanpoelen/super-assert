#define SASSERT_NO_BOOST_SUPPORT
#include "sassert/sassert.hpp"

int main()
{
  int  a = 6;
  int  b = 1;
  int  c = 0;
  int  d = 9;

  std::cerr.exceptions(std::ios::failbit);
  try { std::cerr.clear(std::ios::failbit); } catch (...) {}

  sassert_message(a * b && c < d,
    "unsatisfactory because of operator precedence ;"
    " (c < d) -> " << '(' << c << " < " << d << ')'
  );

  struct A{ bool operator == (A const &){return true;}} aa;
  sassert_message(aa == aa, "no printable");

  sassert(3==3);

  sassert(a * b + c + 'a' < d );
}
