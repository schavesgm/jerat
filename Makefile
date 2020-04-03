LINK_TARGET = prog

CXX = g++
CXFLAGS = -g -W

ROOT := $(shell pwd)
LIB := $(ROOT)/lib
INC := $(ROOT)/include

OBJS = main.o corrs.o io_files.o
$(LINK_TARGET): objs main.o corrs.o
	$(CXX) $(CXFLAGS) -o $@ $(foreach obj,$(OBJS), $</$(obj) )

main.o: objs
	$(CXX) $(CXFLAGS) -o $</$@ -c $*.cpp -I $(INC)

# # Compile all the correlation function codes
CORRS = corr_basics.cpp
corrs.o: objs io_files.o
	$(CXX) $(CXFLAGS) -o $</$@ \
	    -c $(foreach corr,$(CORRS), $(LIB)/corr/$(corr) ) -I $(INC)

# Create the io_file object
io_files.o: objs
	$(CXX) $(CXFLAGS) -o $</$@ -c $(LIB)/$*.cpp -I $(INC)

objs:
	$(shell mkdir -p $@)
