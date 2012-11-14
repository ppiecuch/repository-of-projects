LOCAL_PATH := $(call my-dir)
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
		YonCallback/framework.cpp \
		YonCallback/yon_AndroidGLView.cpp
		
LOCAL_MODULE    := yon
LOCAL_STATIC_LIBRARIES := engine
LOCAL_ARM_MODE   := arm 
LOCAL_LDLIBS := -ldl -llog -lGLESv1_CM
include $(BUILD_SHARED_LIBRARY)
