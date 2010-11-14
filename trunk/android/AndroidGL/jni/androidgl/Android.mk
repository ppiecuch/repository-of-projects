LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := androidgl

#-Wno-psabi to remove warning about GCC 4.4 va_list warning
LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL 
                
LOCAL_DEFAULT_CPP_EXTENSION := cpp 
                
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../libzip/ $(LOCAL_PATH)/../libpng/ $(LOCAL_PATH)/../freetype/

LOCAL_STATIC_LIBRARIES := libzip libpng freetype

LOCAL_SRC_FILES := \
#	math/point3c.cpp \
	math/point3s.cpp \
	math/point3f.cpp \
	math/point2f.cpp \
	math/point3d.cpp \
	math/vector3f.cpp \
	math/sphere.cpp \
	math/cuboid.cpp \
	math/cuboid.cpp \
	font/AsciiFont.cpp \
	math/rectangle.cpp \

LOCAL_SRC_FILES += \
	math/point3f.cpp \
	math/point2f.cpp \
	math/point2d.cpp \
	math/point2s.cpp \
	util.cpp \
	encode/GBnUnicode.cpp \
	font/Font.cpp \
	font/UnicodePainter.cpp \
    androidgl07.cpp \

LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz -lstdc++

#LOCAL_LDLIBS+=  -finput-charset=GB2312

include $(BUILD_SHARED_LIBRARY)
