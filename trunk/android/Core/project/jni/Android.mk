LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL 

LOCAL_CPP_EXTENSION := cpp
LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/libzip/ $(LOCAL_PATH)/libpng/ $(LOCAL_PATH)/freetype/ 
LOCAL_MODULE    := core

LOCAL_STATIC_LIBRARIES :=libzip libpng freetype 

LOCAL_SRC_FILES :=   \
		FontBrush.cpp \
		AndroidGLView.cpp \
#		object3d.cpp \
		
		
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz -lstdc++

#include $(BUILD_STATIC_LIBRARY)
include $(BUILD_SHARED_LIBRARY)
include $(call all-makefiles-under,$(LOCAL_PATH)) 
#include $(call all-subdir-makefiles)
