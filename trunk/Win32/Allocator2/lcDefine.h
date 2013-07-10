#ifndef _LUCID_DEFINE_H_
#define _LUCID_DEFINE_H_

#include "LCConfig.h"

//refer to:http://svn.bgmod.com/code/trunk/public/tier0/platform.h
//��������ǿ����
//refer to:http://book.51cto.com/art/200904/121029.htm

//�������ͻᰴ���¹�������ѡ�񱻶�ζ����ȫ�ַ��ţ�
//����1��������ǿ���ű���ζ��壨����ͬ��Ŀ���ļ��в�����ͬ����ǿ���ţ�������ж��ǿ���Ŷ��壬���������������ظ��������
//����2�����һ��������ĳ��Ŀ���ļ�����ǿ���ţ��������ļ��ж��������ţ���ôѡ��ǿ���š�
//����3�����һ������������Ŀ���ļ��ж��������ţ���ôѡ������ռ�ÿռ�����һ����
//����Ŀ���ļ�A����ȫ�ֱ���globalΪint�ͣ�ռ4���ֽڣ�Ŀ���ļ�B����globalΪdouble�ͣ�ռ8���ֽڣ���ôĿ���ļ�A��B���Ӻ�
//����globalռ8���ֽڣ�������Ҫʹ�ö����ͬ���͵������ţ��������׵��º��ѷ��ֵĳ�����󣩡�

#if defined(LC_OS_WINNT) || defined(LC_OS_WINRT)
#define LC_SELECTANY __declspec(selectany)
#elif defined(LC_OS_MAC) || defined(LC_OS_IOS) || defined(LC_OS_ANDROID) || defined(LC_OS_LINUX)
#define LC_SELECTANY __attribute__((weak))
#else
#define LC_SELECTANY static
#endif


#endif