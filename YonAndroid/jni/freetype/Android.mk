LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -O3 \
                -DFT2_BUILD_LIBRARY \
                -DMYGUI_USE_FREETYPE

LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/include/

freetype_SOURCES :=\
		src/autofit/autofit.c \
		src/base/ftsynth.c \
		src/base/ftstroke.c \
		src/base/ftfstype.c \
		src/base/ftbitmap.c \
		src/base/ftglyph.c \
		src/base/ftgasp.c \
		src/base/fttype1.c \
		src/base/ftinit.c \
		src/base/ftwinfnt.c \
		src/base/ftmm.c \
		src/base/ftbase.c \
		src/base/ftsystem.c \
		src/base/ftpfr.c \
		src/base/ftbbox.c \
		src/bdf/bdf.c \
		src/cache/ftcache.c \
		src/cff/cff.c \
		src/cid/type1cid.c \
		src/gzip/ftgzip.c \
		src/lzw/ftlzw.c \
		src/pcf/pcf.c \
		src/pfr/pfr.c \
		src/psaux/psaux.c \
		src/pshinter/pshinter.c \
		src/psnames/psmodule.c \
		src/raster/raster.c \
		src/sfnt/sfnt.c \
		src/smooth/smooth.c \
		src/truetype/truetype.c \
		src/type1/type1.c \
		src/type42/type42.c \
		src/winfonts/winfnt.c
		
LOCAL_MODULE    := freetype
		
LOCAL_SRC_FILES := \
		$(freetype_SOURCES)
		
		
LOCAL_ARM_MODE   := arm 
LOCAL_LDLIBS := -ldl -llog -lstdc++ -Wl
include $(BUILD_STATIC_LIBRARY)
