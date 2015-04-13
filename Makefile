CXXFLAGS+=-std=c++11 -Wall -Wextra
CPPFLAGS+=-DSASSERT_COLOR
CPPFLAGS+=-I.


test: test.cpp sassert.hpp
