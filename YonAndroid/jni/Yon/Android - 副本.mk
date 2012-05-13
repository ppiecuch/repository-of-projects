LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL \
                -O3 \
                -Z_PREFIX \
                -DAL_BUILD_LIBRARY \
                -DAL_ALEXT_PROTOTYPES \
                -DGL_GLEXT_PROTOTYPES \
                -DFT2_BUILD_LIBRARY \
                -DMYGUI_USE_FREETYPE

LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/zlib \
		$(LOCAL_PATH)/lpng \
		$(LOCAL_PATH)/openal/include \
		$(LOCAL_PATH)/openal/include/AL \
		$(LOCAL_PATH)/openal/OpenAL32/Include \
		$(LOCAL_PATH)/libogg \
		$(LOCAL_PATH)/libvorbis \
		$(LOCAL_PATH)/freetype/include \
		$(LOCAL_PATH)/MyGUIEngine/include \
		$(LOCAL_PATH)/../include/ \
		$(LOCAL_PATH)/ 
		

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
		MyGUIDataManager.cpp \
		COGLES1MyGUIEnvirenment.cpp \
		COGLES1MyGUIRenderManager.cpp \
		COGLES1MyGUITexture.cpp \
		COGLES1MyGUIVertexBuffer.cpp \
		yon.cpp
		
LOCAL_MODULE    := core

LOCAL_STATIC_LIBRARIES :=zlib lpng openal libogg libvorbis freetype mygui
		
LOCAL_SRC_FILES := \
		$(yon_SOURCES)
		
		
LOCAL_ARM_MODE   := arm 
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lstdc++ -Wl
include $(BUILD_STATIC_LIBRARY)
include $(call all-makefiles-under,$(LOCAL_PATH))

