PROG = exec_tests

CXX = g++
CXFLAGS = -g -W

COLLECTIONS = catch coll_IO coll_corr

ROOT := $(shell pwd)
SOURCE_CODE := $(shell cd ../ && pwd)
INCL = -I $(SOURCE_CODE)/include


$(PROG): io_files.o corrs.o 
	$(CXX) -o $@ test.cpp \
	    $(foreach obj,$^, objs/$(obj)) \
	    $(foreach col,$(COLLECTIONS),-I $(col)) $(INCL)

CORRS = corr_basics.cpp
corrs.o: objs io_files.o
	$(CXX) $(CXFLAGS) -o $</$@ \
	-c $(foreach corr,$(CORRS), $(SOURCE_CODE)/lib/corr/$(corr) ) \
	$(INCL)

io_files.o: objs
	$(CXX) -o $</$@ -c $(SOURCE_CODE)/lib/$*.cpp $(INCL)

objs:
	$(shell mkdir -p $@)
