/************************************************************************************

  �ļ��� C���԰˻ʺ� Դ�뼰ע��
  ���ߣ� ��   ǿ
  ���ڣ� 2009-11-3
  Э�飺 Copyleft (C) GPL
 E-mail: yuembed@126.com
  �ռ䲩�� ��20065562's Blog�� ��ˮ�ĵط������� http://hi.baidu.com/20065562

************************************************************************************/
#include <stdio.h>

#define NUM 8

/* �������ݽṹ */
typedef struct {
	int row; /* �� */
	int col; /* �� */
}POSITION;

/* �Ƿ�ռ�ñ�� */
typedef enum {
	notoccued = 0, /* δռ�� */
	occued = 1     /* ռ�� */
}ISOCCUED;

/* ���ĳ���Ƿ�ռ�� */
ISOCCUED col_occu[NUM];

/* ���ϣ�>���� �Ƿ�ռ�� */
ISOCCUED lu_rd[2*NUM -1];

/* ���£�>���� �Ƿ�ռ�� */
ISOCCUED ld_ru[2*NUM -1];

/* �𰸼�¼ */
POSITION answer[NUM];

/* �ļ���Դ����20065562's Blog����ˮ�ĵط������� http://hi.baidu.com/20065562 */


