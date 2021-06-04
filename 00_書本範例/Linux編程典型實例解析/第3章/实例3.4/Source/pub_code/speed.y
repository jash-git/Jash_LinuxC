%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queryopt.h"
#include "speed_utility.h"
#include "speed_sqlpat.h"

typedef struct{
	SQLPATTERN 	*query_stru;
	QueryTime	*query_time;
	TimeCol		*pat_tcol;
}MyExtraType;

#define YYLEX_PARAM  scanner
#define YYPARSE_PARAM scanner

#define	QUERY_STRU	((MyExtraType *)yyget_extra( scanner ))->query_stru
#define	QUERY_TIME	((MyExtraType *)yyget_extra( scanner ))->query_time
#define	PAT_TCOL	((MyExtraType *)yyget_extra( scanner ))->pat_tcol

SQLPATTERN *myParse(const char *sql );
%}
%union	{
	char		*u_str;
	WITHSTRU	*u_withsub;
	SQLPATTERN	*u_qs;
	STRBUF		*u_slist;
	SELECTLIST 	*u_selfield;
	TBFS		*u_tb;
	TBfield		*u_tfield;
	TBatom		*u_tatom;
	JoinTB		*u_join;
	QueryTB		*u_qtb;
	BASICSTRU   	*u_basic;
	COMBISTRU   	*u_combi;
	EXPRTREE	*u_etree;
	HIERARCH	*u_connect;
	GroupField	*u_gr;
	OrderCla	*u_oc;
	OrderField	*u_of;
};

%pure_parser

%start oraselect
%type  <u_str> atom col_ref sign expr as_alias  flashback_query sub_restrict part_sub sample_cla expr_min expr_max
%type  <u_str> outer_type collist gfield rollup_cube_cla grouping_expr_list expr_list grouping_sets_cla otail2 otail3
%type  <u_str> for_update up_of_col opt_escape atom_commalist any_all_some funclist
%type  <u_qs>	oraselect subquery
%type  <u_withsub> sub_factor head_query
%type  <u_slist> list_atom
%type  <u_selfield> select_list sub_list

%type  <u_tb>  table_ref
%type  <u_tfield>	tb_ref
%type  <u_tatom>	tb_atom
%type  <u_join> join_cla
%type  <u_qtb>  query_tb_expr

%type  <u_combi> condition
%type  <u_etree>  where_cla  having_cla
%type  <u_basic> predicate comparison_predicate between_predicate like_predicate  in_predicate test_for_null all_or_any_predicate existence_test  contain_key
%type  <u_connect> hierarch_query
%type  <u_gr> group_cla gfields
%type  <u_oc> order_cla
%type  <u_of> order_tail

%token <u_str> NAME AMMSC INTNUM STRING APPROXNUM  /*define myself*/
%token SELECT UNIQUE DISTINCT SUBPARTITION BLOCK SEED ON USING INNER CROSS NATURAL LEFT RIGHT OUTER LIKE ESCAPE FULL
%token FOR UPDATE OF NOWAIT AS TIMESTAMP MINVALUE MAXVALUE READ ONLY TABLE /*sql statement*/
%token WITH CHECK OPTION CONSTRAINT VERSIONS SCN  BETWEEN AND IS NOT NULLX NULLS ASC DESC FIRST LAST
%token ORDER SIBLINGS ALL ANY SOME EXISTS NOCYCLE START HAVING CUBE  IN BY JOIN PARTITION_BY
%token FROM PARTITION SAMPLE WHERE CONNECT GROUP ROLLUP GROUPING_SETS
%token CONTAINS
%left OR
%left AND
%left NOT
%left <u_str> COMPARISON /* = <> < > <= >= */
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%%

oraselect:
		subquery
		for_update
		{
			$1->sp_updateclause=$2;
			/* $1->sp_usqlstatement=getQueryStr($$); */
			QUERY_STRU=$1;
		}
	;
subquery:
		sub_factor
		SELECT  select_list
		FROM table_ref
		where_cla
		hierarch_query
		order_cla
		{
			SQLPATTERN *tmp;

			tmp=(SQLPATTERN *)FB_MALLOC(sizeof(SQLPATTERN));
			memset( tmp, 0, sizeof(SQLPATTERN) );
			tmp->sp_with=$1;
			tmp->sp_selectlist=$3;
			tmp->sp_fromlist=$5;
			tmp->sp_whereclause=$6;
			tmp->sp_connectclause=$7;
			tmp->sp_orderbyclause=$8;
			//tmp->sp_updateclause=NULL;
			//tmp->sp_groupbyclause=NULL;
			//tmp->sp_havingclause=NULL;
			$$=tmp;
			//$$->sp_osqlstatement=NULL;
			//$$->sp_usqlstatement=NULL;
		}
	|	sub_factor
		SELECT  select_list
		FROM table_ref
		where_cla
		hierarch_query
		group_cla  having_cla
		order_cla
		{
			SQLPATTERN *tmp;

			tmp=(SQLPATTERN *)FB_MALLOC(sizeof(SQLPATTERN));
			memset( tmp, 0, sizeof(SQLPATTERN) );
			tmp->sp_with=$1;
			tmp->sp_selectlist=$3;
			tmp->sp_fromlist=$5;
			tmp->sp_whereclause=$6;
			tmp->sp_connectclause=$7;
			tmp->sp_groupbyclause=$8;
			tmp->sp_havingclause=$9;
			tmp->sp_orderbyclause=$10;
			//tmp->sp_updateclause= NULL;
			$$=tmp;
			//$$->sp_osqlstatement=NULL;
			//$$->sp_usqlstatement=NULL;
		}
	;
/**1.with sub;**/
sub_factor:
	  	/*empty*/
	  	{	$$=NULL;	  }
	|	WITH head_query
	  	{
	  		$$=$2;
	  	}
	;
head_query:
	  	NAME AS '(' subquery ')'
	  	{
	  		WITHSTRU *wsub;
	  		WITHSUB *tmp;

	  		tmp=(WITHSUB *)FB_MALLOC(sizeof(WITHSUB));
	  		upStr($1);/*convert to upcase,'a'->'A'*/
	  		strcpy(tmp->ws_id,$1);
	  		tmp->ws_subquery=$4;

	  		wsub=(WITHSTRU *)FB_MALLOC(sizeof(WITHSTRU));
	  		wsub->w_count=1;
	  		wsub->w_subArray=tmp;

	  		FB_FREE($1);
	  		$$=wsub;
	  	}
	| 	head_query ',' NAME AS '(' subquery ')'
	  	{
	  		int i;
	  		WITHSUB *tmp;

	  		$1->w_count++;
	  		tmp=(WITHSUB *)FB_MALLOC(sizeof(WITHSUB)*$1->w_count);
	  		upStr($3);
	  		for( i=0; (i<$1->w_count-1)&&( compWith( &($1->w_subArray[i]), $3, $6 ) <= 0 ) ; i++)
	  		{
	  			memcpy(&(tmp[i]),&($1->w_subArray[i]),sizeof(WITHSUB));
	  		}
	  		strcpy(tmp[i].ws_id,$3);
	  		tmp[i].ws_subquery=$6;
	  		for(i++ ; i<$1->w_count ; i++ )
	  		{
	  			memcpy(&(tmp[i]),&($1->w_subArray[i-1]),sizeof(WITHSUB));
	  		}
	  		FB_FREE($1->w_subArray);
	  		$1->w_subArray=tmp;

	  		FB_FREE($3);
	  		$$=$1;
	  	}
	;
/**2.select list**/
select_list:
	  	sign '*'
	  	{	SELECTLIST *tmp;
	  		tmp=(SELECTLIST *)FB_MALLOC(sizeof(SELECTLIST));
	  		tmp->sel_list=(STRBUF *)FB_MALLOC(sizeof(STRBUF));
	  		tmp->sel_count=1;
	  		strcpy(tmp->sel_list->sb_value,"*");
	  		tmp->sel_list->sb_alias[0]='\0';

			if($1[0]=='\0')	tmp->sel_type=0;
			else if(strcmp($1,"DISTINCT")==0)
			{
				tmp->sel_type=1;
			}
			else if( strcmp($1,"UNIQUE")==0 )
			{
				tmp->sel_type=2;
			}
			else if( strcmp($1,"ALL")==0 )
			{
				tmp->sel_type=3;
			}

			FB_FREE($1);
	  		$$=tmp;
	  	}
	| 	sign sub_list
	  	{
	  		if($1[0]=='\0')		$2->sel_type=0;
			else if(strcmp($1,"DISTINCT")==0)
			{
				$2->sel_type=1;
			}
			else if( strcmp($1,"UNIQUE")==0 )
			{
				$2->sel_type=2;
			}
			else if( strcmp($1,"ALL")==0 )
			{
				$2->sel_type=3;
			}

			FB_FREE($1);
	  		$$=$2;
	  	}
	;
sign:
	  	/*empty*/
	  	{
	  		$$=strdup(" ");	$$[0]='\0';
	  	}
	|  	ALL
	  	{
	  		$$=strdup("ALL");
	 	}
	| 	UNIQUE
	  	{
	  		$$=strdup("UNIQUE");
	  	}
	| 	DISTINCT
	  	{
		  	$$=strdup("DISTINCT");
	  	}
	;
sub_list:
	  list_atom
	  {
	  	SELECTLIST *tmp;

	  	tmp=(SELECTLIST *)FB_MALLOC(sizeof(SELECTLIST));
		tmp->sel_list=$1;
	  	tmp->sel_count=1;
	  	$$=tmp;
	  }
	| sub_list ',' list_atom
	  {
	  	int i;
		STRBUF *tmp;

	  	$1->sel_count++;
	  	
		tmp=(STRBUF*)FB_MALLOC(sizeof(STRBUF)*($1->sel_count));
		for(i=0; i<$1->sel_count-1; i++)
	  	{
	  		if( (strcmp($1->sel_list[i].sb_value,$3->sb_value)>0) ||
	  			(strcmp($1->sel_list[i].sb_value,$3->sb_value)==0) &&
	  			(strcmp($1->sel_list[i].sb_alias,$3->sb_alias)>0 ) )
	  		break;
	  		memcpy(&(tmp[i]),&($1->sel_list[i]),sizeof(STRBUF));
	  	}
	  	memcpy(&(tmp[i]),$3,sizeof(STRBUF));
	  	for(i++;i<$1->sel_count;i++)
	  	{
	  		memcpy(&(tmp[i]),&($1->sel_list[i-1]),sizeof(STRBUF));
	  	}
		FB_FREE($1->sel_list);
		/*
		tmp = (STRBUF *)FB_REALLOC( $1->sel_list, sizeof(STRBUF)*($1->sel_count) );
		memcpy( &tmp[$1->sel_count-1], $3, sizeof(STRBUF) );
		*/
		
		FB_FREE($3);
		$1->sel_list=tmp;

		$$=$1;
	  }
	;
list_atom:
	  NAME '.' '*'
	  {
	    	STRBUF *tmp;
	  	tmp=(STRBUF *)FB_MALLOC(sizeof(STRBUF));
	  	upStr($1);
	  	/*tmp->sb_value=(char *)FB_MALLOC(sizeof(char)*(strlen($1)+3));*/
	  	sprintf(tmp->sb_value, "%s.*",$1);
	  	tmp->sb_alias[0]='\0';

	  	FB_FREE($1);
	  	$$=tmp;
	  }
	| NAME '.' NAME '.' '*'
	  {
	  	STRBUF *tmp;
	  	tmp=(STRBUF *)FB_MALLOC(sizeof(STRBUF));
	  	upStr($1);  	upStr($3);
	  	sprintf(tmp->sb_value, "%s.%s.*",$1,$3);
	  	tmp->sb_alias[0]='\0';

	  	FB_FREE($1);
	  	FB_FREE($3);
	  	$$=tmp;
	  }
	| expr  as_alias
	  {
	  	STRBUF *tmp;
	  	tmp=(STRBUF *)FB_MALLOC(sizeof(STRBUF));
	  	//memset( tmp, 0, sizeof(STRBUF) );
	  	strcpy(tmp->sb_value,$1);
	  	strcpy(tmp->sb_alias,$2);

	  	FB_FREE($1);
	  	FB_FREE($2);

	  	$$=tmp;
	  }
	;
as_alias:
	  	/*empty*/
	  	{ 	$$=strdup(" ");	$$[0]='\0';	  }
	| 	NAME
	  	{ upStr($1); }
	| 	AS NAME
	  	{ upStr($2);	$$=$2;  }
	;
expr:
	   	atom
	| 	col_ref
	| 	expr '+' expr
	  	{
	  		$$=(char *)FB_MALLOC(sizeof(char)*(strlen($1)+strlen($3)+2));
	  		sprintf($$,"%s+%s",$1,$3);
	  		FB_FREE($1);
	  		FB_FREE($3);
	  	}
	| 	expr '-' expr
	  	{
	  		$$=(char *)FB_MALLOC(sizeof(char)*(strlen($1)+strlen($3)+2));
	  		sprintf($$,"%s-%s",$1,$3);
	  		FB_FREE($1);
	  		FB_FREE($3);
	  	}
	| 	expr '*' expr
	  	{
	  		$$=(char *)FB_MALLOC(sizeof(char)*(strlen($1)+strlen($3)+2));
	  		sprintf($$,"%s*%s",$1,$3);
	  		FB_FREE($1);
	  		FB_FREE($3);
	  	}
	| 	expr '/' expr
	  	{
	  		$$=(char *)FB_MALLOC(sizeof(char)*(strlen($1)+strlen($3)+2));
	  		sprintf($$,"%s/%s",$1,$3);
	  		FB_FREE($1);
	  		FB_FREE($3);
	  	}
	| 	'+' expr %prec UMINUS
	  	{
	  		$$=(char *)FB_MALLOC(sizeof(char)*(strlen($2)+2));
	  		sprintf($$,"+%s",$2);
	  		FB_FREE($2);
	  	}
	| 	'-' expr %prec UMINUS
	  	{
	  		$$=(char *)FB_MALLOC(sizeof(char)*(strlen($2)+2));
	  		sprintf($$,"-%s",$2);
	  		FB_FREE($2);
	  	}
	|  	AMMSC '(' '*' ')'
	  	{
	  		//fprintf(stderr,"start FB_MALLOC for (*) \n");
	  		$$=(char *)FB_MALLOC(sizeof(char)*(strlen($1)+5));
	  		//memset( $$,0, strlen($1)+5 );
	  		//if( $$ ) fprintf(stderr,"succeed FB_MALLOC for (*) \n");
	  		upStr( $1 );
	  		sprintf($$,"%s( * )",$1);
	  		//fprintf(stderr,"after sprintf for (*) \n");
	  		FB_FREE($1);
	  	}
	| 	AMMSC '(' DISTINCT col_ref ')'
	  	{
	  		$$=(char *)FB_MALLOC(sizeof(char)*(strlen($1)+strlen($4)+15));
	  		upStr( $1 );
	  		sprintf($$,"%s( DISTINCT %s )",$1,$4);
	  		FB_FREE($4);
	  		FB_FREE($1);
	  	}
	| 	AMMSC '(' ALL expr ')'
	  	{
	  		$$=(char *)FB_MALLOC(sizeof(char)*(strlen($1)+strlen($4)+10));
	  		upStr( $1 );
	  		sprintf($$,"%s( ALL %s )",$1,$4);
	  		FB_FREE($4);
	  		FB_FREE($1);
	  	}
	| 	AMMSC '(' expr ')'
	  	{
	  		$$=(char *)FB_MALLOC(sizeof(char)*(strlen($1)+strlen($3)+5));
	  		upStr( $1 );
	  		sprintf($$,"%s( %s )",$1,$3);
	  		FB_FREE($3);
	  		FB_FREE($1);
	  	}
	|	NAME  '(' funclist ')'
		{
			$$=(char *)FB_MALLOC(sizeof(char)*(strlen($1)+strlen($3)+5));
			sprintf($$,"%s( %s )",$1,$3);
	  		FB_FREE($3);
	  		FB_FREE($1);
	  		/*printf("function: %s\n", $$);*/
		}
	| 	'(' expr ')'
	  	{
	  		$$=(char *)FB_MALLOC(sizeof(char)*(strlen($2)+5));
	  		sprintf($$,"( %s )",$2);
	  		FB_FREE($2);
	  	}
	;
atom:
	  STRING
	| INTNUM
	| APPROXNUM
	;
funclist:
		/*empty*/
		{  $$=strdup(" ");	}
	|	expr
	|	expr_list
        ;
col_ref:
	  NAME
	  {
	  	upStr($1);
	  }
	| NAME '.' NAME
	  {
	  	$$=(char *)FB_MALLOC(sizeof(char)*(strlen($1)+strlen($3)+2));
	  	upStr($1); 	upStr($3);
	  	sprintf($$,"%s.%s",$1,$3);
	  	FB_FREE($1);
	  	FB_FREE($3);
	  }
	| NAME '.' NAME '.' NAME
	  {
	  	upStr($1); 	upStr($3); 	upStr($5);
	  	$$=(char *)FB_MALLOC(sizeof(char)*(strlen($1)+strlen($3)+strlen($5)+3));
	  	sprintf($$,"%s.%s.%s",$1,$3,$5);

	  	FB_FREE($1);
	  	FB_FREE($3);
	  	FB_FREE($5);
	  }
	;
/**3.from clause**/
table_ref:
		tb_ref
		{
			TBFS *tmp;
			tmp=(TBFS*)FB_MALLOC(sizeof(TBFS));
			tmp->ts_count=1;
			tmp->ts_field=$1;
			$$=tmp;
		}
	| 	table_ref ',' tb_ref
	  	{
	  		int i;
	  		TBfield *tmp;
	  		/*TBatom *toms;*/
	  		$1->ts_count++;
	  		tmp=(TBfield *)FB_MALLOC(sizeof(TBfield)*$1->ts_count);
	  		for(i=0; (i<$1->ts_count-1)&&(compTBfield(&($1->ts_field[i]),$3)<0); i++)
	  		{
	  			memcpy(&(tmp[i]),&($1->ts_field[i]),sizeof(TBfield));
	  		}
	  		memcpy(&(tmp[i]),$3,sizeof(TBfield));
	 		i++;
	  		for( ;i<$1->ts_count;i++)
	  		{
	  			memcpy(&(tmp[i]),&($1->ts_field[i-1]),sizeof(TBfield));
	  		}
	  		FB_FREE($3);
	  		FB_FREE($1->ts_field);
	  		$1->ts_field=tmp;
	  		$$=$1;
	  	}
	;
tb_ref:
	  	tb_atom
	  	{
	  		TBfield *tmp;
	  		tmp=(TBfield *)FB_MALLOC(sizeof(TBfield));
	  		tmp->tf_type=0;
	  		tmp->tf_atom=$1;
	  		$$=tmp;
	  	}
	| 	'(' join_cla ')'
	  	{
	  		TBfield *tmp;
	  		tmp=(TBfield *)FB_MALLOC(sizeof(TBfield));
	  		tmp->tf_type=1;
	  		tmp->tf_join=$2;
	  		$$=tmp;
	  	}
	| 	join_cla
		{
	  		TBfield *tmp;
	  		tmp=(TBfield *)FB_MALLOC(sizeof(TBfield));
	  		tmp->tf_type=1;
	  		tmp->tf_join=$1;
	  		$$=tmp;
	  	}
	;
tb_atom:
	  	ONLY  '('  query_tb_expr  ')'  flashback_query
	  	{
	  		TBatom *tmp;
	  		tmp=(TBatom *)FB_MALLOC(sizeof(TBatom));
	  		tmp->tb_type=1;
	  		tmp->tb_head=$3;	/*struct QueryTB*/
	  		strcpy(tmp->tb_tail,$5);

	  		FB_FREE($5);
	  		$$=tmp;
	  	}
	| 	ONLY '(' query_tb_expr ')' flashback_query NAME
	 	{
	 		TBatom *tmp;
	  		tmp=(TBatom *)FB_MALLOC(sizeof(TBatom));
	  		tmp->tb_type=1;
	  		tmp->tb_head=$3;	/*struct QueryTB*/
	  		upStr($6);
	  		sprintf(tmp->tb_tail,"%s %s",$5,$6);

	  		FB_FREE($5);
	  		FB_FREE($6);
	  		$$=tmp;
	 	}
	| 	query_tb_expr flashback_query
		{
		  	TBatom *tmp;
	  		tmp=(TBatom *)FB_MALLOC(sizeof(TBatom));
	  		tmp->tb_type=0;
	  		tmp->tb_head=$1;	/*struct QueryTB*/
	  		strcpy(tmp->tb_tail,$2);

	  		FB_FREE($2);
	  		$$=tmp;
		}
	| 	query_tb_expr flashback_query  NAME
	  	{
		  	TBatom *tmp;

	  		tmp=(TBatom *)FB_MALLOC(sizeof(TBatom));
	  		tmp->tb_type=0;
	  		tmp->tb_head=$1;	/*struct QueryTB*/
	  		upStr($3);
	  		sprintf(tmp->tb_tail,"%s %s",$2,$3);

	  		FB_FREE($2);
	  		FB_FREE($3);
	  		$$=tmp;
		}
	;
query_tb_expr:
		NAME
		{
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			upStr($1);
			strcpy(tmp->qt_value,$1);
			tmp->qt_subquery=NULL;
			if( !PAT_TCOL )
			{
				PAT_TCOL=(TimeCol *)FB_MALLOC(sizeof(TimeCol));
				strcpy( PAT_TCOL->table_name, $1 );
				//匹配时间表列
				getTimeColName( PAT_TCOL );
				if( ( QUERY_TIME == NULL )
				&& ( PAT_TCOL->col_name[0] != '\0' ) )
				{
					QUERY_TIME = (QueryTime *)FB_MALLOC(sizeof(QueryTime));
					QUERY_TIME->stime[0]='\0';
					QUERY_TIME->etime[0]='\0';
				}
			}

			$$=tmp;
			FB_FREE($1);
		}
	| 	'(' subquery sub_restrict')'
		{
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			strcpy(tmp->qt_value,$3);
			tmp->qt_subquery=$2;

			FB_FREE($3);
			$$=tmp;
		}
	| 	TABLE '(' NAME ')'
		{
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			tmp->qt_subquery=NULL;
			upStr($3);
			sprintf(tmp->qt_value,"TABLE ( %s )",$3);

			$$=tmp;
			FB_FREE($3);
		}
	| 	TABLE '(' NAME ')' '(' '+' ')'
		{
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			tmp->qt_subquery=NULL;
			upStr($3);
			sprintf(tmp->qt_value,"TABLE ( %s ) (+)",$3);

			$$=tmp;
			FB_FREE($3);
		}
	| 	NAME '.' NAME '@' NAME /*dblink*/
		{
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			tmp->qt_subquery=NULL;
			upStr($1);	upStr($3); 	upStr($5);
			sprintf(tmp->qt_value,"%s.%s@%s",$1,$3,$5);

			$$=tmp;
			FB_FREE($1);
			FB_FREE($3);
			FB_FREE($5);
		}
	| 	NAME '@' NAME   /*dblink*/
		{
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			tmp->qt_subquery=NULL;
			upStr($1); 	upStr($3);
			sprintf(tmp->qt_value,"%s@%s",$1,$3);

			$$=tmp;
			FB_FREE($1);
			FB_FREE($3);
		}
	| 	NAME part_sub
		{
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			tmp->qt_subquery=NULL;
			upStr($1);
			sprintf(tmp->qt_value,"%s %s",$1,$2);

			FB_FREE($2);
			$$=tmp;
			FB_FREE($1);
		}
	| 	NAME '.' NAME part_sub
		{
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			tmp->qt_subquery=NULL;
			upStr($1);	upStr($3);
			sprintf(tmp->qt_value,"%s.%s %s",$1,$3,$4);

			FB_FREE($4);
			$$=tmp;
			FB_FREE($1);
			FB_FREE($3);
		}
	| 	NAME part_sub  sample_cla
		{
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			tmp->qt_subquery=NULL;
			upStr($1);
			sprintf(tmp->qt_value,"%s %s %s",$1,$2,$3);

			FB_FREE($2);
			FB_FREE($3);
			$$=tmp;
			FB_FREE($1);
		}
	| 	NAME '.' NAME part_sub sample_cla
		{
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			tmp->qt_subquery=NULL;
			upStr($1);
			upStr($3);
			sprintf(tmp->qt_value,"%s.%s %s %s",$1,$3,$4,$5);

			FB_FREE($4);
			FB_FREE($5);
			$$=tmp;
			FB_FREE($1);
			FB_FREE($3);
		}
	| 	NAME sample_cla
		{
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			tmp->qt_subquery=NULL;
			upStr($1);
			sprintf(tmp->qt_value,"%s %s",$1,$2);

			FB_FREE($2);
			$$=tmp;
			FB_FREE($1);
		}
	| 	NAME '.' NAME sample_cla
		{
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			tmp->qt_subquery=NULL;
			upStr($1);
			upStr($3);
			sprintf(tmp->qt_value,"%s.%s %s",$1,$3,$4);

			FB_FREE($4);
			$$=tmp;
			FB_FREE($1);
			FB_FREE($3);
		}
	;
part_sub:
		PARTITION '(' NAME ')'
		{
			char *tmp;
			tmp=(char *)FB_MALLOC(sizeof(char)*(strlen($3)+15));
			upStr($3);
			sprintf(tmp,"PARTITION ( %s )",$3);
			$$=tmp;
			FB_FREE($3);
		}
	| 	SUBPARTITION '(' NAME ')'
		{
			char *tmp;
			tmp=(char *)FB_MALLOC(sizeof(char)*(strlen($3)+18));
			upStr($3);
			sprintf(tmp,"PARTITION ( %s )",$3);
			$$=tmp;
			FB_FREE($3);
		}
	;
sample_cla:
	  	SAMPLE '(' INTNUM ')'
	  	{
	  		$$=(char *)FB_MALLOC(sizeof(char)*(strlen($3)+12));
	  		sprintf($$,"SAMPLE ( %s )",$3);
	  		FB_FREE($3);
	  	}
	| 	SAMPLE BLOCK '(' INTNUM ')'
	  	{
	  		$$=(char *)FB_MALLOC(sizeof(char)*(strlen($4)+18));
	  		sprintf($$,"SAMPLE BLOCK ( %s )",$4);
	  		FB_FREE($4);
	  	}
	| 	SAMPLE '(' INTNUM ')' SEED '(' INTNUM ')'
	  	{
	  		$$=(char *)FB_MALLOC(sizeof(char)*(strlen($3)+21+strlen($7)));
	  		sprintf($$,"SAMPLE ( %s ) SEED ( %s )",$3,$7);

	  		FB_FREE($3);
	  		FB_FREE($7);
	  	}
	| 	SAMPLE BLOCK '(' INTNUM ')' SEED '(' INTNUM ')'
	  	{
	  		$$=(char *)FB_MALLOC(sizeof(char)*(strlen($4)+27+strlen($8)));
	  		sprintf($$,"SAMPLE BLOCK ( %s ) SEED ( %s )",$4,$8);

	  		FB_FREE($4);
	  		FB_FREE($8);
		}
	;
sub_restrict:
	  	/*empty*/
	  	{	$$=strdup(" ");	$$[0]='\0';	}
	| 	WITH READ ONLY
	  	{	$$=strdup("WITH READ ONLY");	}
	| 	WITH CHECK OPTION
	  	{	$$=strdup("WITH CHECK OPTION");	}
	| 	WITH CHECK OPTION CONSTRAINT NAME
	  	{
	  		$$=(char *)FB_MALLOC(sizeof(char)*(30+strlen($5)));
	  		upStr($5);
	  		sprintf($$,"WITH CHECK OPTION CONSTRAINT %s", $5);

	  		FB_FREE($5);
	  	}
	;
flashback_query:
		/*empty*/
		{
			$$=strdup(" ");
			$$[0]='\0';
		}
	|  	AS OF SCN expr
	  	{
	  		$$=(char *)FB_MALLOC(sizeof(char)*(10+strlen($4)));
	  		sprintf($$,"AS OF SCN %s", $4);

	  		FB_FREE($4);
	  	}
	|	AS OF TIMESTAMP expr
		{
	  		$$=( char *) FB_MALLOC( sizeof(char)*(17+strlen($4)));
	  		sprintf($$,"AS OF TIMESTAMP %s", $4);

	  		FB_FREE($4);
	  	}
	| 	VERSIONS BETWEEN SCN expr_min AND expr_max AS OF SCN expr
		{
		  	$$=(char *) FB_MALLOC( sizeof(char)*(strlen($4)+strlen($6)+strlen($10)+35));
		  	sprintf($$,"VERSIONS BETWEEN SCN %s AND %s AS OF SCN %s", $4, $6, $10 );
		  	FB_FREE($4);
		  	FB_FREE($6);
		  	FB_FREE($10);
		}
	| 	VERSIONS BETWEEN TIMESTAMP expr_min AND expr_max AS OF TIMESTAMP expr
	  	{
		  	$$=(char *) FB_MALLOC( sizeof(char)*(strlen($4)+strlen($6)+strlen($10)+48));
		  	sprintf($$,"VERSIONS BETWEEN TIMESTAMP %s AND %s AS OF TIMESTAMP %s", $4, $6, $10 );
		  	FB_FREE($4);
		  	FB_FREE($6);
		  	FB_FREE($10);
		}
	;
expr_min:
	  expr
	| MINVALUE
	  {	$$=strdup("MINVALUE");	}
	;
expr_max:
	  expr
	| MAXVALUE
	  {	$$=strdup("MAXVALUE");	}
	;

join_cla:
		tb_ref JOIN tb_atom ON condition
		{
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=$3;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=$1;
			strcpy(tmp->jt_op,"JOIN");
			tmp->jt_right=tf;
			tmp->jt_using[0]='\0';
			tmp->jt_condition=$5;
			$$=tmp;
		}
	| 	tb_ref JOIN tb_atom USING '(' collist ')'
		{
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=$3;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=$1;
			strcpy(tmp->jt_op,"JOIN");
			tmp->jt_right=tf;
			sprintf(tmp->jt_using,"USING ( %s )",$6);
			tmp->jt_condition=NULL;

			FB_FREE($6);
			$$=tmp;
		}
	| 	tb_ref INNER JOIN tb_atom ON condition
		{
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=$4;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=$1;
			strcpy(tmp->jt_op,"INNER JOIN");
			tmp->jt_right=tf;
			tmp->jt_using[0]='\0';
			tmp->jt_condition=$6;

			$$=tmp;
		}
	| 	tb_ref INNER JOIN tb_atom USING '(' collist ')'
		{
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=$4;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=$1;
			strcpy(tmp->jt_op,"INNER JOIN");
			tmp->jt_right=tf;
			sprintf(tmp->jt_using,"USING ( %s )",$7);
			tmp->jt_condition=NULL;

			FB_FREE($7);
			$$=tmp;
		}
	| 	tb_ref CROSS JOIN tb_atom
		{
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=$4;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=$1;
			strcpy(tmp->jt_op,"CROSS JOIN");
			tmp->jt_right=tf;
			tmp->jt_using[0]='\0';
			tmp->jt_condition=NULL;

			$$=tmp;
		}
	| 	tb_ref NATURAL JOIN tb_atom
		{
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=$4;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=$1;
			strcpy(tmp->jt_op,"NATURAL JOIN");
			tmp->jt_right=tf;
			tmp->jt_using[0]='\0';
			tmp->jt_condition=NULL;

			$$=tmp;
		}
	| 	tb_ref NATURAL INNER JOIN tb_atom
		{
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=$5;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=$1;
			strcpy(tmp->jt_op,"NATURAL INNER JOIN");
			tmp->jt_right=tf;
			tmp->jt_using[0]='\0';
			tmp->jt_condition=NULL;

			$$=tmp;
		}
	| 	tb_ref outer_type JOIN tb_atom
		{
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=$4;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=$1;
			sprintf(tmp->jt_op,"%s JOIN",$2);
			tmp->jt_right=tf;
			tmp->jt_using[0]='\0';
			tmp->jt_condition=NULL;

			FB_FREE($2);
			$$=tmp;
		}
	| 	tb_ref outer_type JOIN tb_atom ON condition
		{
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=$4;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=$1;
			sprintf(tmp->jt_op,"%s JOIN",$2);
			tmp->jt_right=tf;
			tmp->jt_using[0]='\0';
			tmp->jt_condition=$6;

			FB_FREE($2);
			$$=tmp;
		}
	| 	tb_ref outer_type JOIN tb_atom USING '(' collist ')'
		{
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=$4;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=$1;
			sprintf(tmp->jt_op,"%s JOIN",$2);
			tmp->jt_right=tf;
			sprintf(tmp->jt_using,"USING ( %s )",$7);
			tmp->jt_condition=NULL;

			FB_FREE($2);
			FB_FREE($7);
			$$=tmp;
		}
	|	tb_ref NATURAL outer_type JOIN tb_atom
		{
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=$5;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=$1;
			sprintf(tmp->jt_op,"NATURAL %s JOIN",$3);
			tmp->jt_right=tf;
			tmp->jt_using[0]='\0';
			tmp->jt_condition=NULL;

			FB_FREE($3);
			$$=tmp;
		}
	|	tb_ref NATURAL outer_type JOIN tb_atom ON condition
		{
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=$5;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=$1;
			sprintf(tmp->jt_op,"NATURAL %s JOIN",$3);
			tmp->jt_right=tf;
			tmp->jt_using[0]='\0';
			tmp->jt_condition=$7;

			FB_FREE($3);
			$$=tmp;
		}
	|	tb_ref NATURAL outer_type JOIN tb_atom USING '(' collist ')'
		{
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=$5;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=$1;
			sprintf(tmp->jt_op,"NATURAL %s JOIN",$3);
			tmp->jt_right=tf;
			sprintf(tmp->jt_using,"USING ( %s )",$8);
			tmp->jt_condition=NULL;

			FB_FREE($3);
			FB_FREE($8);
			$$=tmp;
		}
	;
collist:
	  col_ref
	| collist ',' col_ref
	  {/*here need sort*/
		char *tmp,*p;
		int i=0,l1,l3;
		l1 = strlen($1);
		l3 = strlen($3);
		tmp = (char *)FB_MALLOC( sizeof(char)*( l1+l3+3 ) );
		while(1)
		{
			p=strchr($1+i,',');
			if(!p)
			{
				sprintf(tmp,"%s, %s",$1,$3);
				break;
			}
			if( myStrncmp( $1+i, $3, p-$1-i, l3 )>0 )
			{
				memcpy( tmp, $1, i );
				memcpy( tmp + i, $3, l3 );
				*( tmp + i + l3 ) = ',';
				*( tmp + i + l3 + 1 ) = ' ';
				memcpy( tmp + i + l3 + 2, p + 1, $1 + l1 - p );
				tmp[l1+l3+2]='\0';
				break;
			}
			i=p-$1+1;
		}

		FB_FREE($1);	FB_FREE($3);
		$$=tmp;
	  }
	;
/*****
 *this for the outer join
 *
 *part_tb:
 *		tb_ref
 *	|	tb_ref query_partition_cla
 *	;
 *query_partition_cla:
 *	  PARTITION_BY  '(' collist ')'
 *	  ;
 */

outer_type:
	  FULL
	  { $$=strdup("FULL"); }
	| LEFT
	  { $$=strdup("LEFT");  }
	| RIGHT
	  { $$=strdup("RIGHT");  }
	| FULL OUTER
	  { $$=strdup("FULL OUTER"); }
	| LEFT OUTER
	  { $$=strdup("LEFT OUTER");  }
	| RIGHT OUTER
	  { $$=strdup("RIGHT OUTER"); }
	;
/**4. describe the condition**/
condition:
	  	condition OR condition
	  	{
	  	/*NONE(即：一个basic条件表达式)=0,'()'=1,NOT=2,AND=3,OR=4*/
			$$ = ( COMBISTRU * )combi_condition( $1, $3, 4 );
		}
	| 	condition AND condition
		{
			$$ = ( COMBISTRU * )combi_condition( $1, $3, 3 );
		}
	| 	NOT condition
		{
			COMBISTRU *tmp;

			if( $2->cs_combi==0 )
			{
			/*NONE(即：一个basic条件表达式)=0,'()'=1,NOT=2,AND=3,OR=4*/
				$2->cs_combi=2;
				$$=$2;
			}
			else if( $2->cs_combi==2 )/*not(not)=old*/
			{
				if($2->cs_lefttype==0)
				{
					$2->cs_combi=0; /*not(not)basic=basic*/
					$$=$2;
				}
				else /*left child is combined node*/
				{
					$$=$2->cs_leftcs;
					$2->cs_leftcs=NULL;
					FB_FREE($2); /*free the left child*/
				}
			}
			else
			{
				tmp=(COMBISTRU *)FB_MALLOC(sizeof(COMBISTRU));
				tmp->cs_combi=2; /*show as 'NOT'*/
				tmp->cs_lefttype=1;/*say that is a combine condition node*/
				tmp->cs_leftcs=$2;
				tmp->cs_righttype=-1;
				$$=tmp;
			}
		}
	| 	'(' condition ')'
		{
			COMBISTRU *tmp;
			 /*为另一层括号，或者本身符号在逻辑表达式中就比AND和OR优先级高*/
			if( $2->cs_combi <2 )
			{/*n层括号等效于一层括号*/
				$$=$2;
			}
			else
			{
				tmp=(COMBISTRU *)FB_MALLOC(sizeof(COMBISTRU));
				tmp->cs_combi=1; /*show as '()'*/

				tmp->cs_lefttype=1;/*say that is a combine condition node*/
				tmp->cs_leftcs=$2;

				tmp->cs_righttype=-1; /*say there is not a child*/

				$$=tmp;
			}
		}
	| 	predicate
		{
			COMBISTRU *tmp;

			tmp=(COMBISTRU *)FB_MALLOC(sizeof(COMBISTRU));
			if( $1->bs_type == 4 )
			{
				BetweenTime *betw;
				BASICSTRU *l_basic, *r_basic;
				
				betw = ( BetweenTime *)($1->bs_value);
				tmp->cs_combi = 3;
				//left basic child-node
				tmp->cs_lefttype = 0;
				tmp->cs_leftbs = (BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
				memset( tmp->cs_leftbs, 0, sizeof(BASICSTRU));
				tmp->cs_leftbs->bs_type = 3;
				strcpy( tmp->cs_leftbs->bs_colname, $1->bs_colname );
				strcpy( tmp->cs_leftbs->bs_oper,">=" );
				tmp->cs_leftbs->bs_value = (char *)FB_MALLOC(strlen(betw->low_t)+1);
				memset( tmp->cs_leftbs->bs_value, 0, strlen(betw->low_t)+1);
				strcpy( tmp->cs_leftbs->bs_value, betw->low_t );
				//right basic child-node
				tmp->cs_righttype = 0;
				tmp->cs_rightbs = $1;	//(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
				tmp->cs_rightbs->bs_type = 3;
				strcpy( tmp->cs_rightbs->bs_oper, "<=");
				tmp->cs_rightbs->bs_oper[2] = '\0';
				tmp->cs_rightbs->bs_value = (char *)FB_MALLOC(strlen(betw->high_t)+1);
				memset( tmp->cs_rightbs->bs_value, 0, strlen(betw->high_t)+1);
				strcpy( tmp->cs_rightbs->bs_value, betw->high_t );
				//free resource
				FB_FREE( betw );
			}
			else	// bs_type == 1 | 3
			{
				tmp->cs_combi=0;
				/*show as just a basic opertate*/
				tmp->cs_lefttype=0;
				tmp->cs_leftbs=$1;
				tmp->cs_righttype=-1;/*that say havn't a right child */
			}
			$$=tmp;
		}
	;
predicate:
		comparison_predicate
	|	between_predicate
	|	like_predicate
	|	test_for_null
	|	in_predicate
	|	all_or_any_predicate
	|	existence_test
	;
contain_key:
		CONTAINS '(' col_ref ','  STRING ')'
		{
			BASICSTRU *tmp;
			KeyNode *kt;
			char *bp, *ep;
			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			memset( tmp, 0, sizeof(BASICSTRU) );
			strcpy( tmp->bs_colname, $3 );
			kt = (KeyNode *)FB_MALLOC( sizeof(KeyNode) );
			kt->words = (char*)FB_MALLOC( strlen($5)+1 );
			memset( kt->words, 0, strlen($5)+1 );
			bp = strchr( $5, '\'' );
			++bp;
			ep = strrchr( $5, '\'' );
			strncpy( kt->words, bp, ep-bp );
			tmp->bs_value = kt;
			FB_FREE($5);
			FB_FREE( $3 );
			$$=tmp;
		}
	;

comparison_predicate:
		contain_key COMPARISON INTNUM
		{
			/*  COMPARISON    bs_type * flag( AND = 1, OR = -1 )
			 *  =		100
			 *  <>		101
			 *  <		102
			 *  >		103
			 *  <=		104
			 *  >=		105
			 */
			strcpy( $1->bs_oper, $2 );
			if( !strncmp( $2, "=", 1 ) )
				$1->bs_type = 100;
			else if( !strcmp( $2, "<>" ) )
				$1->bs_type = 101;
			else if( $2[0] == '<' && $2[1] != '='  )
				$1->bs_type = 102;
			else if( $2[0]=='>' && $2[1] !='=' )
				$1->bs_type = 103;
			else if( !strncmp( $2, "<=", 2 ) )
				$1->bs_type = 104;
			else if( !strncmp( $2, ">=", 2 ) )
				$1->bs_type = 105;
			else
				$1->bs_type = 1;
			FB_FREE( $2 );
			strcpy( ((KeyNode*)($1->bs_value))->value, $3 );
			//printf("$2: %s\tbs_type: %d\tintnum: %s\n", $1->bs_oper, $1->bs_type, $1->bs_value );
			$$ = $1;
		}
	|	expr COMPARISON expr
		{
			BASICSTRU *tmp;

			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,$1);
			strcpy(tmp->bs_oper,$2);
			tmp->bs_value=$3;
			/*--------modified by szy at 2005/9/23--------*/
			/*tmp->bs_type =1 mean a string
			 *		3 mean a time value (this is a time condition)
			 */
			//printf(" col name: %s:%s\n", $1, PAT_TCOL->col_name );
			if( !strcasecmp( PAT_TCOL->col_name, $1 ) && ($2[0]=='<' || $2[0]=='>') )
			{
				tmp->bs_type=3;

				if( $2[0]=='<' )
				{
					recognizeTime( $3, QUERY_TIME->etime );
					strcpy( QUERY_TIME->ecompare, $2 );
				}
				else /*if( !strcmp( $2, ">" ) || !strcmp( $2, ">=" ) )*/
				{
					recognizeTime( $3, QUERY_TIME->stime );
					strcpy( QUERY_TIME->scompare, $2 );
				}
			}
			else
				tmp->bs_type=1;			/*means a string*/

			FB_FREE($1);
			FB_FREE($2);
			$$=tmp;
		}
	|	expr COMPARISON '(' subquery ')'
		{
			BASICSTRU *tmp;

			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,$1);
			strcpy(tmp->bs_oper,$2);
			tmp->bs_type=2;	/*means a subquery*/
			tmp->bs_value=$4;

			FB_FREE($1);
			FB_FREE($2);
			$$=tmp;
		}
	;

between_predicate:
		expr NOT BETWEEN expr AND expr
		{
			BASICSTRU *tmp;
			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,$1);
			strcpy(tmp->bs_oper,"NOT BETWEEN");
			tmp->bs_type=1;
			tmp->bs_value=(char *)FB_MALLOC(sizeof(char)*(strlen($4)+strlen($6)+6));
			sprintf(tmp->bs_value,"%s AND %s",$4,$6);

			FB_FREE($1);
			FB_FREE($4);
			FB_FREE($6);
			$$=tmp;
		}
	|	expr BETWEEN expr AND expr
		{
			BASICSTRU *tmp;
			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,$1);
			strcpy(tmp->bs_oper,"BETWEEN");
			tmp->bs_type=1;
			//printf("Parse Between now\n");
			//convert between to >= | <=
			if( !strcasecmp( PAT_TCOL->col_name, $1 ) )
			{
				//printf("time-col: %s\n", PAT_TCOL->col_name );
				if( !strncasecmp( $3, "to_date(", 8 ) && !strncasecmp( $5, "to_date(", 8) )
				{
					recognizeTime( $3, QUERY_TIME->stime );
					strcpy( QUERY_TIME->scompare, ">=" );
					recognizeTime( $5, QUERY_TIME->etime );
					strcpy( QUERY_TIME->ecompare, "<=" );
					tmp->bs_type = 4;
				}

			}

			if( tmp->bs_type == 1 )
			{
				tmp->bs_value=(char *)FB_MALLOC(sizeof(char)*(strlen($3)+strlen($5)+6));
				sprintf(tmp->bs_value,"%s AND %s",$3,$5);
				/*printf("between cla: %s\n",tmp->bs_value);*/
			}
			else//bs_type == 4
			{
				BetweenTime *betw=(BetweenTime *)FB_MALLOC(sizeof(BetweenTime));
				memset( betw, 0, sizeof(BetweenTime) );
				strcpy( betw->low_t, $3 );
				strcpy( betw->high_t, $5 );
				tmp->bs_value = betw;
			}

			FB_FREE($1);
			FB_FREE($3);
			FB_FREE($5);
			$$=tmp;
		}
	;

like_predicate:
		expr NOT LIKE atom opt_escape
		{
			BASICSTRU *tmp;
			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,$1);
			strcpy(tmp->bs_oper,"NOT LIKE");
			tmp->bs_type=1;
			tmp->bs_value=(char *)FB_MALLOC(sizeof(char)*(strlen($4)+strlen($5)+3));
			sprintf(tmp->bs_value," %s %s",$4,$5);

			FB_FREE($1);
			FB_FREE($4);
			FB_FREE($5);
			$$=tmp;
		}
	|	expr LIKE atom opt_escape
		{
			BASICSTRU *tmp;
			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,$1);
			strcpy(tmp->bs_oper,"LIKE");
			tmp->bs_type=1;
			tmp->bs_value=(char *)FB_MALLOC(sizeof(char)*(strlen($3)+strlen($4)+3));
			sprintf(tmp->bs_value," %s %s",$3,$4);

			FB_FREE($1);
			FB_FREE($3);
			FB_FREE($4);
			$$=tmp;
		}
	;

opt_escape:
		/* empty */
		{
			$$=strdup(" ");
			$$[0]='\0';
		}
	|	ESCAPE atom
		{
			$$=(char *)FB_MALLOC(sizeof(char)*(strlen($2)+8));
			sprintf($$,"ESCAPE %s", $2);
			FB_FREE($2);
		}
	;
test_for_null:
		col_ref IS NOT NULLX
		{
			BASICSTRU *tmp;
			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,$1);
			strcpy(tmp->bs_oper,"IS NOT NULL");
			tmp->bs_type=1;
			tmp->bs_value=(char *)FB_MALLOC(sizeof(char));
			*((char *)tmp->bs_value)='\0';

			FB_FREE($1);
			$$=tmp;
		}
	|	col_ref IS NULLX
		{
			BASICSTRU *tmp;
			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,$1);
			strcpy(tmp->bs_oper,"IS NULL");
			tmp->bs_type=1;
			tmp->bs_value=(char *)FB_MALLOC(sizeof(char));
			*((char*)tmp->bs_value)='\0';

			FB_FREE($1);
			$$=tmp;
		}
	;

in_predicate:
		expr NOT IN '(' subquery ')'
		{
			BASICSTRU *tmp;

			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,$1);
			strcpy(tmp->bs_oper,"NOT IN");
			tmp->bs_type=2;	/*means a subquery*/
			tmp->bs_value=$5;

			FB_FREE($1);
			$$=tmp;
		}
	|	expr IN '(' subquery ')'
		{
			BASICSTRU *tmp;

			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,$1);
			strcpy(tmp->bs_oper,"IN");
			tmp->bs_type=2;	/*means a subquery*/
			tmp->bs_value=$4;

			FB_FREE($1);
			$$=tmp;
		}
	|	expr NOT IN '(' atom_commalist ')'
		{
			int i,j;
			BASICSTRU *tmp;
			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,$1);
			strcpy(tmp->bs_oper,"NOt IN");
			tmp->bs_type=1;
			tmp->bs_value=(char *)FB_MALLOC(sizeof(char)*(strlen($5)+5));
			sprintf(tmp->bs_value,"( %s )",$5);

			FB_FREE($1);
			FB_FREE($5);
			$$=tmp;
		}
	|	expr IN '(' atom_commalist ')'
		{
			int i,j;
			BASICSTRU *tmp;
			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,$1);
			strcpy(tmp->bs_oper,"IN");
			tmp->bs_type=1;
			tmp->bs_value=(char *)FB_MALLOC(sizeof(char)*(strlen($4)+5));
			sprintf(tmp->bs_value,"( %s )",$4);

			FB_FREE($1);
			FB_FREE($4);
			$$=tmp;
		}
	;
atom_commalist:
		atom
	|	atom_commalist ',' atom
		{/*here need sort*/
			char *tmp,*p;
			int i=0;
			int l1,l3;
			l1 = strlen( $1 );
			l3 = strlen( $3 );
			tmp = (char *)FB_MALLOC(sizeof(char)*( l1+l3+3 ));
			while(1)
			{
				p=strchr($1+i,',');
				if(!p)
				{
					sprintf(tmp,"%s, %s", $1, $3);
					break;
				}
				if(myStrncmp( $1+i, $3, p-$1-i, l3)>0)
				{
					memcpy( tmp, $1, i);
					memcpy( tmp+i, $3, l3);
					tmp[i+l3] = ',';
					tmp[i+l3+1] = ' ';
					memcpy( tmp+i+l3+2, p+1, $1+l1-p);
					tmp[l1+l3+2]='\0';
					break;
				}
				i=p-$1+1;
			}
			FB_FREE($1);	FB_FREE($3);
			$$=tmp;
	  	}
	;
all_or_any_predicate:
		expr COMPARISON any_all_some '(' subquery ')'
		{
			BASICSTRU *tmp;

			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,$1);
			strcpy(tmp->bs_oper,$2);
			sprintf(tmp->bs_oper," %s",$3);
			tmp->bs_type=2;	/*means a subquery*/
			tmp->bs_value=$5;

			FB_FREE($1);
			FB_FREE($2);
			FB_FREE($3);
			$$=tmp;
		}
	;
any_all_some:
		ANY
		{
			$$=strdup("ANY");
		}
	|	ALL
		{
			$$=strdup("ALL");
		}
	|	SOME
		{
			$$=strdup("SOME");
		}
	;
existence_test:
		EXISTS '(' subquery ')'
		{
			BASICSTRU *tmp;

			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			tmp->bs_colname[0]='\0';
			strcpy(tmp->bs_oper,"EXISTS");
			tmp->bs_type=2;	/*means a subquery*/
			tmp->bs_value=$3;

			$$=tmp;
		}
	;
/**5. where clause**/
where_cla:
	  	/*empty*/
		{	$$=NULL;	}
	|  	WHERE condition
	   	{
			EXPRTREE *tmp;
			tmp=(EXPRTREE *)FB_MALLOC(sizeof(EXPRTREE));
			tmp->qt_count=countCondition($2);
			/*
			 * 修枝排序树：主要是contains组合方面
			 */
			tmp->qt_cs=$2;
			//合并其中的同类contains条件
			unite_contains( tmp->qt_cs );
			if( unique_deal( tmp->qt_cs ) == -1 )
			{
				//printf("where clause error\n");
				YYERROR;
			}
			$$=tmp;
		}
	;
/**6. hierach clause**/
hierarch_query:
		/*empty*/
		{
			$$=NULL;
		}
	| 	CONNECT BY condition
		{
			HIERARCH *tmp;
			tmp=(HIERARCH *)FB_MALLOC(sizeof(HIERARCH));
			tmp->hq_type=0;/*none*/
			tmp->hq_start=NULL;	/*not have*/
			tmp->hq_connect=$3;
			$$=tmp;
		}
	| 	CONNECT BY NOCYCLE condition
		{
			HIERARCH *tmp;
			tmp=(HIERARCH *)FB_MALLOC(sizeof(HIERARCH));
			tmp->hq_type=1;/*nocycle*/
			tmp->hq_start=NULL;	/*not have*/
			tmp->hq_connect=$4;
			$$=tmp;
		}
	| 	START WITH condition CONNECT BY condition
		{
			HIERARCH *tmp;
			tmp=(HIERARCH *)FB_MALLOC(sizeof(HIERARCH));
			tmp->hq_type=0;/*none*/
			tmp->hq_start=$3;/* have*/
			tmp->hq_connect=$6;
			$$=tmp;
		}
	| 	START WITH condition CONNECT BY NOCYCLE condition
		{
			HIERARCH *tmp;
			tmp=(HIERARCH *)FB_MALLOC(sizeof(HIERARCH));
			tmp->hq_type=1;/*nocycle*/
			tmp->hq_start=$3;/* have*/
			tmp->hq_connect=$7;
			$$=tmp;
		}
	;
/**7. group by clause**/
group_cla:
		GROUP BY gfields
		{
			$$=$3;
			//$3->sf_type=-1; 	/**havn't used such a sign*/
		}
	;
gfields:
	  	gfield
	  	{
	  		/*
	  		STRFIELDS *tmp;
	  		tmp=(STRFIELDS *)FB_MALLOC(sizeof(STRFIELDS));
	  		tmp->sf_count=1;
	  		strcpy(tmp->sf_value,$1);
	  		*/
	  		GroupField *tmp;
	  		tmp = (GroupField *)FB_MALLOC( sizeof(GroupField) );
	  		tmp->gnext = NULL;
	  		strcpy( tmp->gcol, $1 );
	  		$$=tmp;
	  		FB_FREE($1);
	  	}
	| 	gfields ',' gfield
		{
			/*
			int i,j,len;
			$1->sf_count++;
			i=strlen($1->sf_value);
			$1->sf_value[i]=',';
			i++;
			len=strlen($3);
			for(j=0;j<len; j++)
			{
				$1->sf_value[i+j]=$3[j];
			}
			$1->sf_value[i+j]='\0';
			*/

			GroupField *tmp;
	  		tmp = (GroupField *)FB_MALLOC( sizeof(GroupField) );
	  		tmp->gnext = NULL;
	  		strcpy( tmp->gcol, $3 );
 			FB_FREE($3);
			$1->gnext = tmp;
			$$=$1;
	  	}
	;
gfield:
	  	expr
	| 	rollup_cube_cla
	| 	grouping_sets_cla
	;
rollup_cube_cla:
	  	ROLLUP '(' grouping_expr_list ')'
	  	{
	  		$$=(char *)FB_MALLOC(sizeof(char)*(strlen($3)+11));
	  		sprintf($$,"ROLLUP( %s )",$3);
	  		FB_FREE($3);
	  	}
	| 	CUBE '(' grouping_expr_list ')'
		{
			$$=(char *)FB_MALLOC(sizeof(char)*(strlen($3)+9));
	  		sprintf($$,"CUBE( %s )",$3);
	  		FB_FREE($3);
		}
	;
grouping_expr_list:
	  	expr
	| 	'(' expr_list ')'
		{
			$$=(char *)FB_MALLOC(sizeof(char)*(strlen($2)+5));
			sprintf($$,"( %s )",$2);
			FB_FREE($2);
		}
	| 	grouping_expr_list ',' expr
		{
			char *tmp;
			/*
			tmp=(char *)realloc($1, sizeof(char)*(strlen($1)+strlen($3)+2));
			$$=tmp;
			strcat($$,",");
			strcat($$,$3);
			*/
			tmp=(char *)FB_MALLOC( sizeof(char)*(strlen($1)+strlen($3)+3));
			sprintf(tmp,"%s, %s",$1, $3);
			$$=tmp;
			FB_FREE($1);
			FB_FREE($3);
		}
	| 	grouping_expr_list ',' '(' expr_list ')'
		{
			char *tmp;
			/*
			tmp=(char *)realloc($1, sizeof(char)*(strlen($1)+strlen($4)+7));
			$$=tmp;
			strcat($$,", ( ");
			strcat($$,$4);
			strcat($$," )");
			*/
			tmp=(char *)FB_MALLOC( sizeof(char)*(strlen($1)+strlen($4)+7));
			sprintf(tmp, "%s, (%s)", $1, $4);
			$$=tmp;
			FB_FREE($1);
			FB_FREE($4);
		}
	;
expr_list:
	  	expr ',' expr
	  	{
	  		char *tmp;
			/*
			tmp=(char *)realloc($1, sizeof(char)*(strlen($1)+strlen($3)+2));
			$$=tmp;
	  		strcat($$,",");
	  		strcat($$,$3);
	  		*/
	  		tmp=(char *)FB_MALLOC( sizeof(char)*(strlen($1)+strlen($3)+3));
	  		sprintf( tmp, "%s, %s", $1, $3);
	  		$$=tmp;
	  		FB_FREE($1);
	  		FB_FREE($3);
	  	}
	| 	expr_list ',' expr
		{
	  		char *tmp;
			/*
			tmp=(char *)realloc($1, sizeof(char)*(strlen($1)+strlen($3)+2));
			$$=tmp;
	  		strcat($$,",");
	  		strcat($$,$3);
	  		*/
	  		tmp=(char *)FB_MALLOC( sizeof(char)*(strlen($1)+strlen($3)+3));
	  		sprintf( tmp, "%s, %s", $1, $3);
	  		$$=tmp;
	  		FB_FREE($1);
	  		FB_FREE($3);
	  	}
	;
grouping_sets_cla:
	  	GROUPING_SETS '(' grouping_expr_list ')'
	  	{
	  		$$=(char *)FB_MALLOC(sizeof(char)*(strlen($3)+17));
	  		sprintf($$,"GROUPING SETS( %s )",$3);

	  		FB_FREE($3);
	  	}
	| 	GROUPING_SETS '(' rollup_cube_cla ')'
		{
	  		$$=(char *)FB_MALLOC(sizeof(char)*(strlen($3)+17));
	  		sprintf($$,"GROUPING SETS( %s )",$3);

	  		FB_FREE($3);
	  	}
	;
/**8. having clause**/
having_cla:
		/*empty*/
		{	$$=NULL;	}
	| 	HAVING condition
		{
			EXPRTREE *tmp;
			tmp=(EXPRTREE *)FB_MALLOC(sizeof(EXPRTREE));
			tmp->qt_count=countCondition($2);
			tmp->qt_cs=$2;
			$$=tmp;
		}
	;
/**9. order_by_clause**/
order_cla:
		/*empty*/
		{	$$=NULL;	}
	|  	ORDER BY order_tail
		{
			//$3->sf_type=0;
			//$$=$3;

			OrderCla *tmp;
			tmp = (OrderCla *) FB_MALLOC( sizeof( OrderCla ) );
			tmp->oflag = 0;
			tmp->ofield = $3;

			$$=tmp;
		}
	| 	ORDER SIBLINGS BY order_tail
		{
			/*
			$4->sf_type=1;
			$$=$4;
			*/

			OrderCla *tmp;
			tmp = (OrderCla *) FB_MALLOC( sizeof( OrderCla ) );
			tmp->oflag = 1;
			tmp->ofield = $4;

			$$=tmp;
		}
	;
order_tail:
	  	expr otail2 otail3
	  	{
	  		/*
	  		int i,j;
	  		STRFIELDS *tmp;
			tmp=(STRFIELDS *) FB_MALLOC(sizeof(STRFIELDS));
			tmp->sf_count=1;
			sprintf(tmp->sf_value,"%s %s %s",$1,$2,$3);
			*/
			OrderField *tmp;
			tmp = (OrderField *)FB_MALLOC( sizeof( OrderField ) );
			strcpy( tmp->ocol, $1 );

			if( $2[0] == '\0' )
				tmp->oasc = 0;
			else if( !strcmp( $2, "ASC" ) )
				tmp->oasc = 1;
			else
				tmp->oasc = 2;

			if( $3[0] == '\0' )
				tmp->onull = 0;
			else if( !strcmp( $3, "NULLS FIRST") )
				tmp->onull = 1;
			else
				tmp->onull = 2;

			tmp->onext = NULL;

			FB_FREE($1);
			FB_FREE($2);
			FB_FREE($3);
			$$=tmp;
	  	}
	| 	order_tail ',' expr otail2 otail3
		{
			/*
			int i,j;
			$1->sf_count++;
			sprintf($1->sf_value+strlen($1->sf_value),", %s %s %s",$3,$4,$5);

			FB_FREE($3);	FB_FREE($4);	FB_FREE($5);
			*/
						OrderField *tmp;
			tmp = (OrderField *)FB_MALLOC( sizeof( OrderField ) );
			strcpy( tmp->ocol, $3 );

			if( $4[0] == '\0' )
				tmp->oasc = 0;
			else if( !strcmp( $4, "ASC" ) )
				tmp->oasc = 1;
			else
				tmp->oasc = 2;

			if( $5[0] == '\0' )
				tmp->onull = 0;
			else if( !strcmp( $5, "NULLS FIRST") )
				tmp->onull = 1;
			else
				tmp->onull = 2;

			tmp->onext = NULL;

			$1->onext = tmp;

			FB_FREE($3);
			FB_FREE($4);
			FB_FREE($5);

			$$=$1;
	  	}
	;
otail2:
	  	/*empty*/
	  	{
	  		$$=strdup(" ");
	  		$$[0]='\0';
	  	}
	| 	ASC
		{
			$$=strdup("ASC");
		}
	| 	DESC
		{
			$$=strdup("DESC");
		}
	;
otail3:
	  	/*empty*/
	  	{
	  		$$=strdup(" ");
	  		$$[0]='\0';
	  	}
	| 	NULLS FIRST
		{
			$$=strdup("NULLS FIRST");
		}
	| 	NULLS LAST
		{
			$$=strdup("NULLS LAST");
		}
	;
/**10- write the for_update_clause****/
for_update:
	  	/*empty*/
	  	{	$$=NULL;	}
	| 	FOR UPDATE up_of_col
		{
			$$=(char *)FB_MALLOC(sizeof(char)*(10+strlen($3)));
			sprintf($$," FOR UPDATE %s",$3);
			FB_FREE($3);
		}
	;
up_of_col:
	  	/*empty*/
	  	{
	  		$$=strdup("  ");
	  		$$[1]='\0';
	  	}
	| 	OF collist
		{
			$$=(char *)FB_MALLOC(sizeof(char)*(strlen($2)+4));
			sprintf($$,"OF %s",$2);
			FB_FREE($2);
		}
	| 	OF collist NOWAIT
		{
			$$=(char *)FB_MALLOC(sizeof(char)*(strlen($2)+11));
			sprintf($$,"OF %s NOWAIT",$2);
			FB_FREE($2);
		}
	| 	OF collist INTNUM
		{
			$$=(char *)FB_MALLOC(sizeof(char)*(strlen($2)+11));
			sprintf($$,"OF %s %s",$2,$3);
			FB_FREE($2);
			FB_FREE($3);
		}
	;

%%
/*============================== 语法分析函数入口 ==================================*/
/* --------transfer out the parsed structure--------------------
 * in data:
 *	sql - string; TimeCol - 是存储有表名和对应时间列名称的数组; tcol_nums - 数组元素个数
 * return value:
 *	SQLPATTERN* - 存有分析sql结果的结构的地址指针
 */
SQLPATTERN *myParse(const char *sql )
{
	SQLPATTERN 	*tmp_query;
	MyExtraType *my_data;
	void *scanner;

	my_data = (MyExtraType *)FB_MALLOC(sizeof(MyExtraType));
	memset(my_data, 0, sizeof(MyExtraType) );
	//扫描分析
	yylex_init ( &scanner );
	yyset_extra ( my_data, scanner );
	yy_scan_string( sql, scanner );
	if(!yyparse( scanner ))
	{
		int baselen=0;
	 	//QUERY_STRU->sp_osqlstatement = jn_thread_strdup( sql );
	 	QUERY_STRU->sp_osqlstatement = strdup( sql );
	 	//QUERY_STRU->sp_usqlstatement = getQueryStr( QUERY_STRU );
	 	QUERY_STRU->sp_usqlstatement = FB_MALLOC(sizeof(char)*MAX_SQL_SIZE);
	 	memset( QUERY_STRU->sp_usqlstatement, 0, sizeof(char)*MAX_SQL_SIZE);
	 	sqlpat2str(QUERY_STRU, QUERY_STRU->sp_usqlstatement, &baselen );
	 	//jn_getUniqueSQL( QUERY_STRU );
	 	QUERY_STRU->query_time = QUERY_TIME;
	 	QUERY_STRU->time_col = PAT_TCOL;
	 	tmp_query = QUERY_STRU;
	}
	else
	{
		if( my_data->query_time ) FB_FREE( my_data->query_time );
		if( my_data->pat_tcol ) FB_FREE( my_data->pat_tcol );
		tmp_query = NULL;
	}
	/*
	 * here avoid free the QUERY_STRU... in yylex_destroy ( scanner )
	 * the QUERY_STRU, PAT_TCOL, QUERY_TIME....should return out
	 */
	my_data->query_stru = NULL;
	my_data->query_time = NULL;
	my_data->pat_tcol = NULL;
	FB_FREE( my_data );
	yylex_destroy ( scanner );

	return tmp_query;
}
