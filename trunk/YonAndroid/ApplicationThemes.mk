APP_PROJECT_PATH := $(call my-dir)
#APP_MODULES      := zlib lpng openal libogg libvorbis freetype mygui core yon
APP_MODULES      := yon
APP_BUILD_SCRIPT := $(call my-dir)/jni/AndroidThemes.mk
#APP_STL := stlport_static
APP_CPPFLAGS += -frtti
APP_ABI :=armeabi
APP_PLATFORM :=android-8
