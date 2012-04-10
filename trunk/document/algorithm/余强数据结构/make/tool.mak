# -----------------------------------------------------------------
# 文  件： tool.mak
# 作  者： 余    强
# 日  期： 2009-12-09
# 版  权： Copyleft (c) GPL
# E-mail: yuembed@126.com
# 博  客：『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562
#-------------------------------------------------------------------


# set the command viriable and pre- or suf- fix
CC      = gcc
AR      = ar
LINK    = gcc
LD      = gcc
CP      = cp
MKDIR   = mkdir

INCLUDE_PRIFIX = -I
LIBRARY_PRIFIX = -L

OBJ_SUFFIX = .o
LIB_PREFIX = lib
LIB_SUFFIX = .a

MOD_PREFIX =
MOD_SUFFIX = .ko

PRM_PREFIX = my
PRM_SUFFIX = .elf


ifneq "$(CROSS_COMPILE)" ""
CC      = $(CROSS_COMPILE)gcc
AR      = $(CROSS_COMPILE)ar
LINK    = $(CROSS_COMPILE)gcc
LD      = $(CROSS_COMPILE)ld
AS	  = $(CROSS_COMPILE)as
NM	  = $(CROSS_COMPILE)nm
endif

define FIRST_LIB_IN_PATH
$(if $(wildcard $(addsuffix /$(LIB),$(LIBRARY_PATH))),\
     $(firstword $(wildcard $(addsuffix /$(LIB),$(LIBRARY_PATH)))),\
     $(error Failed to find library $(LIB) on path ($(LIBRARY_PATH))))
endef

define BUILD_LIBRARY
$(if $(wildcard $@),@$(RM) $@)
$(if $(wildcard ar.mac),@$(RM) ar.mac)
$(if $(filter lib%.a, $^),
@$(ECHO) CREATE $@ > ar.mac
@$(ECHO) SAVE >> ar.mac
@$(ECHO) END >> ar.mac
@$(AR) -M < ar.mac
)
$(if $(filter %.o,$^),@$(AR) -q $@ $(filter %.o, $^))
$(if $(filter %.a, $^),
@$(ECHO) OPEN $@ > ar.mac
$(foreach LIB, $(filter lib%.a, $^),
@echo ADDLIB $(FIRST_LIB_IN_PATH) >> ar.mac
)
@$(ECHO) SAVE >> ar.mac
@$(ECHO) END >> ar.mac
@$(AR) -M < ar.mac
@$(RM) ar.mac
)
endef

# set the debug option
ifdef DEBUG
  ifndef OPTLEVEL
    OPTLEVEL := 0
  endif
  PROC_CFLAGS += -g
else
  ifndef OPTLEVEL
    OPTLEVEL := 2
  endif
endif
OTHER_FLAGS += -O$(OPTLEVEL) $(PROC_CFLAGS)

# link program
define LINK_PROGRAM
@$(CC) $(L_LIBRARY) $(DEFAULT_LDFLAGS) $($(basename $@)_CFLAGS) $(OTHER_FLAGS) $(filter %.o,$^) $(addprefix -l,$(patsubst lib%.a,%,$(filter lib%.a,$(notdir $^)))) -o $@
endef

%.o: %.c
	@$(ECHO) [CC] $(patsubst $(YDS_BUILD_DIR)/%.c, %.c, $<) ...
	@$(CC) $(CFLAGS) $(PROC_CFLAGS) $(DEFAULT_CFLAGS) $($(basename $@)_CFLAGS) $(I_INCLUDE) -o $@ -c $< 

# --------------------- end of tool.mak ----------------------------
# 有水的地方就有余 http://hi.baidu.com/20065562
#

