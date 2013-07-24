#!/bin/sh
make CPPFLAGS+=-I. CPPFLAGS+=-DSASSERT_COLOR CXXFLAGS+=-std=c++11 sassert
