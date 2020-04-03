LINK_TARGET = prog

CXX = g++
CXFLAGS = -g -W

ROOT := $(shell pwd)
LIB := $(ROOT)/lib
INC := $(ROOT)/include

OBJS = main io_files $(CORRS)
$(LINK_TARGET): objs main $(CORRS)
	$(CXX) $(CXFLAGS) -o $@ $(foreach obj,$(OBJS), $</$(obj).o ) \
	    -I $(INC)

main: objs corr_basics
	$(CXX) $(CXFLAGS) -o $</$@.o -c $@.cpp -I $(INC)

# Compile all the correlation function codes
CORRS = utils corr_basics
corr_basics: objs utils
	$(CXX) $(CXFLAGS) -o $</$@.o -c $(LIB)/corr/$@.cpp -I $(INC)

utils: objs io_files
	$(CXX) $(CXFLAGS) -o $</$@.o -c $(LIB)/corr/$@.cpp -I $(INC)

# Create the io_file object
io_files: objs
	$(CXX) $(CXFLAGS) -o $</$@.o -c $(LIB)/$@.cpp -I $(INC)

objs:
	$(shell mkdir -p $@)
