LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL \
                -O3
LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/include/

LOCAL_CPP_EXTENSION := cpp
LOCAL_MODULE    := yon

LOCAL_SRC_FILES :=   \
		yon/CLogger.cpp \
		yon/CModel.cpp \
		yon/CGeometryFactory.cpp \
		yon/COrthoCamera.cpp \
		yon/CSceneManager.cpp \
		yon/COGLES1Driver.cpp \
		yon/CYonEngineAndroid.cpp \
		yon/yon.cpp \
		yon_AndroidGLView.cpp
		
		
LOCAL_ARM_MODE   := arm 
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz -lstdc++
include $(BUILD_SHARED_LIBRARY)

