# Nonrecusive Makefile sample


# $(call source_to_object, source_file_list)
source_to_object = 	$(subst .c,.o,$(filter %.c,$1)) \
										$(subst .y,.o,$(filter %.y,$1)) \
										$(subst .l,.o,$(filter %.l,$1))

# $(subdirectory)
subdirectory = $(patsubst %/module.mk,%, \
							 		$(word \
										$(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))

# $(call make_library, library_name, source_file_list)
define make_library
	libraries += $1
	sources 	+= $2

$1: $(call source_to_object, $2)
	$(AR) $(ARFLAGS) $$@ $$^
endef

# $(call generated_source, source_file_list)
generated_source = 	$(subst .y,.c,$(filter %.y,$1)) \
										$(subst .y,.h,$(filter %.y,$1)) \
										$(subst .l,.c,$(filter %.l,$1))


# Collect information from each module in these four variables.
# Initialize them here as simple variables.
modules				:= lib/codec lib/db lib/ui app/player
programs 			:=
libraries			:=
sources  			:=

objects 			= $(call source_to_object, $(sources))
dependencies 	= $(subst .o,.d,$(objects))

include_dirs 	:= lib include
CPPFLAGS			+= $(addprefix -I ,$(include_dirs))
vpath	%.h $(include_dirs)

MV := mv -f
RM := rm -f
SED := sed

all:

include $(addsuffix /module.mk,$(modules))

.PHONY: all
all: $(programs)

.PHONY: libraries
libraries: $(libraries)

.PHONY: clean
clean:
	$(RM) $(objects) $(programs) $(libraries) $(dependencies) \
		$(call generated_source, $(sources))

ifneq "$(MAKECMDGOALS)" "clean"
include $(dependencies)
endif

%.c %.h: %.y
	$(YACC.y) --defines $<
	$(MV) y.tab.c $*.c
	$(MV) y.tab.h $*.h

%.d: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -M $< | \
		$(SED) 's,\($(notdir $*)\.o\) *:,$(dir $@)\1 $@: ,' > $@.tmp
	$(MV) $@.tmp $@





