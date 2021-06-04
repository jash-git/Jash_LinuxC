#ifndef SPEED_UTILITY_H
#define SPEED_UTILITY_H

void upStr(char *src);
int myStrncmp( char *s1, char *s2, int len1, int len2);

int compBasic(BASICSTRU *b1,BASICSTRU *b2);
int compCombi(COMBISTRU *r1,COMBISTRU *r2);
int compTBfield( TBfield *r1, TBfield *r2 );
int compWith(WITHSUB *w, char *as_name, SQLPATTERN *tmp );
void takeSort(COMBISTRU *r);
int countCondition(COMBISTRU *r);
//void getTimeColName( char *colname, const char *sql, TimeCol *tcol, int tcol_nums, int *tb_pos );
void getTimeColName( TimeCol *pat_tcol );
void recognizeTime(const char *src, char *des );

#endif/*end ifdef SPEED_UTILITY_H*/
