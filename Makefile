LINK_TARGET = prog

CXX = g++
CXFLAGS = -g -W

ROOT := $(shell pwd)
LIB := $(ROOT)/lib
INC := $(ROOT)/include

OBJS = main io_files fit hier utils basics

all: $(LINK_TARGET) 
	$(shell rm -rf objs)

install: 
	$(info Copied git pre-commit hook) \
	$(shell cp ./hooks/pre-commit .git/hooks )

$(LINK_TARGET): objs $(OBJS)
	$(CXX) $(CXFLAGS) -o $@ $</*.o -I $(INC)

main: objs
	$(CXX) $(CXFLAGS) -o $</$@.o -c $@.cpp -I $(INC)

basics: objs
	$(CXX) $(CXFLAGS) -o $</corr_$@.o -c lib/corr_$@.cpp \
	    -I $(INC)
utils: objs
	$(CXX) $(CXFLAGS) -o $</corr_$@.o -c lib/corr_$@.cpp \
	    -I $(INC)
hier: objs
	$(CXX) $(CXFLAGS) -o $</$@.o -c lib/$@.cpp -I $(INC)

fit: objs
	$(CXX) $(CXFLAGS) -o $</$@.o -c lib/$@.cpp -I $(INC)

io_files: objs
	$(CXX) $(CXFLAGS) -o $</$@.o -c lib/$@.cpp -I $(INC)

objs:
	$(shell mkdir -p objs)

