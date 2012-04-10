# -----------------------------------------------------------------
# 文  件： default.mak
# 作  者： 余    强
# 日  期： 2009-12-08
# 版  权： Copyleft (c) GPL
# E-mail: yuembed@126.com
# 博  客：『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562
#-------------------------------------------------------------------

#load the user's task
ifdef YDS_TARGETS
  DEFAULT_TARGETS := $(YDS_TARGETS)
else
  DEFAULT_TARGETS := $(TARGETS)
endif

ifdef YDS_IMPORTS
  DEFAULT_IMPORTS := $(YDS_IMPORTS)
else
  DEFAULT_IMPORTS := $(IMPORTS)
endif

ifdef YDS_EXPORTS
  DEFAULT_EXPORTS := $(YDS_EXPORTS)
else
  DEFAULT_EXPORTS := $(EXPORTS)
endif


# load the user's flags
ifdef YDS_CFLAGS
  DEFAULT_CFLAGS := $(YDS_CFLAGS)
else
  DEFAULT_CFLAGS := $(OTHER_CFLAGS)
endif

ifdef YDS_LDFLAGS
  DEFAULT_LDFLAGS := $(YDSLDFLAGS)
else
  DEFAULT_LDFLAGS := $(OTHER_LDFLAGS)
endif

# set global flags
DEFAULT_CFLAGS += $(CFLAGS)

# set the global ldflags
DEFAULT_LDFLAGS += $(LDFLAGS)

# set proper include path
#---------------------------------------------
ifeq "$(YDS_EXPORT_HDRS)" "true"
  INCLUDE_PATH += $(YDS_INCLUDE_EXPORTS)
endif
INCLUDE_PATH += $(YDS_BUILD_DIR)
# add import path
INCLUDE_PATH += $(addprefix $(YDS_ROOT)/,$(DEFAULT_IMPORTS))
# add subdir path
INCLUDE_PATH += $(SUBDIR_HEADER_PATH)
SUBDIR_HEADER_PATH = $(foreach DAT,$(SUBDIRS),$(wildcard $(YDS_BUILD_DIR)/$(DAT)))
# add import_header path
INCLUDE_PATH += $(IMPORT_HDRS_TMP)
IMPORT_HDRS_TMP = $(addprefix $(YDS_ROOT)/,$(IMPORT_HEADERS))


# set proper library path
#---------------------------------------------
LIBRARY_PATH += $(strip $(SUBDIR_LIBRARY_PATH) $(IMPORT_LIB_PATH))
# add import library path
IMPORT_LIB_PATH = $(foreach IMP,$(DEFAULT_IMPORTS), \
                            $(if $(wildcard $(YDS_ROOT)/$(IMP)/$(OBJDIR_NAME)), \
                                 $(wildcard $(YDS_ROOT)/$(IMP)/$(OBJDIR_NAME)), \
                                 $(wildcard $(YDS_ROOT)/$(IMP))))
# add subdir library parh
SUBDIR_LIBRARY_PATH = $(foreach SUB,$(SUBDIRS),\
			    $(if $(wildcard $(YDS_BUILD_DIR)/$(SUB)/$(OBJDIR_NAME)),\
				$(wildcard $(YDS_BUILD_DIR)/$(SUB)/$(OBJDIR_NAME)),\
				$(wildcard $(YDS_BUILD_DIR)/$(SUB))))

# set proper library path
#---------------------------------------------
.PHONY: default start finish

vpath %.c $(YDS_BUILD_DIR)

default:edir_check start $(DEFAULT_TARGETS) finish

start:
	@$(ECHO) 
	@$(ECHO) ---------Building $(DEFAULT_TARGETS)---------

finish:copy_library copy_header copy_program
	@$(ECHO) ---------$(DEFAULT_TARGETS) Complete---------

SUBDIRS_LIBS  := $(addprefix $(LIB_PREFIX),$(addsuffix $(LIB_SUFFIX),$(SUBDIRS)))
# 避免clean子目录操作同名，加上_clean_前缀
SUBDIRS_CLEAN := $(addprefix _clean_,$(SUBDIRS))

$(SUBDIRS_LIBS):
	@$(MAKE) -C $(YDS_BUILD_DIR)/$(patsubst $(LIB_PREFIX)%$(LIB_SUFFIX),%,$@)
 
$(SUBDIRS_CLEAN):
	-@$(MAKE) -C $(YDS_BUILD_DIR)/$(patsubst _clean_%,%,$@) clean

IMPORTS_LIBS := $(addprefix $(LIB_PREFIX),$(addsuffix $(LIB_SUFFIX),$(DEFAULT_IMPORTS)))
IMPORTS_CLEAN:= $(addprefix _clean_,$(DEFAULT_IMPORTS))

$(IMPORTS_LIBS):
	@$(MAKE) -C $(YDS_BUILD_DIR)/$(patsubst $(LIB_PREFIX)%$(LIB_SUFFIX),%,$@)

$(IMPORTS_CLEAN):
	-@$(MAKE) -C $(YDS_BUILD_DIR)/$(patsubst _clean_%,%,$@) clean

# export
#---------------------------------------------
.PHONY:edir_check copy_library copy_header copy_program

edir_check:
ifeq "$(wildcard $(YDS_LIBRARY_EXPORTS)/)" ""
	@$(ECHO) "Creating $(YDS_LIBRARY_EXPORTS)"
	@$(MKDIR) $(YDS_LIBRARY_EXPORTS)
endif
ifeq "$(wildcard $(YDS_INCLUDE_EXPORTS)/)" ""
	@$(ECHO) "Creating $(YDS_INCLUDE_EXPORTS)"
	@$(MKDIR) $(YDS_INCLUDE_EXPORTS)
endif
ifeq "$(wildcard $(YDS_PROGRAM_EXPORTS)/)" ""
	@$(ECHO) "Creating $(YDS_PROGRAM_EXPORTS)"
	@$(MKDIR) $(YDS_PROGRAM_EXPORTS)
endif

ifeq "$(YDS_EXPORT_HDRS)" "true"
EXPORTED_HEADERS := $(addprefix $(YDS_INCLUDE_EXPORTS)/,$(EXPORT_HDRS))
copy_header:$(EXPORTED_HEADERS)
else
copy_header:;
endif

ifeq "$(YDS_EXPORT_LIBS)" "true"
EXPORTED_LIBRARIES := $(addprefix $(YDS_LIBRARY_EXPORTS)/, $(EXPORT_LIBS))
copy_library:$(EXPORTED_LIBRARIES)
else
copy_library:;
endif

ifeq "$(YDS_EXPORT_PRMS)" "true"
EXPORTED_PROGRAMS := $(addprefix $(YDS_PROGRAM_EXPORTS)/,$(EXPORT_PRMS))
copy_program:$(EXPORTED_PROGRAMS)
else
copy_program:;
endif

$(EXPORTED_HEADERS):
	@$(ECHO) Exporting $(@F) To $(YDS_INCLUDE_EXPORTS)
	-@$(CP) $(YDS_BUILD_DIR)/$(@F) $@ > $(NULL)

$(EXPORTED_LIBRARIES):
	@$(ECHO) Exporting $(@F) To $(YDS_LIBRARY_EXPORTS)
	-@$(CP) $(YDS_BUILD_DIR)/$(OBJDIR_NAME)/$(@F) $@ > $(NULL)

$(EXPORTED_PROGRAMS):
	@$(ECHO) Exporting $(@F) To $(YDS_PROGRAM_EXPORTS)
	-@$(CP) $(YDS_BUILD_DIR)/$(OBJDIR_NAME)/$(@F) $@ > $(NULL)

# clean
#---------------------------------------------
clean_all: clean clean_hdrs clean_libs clean_prms 

clean_libs:
ifeq "$(YDS_EXPORT_LIBS)" "true"
	@$(ECHO) Cleaning exported libraries ...
	-$(RM) $(YDS_LIBRARY_EXPORTS)/*$(LIB_SUFFIX)
    ifneq "$(wildcard $(YDS_LIBRARY_EXPORTS)/)" ""
	@$(ECHO) Removing library directory ...
	-@$(RMDIR) $(YDS_LIBRARY_EXPORTS)
    endif
endif

clean_hdrs:
ifeq "$(YDS_EXPORT_HDRS)" "true"
	@$(ECHO) Cleaning exported headers ...
	-$(RM) $(YDS_INCLUDE_EXPORTS)/*.h
    ifneq "$(wildcard $(YDS_INCLUDE_EXPORTS)/)" ""
	@$(ECHO) Removing include directory ...
	-@$(RMDIR) $(YDS_LIBRARY_EXPORTS)
    endif
endif

clean_prms:
ifeq "$(YDS_EXPORT_PRMS)" "true"
	@$(ECHO) CLeaning exported programs ...
	-$(RM) $(YDS_PROGRAM_EXPORTS)/*$(PRM_SUFFIX)
    ifneq "$(wildcard $(YDS_PROGRAM_EXPORTS)/)" ""
	@$(ECHO) Removing program directory ...
	-@$(RMDIR) $(YDS_PROGRAM_EXPORTS)
    endif
endif

helpme:
	@$(ECHO) "-------------------------------------------------"
	@$(ECHO) "make                    编译所有的数据结构"
	@$(ECHO) "make MODULE=红黑树      只编译“红黑树”目录"
	@$(ECHO) "make DEBUG=1            编译调试版本"
	@$(ECHO) ""
	@$(ECHO) "make clean              只清除目标文件，不清除导出文件"
	@$(ECHO) "make clean_libs         只清除导出库文件"
	@$(ECHO) "make clean_hdrs         只清除导出头文件"
	@$(ECHO) "make clean_prms         只清除生成的程序"
	@$(ECHO) "make clean_all          清除所有的目标文件，导出文件"
	@$(ECHO) "make clean MODULE=红黑树 只清除“红黑树”目录"
	@$(ECHO) "make help               显示帮助信息"
	@$(ECHO) "-------------------------------------------------"

makefile %.mak:;
# --------------------- end of environment.mak ----------------------------
# 有水的地方就有余 http://hi.baidu.com/20065562
#
