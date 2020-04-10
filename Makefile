LINK_TARGET = prog

CXX = g++
CXFLAGS = -g -W

ROOT := $(shell pwd)
LIB := $(ROOT)/lib
INC := $(ROOT)/include

OBJS = main io_files fit #$(CORRS)

all: $(LINK_TARGET) 
	$(info Copied git pre-commit hook) \
	$(shell cp ./hooks/pre-commit .git/hooks )

$(LINK_TARGET): objs $(OBJS)
	$(CXX) $(CXFLAGS) -o $@ $</*.o -I $(INC)

main: objs io_files
	$(CXX) $(CXFLAGS) -o $</$@.o -c $@.cpp -I $(INC)

fit: objs io_files
	$(CXX) $(CXFLAGS) -o $</$@.o -c lib/$@.cpp -I $(INC)

io_files: objs
	$(CXX) $(CXFLAGS) -o $</$@.o -c lib/$@.cpp -I $(INC)

objs:
	$(shell mkdir -p objs)

# $(LINK_TARGET): objs main $(CORRS)
# 	$(CXX) $(CXFLAGS) -o $@ $(foreach obj,$(OBJS), $</$(obj).o ) \
# 	    -I $(INC)
# 
# main: objs $(CORRS)
# 	$(CXX) $(CXFLAGS) -o $</$@.o -c $@.cpp -I $(INC)
# 
# # Compile all the correlation function codes
# CORRS = utils corr_basics
# # corr_tree: objs corr_basics
# # 	$(CXX) $(CXFLAGS) -o $</$@.o -c $(LIB)/corr/$@.cpp -I $(INC)
# 
# corr_basics: objs utils
# 	$(CXX) $(CXFLAGS) -o $</$@.o -c $(LIB)/corr/$@.cpp -I $(INC)
# 
# utils: objs fit
# 	$(CXX) $(CXFLAGS) -o $</$@.o -c $(LIB)/corr/$@.cpp -I $(INC)
# 
# # Create the fit object
# fit: objs io_files
# 	$(CXX) $(CXFLAGS) -o $</$@.o -c $(LIB)/$@.cpp -I $(INC)
# 
# # Create the io_file object
# io_files: objs
# 	$(CXX) $(CXFLAGS) -o $</$@.o -c $(LIB)/$@.cpp -I $(INC)
# 
# objs:
# 	$(shell mkdir -p $@)
