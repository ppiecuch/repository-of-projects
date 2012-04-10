# -----------------------------------------------------------------
# 文  件： environment.mak
# 作  者： 余    强
# 日  期： 2009-12-08
# 版  权： Copyleft (c) GPL
# E-mail: yuembed@126.com
# 博  客：『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562
#-------------------------------------------------------------------

# current work path
YDS_BUILD_DIR := $(shell pwd)

# if IN_OBJECT_DIR has been defined
ifneq "$(origin IN_OBJECT_DIR)" "undefined"
  YDS_BUILD_DIR := $(YDS_PARENT_DIR)

  # 确保子目录不继承命令行参数
  MAKEOVERRIDES =
  unexport IN_OBJECT_DIR
  unexport YDS_PARENT_DIR
endif

# object directory name
ifeq "$(origin OBJDIR_NAME)" "undefined"
  OBJDIR_NAME :=objs
else
  OBJDIR_NAME :=$(OBJDIR_NAME_VAR)
endif

# command under linux
DATE   := $(shell date '+%d%m%Y-%H%M')
CAT    := -@cat
CD     := -@cd
CLS    := -@clear
NULL   := /dev/null
CP     := -@cp -fp 2>>$(NULL)
CMP    := -@cmp
ECHO   := -@echo
RM     := -@rm -f 2>>$(NULL)
RMDIR  := -@rmdir -p  --ignore-fail-on-non-empty 2>>$(NULL)
MKDIR  := -@mkdir -p 2>>$(NULL)
FULLRM := -@rm -rf 2>>$(NULL)
RENAME := -@mv 2>>$(NULL)

# set the export path for libraries
ifeq "$(YDS_EXPORT_LIBS)" "true"
  YDS_LIBRARY_EXPORTS := $(YDS_LIBRARY)
endif

# set the export path for headers
ifeq "$(YDS_EXPORT_HDRS)" "true"
  YDS_INCLUDE_EXPORTS := $(YDS_INCLUDE)
endif

# set the export path for program
ifeq "$(YDS_EXPORT_PRMS)" "true"
  YDS_PROGRAM_EXPORTS := $(YDS_PROGRAM)
endif

# --------------------- end of environment.mak ----------------------------
# 有水的地方就有余 http://hi.baidu.com/20065562
#
