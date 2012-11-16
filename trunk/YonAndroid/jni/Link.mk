LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

subdirs := $(LOCAL_PATH)/Android.mk
ifeq ($(APP),PNG)
subdirs += $(LOCAL_PATH)/AndroidPNG.mk
else
ifeq ($(APP),GraphicsWindow)
subdirs += $(LOCAL_PATH)/AndroidGraphicsWindow.mk
else
ifeq ($(APP),StateMachine)
subdirs += $(LOCAL_PATH)/AndroidStateMachine.mk
else
ifeq ($(APP),RTT)
subdirs += $(LOCAL_PATH)/AndroidRTT.mk
else
ifeq ($(APP),Callback)
subdirs += $(LOCAL_PATH)/AndroidCallback.mk
else
ifeq ($(APP),LayerElement)
subdirs += $(LOCAL_PATH)/AndroidLayerElement.mk
endif
endif
endif
endif
endif
endif

include $(subdirs)