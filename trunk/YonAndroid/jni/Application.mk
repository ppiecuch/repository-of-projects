APP_PROJECT_PATH := $(call my-dir)/../
APP_MODULES      := engine yon
APP_BUILD_SCRIPT := $(call my-dir)/Link.mk
APP_STL:=gnustl_static
APP_GNUSTL_FORCE_CPP_FEATURES := exceptions rtti
APP_PLATFORM :=android-8