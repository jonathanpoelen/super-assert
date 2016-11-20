/**
 * \author Jonathan Poelen, jonathan.poelen+sassert@gmail.com
 * \brief Improved display of the assert() macro from C++.
 */

#ifndef SASSERT_HPP
#define SASSERT_HPP

#ifdef NDEBUG

#define sassert_msg(expr, msg)
#define sassert(expr)

#else

#include <string>
#include <iostream>
#include <type_traits>

#include <cstdlib> // std::abort

#ifdef NO_BOOST_SUPPORT
#  ifndef SASSERT_NO_BOOST_SUPPORT
#    define SASSERT_NO_BOOST_SUPPORT
#  endif
#endif

#ifndef SASSERT_NO_BOOST_SUPPORT
#  include <boost/current_function.hpp>
#  include <boost/preprocessor/stringize.hpp>
#  include <boost/container/string.hpp>
#endif

// std::string_view / std::experimental::string_view
#if __cplusplus >= 201402L && defined __has_include
#  if __has_include(<string_view>)
#    include <string_view>
#    define SASSERT_STD_STRING_VIEW std::string_view
#  elif __has_include(<experimental/string_view>)
#    include <experimental/string_view>
#    define SASSERT_STD_STRING_VIEW std::experimental::string_view
#  endif
#endif

namespace Super_Assert {

#if defined(SASSERT_COLOR) && !defined(SASSERT_NO_COLOR)
#  ifndef SASSERT_COLOR_CHARACTER
#    define SASSERT_COLOR_CHARACTER "\033[00;33m"
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
#  ifndef SASSERT_COLOR_UNKNOWN
#    define SASSERT_COLOR_UNKNOWN "\033[00;36m"
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
#  ifdef SASSERT_COLOR_CHARACTER
#    undef SASSERT_COLOR_CHARACTER
#  endif
#  ifdef SASSERT_COLOR_FLOATING
#    undef SASSERT_COLOR_FLOATING
#  endif
#  ifdef SASSERT_COLOR_INTEGRAL
#    undef SASSERT_COLOR_INTEGRAL
#  endif
#  ifdef SASSERT_COLOR_BOOLEAN
#    undef SASSERT_COLOR_BOOLEAN
#  endif
#  ifdef SASSERT_COLOR_STRING
#    undef SASSERT_COLOR_STRING
#  endif
#  ifdef SASSERT_COLOR_OTHER
#    undef SASSERT_COLOR_OTHER
#  endif
#  ifdef SASSERT_COLOR_UNKNOWN
#    undef SASSERT_COLOR_UNKNOWN
#  endif
#  ifdef SASSERT_COLOR_SYMBOLE
#    undef SASSERT_COLOR_SYMBOLE
#  endif
#  ifdef SASSERT_COLOR_EXPR
#    undef SASSERT_COLOR_EXPR
#  endif
#  ifdef SASSERT_COLOR_MSG
#    undef SASSERT_COLOR_MSG
#  endif
#  ifdef SASSERT_COLOR_FILE
#    undef SASSERT_COLOR_FILE
#  endif
#  ifdef SASSERT_COLOR_LINE
#    undef SASSERT_COLOR_LINE
#  endif
#  ifdef SASSERT_COLOR_FUNCTION
#    undef SASSERT_COLOR_FUNCTION
#  endif
#  ifdef SASSERT_COLOR_RESET
#    undef SASSERT_COLOR_RESET
#  endif
#  define SASSERT_COLOR_CHARACTER ""
#  define SASSERT_COLOR_FLOATING ""
#  define SASSERT_COLOR_INTEGRAL ""
#  define SASSERT_COLOR_BOOLEAN ""
#  define SASSERT_COLOR_STRING ""
#  define SASSERT_COLOR_OTHER ""
#  define SASSERT_COLOR_UNKNOWN ""
#  define SASSERT_COLOR_SYMBOLE ""
#  define SASSERT_COLOR_EXPR ""
#  define SASSERT_COLOR_MSG ""
#  define SASSERT_COLOR_FILE ""
#  define SASSERT_COLOR_LINE ""
#  define SASSERT_COLOR_FUNCTION ""
#  define SASSERT_COLOR_RESET ""
#endif


// is_character
//@{
template<class> struct is_character : std::false_type {};

template<> struct is_character<signed char> : std::true_type {};
template<> struct is_character<unsigned char> : std::true_type {};
template<> struct is_character<char> : std::true_type {};
template<> struct is_character<wchar_t> : std::true_type {};
template<> struct is_character<char16_t> : std::true_type {};
template<> struct is_character<char32_t> : std::true_type {};
//@}


// is_string_like
//@{
template<class T> struct is_string_like : std::false_type {};

template<class T> struct is_string_like<T*> : is_character<T> {};
template<class T, std::size_t N>
struct is_string_like<T[N]> : is_character<T> {};

template<class CharT, class Traits, class Allocator>
struct is_string_like<std::basic_string<CharT, Traits, Allocator> > : std::true_type {};

#ifdef SASSERT_STD_STRING_VIEW
template<class CharT, class Traits>
struct is_string_like<SASSERT_STD_STRING_VIEW<CharT, Traits> > : std::true_type {};
#endif

#ifndef SASSERT_NO_BOOST_SUPPORT
template<class CharT, class Traits, class Allocator>
struct is_string_like<boost::basic_string<CharT, Traits, Allocator> > : std::true_type {};
#endif
//@}


template<bool b>
using require_c = typename std::enable_if<b>::type;

template<class B>
using require = require_c<B::value>;


using std::to_string;

template<class T, class = void>
struct Printer
{
  static void print(const T& x)
  { dispatch_print(x, 0); }

  template<class U>
  static auto dispatch_print(const U& x, int)
  -> decltype(std::cerr << x)
  { return std::cerr << SASSERT_COLOR_OTHER "{ " << x << " }" SASSERT_COLOR_RESET; }

  template<class U>
  static void dispatch_print(const U& x, char)
  { dispatch_with_to_string(x, 0); }

  template<class U>
  static auto dispatch_with_to_string(const U& x, int)
  -> decltype(std::cerr << to_string(x))
  { return std::cerr << SASSERT_COLOR_STRING "\"" << to_string(x) << "\"" SASSERT_COLOR_RESET; }

  template<class U>
  static void dispatch_with_to_string(const U&, char)
  { std::cerr << SASSERT_COLOR_UNKNOWN "<unknown>" SASSERT_COLOR_RESET; }
};

template<class T>
struct Printer<
  T, require_c<(
    std::is_integral<T>::value &&
    !std::is_same<T, bool>::value &&
    !is_character<T>::value
  )>
>
{
  static void print(const T& x)
  { std::cerr << "" SASSERT_COLOR_INTEGRAL << x << "" SASSERT_COLOR_RESET; }
};

template<class T>
struct Printer<T, require<std::is_floating_point<T>>>
{
  static void print(const T& x)
  { std::cerr << "" SASSERT_COLOR_FLOATING << x << "" SASSERT_COLOR_RESET; }
};

template<class T>
struct Printer<T, require<is_string_like<T>>>
{
  static void print(const T& x)
  { dispatch_print(x, 0); }

  static auto dispatch_print(const T& x, int)
  -> decltype(std::cerr << x)
  { return std::cerr << SASSERT_COLOR_STRING "\"" << x << "\"" SASSERT_COLOR_RESET; }

  static void dispatch_print(const T& x, char)
  { std::wcerr << SASSERT_COLOR_STRING "\"" << x << "\"" SASSERT_COLOR_RESET; }
};

template<class T>
struct Printer<T, require<is_character<T>>>
{
  static void print(char x)
  {
    if (x == std::char_traits<char>::to_char_type('\'')) {
      std::cerr << SASSERT_COLOR_CHARACTER "'\''" SASSERT_COLOR_RESET;
    }
    else {
      std::cerr << SASSERT_COLOR_CHARACTER "'" << x << "'" SASSERT_COLOR_RESET;
    }
  }

  template<class U>
  static void print(const U& x)
  {
    if (x == std::char_traits<T>::to_char_type('\'')) {
      std::wcerr << SASSERT_COLOR_CHARACTER "'\''" SASSERT_COLOR_RESET;
    }
    else {
      std::wcerr << SASSERT_COLOR_CHARACTER "'" << x << "'" SASSERT_COLOR_RESET;
    }
  }
};

template<class T>
struct Printer<T, require<std::is_same<T, bool>>>
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
  static void print(const P& p)
  { Printer<P>::print(p); }

  template<typename TT, typename UU>
  static void print(const SAssert<TT,UU>& a)
  { a.print(); }

  template<typename R>
  SAssert<const SAssert&, const R&> operator=(const R& x)
  { return {*this, "=", x}; }
};

template<>
struct SAssert<void,void>
{ constexpr SAssert(){} };

template<class T>
struct SAssert<T,void>
{
  T l;

  void print()
  { Printer<T>::print(l); }

  template<class U>
  SAssert<T, const U&> operator=(const U& x)
  { return {l, "=", x}; }
};

template<class T>
SAssert<const T&>
operator*(const SAssert<>&, const T& x)
{ return {x}; }

constexpr SAssert<> start() { return {}; }

#define SASSERT_OP(op)                           \
  template<class T, class U>               \
  SAssert<const T&, const U&>                    \
  operator op(const SAssert<T>& a, const U& x)   \
  { return {a.l, #op, x}; }                      \
                                                 \
  template<class T, class U, typename R>   \
  SAssert<const SAssert<T,U>&, const R&>         \
  operator op(const SAssert<T,U>& a, const R& x) \
  { return {a, #op, x}; }

SASSERT_OP(&)
SASSERT_OP(^)
SASSERT_OP(|)
SASSERT_OP(<<)
SASSERT_OP(>>)
SASSERT_OP(||)
SASSERT_OP(&&)
SASSERT_OP(*)
SASSERT_OP(+)
SASSERT_OP(-)
SASSERT_OP(/)
SASSERT_OP(%)
SASSERT_OP(==)
SASSERT_OP(!=)
SASSERT_OP(<)
SASSERT_OP(<=)
SASSERT_OP(>)
SASSERT_OP(>=)

#undef SASSERT_OP


#ifndef SASSERT_NO_BOOST_SUPPORT
#  define SASSERT_PP_STRINGIZE BOOST_PP_STRINGIZE
#  define SASSERT_FUNCTION BOOST_CURRENT_FUNCTION
#else
#  define SASSERT_PP_STRINGIZE_I(e) #e
#  define SASSERT_PP_STRINGIZE(e) SASSERT_PP_STRINGIZE_I(e)

#  if defined __GNUC__
#    define SASSERT_FUNCTION __PRETTY_FUNCTION__
#  elif defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#    define SASSERT_FUNCTION __func__
#  elif defined __cplusplus && __cplusplus >= 201103
#    define SASSERT_FUNCTION __func__
#  else
#    define SASSERT_FUNCTION ""
#  endif
#endif

inline void sassert_pre()
{
  std::cerr.clear();
  std::cerr.exceptions(std::ios::goodbit);
  std::wcerr.clear();
  std::wcerr.exceptions(std::ios::goodbit);
  std::ios_base::sync_with_stdio(1);
}

[[noreturn]] inline void assert_fail() noexcept
{
  std::abort();
}

#define SASSERT_FAIL_(expr)                                                   \
    ::std::cerr << SASSERT_COLOR_FILE __FILE__ SASSERT_COLOR_RESET            \
    ":" SASSERT_COLOR_LINE SASSERT_PP_STRINGIZE(__LINE__) SASSERT_COLOR_RESET \
    ":\n" SASSERT_COLOR_FUNCTION << SASSERT_FUNCTION << SASSERT_COLOR_RESET   \
    ":\nAssertion `" SASSERT_COLOR_EXPR SASSERT_PP_STRINGIZE(expr)            \
                     SASSERT_COLOR_RESET "` failed"                           \
     "\n           ";                                                         \
    (::Super_Assert::start() * expr).print();                                 \
    ::std::cerr << std::endl;                                                 \
    ::Super_Assert::assert_fail()

#define sassert_msg(expr, msg) do {                                         \
  if (!bool(expr)) {                                                        \
    ::Super_Assert::sassert_pre();                                          \
    try {                                                                   \
      ::std::cerr << SASSERT_COLOR_MSG << msg << SASSERT_COLOR_RESET ":\n"; \
      SASSERT_FAIL_(expr);                                                  \
    } catch (...) {                                                         \
    }                                                                       \
  }                                                                         \
} while(0)

#define sassert(expr) do {         \
  if (!bool(expr)) {               \
    ::Super_Assert::sassert_pre(); \
    try {                          \
      SASSERT_FAIL_(expr);         \
    } catch (...) {                \
    }                              \
  }                                \
} while(0)

#endif

}

#ifdef SASSERT_STD_STRING_VIEW
#  undef SASSERT_STD_STRING_VIEW
#endif

#endif
