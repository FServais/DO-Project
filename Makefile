# Copyright 2013, Gurobi Optimization, Inc.

PLATFORM = mac64
INC      = /Library/gurobi563/mac64/include/
CPP      = g++
CARGS    = -m64 -g
CLIB     = -L/Library/gurobi563/mac64/lib/ -lgurobi56
RELEASE := $(shell uname -r | cut -f 1 -d .)
ifneq ($(RELEASE), 11)
	CPPSTDLIB = -stdlib=libstdc++
endif
CPPLIB   = -L/Library/gurobi563/mac64/lib/ -lgurobi_c++ -lgurobi56 $(CPPSTDLIB)

all: main

#run: run_c++

#run_c++: run_main

main: main.cpp
	$(CPP) $(CARGS) -o main main.cpp -I$(INC) $(CPPLIB) -lpthread -lm

#run_main: main
#	./main

clean:
	rm -rf *.o *_c *_c++ *.class *.log *.rlp *.lp *.bas *.ilp *.dSYM
