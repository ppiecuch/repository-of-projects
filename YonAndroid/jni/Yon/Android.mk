LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL \
                -O3 \
                -DAL_BUILD_LIBRARY \
                -DAL_ALEXT_PROTOTYPES \
                -DGL_GLEXT_PROTOTYPES 

LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/zlib \
		$(LOCAL_PATH)/lpng \
		$(LOCAL_PATH)/openal/include \
		$(LOCAL_PATH)/openal/include/AL \
		$(LOCAL_PATH)/openal/OpenAL32/Include \
		$(LOCAL_PATH)/libogg \
		$(LOCAL_PATH)/libvorbis \
		$(LOCAL_PATH)/../include \
		$(LOCAL_PATH)/ 
		

		
lpng_SOURCES := \
		lpng/png.c \
		lpng/pngerror.c \
		lpng/pngget.c \
		lpng/pngmem.c \
		lpng/pngpread.c \
		lpng/pngread.c \
		lpng/pngrio.c \
		lpng/pngrtran.c \
		lpng/pngrutil.c \
		lpng/pngset.c \
		lpng/pngtrans.c \
		lpng/pngwio.c \
		lpng/pngwrite.c \
		lpng/pngwtran.c \
		lpng/pngwutil.c 
		
zlib_SOURCES :=\
		zlib/adler32.c \
		zlib/compress.c \
		zlib/crc32.c \
		zlib/deflate.c \
		zlib/inffast.c \
		zlib/inflate.c \
		zlib/inftrees.c \
		zlib/trees.c \
		zlib/uncompr.c \
		zlib/zutil.c
		
openal_SOURCES :=\
		openal/OpenAL32/alAuxEffectSlot.c \
		openal/OpenAL32/alBuffer.c \
		openal/OpenAL32/alDatabuffer.c \
		openal/OpenAL32/alEffect.c \
		openal/OpenAL32/alError.c \
		openal/OpenAL32/alExtension.c \
		openal/OpenAL32/alFilter.c \
		openal/OpenAL32/alListener.c \
		openal/OpenAL32/alSource.c \
		openal/OpenAL32/alState.c \
		openal/OpenAL32/alThunk.c \
		openal/Alc/ALc.c \
		openal/Alc/alcConfig.c \
		openal/Alc/alcEcho.c \
		openal/Alc/alcModulator.c \
		openal/Alc/alcReverb.c \
		openal/Alc/alcRing.c \
		openal/Alc/alcThread.c \
		openal/Alc/ALu.c \
		openal/Alc/android.c \
		openal/Alc/bs2b.c \
		openal/Alc/null.c 
		
		

ogg_SOURCES :=\
		libogg/bitwise.c \
		libogg/framing.c
		
vorbis_SOURCES :=\
		libvorbis/analysis.c \
		libvorbis/bitrate.c \
		libvorbis/block.c \
		libvorbis/codebook.c \
		libvorbis/envelope.c \
		libvorbis/floor0.c \
		libvorbis/floor1.c \
		libvorbis/info.c \
		libvorbis/lookup.c \
		libvorbis/lpc.c \
		libvorbis/lsp.c \
		libvorbis/mapping0.c \
		libvorbis/mdct.c \
		libvorbis/psy.c \
		libvorbis/registry.c \
		libvorbis/res0.c \
		libvorbis/sharedbook.c \
		libvorbis/smallft.c \
		libvorbis/synthesis.c \
		libvorbis/vorbisenc.c \
		libvorbis/vorbisfile.c \
		libvorbis/window.c 
		

yon_SOURCES :=   \
		CWave.cpp \
		CWaveLoaderWAV.cpp \
		CWaveLoaderOGG.cpp \
		COALSound.cpp \
		COALAudioDriver.cpp \
		CDebugPrinter.cpp \
		CLogger.cpp \
		CFileSystem.cpp \
		CReadFile.cpp \
		CReadFileStream.cpp \
		CReadMemoryStream.cpp \
		COrthoCamera.cpp \
		CGeometryFactory.cpp \
		CEntity.cpp \
		CModel.cpp \
		CAnimatorFactory.cpp \
		CGraphicsAdapter.cpp \
		CSceneManager.cpp \
		CTimerAndroid.cpp \
		CFPSCounter.cpp \
		COGLES1ExtensionHandler.cpp \
		COGLES1Driver.cpp \
		COGLES1Texture.cpp \
		COGLES1FBOTexture.cpp \
		COGLES1HardwareBuffer.cpp \
		CYonEngineAndroid.cpp \
		CImage.cpp \
		CImageLoaderPNG.cpp \
		yon.cpp
		
LOCAL_MODULE    := engine
		
LOCAL_SRC_FILES := \
		$(zlib_SOURCES) \
		$(lpng_SOURCES) \
		$(openal_SOURCES) \
		$(ogg_SOURCES) \
		$(vorbis_SOURCES) \
		$(yon_SOURCES)
		
		
LOCAL_ARM_MODE   := arm 
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lstdc++ -Wl
include $(BUILD_STATIC_LIBRARY)
#include $(call all-makefiles-under,$(LOCAL_PATH))

