# ------------------------------------------------------------------------------------------
#
# 文  件： Makefile for 8皇后
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
COMPONENT_NAME := 8Empress

YDS_TARGETS := $(PRM_PREFIX)$(COMPONENT_NAME)$(PRM_SUFFIX)
YDS_EXPORTS := $(PRM_PREFIX)$(COMPONENT_NAME)$(PRM_SUFFIX) 8empress.h
YDS_IMPORTS :=

#---------------------------------------------------
# define the Object files
#---------------------------------------------------
EMPRESS_OBJS := 8empress.o 8empress_test.o

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
$(YDS_TARGETS):$(EMPRESS_OBJS)
	@$(ECHO) [LD] $(YDS_TARGETS)
	$(LINK_PROGRAM)

#---------------------------------------------------
# clean the library and object
#---------------------------------------------------
clean:
	@$(ECHO) [CL] $(YDS_TARGETS)
	@$(ECHO) [CL] $(EMPRESS_OBJS)
	-$(RM)  $(YDS_TARGETS)
	-$(RM)  $(EMPRESS_OBJS)

else
include $(YDS_MAKE)/builddir.mak
endif


# ----------------end of Makefile-------------------------------------
# 有水的地方就有余 http://hi.baidu.com/20065562
#

