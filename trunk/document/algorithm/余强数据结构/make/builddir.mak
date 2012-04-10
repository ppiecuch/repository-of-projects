# -----------------------------------------------------------------
# 文  件： builddir.mak
# 作  者： 余    强
# 日  期： 2009-12-08
# 版  权： Copyleft (c) GPL
# E-mail: yuembed@126.com
# 博  客：『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562
#-------------------------------------------------------------------

include $(YDS_MAKE)/environment.mak

# search the makefile or Makefile
ifneq "$(wildcard makefile.mak)" ""
USE_MAKEFILE := makefile
else
USE_MAKEFILE := Makefile
endif

# compiling in object directory
define MAKE_TARGET_CLEAN
	@$(MAKE) -C $(YDS_BUILD_DIR)/$(OBJDIR_NAME) -f $(YDS_BUILD_DIR)/$(USE_MAKEFILE) IN_OBJECT_DIR=1 YDS_PARENT_DIR=$(YDS_BUILD_DIR) $@
endef

define MAKE_DEFAULT_TARGET
	@$(MAKE) -C $(YDS_BUILD_DIR)/$(OBJDIR_NAME) -f $(YDS_BUILD_DIR)/$(USE_MAKEFILE) IN_OBJECT_DIR=1 YDS_PARENT_DIR=$(YDS_BUILD_DIR)
endef

#-------------------------------------------------------------------
.PHONY:create_objs_dir

default:create_objs_dir
	$(MAKE_DEFAULT_TARGET)

lib%.a: create_objs_dir
	$(MAKE_TARGET_CLEAN)

%.elf: create_objs_dir
	$(MAKE_TARGET_CLEAN)

# create object directory
create_objs_dir:
	$(MKDIR) $(YDS_BUILD_DIR)/$(OBJDIR_NAME)

# clean rules
.PHONY:clean_all clean clean_hdrs clean_libs clean_prms

clean_all:clean clean_hdrs clean_libs clean_prms


# clean the expored files
clean_hdrs:
	@$(MAKE) IN_OBJECT_DIR=1 YDS_PARENT_DIR=$(YDS_BUILD_DIR) $@

clean_libs:
	@$(MAKE) IN_OBJECT_DIR=1 YDS_PARENT_DIR=$(YDS_BUILD_DIR) $@

clean_prms:
	@$(MAKE) IN_OBJECT_DIR=1 YDS_PARENT_DIR=$(YDS_BUILD_DIR) $@

# clean objects and directory
clean:create_objs_dir
	$(MAKE_TARGET_CLEAN)
	-$(RMDIR) $(OBJDIR_NAME)

# --------------------- end of builddir.mak ----------------------------
# 有水的地方就有余 http://hi.baidu.com/20065562
#

