LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
		
zlib_SOURCES :=\
		adler32.c \
		compress.c \
		crc32.c \
		deflate.c \
		inffast.c \
		inflate.c \
		inftrees.c \
		trees.c \
		uncompr.c \
		zutil.c
		
LOCAL_MODULE    := zlib
		
LOCAL_SRC_FILES := \
		$(zlib_SOURCES)
		
LOCAL_ARM_MODE   := arm 
LOCAL_LDLIBS := -lz -ldl -llog -lstdc++ -Wl
include $(BUILD_STATIC_LIBRARY)


