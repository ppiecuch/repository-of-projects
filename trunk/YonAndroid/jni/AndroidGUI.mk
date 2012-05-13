LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL \
                -O3
                
#LOCAL_CPP_EXTENSION := .c

LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/Yon/MyGUIEngine/include \
		$(LOCAL_PATH)/include/ \
		$(LOCAL_PATH)/ 

LOCAL_MODULE    := yon

#LOCAL_STATIC_LIBRARIES :=zlib lpng openal libogg libvorbis freetype mygui core
LOCAL_STATIC_LIBRARIES :=core
		
LOCAL_SRC_FILES := \
		YonGUI/framework.cpp \
		YonGUI/yon_AndroidGLView.cpp
		
#LOCAL_CPP_FEATURES := rtti 
LOCAL_ARM_MODE   := arm 
#LOCAL_ARM_MODE   := thumb
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lstdc++ -Wl -lz
#LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -Wl
include $(BUILD_SHARED_LIBRARY)
include $(call all-makefiles-under,$(LOCAL_PATH)) 
