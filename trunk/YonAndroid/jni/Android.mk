LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL \
                -O3 \
                -DAL_BUILD_LIBRARY \
                -DAL_ALEXT_PROTOTYPES \
                -DGL_GLEXT_PROTOTYPES \
                -DMYGUI_USE_FREETYPE \
                -DFT2_BUILD_LIBRARY \
                -DMYGUI_USE_FREETYPE \
				-D_STLP_USE_NEWALLOC

LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/freetype/include \
		$(LOCAL_PATH)/Spark/include \
		$(LOCAL_PATH)/FMOD/include \
		$(LOCAL_PATH)/MyGUIEngine/include \
		$(LOCAL_PATH)/MyGUIAdapter \
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
		
freetype_SOURCES :=\
		freetype/src/autofit/autofit.c \
		freetype/src/base/ftsynth.c \
		freetype/src/base/ftstroke.c \
		freetype/src/base/ftfstype.c \
		freetype/src/base/ftbitmap.c \
		freetype/src/base/ftglyph.c \
		freetype/src/base/ftgasp.c \
		freetype/src/base/fttype1.c \
		freetype/src/base/ftinit.c \
		freetype/src/base/ftwinfnt.c \
		freetype/src/base/ftmm.c \
		freetype/src/base/ftbase.c \
		freetype/src/base/ftsystem.c \
		freetype/src/base/ftpfr.c \
		freetype/src/base/ftbbox.c \
		freetype/src/bdf/bdf.c \
		freetype/src/cache/ftcache.c \
		freetype/src/cff/cff.c \
		freetype/src/cid/type1cid.c \
		freetype/src/gzip/ftgzip.c \
		freetype/src/lzw/ftlzw.c \
		freetype/src/pcf/pcf.c \
		freetype/src/pfr/pfr.c \
		freetype/src/psaux/psaux.c \
		freetype/src/pshinter/pshinter.c \
		freetype/src/psnames/psmodule.c \
		freetype/src/raster/raster.c \
		freetype/src/sfnt/sfnt.c \
		freetype/src/smooth/smooth.c \
		freetype/src/truetype/truetype.c \
		freetype/src/type1/type1.c \
		freetype/src/type42/type42.c \
		freetype/src/winfonts/winfnt.c
		
Spark_SOURCES :=\
		Spark/src/SPK_All.cpp \
		Spark/src/SPK_YON_ALL.cpp
		
MyGUIEngine_SOURCES :=\
		MyGUIEngine/src/MyGUI_ScrollViewBase.cpp \
		MyGUIEngine/src/MyGUI_TextureUtility.cpp \
		MyGUIEngine/src/MyGUI_Any.cpp \
		MyGUIEngine/src/MyGUI_ProgressBar.cpp \
		MyGUIEngine/src/MyGUI_PluginManager.cpp \
		MyGUIEngine/src/MyGUI_EditText.cpp \
		MyGUIEngine/src/MyGUI_UString.cpp \
		MyGUIEngine/src/MyGUI_Exception.cpp \
		MyGUIEngine/src/MyGUI_TextIterator.cpp \
		MyGUIEngine/src/MyGUI_Precompiled.cpp \
		MyGUIEngine/src/MyGUI_MultiListItem.cpp \
		MyGUIEngine/src/MyGUI_MenuControl.cpp \
		MyGUIEngine/src/MyGUI_ResourceImageSet.cpp \
		MyGUIEngine/src/MyGUI_ResourceImageSetPointer.cpp \
		MyGUIEngine/src/MyGUI_LayerManager.cpp \
		MyGUIEngine/src/MyGUI_ItemBox.cpp \
		MyGUIEngine/src/MyGUI_Window.cpp \
		MyGUIEngine/src/MyGUI_DynLibManager.cpp \
		MyGUIEngine/src/MyGUI_ResourceSkin.cpp \
		MyGUIEngine/src/MyGUI_Timer.cpp \
		MyGUIEngine/src/MyGUI_ResourceManager.cpp \
		MyGUIEngine/src/MyGUI_LayerNode.cpp \
		MyGUIEngine/src/MyGUI_MenuItem.cpp \
		MyGUIEngine/src/MyGUI_ListBox.cpp \
		MyGUIEngine/src/MyGUI_TabItem.cpp \
		MyGUIEngine/src/MyGUI_PopupMenu.cpp \
		MyGUIEngine/src/MyGUI_PolygonalSkin.cpp \
		MyGUIEngine/src/MyGUI_ScrollView.cpp \
		MyGUIEngine/src/MyGUI_DDContainer.cpp \
		MyGUIEngine/src/MyGUI_LayerItem.cpp \
		MyGUIEngine/src/MyGUI_ImageBox.cpp \
		MyGUIEngine/src/MyGUI_TextBox.cpp \
		MyGUIEngine/src/MyGUI_RenderItem.cpp \
		MyGUIEngine/src/MyGUI_EditBox.cpp \
		MyGUIEngine/src/MyGUI_SimpleText.cpp \
		MyGUIEngine/src/MyGUI_SkinManager.cpp \
		MyGUIEngine/src/MyGUI_FontManager.cpp \
		MyGUIEngine/src/MyGUI_ControllerEdgeHide.cpp \
		MyGUIEngine/src/MyGUI_SharedLayer.cpp \
		MyGUIEngine/src/MyGUI_DataManager.cpp \
		MyGUIEngine/src/MyGUI_LayoutManager.cpp \
		MyGUIEngine/src/MyGUI_ComboBox.cpp \
		MyGUIEngine/src/MyGUI_SharedLayerNode.cpp \
		MyGUIEngine/src/MyGUI_RenderManager.cpp \
		MyGUIEngine/src/MyGUI_Canvas.cpp \
		MyGUIEngine/src/MyGUI_Button.cpp \
		MyGUIEngine/src/MyGUI_LogManager.cpp \
		MyGUIEngine/src/MyGUI_TextView.cpp \
		MyGUIEngine/src/MyGUI_DynLib.cpp \
		MyGUIEngine/src/MyGUI_SkinItem.cpp \
		MyGUIEngine/src/MyGUI_Widget.cpp \
		MyGUIEngine/src/MyGUI_ChildSkinInfo.cpp \
		MyGUIEngine/src/MyGUI_ResourceLayout.cpp \
		MyGUIEngine/src/MyGUI_DataFileStream.cpp \
		MyGUIEngine/src/MyGUI_DataStream.cpp \
		MyGUIEngine/src/MyGUI_ResourceManualPointer.cpp \
		MyGUIEngine/src/MyGUI_FileLogListener.cpp \
		MyGUIEngine/src/MyGUI_OverlappedLayer.cpp \
		MyGUIEngine/src/MyGUI_Colour.cpp \
		MyGUIEngine/src/MyGUI_TabControl.cpp \
		MyGUIEngine/src/MyGUI_ControllerPosition.cpp \
		MyGUIEngine/src/MyGUI_BackwardCompatibility.cpp \
		MyGUIEngine/src/MyGUI_ControllerFadeAlpha.cpp \
		MyGUIEngine/src/MyGUI_TileRect.cpp \
		MyGUIEngine/src/MyGUI_WidgetUserData.cpp \
		MyGUIEngine/src/MyGUI_BiIndexBase.cpp \
		MyGUIEngine/src/MyGUI_LanguageManager.cpp \
		MyGUIEngine/src/MyGUI_ControllerManager.cpp \
		MyGUIEngine/src/MyGUI_WidgetManager.cpp \
		MyGUIEngine/src/MyGUI_LevelLogFilter.cpp \
		MyGUIEngine/src/MyGUI_ActionController.cpp \
		MyGUIEngine/src/MyGUI_Gui.cpp \
		MyGUIEngine/src/MyGUI_ToolTipManager.cpp \
		MyGUIEngine/src/MyGUI_SubWidgetBinding.cpp \
		MyGUIEngine/src/MyGUI_SubWidgetManager.cpp \
		MyGUIEngine/src/MyGUI_MenuBar.cpp \
		MyGUIEngine/src/MyGUI_ClipboardManager.cpp \
		MyGUIEngine/src/MyGUI_InputManager.cpp \
		MyGUIEngine/src/MyGUI_MultiListBox.cpp \
		MyGUIEngine/src/MyGUI_XmlDocument.cpp \
		MyGUIEngine/src/MyGUI_LogSource.cpp \
		MyGUIEngine/src/MyGUI_FactoryManager.cpp \
		MyGUIEngine/src/MyGUI_WidgetInput.cpp \
		MyGUIEngine/src/MyGUI_MainSkin.cpp \
		MyGUIEngine/src/MyGUI_ConsoleLogListener.cpp \
		MyGUIEngine/src/MyGUI_PointerManager.cpp \
		MyGUIEngine/src/MyGUI_ResourceManualFont.cpp \
		MyGUIEngine/src/MyGUI_Constants.cpp \
		MyGUIEngine/src/MyGUI_RotatingSkin.cpp \
		MyGUIEngine/src/MyGUI_MaskPickInfo.cpp \
		MyGUIEngine/src/MyGUI_SubSkin.cpp \
		MyGUIEngine/src/MyGUI_GeometryUtility.cpp \
		MyGUIEngine/src/MyGUI_ScrollBar.cpp \
		MyGUIEngine/src/MyGUI_ScriptSupport.cpp \
		MyGUIEngine/src/MyGUI_ResourceTrueTypeFont.cpp
		
MyGUIAdapter_SOURCES := \
		MyGUIAdapter/MyGUIAdapter.cpp \
		MyGUIAdapter/MyGUIDataManager.cpp \
		MyGUIAdapter/MyGUIRenderManager.cpp \
		MyGUIAdapter/MyGUITexture.cpp \
		MyGUIAdapter/MyGUIRTTexture.cpp \
		MyGUIAdapter/MyGUIVertexBuffer.cpp

		
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
		CFMODSound.cpp \
		CFMODAudioDriver.cpp \
		CDebugPrinter.cpp \
		CLogger.cpp \
		CProfile.cpp \
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
		CEntityLoaderDAE.cpp \
		CEntityLoaderXC3D.cpp \
		CEntityLoaderMS3D.cpp \
		CEntityLoaderY3D.cpp \
		CSkinnedEntity.cpp \
		EncodeConvertor.cpp \
		COrthoCamera.cpp \
		CPerspCamera.cpp \
		CViewFrustum.cpp \
		CWindowOrthoCamera.cpp \
		CGeometryFactory.cpp \
		CEntity.cpp \
		CSceneNode.cpp \
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
		CTriangleSelector.cpp \
		CFT2FontCache.cpp \
		CFT2FontPage.cpp \
		CFT2Library.cpp \
		CFT2TextSystem.cpp \
		CTimerUnix.cpp \
		CThreadPosix.cpp \
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
		CImageLoaderTGA.cpp \
		CImageLoaderDDS.cpp \
		CImageLoaderPKM.cpp \
		CImageLoaderKTX.cpp \
		CImageLoaderPVR.cpp \
		yon.cpp \
)
		
LOCAL_MODULE    := engine
		
LOCAL_SRC_FILES := \
		$(Spark_SOURCES) \
		$(MyGUIEngine_SOURCES) \
		$(MyGUIAdapter_SOURCES) \
		$(iconv_SOURCES) \
		$(zlib_SOURCES) \
		$(lpng_SOURCES) \
		$(openal_SOURCES) \
		$(ogg_SOURCES) \
		$(vorbis_SOURCES) \
		$(freetype_SOURCES) \
		$(yon_SOURCES)
		
		
LOCAL_ARM_MODE   := arm 
LOCAL_SHORT_COMMANDS := true
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lstdc++
include $(BUILD_STATIC_LIBRARY)

