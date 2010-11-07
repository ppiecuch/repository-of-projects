LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := androidgl

#-Wno-psabi to remove warning about GCC 4.4 va_list warning
LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL 
                
LOCAL_DEFAULT_CPP_EXTENSION := cpp 
                
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../libzip/ $(LOCAL_PATH)/../libpng/

LOCAL_STATIC_LIBRARIES := libzip libpng

LOCAL_SRC_FILES := \
	math/point3c.cpp \
	math/point3s.cpp \
	math/point3f.cpp \
	math/point2f.cpp \
	math/point3d.cpp \
	math/vector3f.cpp \
	math/sphere.cpp \
	math/cuboid.cpp \
	util.cpp \
    androidgl05.cpp \

LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz -lstdc++

include $(BUILD_SHARED_LIBRARY)
