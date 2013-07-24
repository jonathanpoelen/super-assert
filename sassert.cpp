#include <iostream>
#include <type_traits>
#include <falcon/type_traits/eval_if.hpp>
#include <falcon/type_traits/is_character.hpp>
#include <falcon/type_traits/is_string.hpp>
#include <cstdlib>

#ifndef NO_BOOST
# include <boost/current_function.hpp>
#endif

namespace Super_Assert {

#ifdef NDEBUG

#define sassert(msg, expr)

#else

#if defined(SASSERT_COLOR) && !defined(SASSERT_NO_COLOR)
//TODO SASSERT_DYNAMIC_COLOR (read file configuration)
#  ifndef SASSERT_COLOR_CHARACTER
#  define SASSERT_COLOR_CHARACTER "\033[00;33m"
#  endif
#  ifndef SASSERT_COLOR_FLOATING
#    define SASSERT_COLOR_FLOATING "\033[00;32m"
#  endif
#  ifndef SASSERT_COLOR_INTEGRAL
#    define SASSERT_COLOR_INTEGRAL "\033[00;34m"
#  endif
#  ifndef SASSERT_COLOR_BOOLEAN
#    define SASSERT_COLOR_BOOLEAN "\033[00;36m"
#  endif
#  ifndef SASSERT_COLOR_STRING
#    define SASSERT_COLOR_STRING "\033[00;31m"
#  endif
#  ifndef SASSERT_COLOR_OTHER
#    define SASSERT_COLOR_OTHER "\033[00;35m"
#  endif
#  ifndef SASSERT_COLOR_UNKNOW
#    define SASSERT_COLOR_UNKNOW "\033[00;36m"
#  endif
#  ifndef SASSERT_COLOR_SYMBOLE
#    define SASSERT_COLOR_SYMBOLE "\033[00;35m"
#  endif
#  ifndef SASSERT_COLOR_EXPR
#    define SASSERT_COLOR_EXPR "\033[01;30m"
#  endif
#  ifndef SASSERT_COLOR_MSG
#    define SASSERT_COLOR_MSG "\033[01;31m"
#  endif
#  ifndef SASSERT_COLOR_FILE
#    define SASSERT_COLOR_FILE "\033[01;31m"
#  endif
#  ifndef SASSERT_COLOR_LINE
#    define SASSERT_COLOR_LINE "\033[00;33m"
#  endif
#  ifndef SASSERT_COLOR_FUNCTION
#    define SASSERT_COLOR_FUNCTION "\033[01;34m"
#  endif
#  ifndef SASSERT_COLOR_RESET
#    define SASSERT_COLOR_RESET "\033[00m"
#  endif
#else
#  define SASSERT_COLOR_CHARACTER ""
#  define SASSERT_COLOR_FLOATING ""
#  define SASSERT_COLOR_INTEGRAL ""
#  define SASSERT_COLOR_BOOLEAN ""
#  define SASSERT_COLOR_STRING ""
#  define SASSERT_COLOR_OTHER ""
#  define SASSERT_COLOR_UNKNOW ""
#  define SASSERT_COLOR_SYMBOLE ""
#  define SASSERT_COLOR_EXPR ""
#  define SASSERT_COLOR_MSG ""
#  define SASSERT_COLOR_FILE ""
#  define SASSERT_COLOR_LINE ""
#  define SASSERT_COLOR_FUNCTION ""
#  define SASSERT_COLOR_RESET ""
#endif

enum class PrinterType
{
  integral,
  floating_point,
  boolean,
  string,
  character,
  other
};

template<PrinterType val>
struct printer_type
{
  static constexpr PrinterType value = val;
  typedef printer_type type;
};

struct unless_for_to_string {};
void to_string(unless_for_to_string);

template<typename T, PrinterType =
falcon::eval_if_c<falcon::is_character<T>, printer_type<PrinterType::character>,
falcon::eval_if_c<std::is_same<T, bool>, printer_type<PrinterType::boolean>,
falcon::eval_if_c<std::is_integral<T>, printer_type<PrinterType::integral>,
falcon::eval_if_c<std::is_floating_point<T>, printer_type<PrinterType::floating_point>,
falcon::eval_if_c<falcon::is_string<T>, printer_type<PrinterType::string>,
printer_type<PrinterType::other>
>>>>>::type::value>
struct Printer
{
  static void print(const T& x)
  { dispatch_print(x, 0); }

  template<typename U>
  static auto dispatch_print(const U& x, int)
  -> decltype(std::cerr << x)
  { return std::cerr << SASSERT_COLOR_OTHER "{ " << x << " }" SASSERT_COLOR_RESET; }

  template<typename U>
  static void dispatch_print(const U& x, unsigned)
  { dispatch_with_to_string(x, 0); }

  template<typename U>
  static auto dispatch_with_to_string(const U& x, int)
  -> decltype(std::cerr << to_string(x))
  { return std::cerr << SASSERT_COLOR_STRING "\"" << to_string(x) << "\"" SASSERT_COLOR_RESET; }

  template<typename U>
  static void dispatch_with_to_string(const U&, unsigned)
  { std::cerr << SASSERT_COLOR_UNKNOW "<unknow>" SASSERT_COLOR_RESET; }
};

template<typename T>
struct Printer<T, PrinterType::integral>
{
  static void print(const T& x)
  { std::cerr << "" SASSERT_COLOR_INTEGRAL << x << "" SASSERT_COLOR_RESET; }
};

template<typename T>
struct Printer<T, PrinterType::floating_point>
{
  static void print(const T& x)
  { std::cerr << "" SASSERT_COLOR_FLOATING << x << "" SASSERT_COLOR_RESET; }
};

template<typename T>
struct Printer<T, PrinterType::string>
{
  static void print(const T& x)
  { dispatch_print(x, 0); }

  static auto dispatch_print(const T& x, int)
  -> decltype(std::cerr << x)
  { return std::cerr << SASSERT_COLOR_STRING "\"" << x << "\"" SASSERT_COLOR_RESET; }

  static void dispatch_print(const T& x, unsigned)
  { std::wcerr << SASSERT_COLOR_STRING "\"" << x << "\"" SASSERT_COLOR_RESET; }
};

template<typename T>
struct Printer<T, PrinterType::character>
{
  static void print(char x)
  {
    if (x == std::char_traits<char>::to_char_type('\'')) {
      std::cerr << SASSERT_COLOR_CHARACTER"'\''" SASSERT_COLOR_RESET;
    }
    else {
      std::cerr << SASSERT_COLOR_CHARACTER "'" << x << "'" SASSERT_COLOR_RESET;
    }
  }

  template<typename U>
  static void print(const U& x)
  {
    if (x == std::char_traits<T>::to_char_type('\'')) {
      std::wcerr << SASSERT_COLOR_CHARACTER"'\''" SASSERT_COLOR_RESET;
    }
    else {
      std::wcerr << SASSERT_COLOR_CHARACTER "'" << x << "'" SASSERT_COLOR_RESET;
    }
  }
};

template<typename T>
struct Printer<T, PrinterType::boolean>
{
  static void print(const T& x)
  {
    if (x) {
      std::cerr << SASSERT_COLOR_BOOLEAN "true" SASSERT_COLOR_RESET;
    }
    else {
      std::cerr << SASSERT_COLOR_BOOLEAN "false" SASSERT_COLOR_RESET;
    }
  }
};


template<typename T = void, typename U = void>
struct SAssert
{
  T l;
  const char * op;
  U r;

  void print() const
  {
    print(l);
    std::cerr << " " SASSERT_COLOR_SYMBOLE << op << SASSERT_COLOR_RESET " ";
    print(r);
  }

  template<typename P>
  void print(const P& p) const
  { Printer<P>::print(p); }

  template<typename TT, typename UU>
  void print(const SAssert<TT,UU>& a) const
  { a.print(); }

  template<typename R>
  SAssert<const SAssert&, const R&> operator=(const R& x)
  { return {*this, "=", x}; }
};

template<>
struct SAssert<void,void>
{};

template<typename T>
struct SAssert<T,void>
{
  T l;

  void print() const
  { Printer<T>::print(l); }

  template<typename U>
  SAssert<T, const U&> operator=(const U& x)
  { return {l, "=", x}; }
};

template<typename T>
SAssert<const T&>
operator*(const SAssert<>&, const T& x)
{ return {x}; }

constexpr SAssert<> B;

#define MOP(op)\
template<typename T, typename U>\
SAssert<const T&, const U&>\
operator op(const SAssert<T>& a, const U& x)\
{ return {a.l, #op, x}; }\
\
template<typename T, typename U, typename R>\
SAssert<const SAssert<T,U>&, const R&>\
operator op(const SAssert<T,U>& a, const R& x)\
{ return {a, #op, x}; }

MOP(&)
MOP(^)
MOP(|)
MOP(||)
MOP(&&)
MOP(*)
MOP(+)
MOP(-)
MOP(/)
MOP(%)
MOP(==)
MOP(!=)
MOP(<)
MOP(<=)
MOP(>)
MOP(>=)

#undef MOP
}

//BOOST_PP_CAT
#define PP_CAT_I(e) #e
#define PP_CAT(e) PP_CAT_I(e)

#ifndef NO_BOOST
#  define SASSERT_FUNCTION BOOST_CURRENT_FUNCTION
#elif defined __GNU && defined __cplusplus ? __GNUC_PREREQ (2, 6) : __GNUC_PREREQ (2, 4)
#  define SASSERT_FUNCTION __PRETTY_FUNCTION__
#else
#  if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#    define SASSERT_FUNCTION __func__
#  else
#    define SASSERT_FUNCTION ""
#  endif
# endif

#define sassert(msg, expr) do {\
  if ((expr) ? 0 : 1) {\
    std::ios_base::sync_with_stdio(1);\
    std::cerr << SASSERT_COLOR_MSG << (msg) << SASSERT_COLOR_RESET ": ";\
    (Super_Assert::B * expr).print();\
    std::cerr << \
    "\n" SASSERT_COLOR_FILE __FILE__ SASSERT_COLOR_RESET \
    ":" SASSERT_COLOR_LINE PP_CAT(__LINE__) SASSERT_COLOR_RESET \
    ": " SASSERT_COLOR_FUNCTION << SASSERT_FUNCTION << SASSERT_COLOR_RESET \
    ": '" SASSERT_COLOR_EXPR PP_CAT(expr) SASSERT_COLOR_RESET "' failed" \
    << std::endl;\
    abort();\
  }\
} while(0)

#endif

int main()
{
  int  a = 6;
  int  b = 1;
  int  c = 0;
  int  d = 9;
  sassert("ok",
          a * b + c + 'a' < d );
  sassert("non satisfaisant à cause de la priorité des opérateurs",
          a * b && c < d );
  struct A{ operator bool (){return true;}} aa;
  sassert("no printable", aa);
  sassert("s==s", ""=="");
}
