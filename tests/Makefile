PROG = test.exec

CXX = g++
CXFLAGS = -g -W

ROOT := $(shell pwd)
SOURCE_CODE := $(shell cd ../ && pwd)

CATCH = -I catch
INCLD = -I ../include
OBJS = ../objs

$(PROG):
	$(CXX) -o $@ test.cpp $(OBJS)/io_files.o $(OBJS)/corr.o \
	    -I coll_IO -I coll_corr $(CATCH) $(INCLD)
