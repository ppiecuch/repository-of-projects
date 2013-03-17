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
else
ifeq ($(APP),IO)
subdirs += $(LOCAL_PATH)/AndroidIO.mk
else
ifeq ($(APP),CompressDDS)
subdirs += $(LOCAL_PATH)/AndroidCompressDDS.mk
else
ifeq ($(APP),SparkRain)
subdirs += $(LOCAL_PATH)/AndroidSparkRain.mk
else
ifeq ($(APP),Thread)
subdirs += $(LOCAL_PATH)/AndroidThread.mk
else
ifeq ($(APP),ThreadPool)
subdirs += $(LOCAL_PATH)/AndroidThreadPool.mk
else
ifeq ($(APP),Text)
subdirs += $(LOCAL_PATH)/AndroidText.mk
else
ifeq ($(APP),Logger)
subdirs += $(LOCAL_PATH)/AndroidLogger.mk
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
endif
endif
endif
endif
endif
endif
endif

include $(subdirs)