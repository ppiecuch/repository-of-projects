LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL \
                -O3 \
                -DMYGUI_USE_FREETYPE \
                -DFT2_BUILD_LIBRARY \
                -DMYGUI_USE_FREETYPE

LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/freetype/include \
		$(LOCAL_PATH)/MyGUIEngine/include \
		$(LOCAL_PATH)/MyGUIAdapter \
		$(LOCAL_PATH)/include/ \
		$(LOCAL_PATH)/ 
		
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

LOCAL_SRC_FILES := \
		$(MyGUIEngine_SOURCES) \
		$(myguiadapter_SOURCES) \
		YonExample/framework.cpp \
		YonExample/yon_AndroidGLView.cpp
		
LOCAL_STATIC_LIBRARIES := freetype engine
LOCAL_LDFLAGS := \
	$(LOCAL_PATH)/libengine.a \
	$(LOCAL_PATH)/libfreetype.a \
	$(LOCAL_PATH)/libmyguiadapter.a
	
LOCAL_MODULE    := yon
		
#LOCAL_CPP_FEATURES := rtti 
LOCAL_ARM_MODE   := arm 
#LOCAL_ARM_MODE   := thumb 
#LOCAL_LDLIBS := -L$(LOCAL_PATH) -lmygui -lcore -lfreetype -lGLESv1_CM -ldl -llog -Wl -lz -lstdc++
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -Wl -lz -lstdc++
#LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -Wl
include $(BUILD_SHARED_LIBRARY)
#include $(call all-makefiles-under,$(LOCAL_PATH)) 
