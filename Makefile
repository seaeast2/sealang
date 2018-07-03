#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
#               # 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

# define project root
PROJECT:=$(CURDIR)

# define the C compiler to use
CC = gcc
# define the C++ compiler to use
CXX = g++

# define any compile-time flags
CFLAGS = -Wall -g -std=c++11 -Wno-write-strings -D ENABLE_DEBUG

# define any directories containing header files other than /usr/include
#INCLUDES = -I/home/newhall/include  -I../include
INCLUDES = -I $(PROJECT)/include

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
#LFLAGS = -L/home/newhall/lib  -L../lib
LFLAGS =


# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
#LIBS = -lmylib -lm
LIBS =


# define the C source files
SRCS = ast_node.cpp ast_type.cpp ast_printer.cpp astcontext.cpp error_diagnosis.cpp lexer.cpp parser.cpp parser_grammar.cpp main.cpp vm.cpp

# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix

OBJS = $(SRCS:.cpp=.o)

# define the executable file 
MAIN = main 

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

VPATH=src include

.PHONY: depend clean test

all:    $(MAIN)
	@echo  Simple compiler named mycc has been compiled

$(MAIN): $(OBJS) 
	$(CXX) $(LFLAGS) -o $(MAIN) $(OBJS) $(LIBS) # Link

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
%.o : %.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $<  -o $@ # Compile

%.o : %.cc
	$(CXX) $(CFLAGS) $(INCLUDES) -c $<  -o $@ # Compile

%.o : %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@ # Compile

clean:
	$(RM) *.o *~ $(MAIN)
	$(RM) log.txt

test: $(MAIN)
	./$(MAIN)

