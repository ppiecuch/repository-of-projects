LOCAL_PATH := $(call my-dir)

#
# FMOD Ex Shared Library
# 
include $(CLEAR_VARS)
LOCAL_MODULE            := fmodex
LOCAL_SRC_FILES         := FMOD/lib/android/libfmodex.so
LOCAL_EXPORT_C_INCLUDES := FMOD/include
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

ifeq ($(APP_OPTIM),debug)
LOCAL_CFLAGS := -DANDROID_NDK \
				-D_DEBUG_
else
LOCAL_CFLAGS := -DANDROID_NDK \
				-O3
endif

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include

LOCAL_SRC_FILES := \
		YonThreadPool/framework.cpp \
		YonThreadPool/yon_AndroidGLView.cpp
		
LOCAL_MODULE    := yon
LOCAL_STATIC_LIBRARIES := engine
LOCAL_ARM_MODE   := arm 
LOCAL_LDLIBS := -ldl -llog -lGLESv1_CM
LOCAL_SHARED_LIBRARIES := fmodex
include $(BUILD_SHARED_LIBRARY)
