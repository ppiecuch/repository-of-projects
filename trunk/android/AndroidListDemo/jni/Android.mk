LOCAL_PATH := $(call my-dir)

LOCAL_CFLAGS := -DANDROID_NDK \
                -O3
LOCAL_ARM_MODE   := arm 
LOCAL_LDLIBS := -ldl -llog -lstdc++ -Wl

include $(CLEAR_VARS)
LOCAL_MODULE    := demo00
LOCAL_SRC_FILES := \
		com_demo_list_examples_Example00.cpp
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := demo01
LOCAL_SRC_FILES := \
		com_demo_list_examples_Example01.cpp
include $(BUILD_SHARED_LIBRARY)

