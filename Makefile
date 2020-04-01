LINK_TARGET = prog

CXX = g++
CXFLAGS = -g -W

ROOT := $(shell pwd)
LIB := $(ROOT)/lib
INC := $(ROOT)/include

$(LINK_TARGET): objs main.o corr.o io_files.o
	$(CXX) $(CXFLAGS) -o $@ $</main.o $</corr.o $</io_files.o

main.o: objs corr.o io_files.o
	$(CXX) $(CXFLAGS) -o $</$@ -c $*.cpp -I $(INC)

corr.o: objs io_files.o
	$(CXX) $(CXFLAGS) -o $</$@ -c $(LIB)/$*.cpp -I $(INC)

# Create the io_file object
io_files.o: objs
	$(CXX) $(CXFLAGS) -o $</$@ -c $(LIB)/$*.cpp -I $(INC)

objs:
	$(shell mkdir -p $@)
