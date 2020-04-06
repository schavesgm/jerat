PROG = exec_tests

CXX = g++
CXFLAGS = -g -W


ROOT := $(shell pwd)
SOURCE_CODE := $(shell cd ../ && pwd)
INCL = -I $(SOURCE_CODE)/include

COLS = catch coll_IO coll_corr
$(PROG): io_files utils corr_basics corr_tree
	$(CXX) $(CXFLAGS) -o $@ test.cpp objs/*.o \
	    $(foreach col,$(COLS), -I $(col)) $(INCL)

corr_tree: objs corr_basics
	$(CXX) $(CXFLAGS) -o $</$@.o \
	    -c $(SOURCE_CODE)/lib/corr/$@.cpp $(INCL)
corr_basics: objs utils
	$(CXX) $(CXFLAGS) -o $</$@.o \
	    -c $(SOURCE_CODE)/lib/corr/$@.cpp $(INCL)

utils: objs io_files
	$(CXX) $(CXFLAGS) -o $</$@.o \
	    -c $(SOURCE_CODE)/lib/corr/$@.cpp $(INCL)

io_files: objs
	$(CXX) -o $</$@.o -c $(SOURCE_CODE)/lib/$@.cpp $(INCL)

objs:
	$(shell mkdir -p $@)
