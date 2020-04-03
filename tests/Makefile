PROG = exec_tests

CXX = g++
CXFLAGS = -g -W

COLS = catch coll_IO coll_corr

ROOT := $(shell pwd)
SOURCE_CODE := $(shell cd ../ && pwd)
INCL = -I $(SOURCE_CODE)/include

define NEWLINE

endef


# $(PROG): io_files.o corrs.o
# 	$(CXX) -o $@ test.cpp \
# 	    $(foreach obj,$^, objs/$(obj)) \
# 	    $(foreach col,$(COLLECTIONS),-I $(col)) $(INCL)

CORRS = corr_basics utils
# $(CORRS): objs $(CORRS)
# 	$(CXX) $(CXFLAGS) -o $</$(CORRS).o \
# 	    -c $(SOURCE_CODE)/lib/corr/$(CORRS).cpp $(INCL) 
# 	# $(foreach corr,$(CORRS), \
# 	# $(CXX) $(CXFLAGS) -o $</$(CORRS).o \
# 	# -c $(SOURCE_CODE)/lib/corr/$(corr).cpp $(INCL))

$(PROG): utils 
	$(CXX) $(CXFLAGS) -o $@ test.cpp objs/*.o \
	    $(foreach col,$(COLS), -I $(col)) $(INCL)

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
