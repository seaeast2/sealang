# Sample nonrecusive make module.mk
#

#local_dir := lib/codec
#local_lib := $(local_dir)/libcodec.a

local_dir := 
local_lib := 
local_src := $(wildcard $(subdirectory)/*.c) $(wildcard $(subdirectory)/*.cpp)
local_objs := $(call source_to_object,$(local_src))

libraries += $(local_lib)
sources += $(local_src)

$(local_lib): $(local_objs)
	$(AR) $(ARFLAGS) $@ $^

# build test programs
test_objs := $(call source_to_object,$(local_src)) 
test_programs := $(patsubst %.o,%,$(test_objs))


