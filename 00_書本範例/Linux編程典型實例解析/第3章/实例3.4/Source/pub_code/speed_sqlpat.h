#ifndef SPEED_SQLPAT_H
#define SPEED_SQLPAT_H

#define MAX_SQL_SIZE	4000
#define ADD_LEN( x, y )	if( *x + y >= MAX_SQL_SIZE ) return -1; else *x += y

extern TimeCol		*tcol;
extern int		tcol_nums;

void treeout(COMBISTRU   *rr);
void addStr( char **tmp, char *src, int *len, int *limit );
char *getConstr(COMBISTRU *r);
char *getTBfield(TBfield *r);
char *getTBRefStr(TBFS *tb);

/* provide for out use */
void freeBasic( BASICSTRU *r );
void freeCondition( COMBISTRU *r );
void freeTBfield( TBfield *r );
void freePattern(SQLPATTERN *r);

char *getQueryStr(SQLPATTERN *r);
char *jn_db_noWhereSQL( SQLPATTERN *r );
char *jn_db_querySQL( const int batsize, SQLPATTERN *r );
char *jn_db_timeRowidSQL(  SQLPATTERN *r, char *start_time, char *end_time );
char *jn_getRowidSQL( SQLPATTERN *r );
char *jn_getUniqueSQL( SQLPATTERN *r );
int sqlpat2str( SQLPATTERN *sp, char out[], int *baselen );
int sql_selectROWID( SQLPATTERN *sp, char out[], int *baselen );
int sqlField( SQLPATTERN *sp, const char *field, char out[], int *baselen );
char *sql_with_time( SQLPATTERN *sp, char *selist, char *start_time, char *end_time );
//提供读取配置时间表文件的初始化
void sql_parse_init( char *file_name );
void sql_parse_destroy( );
//暂时没有作用
int sql_parse_getTabNum( SQLPATTERN *r );

#endif/*end ifndef SPEED_SQLPAR_H */
