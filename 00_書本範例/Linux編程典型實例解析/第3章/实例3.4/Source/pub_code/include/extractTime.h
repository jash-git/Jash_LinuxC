#ifndef EXTRACT_TIME_H
#define EXTRACT_TIME_H

#include <stdio.h>
#include <stdlib.h>
struct timeCol{
	char	table_name[64];		//������
	char	col_name[64];		//���б�ʾʱ���������
};
typedef struct timeCol TimeCol;
typedef struct {
	char	stime[32];		//��ʼʱ��
	char	scompare[4];
	char	etime[32];		//��ֹʱ��
	char	ecompare[4];
}QueryTime;

#endif
