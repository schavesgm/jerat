LINK_TARGET = prog

CXX = g++
CXFLAGS = -g -W

ROOT := $(shell pwd)
LIB := $(ROOT)/lib
INC := $(ROOT)/include

$(LINK_TARGET): objs main.o corr_basics.o corr_tree.o io_files.o
	$(CXX) $(CXFLAGS) -o $@ $</main.o \
	    $</utils.o $</corr_basics.o $</corr_tree.o $</io_files.o

main.o: objs corr_tree.o
	$(CXX) $(CXFLAGS) -o $</$@ -c $*.cpp -I $(INC)

# # Compile all the correlation function codes
corr_tree.o: objs corr_basics.o
	$(CXX) $(CXFLAGS) -o $</$@ \
	    -c $(LIB)/corr/$*.cpp -I $(INC)

corr_basics.o: objs utils.o
	$(CXX) $(CXFLAGS) -o $</$@ \
	    -c $(LIB)/corr/$*.cpp -I $(INC)

utils.o: objs io_files.o
	$(CXX) $(CXFLAGS) -o $</$@ -c $(LIB)/corr/$*.cpp -I $(INC)

# Create the io_file object
io_files.o: objs
	$(CXX) $(CXFLAGS) -o $</$@ -c $(LIB)/$*.cpp -I $(INC)

objs:
	$(shell mkdir -p $@)
