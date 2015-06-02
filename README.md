super-sassert
=============

Improved display of the assert() macro from C++.

![Sassert example](https://raw.githubusercontent.com/jonathanpoelen/super-assert/master/sample_output.gif)

Build
-----

  `make` or `make CPPFLAGS+=-DSASSERT_COLOR`


Defined macros
--------------

* `SASSERT_COLOR_CHARACTER`   "\033[00;33m"
* `SASSERT_COLOR_FLOATING`    "\033[00;32m"
* `SASSERT_COLOR_INTEGRAL`    "\033[00;34m"
* `SASSERT_COLOR_BOOLEAN`     "\033[00;36m"
* `SASSERT_COLOR_STRING`      "\033[00;31m"
* `SASSERT_COLOR_OTHER`       "\033[00;35m"
* `SASSERT_COLOR_UNKNOW`      "\033[00;36m"
* `SASSERT_COLOR_SYMBOLE`     "\033[00;35m"
* `SASSERT_COLOR_EXPR`        "\033[01;30m"
* `SASSERT_COLOR_MSG`         "\033[01;31m"
* `SASSERT_COLOR_FILE`        "\033[01;31m"
* `SASSERT_COLOR_LINE`        "\033[00;33m"
* `SASSERT_COLOR_FUNCTION`    "\033[01;34m"
* `SASSERT_COLOR_RESET`       "\033[00m"


If `NDEBUG` is defined then sassert does nothing.

If `NO_BOOST` is defined then boost isn't use (boost/current_function.hpp and boost/preprocessor/stringize.hpp).

If `SASSERT_COLOR` is defined then colors are initialized.

If `SASSERT_NO_COLOR` is defined then `SASSERT_COLOR` is cancel
