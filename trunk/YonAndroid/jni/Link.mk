LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

subdirs := $(LOCAL_PATH)/Android.mk
ifeq ($(APP),PNG)
subdirs += $(LOCAL_PATH)/AndroidPNG.mk
endif

include $(subdirs)