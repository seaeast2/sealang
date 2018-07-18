# Nonrecusive Makefile sample

# Collect information from each module in these four variables.
# Initialize them here as simple variables.

# $(call source-to-object, source-file-list)
source-to-object = $(subst .c,.o,$(filter %.c,$1)) \

# $(call make-library, library-name, source-file-list)
define make-library
	libraries += $1
	sources 	+= $2
	$1: $(call source-to-object, $2)
		$(AR) $(ARFLAGS) $$@ $$^
endif

programs 			:=
sources  			:=
libraries			:=
extra_clean 	:=


objects 			= $(subst .c,.o,$(sources))
dependencies 	= $(subst	.c,.d,$(sources))

include_dirs 	:= lib include
CPPFLAGS			+= $(addprefix -I ,$(include_dirs))
vpath	%.h $(include_dirs)

MV := mv -f
RM := rm -f
SED := sed

all:


include lib/codec/module.mk
include lib/db/module.mk
include lib/ui/module.mk
include app/player/module.mk

.PHONY: all
all: $(programs)

.PHONY: libraries
libraries: $(libraries)

.PHONY: clean
clean:
	$(RM) $(objects) $(programs) $(libraries) \
		$(dependencies) $(extra_clean)

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





