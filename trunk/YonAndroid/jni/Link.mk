LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

subdirs := $(LOCAL_PATH)/Android.mk
ifeq ($(APP),Logger)
subdirs += $(LOCAL_PATH)/AndroidLogger.mk
else
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
else
ifeq ($(APP),RTTPartial)
subdirs += $(LOCAL_PATH)/AndroidRTTPartial.mk
else
ifeq ($(APP),CompressKTX)
subdirs += $(LOCAL_PATH)/AndroidCompressKTX.mk
else
ifeq ($(APP),Sound)
subdirs += $(LOCAL_PATH)/AndroidSound.mk
else
ifeq ($(APP),GUIRTT)
subdirs += $(LOCAL_PATH)/AndroidGUIRTT.mk
else
ifeq ($(APP),XC3D)
subdirs += $(LOCAL_PATH)/AndroidXC3D.mk
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif

include $(subdirs)