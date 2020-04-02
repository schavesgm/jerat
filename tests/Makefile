PROG = exec_tests

CXX = g++
CXFLAGS = -g -W

ROOT := $(shell pwd)
SOURCE_CODE := $(shell cd ../ && pwd)

CATCH = -I catch
INCLD = -I ../include
OBJS = ../objs

$(PROG):
	$(CXX) -o $@ test.cpp $(OBJS)/io_files.o \
	    $(OBJS)/corr_tree.o $(OBJS)/corr_basics.o $(OBJS)/utils.o \
	    -I coll_IO -I coll_corr $(CATCH) $(INCLD)
