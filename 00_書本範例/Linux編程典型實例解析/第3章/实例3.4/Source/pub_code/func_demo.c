/*
	SQL��ѯ����﷨������Flex��Bison���������ܲ�������
	1 ��ѯ����һ��
	2 ��ѯ�ֶ��滻ΪROWID
	3 ��ѯ�ֶ��滻Ϊ����Ϸ���ʾ���磺count(*)
	4 Ϊ��ѯ������ʱ������
	5 ��ȡ��ѯ����ʱ������
	
	2006��7��10����	������ ����
*/
#include <stdio.h>
#include <sys/time.h>
#include "queryopt.h"
#include "speed_sqlpat.h"		//�������������
#include "speed.tab.h"			//�����������

char *readSql(FILE *fp);
int sql_unit( const char *sqlfile, FILE *log );

/*
  ���ܣ���SQLԴ�ļ��ж�ȡÿ�ζ�ȡһ�����
  ���룺FILE*ָ��
  �����char*ָ�룬ָ��洢һ��SQL�����ַ���
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
	���ܣ�������������ʽ��һ����Ч����ͬ��SQL���ת����Ϊͳһ��ʽ
	���룺�洢���������ļ����ơ���־�ļ�FILE*log
	�������Ļ��ӡ����־�ļ�
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
		printf("��һ��������������ļ�������\n");
		fprintf( log, "��һ��������������ļ�������\n");
		return -1;
	}
	for( i=0; i<5; i++ )
	{
		s[i]=NULL;
		tmp = readSql(fp);
		if(tmp==NULL) 
			break;
		s[i]=myParse(tmp);			//�������з���
		printf("NO %d: %s\n", i, tmp );
		fprintf( log, "NO %d: %s\n", i, tmp );
		if( s[i] )
		{
			printf("--- %s\n\n", s[i]->sp_usqlstatement );
			fprintf( log, "--- %s\n\n", s[i]->sp_usqlstatement );
		}
		free( tmp );
	}
	printf("��Ա�%d�����ת������Ƿ�һ�£�\n", i );
//	getchar();
	
	for( --i; i>=0; --i )
		freePattern(s[i]);
		
	return 0;
}

/*
 ���ܣ����Ʋ�ѯ����ʱ������
 ���룺���Բ�ѯ����ļ�����־�ļ�FILE *log
 �������Ļ��ӡ��д����־
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
	//1-��һ��������
	printf("####1-��ѯ����һ����ʾ#####\n");
	fprintf( flog, "####1-��ѯ����һ����ʾ#####\n");
	sql_unit( "usql.txt", flog );
	printf("##### END #####\n\n");
	fprintf( flog, "##### END #####\n\n");
	
	fp = fopen( "ssql.txt", "r" );
	if( fp == NULL )
	{
		printf("open source file to read fail\n");
		return -1;
	}
	//2-��ѯ���Ĳ�ѯ�ֶ��滻ΪROWID��COUNT(*)
	sql = readSql( fp );
	if( sql == NULL )
	{
		printf("you have not prepare enough SQL clauses\n");
		return -1;
	}
	printf("####2-��ѯ���Ĳ�ѯ�ֶ��滻ΪROWID��COUNT(*)#####\n");
	fprintf( flog, "####2-��ѯ���Ĳ�ѯ�ֶ��滻ΪROWID��COUNT(*)#####\n");
	pat = myParse( sql );
	if( pat == NULL )
	{
		printf("meet uncorrect sql clause\n");
		fprintf( flog, "meet uncorrect sql clause\n");
		return -1;
	}
	tmp1 = sql_with_time( pat, "ROWID ", "", "" );
	tmp2 = sql_with_time( pat, "COUNT(*) ", "", "" );
	printf("ԭʼ��%s\n��һ����%s\n�滻ROWID��%s\n�滻COUNT(*)��%s\n",
		sql, 
		pat->sp_usqlstatement,
		tmp1, 
		tmp2 );
	fprintf( flog, "ԭʼ��%s\n��һ����%s\n�滻ROWID��%s\n�滻COUNT(*)��%s\n",
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
	
	//3-Ϊ��ѯ��䶨��ʱ������
	sql_parse_init( "time_col.conf" );
	sql = readSql( fp );
	if( sql == NULL )
	{
		printf("you have not prepare enough SQL clauses\n");
		return -1;
	}
	printf( "####3-Ϊ��ѯ��䶨��ʱ������#######\n");
	fprintf( flog, "####3-Ϊ��ѯ��䶨��ʱ������#######\n");
	pat = myParse( sql );
	if( pat == NULL )
	{
		printf("meet uncorrect sql clause\n");
		fprintf( flog, "meet uncorrect sql clause\n");
		return -1;
	}
	tmp1 = sql_with_time( pat, NULL, "2006-04-01 12:22:00", "2006-07-10 23:12:24" );
	printf( "ԭʼ��%s\n��ȡʱ���%s\n����ʱ�䣺%s\n",
		sql,
		pat->sp_usqlstatement,
		tmp1
		);
	fprintf( flog, "ԭʼ��%s\n��ȡʱ���%s\n����ʱ�䣺%s\n",
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



