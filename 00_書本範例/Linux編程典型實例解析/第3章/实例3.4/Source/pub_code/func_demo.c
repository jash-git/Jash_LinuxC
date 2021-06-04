/*
	SQL查询语句语法分析（Flex、Bison）基本功能测试例程
	1 查询语句归一化
	2 查询字段替换为ROWID
	3 查询字段替换为任意合法表示，如：count(*)
	4 为查询语句加入时间条件
	5 抽取查询语句的时间条件
	
	2006年7月10日晚	孙兆玉 整理
*/
#include <stdio.h>
#include <sys/time.h>
#include "queryopt.h"
#include "speed_sqlpat.h"		//结果处理函数声明
#include "speed.tab.h"			//分析函数入口

char *readSql(FILE *fp);
int sql_unit( const char *sqlfile, FILE *log );

/*
  功能：从SQL源文件中读取每次读取一条语句
  输入：FILE*指针
  输出：char*指针，指向存储一条SQL语句的字符串
 */
char *readSql(FILE *fp)
{
	char *str,c;
	int linelength;

	str=NULL;
	linelength = 0;
	while ( (c = fgetc (fp)) != EOF ) {
		if (c != '\n') {
			linelength ++;
		}
		else  {
			str = (char *) malloc (sizeof (char) * ( linelength+1 ) );
			memset( str, 0, linelength+1 );
			if (str == NULL) {
				return str;
			}
			else {
				fseek (fp, -linelength-1, SEEK_CUR );
				fgets (str, linelength + 1,fp);
			}
			fseek (fp,  1, SEEK_CUR);
			linelength = 0;
			break;
		}
	}
	if ( linelength != 0) {
		str = (char *) malloc (sizeof (char) * linelength );
		memset( str, 0, linelength+1 );
		if (str == NULL) {
			return str;
		}
		else {
			fseek (fp, -linelength, SEEK_CUR );
			fgets (str, linelength + 1, fp);
		}
	}

	return str;
}

/*
	功能：将几条输入形式不一样但效果相同的SQL语句转换成为统一形式
	输入：存储输入语句的文件名称、日志文件FILE*log
	输出：屏幕打印、日志文件
 */
int sql_unit( const char *sqlfile, FILE *log )
{
	FILE *fp;
	int i;
	SQLPATTERN	*s[5];
	char *tmp;
	
	fp = fopen( sqlfile, "r" );
	if( fp == NULL )
	{
		printf("归一化测试输入语句文件不存在\n");
		fprintf( log, "归一化测试输入语句文件不存在\n");
		return -1;
	}
	for( i=0; i<5; i++ )
	{
		s[i]=NULL;
		tmp = readSql(fp);
		if(tmp==NULL) 
			break;
		s[i]=myParse(tmp);			//对语句进行分析
		printf("NO %d: %s\n", i, tmp );
		fprintf( log, "NO %d: %s\n", i, tmp );
		if( s[i] )
		{
			printf("--- %s\n\n", s[i]->sp_usqlstatement );
			fprintf( log, "--- %s\n\n", s[i]->sp_usqlstatement );
		}
		free( tmp );
	}
	printf("请对比%d条语句转换结果是否一致？\n", i );
//	getchar();
	
	for( --i; i>=0; --i )
		freePattern(s[i]);
		
	return 0;
}

/*
 功能：定制查询语句的时间条件
 输入：测试查询语句文件，日志文件FILE *log
 输出：屏幕打印，写入日志
 */

int main( )
{
	FILE *flog, *fp;
	char *sql;
	SQLPATTERN	*pat;
	char *tmp1, *tmp2;
	
	flog = fopen( "func.log", "w" );
	if( flog == NULL )
	{
		printf( "open file func.log to write fail\n");
		return -1;
	}
	//1-归一化语句测试
	printf("####1-查询语句归一化演示#####\n");
	fprintf( flog, "####1-查询语句归一化演示#####\n");
	sql_unit( "usql.txt", flog );
	printf("##### END #####\n\n");
	fprintf( flog, "##### END #####\n\n");
	
	fp = fopen( "ssql.txt", "r" );
	if( fp == NULL )
	{
		printf("open source file to read fail\n");
		return -1;
	}
	//2-查询语句的查询字段替换为ROWID，COUNT(*)
	sql = readSql( fp );
	if( sql == NULL )
	{
		printf("you have not prepare enough SQL clauses\n");
		return -1;
	}
	printf("####2-查询语句的查询字段替换为ROWID，COUNT(*)#####\n");
	fprintf( flog, "####2-查询语句的查询字段替换为ROWID，COUNT(*)#####\n");
	pat = myParse( sql );
	if( pat == NULL )
	{
		printf("meet uncorrect sql clause\n");
		fprintf( flog, "meet uncorrect sql clause\n");
		return -1;
	}
	tmp1 = sql_with_time( pat, "ROWID ", "", "" );
	tmp2 = sql_with_time( pat, "COUNT(*) ", "", "" );
	printf("原始：%s\n归一化：%s\n替换ROWID：%s\n替换COUNT(*)：%s\n",
		sql, 
		pat->sp_usqlstatement,
		tmp1, 
		tmp2 );
	fprintf( flog, "原始：%s\n归一化：%s\n替换ROWID：%s\n替换COUNT(*)：%s\n",
		sql, 
		pat->sp_usqlstatement,
		tmp1, 
		tmp2 );
	printf("##### END #####\n\n");
	fprintf( flog, "##### END #####\n\n");
	
	free( tmp1 );
	free( tmp2 );
	free( sql );
	freePattern( pat );
	
	//3-为查询语句定制时间条件
	sql_parse_init( "time_col.conf" );
	sql = readSql( fp );
	if( sql == NULL )
	{
		printf("you have not prepare enough SQL clauses\n");
		return -1;
	}
	printf( "####3-为查询语句定制时间条件#######\n");
	fprintf( flog, "####3-为查询语句定制时间条件#######\n");
	pat = myParse( sql );
	if( pat == NULL )
	{
		printf("meet uncorrect sql clause\n");
		fprintf( flog, "meet uncorrect sql clause\n");
		return -1;
	}
	tmp1 = sql_with_time( pat, NULL, "2006-04-01 12:22:00", "2006-07-10 23:12:24" );
	printf( "原始：%s\n抽取时间后：%s\n定制时间：%s\n",
		sql,
		pat->sp_usqlstatement,
		tmp1
		);
	fprintf( flog, "原始：%s\n抽取时间后：%s\n定制时间：%s\n",
		sql,
		pat->sp_usqlstatement,
		tmp1
		);
	printf("##### END #####\n\n");
	fprintf( flog, "##### END #####\n\n");
		
	fclose( fp );
	fclose( flog );
	return 0;
}



