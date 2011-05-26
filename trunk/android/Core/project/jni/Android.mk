LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL 

LOCAL_CPP_EXTENSION := cpp
LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/freetype/
LOCAL_MODULE    := core

LOCAL_SRC_FILES :=   \
		object3d.cpp \
#		AsciiBrush.cpp \
		
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz -lstdc++

include $(BUILD_STATIC_LIBRARY)
#include $(call all-subdir-makefiles)