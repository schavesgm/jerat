LINK_TARGET = prog

CXX = g++
CXFLAGS = -g -W

ROOT := $(shell pwd)
LIB := $(ROOT)/lib
INC := $(ROOT)/include

$(LINK_TARGET): objs main.o io_files.o
	$(CXX) $(CXFLAGS) -o $@ $</main.o $</io_files.o

main.o: objs
	$(CXX) $(CXFLAGS) -o $</$@ -c $*.cpp -I $(INC)

io_files.o: objs
	$(CXX) $(CXFLAGS) -o $</$@ -c $(LIB)/$*.cpp -I $(INC)

objs:
	$(shell mkdir -p $@)
