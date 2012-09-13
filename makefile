# makefile for RealNumber code

# builds object files for:
# real_number.o real_number_test.o
# array_arithmetic.o
# convergent_series.o
# find_pi.o
# makes executables:
# real_number_test array_arithmetic_test find_pi
# Use Boost unit test library for testing

# Created by Marshall Farrier on 6/22/2012.
# Copyright (c) 2012 Marshall Farrier
# license http://opensource.org/licenses/gpl-license.php GNU Public License
# http://www.cs.umd.edu/class/fall2002/cmsc214/Tutorial/makefile.html

# cf. Mecklenburg, GNU Make 2005

OBJS = find_pi.o real_number.o convergent_series.o array_arithmetic.o
OBJSTEST_RN = real_number_test.o real_number.o array_arithmetic.o
OBJSTEST_ARITH = array_arithmetic_test.o array_arithmetic.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
CFLAGSTEST = $(CFLAGS) -I/usr/local/boost_1_49_0
LFLAGS = -Wall $(DEBUG)
LFLAGSTEST = $(LFLAGS) -L/Users/marshallfarrier/boost_1_49_0 -lboost_unit_test_framework

# TODO: This part isn't working so reverting for the moment to flat structure
# Use desired directory structure
# Cf. Mecklenburg, p. 20
#vpath %.cpp src
#vpath %.h include

# Link commands:
# Cf. Mecklenburg, pp. 13ff.
.PHONY: all
all: real_number_test array_arithmetic_test find_pi

find_pi: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o find_pi

real_number_test: $(OBJSTEST_RN)
	$(CC) $(LFLAGSTEST) $(OBJSTEST_RN) -o real_number_test

array_arithmetic_test: $(OBJSTEST_ARITH)
	$(CC) $(LFLAGSTEST) $(OBJSTEST_ARITH) -o array_arithmetic_test

# Compilation commands:
find_pi.o: find_pi.cpp real_number.h convergent_series.h array_arithmetic.h
	$(CC) $(CFLAGS) find_pi.cpp

real_number.o: real_number.cpp real_number.h array_arithmetic.h
	$(CC) $(CFLAGS) real_number.cpp

real_number_test.o: real_number_test.cpp
	$(CC) $(CFLAGSTEST) real_number_test.cpp 

convergent_series.o: convergent_series.cpp convergent_series.h real_number.h
	$(CC) $(CFLAGS) convergent_series.cpp

array_arithmetic.o: array_arithmetic.cpp array_arithmetic.h
	$(CC) $(CFLAGS) array_arithmetic.cpp

array_arithmetic_test.o: array_arithmetic_test.cpp
	$(CC) $(CFLAGSTEST) array_arithmetic_test.cpp 

.PHONY: clean
clean:
	\rm *.o *~ find_pi real_number_test array_arithmetic_test

tar:
	tar cfv RealNumber.tar find_pi.cpp real_number.h real_number.cpp convergent_series.h \
			convergent_series.cpp real_number_test.cpp array_arithmetic.h array_arithmetic.cpp
