LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL \
                -O3

LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/Yon/zlib \
		$(LOCAL_PATH)/Yon/lpng \
		$(LOCAL_PATH)/include/ \
		$(LOCAL_PATH)/ 
		
lpng_SOURCES := \
		Yon/lpng/png.c \
		Yon/lpng/pngerror.c \
		Yon/lpng/pngget.c \
		Yon/lpng/pngmem.c \
		Yon/lpng/pngpread.c \
		Yon/lpng/pngread.c \
		Yon/lpng/pngrio.c \
		Yon/lpng/pngrtran.c \
		Yon/lpng/pngrutil.c \
		Yon/lpng/pngset.c \
		Yon/lpng/pngtrans.c \
		Yon/lpng/pngwio.c \
		Yon/lpng/pngwrite.c \
		Yon/lpng/pngwtran.c \
		Yon/lpng/pngwutil.c 
		
zlib_SOURCES :=\
		Yon/zlib/adler32.c \
		Yon/zlib/compress.c \
		Yon/zlib/crc32.c \
		Yon/zlib/deflate.c \
		Yon/zlib/inffast.c \
		Yon/zlib/inflate.c \
		Yon/zlib/inftrees.c \
		Yon/zlib/trees.c \
		Yon/zlib/uncompr.c \
		Yon/zlib/zutil.c

yon_SOURCES :=   \
		Yon/CLogger.cpp \
		Yon/CFileSystem.cpp \
		Yon/CReadFile.cpp \
		Yon/COrthoCamera.cpp \
		Yon/CGeometryFactory.cpp \
		Yon/CModel.cpp \
		Yon/CSceneManager.cpp \
		Yon/COGLES1Driver.cpp \
		Yon/COGLES1Texture.cpp \
		Yon/CYonEngineAndroid.cpp \
		Yon/CImage.cpp \
		Yon/CImageLoaderPNG.cpp \
		Yon/yon.cpp \
		yon_AndroidGLView.cpp
		
LOCAL_MODULE    := yon
		
LOCAL_SRC_FILES := \
		$(zlib_SOURCES) \
		$(lpng_SOURCES) \
		$(yon_SOURCES)
		
		
LOCAL_ARM_MODE   := arm 
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lstdc++
include $(BUILD_SHARED_LIBRARY)

