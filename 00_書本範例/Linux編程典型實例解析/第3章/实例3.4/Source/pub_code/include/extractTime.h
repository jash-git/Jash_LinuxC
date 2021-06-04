#ifndef EXTRACT_TIME_H
#define EXTRACT_TIME_H

#include <stdio.h>
#include <stdlib.h>
struct timeCol{
	char	table_name[64];		//表名称
	char	col_name[64];		//表中表示时间的列名称
};
typedef struct timeCol TimeCol;
typedef struct {
	char	stime[32];		//起始时间
	char	scompare[4];
	char	etime[32];		//终止时间
	char	ecompare[4];
}QueryTime;

#endif
