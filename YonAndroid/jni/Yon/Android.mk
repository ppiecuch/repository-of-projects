LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL \
                -O3 \
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
		

LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL \
                -O3 \
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
		
LOCAL_SRC_FILES := \
		$(zlib_SOURCES) \
		$(lpng_SOURCES) \
		$(openal_SOURCES) \
		$(ogg_SOURCES) \
		$(vorbis_SOURCES) \
		$(freetype_SOURCES) \
		$(MyGUIEngine_SOURCES) \
		$(yon_SOURCES)
		
		
LOCAL_ARM_MODE   := arm 
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lstdc++ -Wl
include $(BUILD_STATIC_LIBRARY)
#include $(call all-makefiles-under,$(LOCAL_PATH))

