# Copyright (C) 2012 ӯ����Ϣ�Ƽ�
# @Desc  : toLua library Makefile
# @Author: Guo Huafeng
# @Date	 : 2012-05-08
# @Revise   ������tolua�ϲ���lua����   2012-05-09
#			��lua���ж�������		   2012-05-10


LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := tolua
LOCAL_SRC_FILES := ../../src/lib/tolua_event.c \
		  ../../src/lib/tolua_is.c \
		  ../../src/lib/tolua_map.c \
		  ../../src/lib/tolua_push.c \
		  ../../src/lib/tolua_to.c
		  
		  
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include \
					$(LOCAL_PATH)/../../../lua-5.1.4/include \
					
LOCAL_LDLIBS := -L$(LOCAL_PATH)/../../../lua-5.1.4/proj.android/libs/armeabi \
				-ldl -llog -llua

#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)

#subdirs := $(LOCAL_PATH)/../../../lua-5.1.4/src/Android.mk 
#include $(subdirs)
