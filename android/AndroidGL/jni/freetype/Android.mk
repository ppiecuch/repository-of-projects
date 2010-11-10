# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.cpprg/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := freetype

LOCAL_SRC_FILES := src/base/ftsystem.c \
      src/base/ftinit.c \
      src/base/ftdebug.c \
      src/base/ftbase.c \
      src/base/ftbbox.c \
      src/base/ftglyph.c \
      src/base/ftbitmap.c \
#      src/base/ftbdf.c \
      src/base/ftcid.c \
      src/base/ftfstype.c \
      src/base/ftgasp.c \
      src/base/ftgxval.c \
      src/base/ftlcdfil.c \
      src/base/ftmm.c \
      src/base/ftotval.c \
      src/base/ftpatent.c \
      src/base/ftpfr.c \
      src/base/ftstroke.c \
      src/base/ftsynth.c \
      src/base/fttype1.c \
      src/base/ftwinfnt.c \
      src/base/ftxf86.c \
      src/base/ftmac.c

LOCAL_SRC_FILES += src/truetype/truetype.c \
      src/sfnt/sfnt.c \
#      src/cff/cff.c \
      src/cid/type1cid.c \
      src/pcf/pcf.c \
      src/pfr/pfr.c \
      src/bdf/bdf.c \
      src/type1/type1.c \
      src/type42/type42.c \
      src/winfonts/winfnt.c \

#    -- rasterizers (optional; at least one is needed for vector formats)

LOCAL_SRC_FILES += src/raster/raster.c \
      src/smooth/smooth.c

#    -- auxiliary modules (optional)

LOCAL_SRC_FILES += src/autofit/autofit.c \
      src/gxvalid/gxvalid.c \
      src/otvalid/otvalid.c \
      src/psaux/psaux.c \
      src/pshinter/pshinter.c \
      src/psnames/psnames.c \
#      src/cache/ftcache.c \
      src/gzip/ftgzip.c \
      src/lzw/ftlzw.c \


LOCAL_CFLAGS    :=  \
 -Isources/freetype/src/autofit/ \
 -Isources/freetype/src/base/ \
 -Isources/freetype/src/bdf/ \
 -Isources/freetype/src/cff/ \
 -Isources/freetype/src/cid/ \
 -Isources/freetype/src/gxvalid/ \
 -Isources/freetype/src/gzip/ \
 -Isources/freetype/src/lzw/ \
 -Isources/freetype/src/otvalid/ \
 -Isources/freetype/src/pcf/ \
 -Isources/freetype/src/pfr/ \
 -Isources/freetype/src/psaux/ \
 -Isources/freetype/src/pshinter/ \
 -Isources/freetype/src/psnames/ \
 -Isources/freetype/src/raster/ \
 -Isources/freetype/src/sfnt/ \
 -Isources/freetype/src/smooth/ \
 -Isources/freetype/src/tools/ \
 -Isources/freetype/src/tools/docmaker/ \
 -Isources/freetype/src/tools/ftrandom/ \
 -Isources/freetype/src/truetype/ \
 -Isources/freetype/src/type1/ \
 -Isources/freetype/src/type42/ \
 -Isources/freetype/src/winfonts/ \
 -Isources/freetype/include \
 -Isources/freetype/include/freetype/ \
 -Isources/freetype/include/freetype/config/ \
 -Isources/freetype/include/freetype/internal/ \
 -Isources/freetype/include/freetype/internal/services/ 
 
 
LOCAL_CFLAGS  += -D__LINUX_CPL__ \
     -DUNICODE \
     -DFT2_BUILD_LIBRARY \
#     -D__NEW__ \
#     -D__SGI_STL_INTERNAL_PAIR_H \
#     -D_MBSTATE_T \
#     -DANDROID \
#     -DOS_ANDROID \
#     -DTN_PLATFORM_ANDROID \
#     -DSERVICEPROXY_DEBUG 
#     -DFT_OPTION_AUTOFIT2 \
#     -DFT_CONFIG_OPTION_SYSTEM_ZLIB \
#     -DFT_CONFIG_CONFIG_H="<ftconfig.h>" \

include $(BUILD_STATIC_LIBRARY)