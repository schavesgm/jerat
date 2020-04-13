LINK_TARGET = exec_tests

CXX = g++
CXFLAGS = -g -W


ROOT := $(shell pwd)
SOURCE_CODE := $(shell cd ../ && pwd)
INCL = -I $(SOURCE_CODE)/include

# Collection to take care of
COLS = catch coll_IO coll_fit coll_corr

all: $(LINK_TARGET)
	$(shell rm -rf objs)

$(LINK_TARGET): io_files fit utils basics
	@$(CXX) $(CXFLAGS) -o $@ test.cpp objs/*.o \
	    $(foreach col,$(COLS), -I $(col)) $(INCL)

basics: objs utils
	@$(CXX) $(CXFLAGS) -o $</$@.o \
	    -c $(SOURCE_CODE)/lib//corr_$@.cpp $(INCL)

utils: objs fit
	@$(CXX) $(CXFLAGS) -o $</$@.o \
	    -c $(SOURCE_CODE)/lib//corr_$@.cpp $(INCL)

fit: objs io_files
	@$(CXX) $(CXFLAGS) -o $</$@.o \
	    -c $(SOURCE_CODE)/lib/$@.cpp $(INCL)

io_files: objs
	@$(CXX) -o $</$@.o -c $(SOURCE_CODE)/lib/$@.cpp $(INCL)

objs:
	@$(shell mkdir -p $@)
