# Build small compiler 

# define the C compiler to use
CC := gcc
# define the C++ compiler to use
CXX := g++

# $(call source_to_object, source_file_list)
source_to_object = 	$(subst .c,.o,$(filter %.c,$1)) \
										$(subst .cpp,.o,$(filter %.cpp,$1))

# $(subdirectory)
subdirectory = $(patsubst %/module.mk,%, \
							 		$(word \
										$(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))

# TODO : working here

# define any compile-time flags
#CFLAGS := -Wall -g -std=c++11 -Wno-write-strings -D ENABLE_DEBUG



.PHONY: depend clean test

all: $(MAIN)
	@echo  Simple compiler named mycc has been compiled

$(MAIN): $(OBJS) 
	$(CXX) $(LFLAGS) -o $(MAIN) $(OBJS) $(LIBS)

%.o : %.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $<  -o $@

%.o : %.cc
	$(CXX) $(CFLAGS) $(INCLUDES) -c $<  -o $@

%.o : %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)
	$(RM) log.txt

test: $(MAIN)
	./$(MAIN)

