LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL \
                -O3 \
                -DAL_BUILD_LIBRARY \
                -DAL_ALEXT_PROTOTYPES 

LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/Yon/zlib \
		$(LOCAL_PATH)/Yon/lpng \
		$(LOCAL_PATH)/Yon/openal/include \
		$(LOCAL_PATH)/Yon/openal/include/AL \
		$(LOCAL_PATH)/Yon/openal/OpenAL32/Include \
		$(LOCAL_PATH)/Yon/libogg \
		$(LOCAL_PATH)/Yon/libvorbis \
		$(LOCAL_PATH)/include/ \
		$(LOCAL_PATH)/Yon/ \
		$(LOCAL_PATH)/ 
		
lpng_SOURCES := \
		$(LOCAL_PATH)/Yon/lpng/png.c \
		$(LOCAL_PATH)/Yon/lpng/pngerror.c \
		$(LOCAL_PATH)/Yon/lpng/pngget.c \
		$(LOCAL_PATH)/Yon/lpng/pngmem.c \
		$(LOCAL_PATH)/Yon/lpng/pngpread.c \
		$(LOCAL_PATH)/Yon/lpng/pngread.c \
		$(LOCAL_PATH)/Yon/lpng/pngrio.c \
		$(LOCAL_PATH)/Yon/lpng/pngrtran.c \
		$(LOCAL_PATH)/Yon/lpng/pngrutil.c \
		$(LOCAL_PATH)/Yon/lpng/pngset.c \
		$(LOCAL_PATH)/Yon/lpng/pngtrans.c \
		$(LOCAL_PATH)/Yon/lpng/pngwio.c \
		$(LOCAL_PATH)/Yon/lpng/pngwrite.c \
		$(LOCAL_PATH)/Yon/lpng/pngwtran.c \
		$(LOCAL_PATH)/Yon/lpng/pngwutil.c 
		
zlib_SOURCES :=\
		$(LOCAL_PATH)/Yon/zlib/adler32.c \
		$(LOCAL_PATH)/Yon/zlib/compress.c \
		$(LOCAL_PATH)/Yon/zlib/crc32.c \
		$(LOCAL_PATH)/Yon/zlib/deflate.c \
		$(LOCAL_PATH)/Yon/zlib/inffast.c \
		$(LOCAL_PATH)/Yon/zlib/inflate.c \
		$(LOCAL_PATH)/Yon/zlib/inftrees.c \
		$(LOCAL_PATH)/Yon/zlib/trees.c \
		$(LOCAL_PATH)/Yon/zlib/uncompr.c \
		$(LOCAL_PATH)/Yon/zlib/zutil.c
		
openal_SOURCES :=\
		$(LOCAL_PATH)/Yon/openal/OpenAL32/alAuxEffectSlot.c \
		$(LOCAL_PATH)/Yon/openal/OpenAL32/alBuffer.c \
		$(LOCAL_PATH)/Yon/openal/OpenAL32/alDatabuffer.c \
		$(LOCAL_PATH)/Yon/openal/OpenAL32/alEffect.c \
		$(LOCAL_PATH)/Yon/openal/OpenAL32/alError.c \
		$(LOCAL_PATH)/Yon/openal/OpenAL32/alExtension.c \
		$(LOCAL_PATH)/Yon/openal/OpenAL32/alFilter.c \
		$(LOCAL_PATH)/Yon/openal/OpenAL32/alListener.c \
		$(LOCAL_PATH)/Yon/openal/OpenAL32/alSource.c \
		$(LOCAL_PATH)/Yon/openal/OpenAL32/alState.c \
		$(LOCAL_PATH)/Yon/openal/OpenAL32/alThunk.c \
		$(LOCAL_PATH)/Yon/openal/Alc/ALc.c \
		$(LOCAL_PATH)/Yon/openal/Alc/alcConfig.c \
		$(LOCAL_PATH)/Yon/openal/Alc/alcEcho.c \
		$(LOCAL_PATH)/Yon/openal/Alc/alcModulator.c \
		$(LOCAL_PATH)/Yon/openal/Alc/alcReverb.c \
		$(LOCAL_PATH)/Yon/openal/Alc/alcRing.c \
		$(LOCAL_PATH)/Yon/openal/Alc/alcThread.c \
		$(LOCAL_PATH)/Yon/openal/Alc/ALu.c \
		$(LOCAL_PATH)/Yon/openal/Alc/android.c \
		$(LOCAL_PATH)/Yon/openal/Alc/bs2b.c \
		$(LOCAL_PATH)/Yon/openal/Alc/null.c 
		
		

ogg_SOURCES :=\
		$(LOCAL_PATH)/Yon/libogg/bitwise.c \
		$(LOCAL_PATH)/Yon/libogg/framing.c
		
vorbis_SOURCES :=\
		$(LOCAL_PATH)/Yon/libvorbis/analysis.c \
		$(LOCAL_PATH)/Yon/libvorbis/bitrate.c \
		$(LOCAL_PATH)/Yon/libvorbis/block.c \
		$(LOCAL_PATH)/Yon/libvorbis/codebook.c \
		$(LOCAL_PATH)/Yon/libvorbis/envelope.c \
		$(LOCAL_PATH)/Yon/libvorbis/floor0.c \
		$(LOCAL_PATH)/Yon/libvorbis/floor1.c \
		$(LOCAL_PATH)/Yon/libvorbis/info.c \
		$(LOCAL_PATH)/Yon/libvorbis/lookup.c \
		$(LOCAL_PATH)/Yon/libvorbis/lpc.c \
		$(LOCAL_PATH)/Yon/libvorbis/lsp.c \
		$(LOCAL_PATH)/Yon/libvorbis/mapping0.c \
		$(LOCAL_PATH)/Yon/libvorbis/mdct.c \
		$(LOCAL_PATH)/Yon/libvorbis/psy.c \
		$(LOCAL_PATH)/Yon/libvorbis/registry.c \
		$(LOCAL_PATH)/Yon/libvorbis/res0.c \
		$(LOCAL_PATH)/Yon/libvorbis/sharedbook.c \
		$(LOCAL_PATH)/Yon/libvorbis/smallft.c \
		$(LOCAL_PATH)/Yon/libvorbis/synthesis.c \
		$(LOCAL_PATH)/Yon/libvorbis/vorbisenc.c \
		$(LOCAL_PATH)/Yon/libvorbis/vorbisfile.c \
		$(LOCAL_PATH)/Yon/libvorbis/window.c 

yon_SOURCES :=   \
		$(LOCAL_PATH)/Yon/CWave.cpp \
		$(LOCAL_PATH)/Yon/CWaveLoaderWAV.cpp \
		$(LOCAL_PATH)/Yon/CWaveLoaderOGG.cpp \
		$(LOCAL_PATH)/Yon/COALSound.cpp \
		$(LOCAL_PATH)/Yon/COALAudioDriver.cpp \
		$(LOCAL_PATH)/Yon/CDebugPrinter.cpp \
		$(LOCAL_PATH)/Yon/CLogger.cpp \
		$(LOCAL_PATH)/Yon/CFileSystem.cpp \
		$(LOCAL_PATH)/Yon/CReadFile.cpp \
		$(LOCAL_PATH)/Yon/CReadFileStream.cpp \
		$(LOCAL_PATH)/Yon/CReadMemoryStream.cpp \
		$(LOCAL_PATH)/Yon/COrthoCamera.cpp \
		$(LOCAL_PATH)/Yon/CGeometryFactory.cpp \
		$(LOCAL_PATH)/Yon/CEntity.cpp \
		$(LOCAL_PATH)/Yon/CModel.cpp \
		$(LOCAL_PATH)/Yon/CAnimatorFactory.cpp \
		$(LOCAL_PATH)/Yon/CGraphicsAdapter.cpp \
		$(LOCAL_PATH)/Yon/CSceneManager.cpp \
		$(LOCAL_PATH)/Yon/CTimerAndroid.cpp \
		$(LOCAL_PATH)/Yon/CFPSCounter.cpp \
		$(LOCAL_PATH)/Yon/COGLES1Driver.cpp \
		$(LOCAL_PATH)/Yon/COGLES1Texture.cpp \
		$(LOCAL_PATH)/Yon/COGLES1HardwareBuffer.cpp \
		$(LOCAL_PATH)/Yon/CYonEngineAndroid.cpp \
		$(LOCAL_PATH)/Yon/CImage.cpp \
		$(LOCAL_PATH)/Yon/CImageLoaderPNG.cpp \
		$(LOCAL_PATH)/Yon/yon.cpp \
		$(LOCAL_PATH)/YonExample/framework.cpp \
		$(LOCAL_PATH)/yon_AndroidGLView.cpp
		
LOCAL_MODULE    := yon
		
LOCAL_SRC_FILES := \
		$(openal_SOURCES) \
		$(ogg_SOURCES) \
		$(zlib_SOURCES) \
		$(lpng_SOURCES) \
		$(vorbis_SOURCES) \
		$(yon_SOURCES)
		
		
LOCAL_ARM_MODE   := arm 
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lstdc++ -Wl
include $(BUILD_SHARED_LIBRARY)

