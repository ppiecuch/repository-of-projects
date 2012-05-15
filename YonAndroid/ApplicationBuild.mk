APP_PROJECT_PATH := $(call my-dir)
APP_MODULES      := core freetype myguiadapter yon
APP_BUILD_SCRIPT := $(call my-dir)/jni/AndroidBuild.mk
APP_CPPFLAGS += -frtti
APP_ABI :=armeabi
APP_PLATFORM :=android-8
