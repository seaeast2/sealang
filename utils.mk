# 
# makefile util functions
#


# $(call source-to-object, source-file-list)
source-to-object = 	$(subst .c,.o,$(filter %.c,$1)) \
										$(subst .cpp,.o,$(filter %.cpp,$1)) \
										$(subst .cc,.o,$(filter %.cc,$1)) \
										$(subst .y,.o,$(filter %.y,$1)) \
										$(subst .l,.o,$(filter %.l,$1))

# $(subdirectory)
subdirectory = $(patsubst $(SOURCE_DIR)/%/module.mk,%, \
							 	$(word \
									$(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))

