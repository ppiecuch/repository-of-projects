LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL \
                -O3 \
                -DAL_BUILD_LIBRARY \
                -DAL_ALEXT_PROTOTYPES \
                -DGL_GLEXT_PROTOTYPES

LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/Yon/zlib \
		$(LOCAL_PATH)/Yon/lpng \
		$(LOCAL_PATH)/Yon/openal/include \
		$(LOCAL_PATH)/Yon/openal/include/AL \
		$(LOCAL_PATH)/Yon/openal/OpenAL32/Include \
		$(LOCAL_PATH)/Yon/libogg \
		$(LOCAL_PATH)/Yon/libvorbis \
		$(LOCAL_PATH)/Yon/libiconv/include \
		$(LOCAL_PATH)/Yon/libiconv/src \
		$(LOCAL_PATH)/include/ \
		$(LOCAL_PATH)/Yon/ \
		$(LOCAL_PATH)/ 
		
		
iconv_SOURCES := \
		Yon/libiconv/src/iconv.c \
		Yon/libiconv/src/relocatable.c \
		Yon/libiconv/src/localcharset.c
		
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
		
openal_SOURCES :=\
		Yon/openal/OpenAL32/alAuxEffectSlot.c \
		Yon/openal/OpenAL32/alBuffer.c \
		Yon/openal/OpenAL32/alDatabuffer.c \
		Yon/openal/OpenAL32/alEffect.c \
		Yon/openal/OpenAL32/alError.c \
		Yon/openal/OpenAL32/alExtension.c \
		Yon/openal/OpenAL32/alFilter.c \
		Yon/openal/OpenAL32/alListener.c \
		Yon/openal/OpenAL32/alSource.c \
		Yon/openal/OpenAL32/alState.c \
		Yon/openal/OpenAL32/alThunk.c \
		Yon/openal/Alc/ALc.c \
		Yon/openal/Alc/alcConfig.c \
		Yon/openal/Alc/alcEcho.c \
		Yon/openal/Alc/alcModulator.c \
		Yon/openal/Alc/alcReverb.c \
		Yon/openal/Alc/alcRing.c \
		Yon/openal/Alc/alcThread.c \
		Yon/openal/Alc/ALu.c \
		Yon/openal/Alc/android.c \
		Yon/openal/Alc/bs2b.c \
		Yon/openal/Alc/null.c 
		
		

ogg_SOURCES :=\
		Yon/libogg/bitwise.c \
		Yon/libogg/framing.c
		
vorbis_SOURCES :=\
		Yon/libvorbis/analysis.c \
		Yon/libvorbis/bitrate.c \
		Yon/libvorbis/block.c \
		Yon/libvorbis/codebook.c \
		Yon/libvorbis/envelope.c \
		Yon/libvorbis/floor0.c \
		Yon/libvorbis/floor1.c \
		Yon/libvorbis/info.c \
		Yon/libvorbis/lookup.c \
		Yon/libvorbis/lpc.c \
		Yon/libvorbis/lsp.c \
		Yon/libvorbis/mapping0.c \
		Yon/libvorbis/mdct.c \
		Yon/libvorbis/psy.c \
		Yon/libvorbis/registry.c \
		Yon/libvorbis/res0.c \
		Yon/libvorbis/sharedbook.c \
		Yon/libvorbis/smallft.c \
		Yon/libvorbis/synthesis.c \
		Yon/libvorbis/vorbisenc.c \
		Yon/libvorbis/vorbisfile.c \
		Yon/libvorbis/window.c 

yon_SOURCES :=   \
	$(addprefix Yon/, \
		CRandomizer.cpp \
		CWave.cpp \
		CWaveLoaderWAV.cpp \
		CWaveLoaderOGG.cpp \
		COALSound.cpp \
		COALAudioDriver.cpp \
		CDebugPrinter.cpp \
		CLogger.cpp \
		CFileSystem.cpp \
		CReadFileStream.cpp \
		CWriteFileStream.cpp \
		CReadMemoryStream.cpp \
		CI18NManager.cpp \
		CNetManager.cpp \
		CSocket.cpp \
		NetCode.cpp \
		CAnimatedSceneNode.cpp \
		CBoneSceneNode.cpp \
		CEntityLoaderXC3D.cpp \
		CSkinnedEntity.cpp \
		EncodeConvertor.cpp \
		COrthoCamera.cpp \
		CPerspCamera.cpp \
		CViewFrustum.cpp \
		CWindowOrthoCamera.cpp \
		CGeometryFactory.cpp \
		CEntity.cpp \
		CModel.cpp \
		CSkyBox.cpp \
		CGeomipmapTerrain.cpp \
		CGeomipmapTerrain2.cpp \
		CQuadtreeTerrain.cpp \
		CROAMTerrain.cpp \
		CWaterModel.cpp \
		CAnimatorFactory.cpp \
		CAnimatorCameraFPS.cpp \
		CGraphicsAdapter.cpp \
		CGraphicsAdapterWindow.cpp \
		CSceneManager.cpp \
		CTimerUnix.cpp \
		CFPSCounter.cpp \
		CColorConverter.cpp \
		COGLES1ExtensionHandler.cpp \
		COGLES1Driver.cpp \
		COGLES1Texture.cpp \
		COGLES1FBOTexture.cpp \
		COGLES1HardwareBuffer.cpp \
		CYonEngineAndroid.cpp \
		CImage.cpp \
		CImageLoaderPNG.cpp \
		CImageLoaderDDS.cpp \
		CImageLoaderPKM.cpp \
		CImageLoaderKTX.cpp \
		CImageLoaderPVR.cpp \
		yon.cpp \
)
		
LOCAL_MODULE    := engine
		
LOCAL_SRC_FILES := \
		$(iconv_SOURCES) \
		$(zlib_SOURCES) \
		$(lpng_SOURCES) \
		$(openal_SOURCES) \
		$(ogg_SOURCES) \
		$(vorbis_SOURCES) \
		$(yon_SOURCES)
		
		
LOCAL_ARM_MODE   := arm 
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog
include $(BUILD_STATIC_LIBRARY)

