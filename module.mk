# Sample nonrecusive make module.mk
#

local_dir := lib/codec
local_lib := $(local_dir)/libcodec.a
local_src := $(wildcard $(subdirectory)/*.c)
local_objs := $(subst .c,.o,$(local_src)

libraries += $(local_lib)
sources += $(local_src)

$(local_lib): $(local_objs)
	$(AR) $(ARFLAGS) $@ $^


