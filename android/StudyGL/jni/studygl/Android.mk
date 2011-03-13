LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := studygl

#-Wno-psabi to remove warning about GCC 4.4 va_list warning
LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL 
                
LOCAL_DEFAULT_CPP_EXTENSION := cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../core/

LOCAL_LDFLAGS = $(LOCAL_PATH)/../core/libcore.a \

LOCAL_SRC_FILES := \
	studygl02.cpp \
#	../core/collide/aabb.cpp \
#	../util/util.cpp \
#	../util/encode/GBnUnicode.cpp \
#	../util/font/AsciiFont.cpp \
#	../util/font/Font.cpp \
#	../util/font/UnicodePainter.cpp \

LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz -lstdc++

#LOCAL_LDLIBS+=  -finput-charset=GB2312

include $(BUILD_SHARED_LIBRARY)
