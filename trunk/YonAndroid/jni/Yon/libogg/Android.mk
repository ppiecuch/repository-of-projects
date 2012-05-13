LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -DANDROID_NDK \
                -O3

LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/ 

ogg_SOURCES :=\
		bitwise.c \
		framing.c
		
LOCAL_MODULE    := libogg
		
LOCAL_SRC_FILES := \
		$(ogg_SOURCES)
		
LOCAL_ARM_MODE   := arm 
LOCAL_LDLIBS := -ldl -llog -lstdc++ -Wl
include $(BUILD_STATIC_LIBRARY)

