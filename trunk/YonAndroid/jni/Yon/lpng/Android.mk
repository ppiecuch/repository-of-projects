LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -O3 \
				-DZ_PREFIX
				
LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/../zlib \
		$(LOCAL_PATH)/ 
		
lpng_SOURCES := \
		png.c \
		pngerror.c \
		pngget.c \
		pngmem.c \
		pngpread.c \
		pngread.c \
		pngrio.c \
		pngrtran.c \
		pngrutil.c \
		pngset.c \
		pngtrans.c \
		pngwio.c \
		pngwrite.c \
		pngwtran.c \
		pngwutil.c 
		
LOCAL_MODULE    := lpng
		
LOCAL_SRC_FILES := \
		$(lpng_SOURCES)
		
LOCAL_ARM_MODE   := arm 
LOCAL_LDLIBS := -lz -ldl -llog -lstdc++ -Wl
include $(BUILD_STATIC_LIBRARY)

