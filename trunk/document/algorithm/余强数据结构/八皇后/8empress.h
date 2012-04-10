/************************************************************************************

  文件： C语言八皇后 源码及注解
  作者： 余   强
  日期： 2009-11-3
  协议： Copyleft (C) GPL
 E-mail: yuembed@126.com
  空间博客 『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562

************************************************************************************/
#include <stdio.h>

#define NUM 8

/* 所需数据结构 */
typedef struct {
	int row; /* 行 */
	int col; /* 列 */
}POSITION;

/* 是否占用标记 */
typedef enum {
	notoccued = 0, /* 未占用 */
	occued = 1     /* 占用 */
}ISOCCUED;

/* 标记某列是否被占用 */
ISOCCUED col_occu[NUM];

/* 左上－>右下 是否被占用 */
ISOCCUED lu_rd[2*NUM -1];

/* 左下－>右上 是否被占用 */
ISOCCUED ld_ru[2*NUM -1];

/* 答案记录 */
POSITION answer[NUM];

/* 文件来源：『20065562's Blog』有水的地方就有余 http://hi.baidu.com/20065562 */


