LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -DANDROID_NDK \
                -O3

LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/../libogg \
		$(LOCAL_PATH)/ 
		
vorbis_SOURCES :=\
		analysis.c \
		bitrate.c \
		block.c \
		codebook.c \
		envelope.c \
		floor0.c \
		floor1.c \
		info.c \
		lookup.c \
		lpc.c \
		lsp.c \
		mapping0.c \
		mdct.c \
		psy.c \
		registry.c \
		res0.c \
		sharedbook.c \
		smallft.c \
		synthesis.c \
		vorbisenc.c \
		vorbisfile.c \
		window.c 

LOCAL_MODULE    := libvorbis
		
LOCAL_SRC_FILES := \
		$(vorbis_SOURCES)
			
LOCAL_ARM_MODE   := arm 
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lstdc++ -Wl
include $(BUILD_STATIC_LIBRARY)

