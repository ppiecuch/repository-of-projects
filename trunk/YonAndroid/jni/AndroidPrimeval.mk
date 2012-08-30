LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL \
                -fexceptions \
                -O3 
LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/ 

yon_SOURCES :=   \
		Primeval/yon_AndroidGLView2.cpp
		
LOCAL_MODULE    := yon
		
LOCAL_SRC_FILES := \
		$(yon_SOURCES)
		
		
LOCAL_ARM_MODE   := arm 
LOCAL_CPPFLAGS += -fexceptions
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lstdc++ -Wl
include $(BUILD_SHARED_LIBRARY)

