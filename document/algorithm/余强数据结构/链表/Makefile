# ------------------------------------------------------------------------------------------
#
# 文  件： Makefile for AVL树
# 作  者： 余    强
# 日  期： 2009-12-09
# 版  权： Copyleft (c) GPL
# E-mail: yuembed@126.com
# 博  客：『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562
#
# ------------------------------------------------------------------------------------------

ifdef IN_OBJECT_DIR
#-------------------------------------------------------------------
# pick up generic rules
#-------------------------------------------------------------------
include $(YDS_MAKE)/common.mak

#---------------------------------------------------
# define the targets and exported files
#---------------------------------------------------
COMPONENT_NAME := List

YDS_TARGETS := $(PRM_PREFIX)$(COMPONENT_NAME)$(PRM_SUFFIX) $(LIB_PREFIX)$(COMPONENT_NAME)$(LIB_SUFFIX)
YDS_EXPORTS := $(PRM_PREFIX)$(COMPONENT_NAME)$(PRM_SUFFIX) $(LIB_PREFIX)$(COMPONENT_NAME)$(LIB_SUFFIX) list.h
YDS_IMPORTS :=

#---------------------------------------------------
# define the Object files
#---------------------------------------------------
LIST_OBJS := list.o list_test.o
LIBRARY_OBJS:=list.o
#---------------------------------------------------
# subdirectory
#---------------------------------------------------
SUBDIRS :=

#---------------------------------------------------
# CFLAGS
#---------------------------------------------------


# pick up standars rules
include $(YDS_MAKE)/default.mak

#---------------------------------------------------
# Build library and object
#---------------------------------------------------
$(LIB_PREFIX)$(COMPONENT_NAME)$(LIB_SUFFIX):$(LIBRARY_OBJS)
	@$(ECHO) [AR] $@
	$(BUILD_LIBRARY)

$(PRM_PREFIX)$(COMPONENT_NAME)$(PRM_SUFFIX):$(LIST_OBJS)
	@$(ECHO) [LD] $@
	$(LINK_PROGRAM)

#---------------------------------------------------
# clean the library and object
#---------------------------------------------------
clean:
	@$(ECHO) [CL] $(YDS_TARGETS)
	@$(ECHO) [CL] $(LIST_OBJS)
	-$(RM)  $(YDS_TARGETS)
	-$(RM)  $(LIST_OBJS)

else
include $(YDS_MAKE)/builddir.mak
endif


# ----------------end of Makefile-------------------------------------
# 有水的地方就有余 http://hi.baidu.com/20065562
#

