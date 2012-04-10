/************************************************************************************

  文件： C语言八皇后 源码及注解
  作者： 余   强
  日期： 2009-11-3
  协议： Copyleft (C) GPL
 E-mail: yuembed@126.com
  空间博客 『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562

************************************************************************************/

#include <stdio.h>
#include "8empress.h"

/* ---------------------------------------------------------------------
 函数名称： NextLine
 参    数： index -- 基于0的行索引
 返回参数： 无
 功能描述： 寻找下一行可用的位置
 -----------------------------------------------------------------------
*/
void NextLine(int index)
{
	static asrnum = 0; /* 答案个数 */
	int Y = 0; /* 列索引Y:每行中的列[0～7] */
	int P = 0;

	/* 在 行index中 按列开始遍历 */
	for(Y=0; Y<NUM; Y++){
		/* 如果列和两个斜角现都没有被占用，则占用 */
		if((col_occu[Y] == notoccued)&&\
			(lu_rd[Y - index + NUM -1] == notoccued)&&\
			(ld_ru[Y + index] == notoccued)){
			/* 占领这个位置 */
			col_occu[Y]               = occued;
			lu_rd[Y - index + NUM -1] = occued;
			ld_ru[Y + index]          = occued;

			/* 找到一个合适的位置 */
			answer[Y].row = index;  
			answer[Y].col = Y;

			/* 如果不是最后一行，继续查找下一行 */
			if(index + 1 < NUM)
				NextLine(index + 1);
			else{/* 如果是最后一行了，输出结果 */
				asrnum++; /* 找到一种答案了 */
				printf("\n The %dth answer is: ", asrnum);
				for(P=0; P<NUM; P++)
					printf("(%d, %d) ", answer[P].row + 1, answer[P].col + 1);
				if((asrnum % 10 ) == 0)
					printf("\n\n");
			}

			/* 清除指定的标志，寻找下一组 */
			col_occu[Y]               = notoccued;
			lu_rd[Y - index + NUM -1] = notoccued;
			ld_ru[Y + index]          = notoccued;
		}
	}
}

/* 文件来源：『20065562's Blog』有水的地方就有余 http://hi.baidu.com/20065562 */

