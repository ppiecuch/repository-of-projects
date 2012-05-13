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
		$(LOCAL_PATH)/Yon/zlib \
		$(LOCAL_PATH)/Yon/lpng \
		$(LOCAL_PATH)/Yon/openal/include \
		$(LOCAL_PATH)/Yon/openal/include/AL \
		$(LOCAL_PATH)/Yon/openal/OpenAL32/Include \
		$(LOCAL_PATH)/Yon/libogg \
		$(LOCAL_PATH)/Yon/libvorbis \
		$(LOCAL_PATH)/Yon/freetype/include \
		$(LOCAL_PATH)/Yon/MyGUIEngine/include \
		$(LOCAL_PATH)/include/ \
		$(LOCAL_PATH)/Yon/ \
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
		
freetype_SOURCES :=\
		Yon/freetype/src/autofit/autofit.c \
		Yon/freetype/src/base/ftsynth.c \
		Yon/freetype/src/base/ftstroke.c \
		Yon/freetype/src/base/ftfstype.c \
		Yon/freetype/src/base/ftbitmap.c \
		Yon/freetype/src/base/ftglyph.c \
		Yon/freetype/src/base/ftgasp.c \
		Yon/freetype/src/base/fttype1.c \
		Yon/freetype/src/base/ftinit.c \
		Yon/freetype/src/base/ftwinfnt.c \
		Yon/freetype/src/base/ftmm.c \
		Yon/freetype/src/base/ftbase.c \
		Yon/freetype/src/base/ftsystem.c \
		Yon/freetype/src/base/ftpfr.c \
		Yon/freetype/src/base/ftbbox.c \
		Yon/freetype/src/bdf/bdf.c \
		Yon/freetype/src/cache/ftcache.c \
		Yon/freetype/src/cff/cff.c \
		Yon/freetype/src/cid/type1cid.c \
		Yon/freetype/src/gzip/ftgzip.c \
		Yon/freetype/src/lzw/ftlzw.c \
		Yon/freetype/src/pcf/pcf.c \
		Yon/freetype/src/pfr/pfr.c \
		Yon/freetype/src/psaux/psaux.c \
		Yon/freetype/src/pshinter/pshinter.c \
		Yon/freetype/src/psnames/psmodule.c \
		Yon/freetype/src/raster/raster.c \
		Yon/freetype/src/sfnt/sfnt.c \
		Yon/freetype/src/smooth/smooth.c \
		Yon/freetype/src/truetype/truetype.c \
		Yon/freetype/src/type1/type1.c \
		Yon/freetype/src/type42/type42.c \
		Yon/freetype/src/winfonts/winfnt.c
		
MyGUIEngine_SOURCES :=\
		Yon/MyGUIEngine/src/MyGUI_ScrollViewBase.cpp \
		Yon/MyGUIEngine/src/MyGUI_TextureUtility.cpp \
		Yon/MyGUIEngine/src/MyGUI_Any.cpp \
		Yon/MyGUIEngine/src/MyGUI_ProgressBar.cpp \
		Yon/MyGUIEngine/src/MyGUI_PluginManager.cpp \
		Yon/MyGUIEngine/src/MyGUI_EditText.cpp \
		Yon/MyGUIEngine/src/MyGUI_UString.cpp \
		Yon/MyGUIEngine/src/MyGUI_Exception.cpp \
		Yon/MyGUIEngine/src/MyGUI_TextIterator.cpp \
		Yon/MyGUIEngine/src/MyGUI_Precompiled.cpp \
		Yon/MyGUIEngine/src/MyGUI_MultiListItem.cpp \
		Yon/MyGUIEngine/src/MyGUI_MenuControl.cpp \
		Yon/MyGUIEngine/src/MyGUI_ResourceImageSet.cpp \
		Yon/MyGUIEngine/src/MyGUI_ResourceImageSetPointer.cpp \
		Yon/MyGUIEngine/src/MyGUI_LayerManager.cpp \
		Yon/MyGUIEngine/src/MyGUI_ItemBox.cpp \
		Yon/MyGUIEngine/src/MyGUI_Window.cpp \
		Yon/MyGUIEngine/src/MyGUI_DynLibManager.cpp \
		Yon/MyGUIEngine/src/MyGUI_ResourceSkin.cpp \
		Yon/MyGUIEngine/src/MyGUI_Timer.cpp \
		Yon/MyGUIEngine/src/MyGUI_ResourceManager.cpp \
		Yon/MyGUIEngine/src/MyGUI_LayerNode.cpp \
		Yon/MyGUIEngine/src/MyGUI_MenuItem.cpp \
		Yon/MyGUIEngine/src/MyGUI_ListBox.cpp \
		Yon/MyGUIEngine/src/MyGUI_TabItem.cpp \
		Yon/MyGUIEngine/src/MyGUI_PopupMenu.cpp \
		Yon/MyGUIEngine/src/MyGUI_PolygonalSkin.cpp \
		Yon/MyGUIEngine/src/MyGUI_ScrollView.cpp \
		Yon/MyGUIEngine/src/MyGUI_DDContainer.cpp \
		Yon/MyGUIEngine/src/MyGUI_LayerItem.cpp \
		Yon/MyGUIEngine/src/MyGUI_ImageBox.cpp \
		Yon/MyGUIEngine/src/MyGUI_TextBox.cpp \
		Yon/MyGUIEngine/src/MyGUI_RenderItem.cpp \
		Yon/MyGUIEngine/src/MyGUI_EditBox.cpp \
		Yon/MyGUIEngine/src/MyGUI_SimpleText.cpp \
		Yon/MyGUIEngine/src/MyGUI_SkinManager.cpp \
		Yon/MyGUIEngine/src/MyGUI_FontManager.cpp \
		Yon/MyGUIEngine/src/MyGUI_ControllerEdgeHide.cpp \
		Yon/MyGUIEngine/src/MyGUI_SharedLayer.cpp \
		Yon/MyGUIEngine/src/MyGUI_DataManager.cpp \
		Yon/MyGUIEngine/src/MyGUI_LayoutManager.cpp \
		Yon/MyGUIEngine/src/MyGUI_ComboBox.cpp \
		Yon/MyGUIEngine/src/MyGUI_SharedLayerNode.cpp \
		Yon/MyGUIEngine/src/MyGUI_RenderManager.cpp \
		Yon/MyGUIEngine/src/MyGUI_Canvas.cpp \
		Yon/MyGUIEngine/src/MyGUI_Button.cpp \
		Yon/MyGUIEngine/src/MyGUI_LogManager.cpp \
		Yon/MyGUIEngine/src/MyGUI_TextView.cpp \
		Yon/MyGUIEngine/src/MyGUI_DynLib.cpp \
		Yon/MyGUIEngine/src/MyGUI_SkinItem.cpp \
		Yon/MyGUIEngine/src/MyGUI_Widget.cpp \
		Yon/MyGUIEngine/src/MyGUI_ChildSkinInfo.cpp \
		Yon/MyGUIEngine/src/MyGUI_ResourceLayout.cpp \
		Yon/MyGUIEngine/src/MyGUI_DataFileStream.cpp \
		Yon/MyGUIEngine/src/MyGUI_DataStream.cpp \
		Yon/MyGUIEngine/src/MyGUI_ResourceManualPointer.cpp \
		Yon/MyGUIEngine/src/MyGUI_FileLogListener.cpp \
		Yon/MyGUIEngine/src/MyGUI_OverlappedLayer.cpp \
		Yon/MyGUIEngine/src/MyGUI_Colour.cpp \
		Yon/MyGUIEngine/src/MyGUI_TabControl.cpp \
		Yon/MyGUIEngine/src/MyGUI_ControllerPosition.cpp \
		Yon/MyGUIEngine/src/MyGUI_BackwardCompatibility.cpp \
		Yon/MyGUIEngine/src/MyGUI_ControllerFadeAlpha.cpp \
		Yon/MyGUIEngine/src/MyGUI_TileRect.cpp \
		Yon/MyGUIEngine/src/MyGUI_WidgetUserData.cpp \
		Yon/MyGUIEngine/src/MyGUI_BiIndexBase.cpp \
		Yon/MyGUIEngine/src/MyGUI_LanguageManager.cpp \
		Yon/MyGUIEngine/src/MyGUI_ControllerManager.cpp \
		Yon/MyGUIEngine/src/MyGUI_WidgetManager.cpp \
		Yon/MyGUIEngine/src/MyGUI_LevelLogFilter.cpp \
		Yon/MyGUIEngine/src/MyGUI_ActionController.cpp \
		Yon/MyGUIEngine/src/MyGUI_Gui.cpp \
		Yon/MyGUIEngine/src/MyGUI_ToolTipManager.cpp \
		Yon/MyGUIEngine/src/MyGUI_SubWidgetBinding.cpp \
		Yon/MyGUIEngine/src/MyGUI_SubWidgetManager.cpp \
		Yon/MyGUIEngine/src/MyGUI_MenuBar.cpp \
		Yon/MyGUIEngine/src/MyGUI_ClipboardManager.cpp \
		Yon/MyGUIEngine/src/MyGUI_InputManager.cpp \
		Yon/MyGUIEngine/src/MyGUI_MultiListBox.cpp \
		Yon/MyGUIEngine/src/MyGUI_XmlDocument.cpp \
		Yon/MyGUIEngine/src/MyGUI_LogSource.cpp \
		Yon/MyGUIEngine/src/MyGUI_FactoryManager.cpp \
		Yon/MyGUIEngine/src/MyGUI_WidgetInput.cpp \
		Yon/MyGUIEngine/src/MyGUI_MainSkin.cpp \
		Yon/MyGUIEngine/src/MyGUI_ConsoleLogListener.cpp \
		Yon/MyGUIEngine/src/MyGUI_PointerManager.cpp \
		Yon/MyGUIEngine/src/MyGUI_ResourceManualFont.cpp \
		Yon/MyGUIEngine/src/MyGUI_Constants.cpp \
		Yon/MyGUIEngine/src/MyGUI_RotatingSkin.cpp \
		Yon/MyGUIEngine/src/MyGUI_MaskPickInfo.cpp \
		Yon/MyGUIEngine/src/MyGUI_SubSkin.cpp \
		Yon/MyGUIEngine/src/MyGUI_GeometryUtility.cpp \
		Yon/MyGUIEngine/src/MyGUI_ScrollBar.cpp \
		Yon/MyGUIEngine/src/MyGUI_ScriptSupport.cpp \
		Yon/MyGUIEngine/src/MyGUI_ResourceTrueTypeFont.cpp
		
		

yon_SOURCES :=   \
		Yon/CWave.cpp \
		Yon/CWaveLoaderWAV.cpp \
		Yon/CWaveLoaderOGG.cpp \
		Yon/COALSound.cpp \
		Yon/COALAudioDriver.cpp \
		Yon/CDebugPrinter.cpp \
		Yon/CLogger.cpp \
		Yon/CFileSystem.cpp \
		Yon/CReadFile.cpp \
		Yon/CReadFileStream.cpp \
		Yon/CReadMemoryStream.cpp \
		Yon/COrthoCamera.cpp \
		Yon/CGeometryFactory.cpp \
		Yon/CEntity.cpp \
		Yon/CModel.cpp \
		Yon/CAnimatorFactory.cpp \
		Yon/CGraphicsAdapter.cpp \
		Yon/CSceneManager.cpp \
		Yon/CTimerAndroid.cpp \
		Yon/CFPSCounter.cpp \
		Yon/COGLES1ExtensionHandler.cpp \
		Yon/COGLES1Driver.cpp \
		Yon/COGLES1Texture.cpp \
		Yon/COGLES1FBOTexture.cpp \
		Yon/COGLES1HardwareBuffer.cpp \
		Yon/CYonEngineAndroid.cpp \
		Yon/CImage.cpp \
		Yon/CImageLoaderPNG.cpp \
		Yon/MyGUIDataManager.cpp \
		Yon/COGLES1MyGUIEnvirenment.cpp \
		Yon/COGLES1MyGUIRenderManager.cpp \
		Yon/COGLES1MyGUITexture.cpp \
		Yon/COGLES1MyGUIVertexBuffer.cpp \
		Yon/yon.cpp \
		YonGUI/framework.cpp \
		yonGUI/yon_AndroidGLView.cpp
		
LOCAL_MODULE    := yon
		
LOCAL_SRC_FILES := \
		$(zlib_SOURCES) \
		$(lpng_SOURCES) \
		$(openal_SOURCES) \
		$(ogg_SOURCES) \
		$(vorbis_SOURCES) \
		$(freetype_SOURCES) \
		$(MyGUIEngine_SOURCES) \
		$(yon_SOURCES)
		
LOCAL_CPP_FEATURES := rtti 
#LOCAL_ARM_MODE   := arm 
LOCAL_ARM_MODE   := thumb
#LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lstdc++ -Wl
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -Wl
include $(BUILD_SHARED_LIBRARY)

