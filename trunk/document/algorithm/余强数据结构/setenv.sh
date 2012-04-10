#!/bin/sh
#----------------------------------------------------------------
# 文    件: C语言数据结构make系统环境配置文件（项目名称：YDS - Yu Data Structure）
# 作    者: 余   强
# 日    期: 2009-12-08
# 协    议: Copyleft (C) GPL
#  E-mail: yuembed@126.com
# 博    客:『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562
#----------------------------------------------------------------

# set the make flags
export MAKEFLAGS=--no-print-directory

# the root of project
if [ -z "$YUERROOT" ] ; then
    export YUERROOT=`pwd`
fi

# clear the environment viriable
unset YDS_ROOT
unset YDS_MAKE
unset YDS_INCLUDE
unset YDS_LIBRARY
unset YDS_PROGRAM

unset YDS_EXPORT_LIBS
unset YDS_EXPORT_HDRS
unset YDS_EXPORT_PRMS
unset CROSS_COMPILE
# Version information
echo "==================================="
echo "欢迎使用 C 语言常用数据结构源代码"
echo "当前版本号 V1.0"
echo "作  者：余   强"
echo "网  址：http://hi.baidu.com/20065562"
echo "==================================="

# export environment viriable
export YDS_ROOT=${YUERROOT}
export YDS_MAKE=${YDS_ROOT}/make
export YDS_INCLUDE=${YDS_ROOT}/include
export YDS_LIBRARY=${YDS_ROOT}/lib
export YDS_PROGRAM=${YDS_ROOT}/bin

export YDS_EXPORT_LIBS="true"
export YDS_EXPORT_HDRS="true"
export YDS_EXPORT_PRMS="true"

CROSS_COMPILE=
if [ -z "${CROSS_COMPILE}" ]; then
    export PATH=$PATH:/usr/local/arm/3.4.6/bin
fi

if [ -z "${YDS_ROOT}" ]; then
    echo "==================================="
    echo "参数错误:请这样配置环境变量:source setenv.sh"
    echo "==================================="
else
    echo ""
    echo "现在你可以编译C语言常用数据结构了！"
    echo "Good luck!"
fi


