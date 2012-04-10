# -----------------------------------------------------------------
# 文  件： common.mak
# 作  者： 余    强
# 日  期： 2009-12-08
# 版  权： Copyleft (c) GPL
# E-mail: yuembed@126.com
# 博  客：『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562
#-------------------------------------------------------------------


include $(YDS_MAKE)/environment.mak
include $(YDS_MAKE)/tool.mak

# sepretor
EXPORT_HDRS = $(filter %.h,$(DEFAULT_EXPORTS))
EXPORT_LIBS = $(filter %$(LIB_SUFFIX),$(DEFAULT_EXPORTS))
EXPORT_PRMS = $(filter %$(PRM_SUFFIX),$(DEFAULT_EXPORTS))

IMPORT_HDRS = $(addsuffix .h,$(DEFAULT_IMPORTS))
IMPORT_LIBS = $(addprefix $(LIB_PREFIX),$(addsuffix $(LIB_SUFFIX),$(DEFAULT_IMPORTS)))

# add prefix -I -L
L_LIBRARY   = $(addprefix -L,$(LIBRARY_PATH))
I_INCLUDE   = $(addprefix -I,$(INCLUDE_PATH))

# --------------------- end of common.mak ----------------------------
# 有水的地方就有余 http://hi.baidu.com/20065562
#

