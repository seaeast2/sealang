# Build small compiler 

BINARY_OUT := out
TEST_OUT := $(BINARY_OUT)/test

# Utility functions =========================================================

# $(call source_dir_to_binary_out, directory_list)
source_dir_to_binary_out= $(addprefix $(BINARY_OUT)/, $1)

# $(call source_to_object, source_file_list)
source_to_object = 	$(call source_dir_to_binary_out, \
											$(subst .c,.o,$(filter %.c,$1)) \
											$(subst .cpp,.o,$(filter %.cpp,$1)))

# $(subdirectory)
subdirectory = $(patsubst %/module.mk,%, \
							 		$(word \
										$(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))


# $(call make_library, library_name, source_file_list)
define make_library
	libraries += $(BINARY_OUT)/$1
	sources 	+= $2

$(BINARY_OUT)/$1: $(call source_dir_to_binary_out, \
										$(subst .c,.o,$(filter %.c,$1)) \
										$(subst .cpp,.o,$(filter %.cpp,$1)))
	$(AR) $(ARFLAGS) $$@ $$^
endef


# $(call generated_source, source_file_list)
generated_source = 	$(subst .y,.c,$(filter %.y,$1)) \
										$(subst .y,.h,$(filter %.y,$1)) \
										$(subst .l,.c,$(filter %.l,$1))


# Local Variables ==========================================================
# Collect information from each module in these four variables.
# Initialize them here as simple variables.
modules				:= src test
programs 			:= $(BINARY_OUT)/main
test_programs :=
libraries			:=
sources  			:=

objects 			= $(call source_to_object, $(sources))
dependencies 	= $(subst .o,.d,$(objects))

include_dirs 	:= $(CURDIR)/include
CPPFLAGS			+= $(addprefix -I ,$(include_dirs))
vpath	%.h $(include_dirs)

compile_log		:= log.txt

# Compile Options ==========================================================
# CFLAGS : C compile options
# CPPFLAGS : C preprocessor options
# CXXFLAGS : C++ compile options
# LDFLAGS : linker options


# define the C compiler to use
CC := gcc
# define the C++ compiler to use
CXX := g++

# define any compile-time flags
CXXFLAGS += -Wall -g -std=c++11 -Wno-write-strings

# C preprocessor options
CPPFLAGS += -D ENABLE_DEBUG

# linker options
LDFLAGS +=

MKDIR := mkdir -p
MV := mv -f
RM := rm -f
SED := sed
TEST := test

create_output_directories := \
	$(shell for f in $(call source_dir_to_binary_out,$(modules)); \
			do \
				$(TEST) -d $$f || $(MKDIR) $$f; \
			done)


all:

include $(addsuffix /module.mk,$(modules))

.PHONY: all
all: $(programs)
	@echo Simple compiler has been compiled.

.PHONY: libraries
libraries: $(libraries)

.PHONY: clean
clean:
	$(RM) -r $(BINARY_OUT)

.PHONY: test
test: $(programs) $(test_programs)
	@echo run test

$(TEST_OUT)/% : $(TEST_OUT)/%.o $(programs_objs)
	$(CXX) $(LDFLAGS) -o $@ $^ $(libraries)

#ifneq "$(MAKECMDGOALS)" "clean"
#include $(dependencies)
#endif

$(programs) : $(program_main_obj) $(programs_objs)
	$(CXX) $(LDFLAGS) -o $@ $^ $(libraries)
	
$(BINARY_OUT)/%.o : %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<  -o $@

$(BINARY_OUT)/%.o : %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<  -o $@

