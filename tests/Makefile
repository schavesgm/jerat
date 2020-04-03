PROG = exec_tests

CXX = g++
CXFLAGS = -g -W

COLLECTIONS = catch coll_IO

ROOT := $(shell pwd)
SOURCE_CODE := $(shell cd ../ && pwd)
INCL = -I $(SOURCE_CODE)/include


$(PROG): io_files.o
	$(CXX) -o $@ test.cpp ./objs/io_files.o \
	    $(foreach col,$(COLLECTIONS),-I $(col)) $(INCL)

test_IO.o: io_files.o
	$(CXX) -o objs/$@ -c coll_IO/test_io.hpp $(INCL) -I catch

io_files.o: objs
	$(CXX) -o $</$@ -c $(SOURCE_CODE)/lib/$*.cpp $(INCL)

objs:
	$(shell mkdir -p $@)
