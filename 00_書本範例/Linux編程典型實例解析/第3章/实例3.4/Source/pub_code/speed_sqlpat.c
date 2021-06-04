/*
 * jn_util.c: Dealing with the common utility routines
 *
 *
 * 09-10-05  SZY wrote original code
 * 09-30-05  ZHY replace all memory functions
 *           with thread pool functions
 * 01-20-06  SZY add "CONTAINS" parse function
 *
 */
//#include "jndbcod.h"
#include "queryopt.h"
#include "speed_utility.h"
#include "speed_sqlpat.h"

#define SIZE_STEP  2048

TimeCol		*tcol;
int		tcol_nums;
/*
 * 功能
 * 	添加字符串
 * 输入
 *	src - 源串
 * 输出
 *	*tmp - 目标串
 *	*len - 记录目标串实际长度
 *	*limit - 记录目标串目前最大容量
 */
void addStr( char **tmp, char *src, int *len, int *limit )
{
	int i,j;
	char *p;
	i=strlen(src);
	/*printf("strlen:%d+%d,limit:%d\n",*len,i,*limit);*/
	if(*len+i>*limit)
	{
		*limit += SIZE_STEP;
		p=FB_REALLOC(*tmp,*limit);
		*tmp=p;
	}
	memcpy((*tmp)+*len,src,i);
	*len+=i;
	*(*tmp+*len)='\0';
}
/*========================= 打印结果 ===================================*/
/*
 * 根据条件语法树打印条件语句
 */
void treeout(COMBISTRU   *rr)
{/*组合方式，NONE(即：一个basic条件表达式)=0,'()'=1,NOT=2,AND=3,OR=4*/
	if( !rr )
	{
		/*printf("no clause\n");*/
		return;
	}
	if(rr->cs_combi == 2 )
	printf(" NOT");
	else if(rr->cs_combi==1)
	printf(" (");

	if(rr->cs_lefttype==0)
	{
		printf(" %s %s",rr->cs_leftbs->bs_colname,rr->cs_leftbs->bs_oper);
		if(rr->cs_leftbs->bs_type==1)/*that mean just a string*/
		{
			printf(" %s",(char *)rr->cs_leftbs->bs_value);
		}
	}
	else treeout(rr->cs_leftcs);

	if(rr->cs_combi==0) return;/*mean just single basic struct*/

	switch(rr->cs_combi)
	{
		case 3: printf(" AND"); break;
		case 4: printf(" OR");  break;
	}

	if( rr->cs_righttype == 0 )
	{
		printf(" %s %s",rr->cs_rightbs->bs_colname,rr->cs_rightbs->bs_oper);
		if(rr->cs_rightbs->bs_type==1)
		{
			printf(" %s",(char *)rr->cs_rightbs->bs_value);
		}
	}
	else  treeout(rr->cs_rightcs);

	if(rr->cs_combi==1)
	printf(" )");
}

/*======================语法树结构－>生成－>sql命令串 =========================*/
/*
 * convert a condition tree structure into a string
 */
 /*--------------------------------\
  * 增加屏蔽时间条件功能	   |
  *		--2005/9/23 szy    |
  * 标号 '+++++++' 为添加记号      |
  *\-------------------------------*/
char * getBasicstr(BASICSTRU *bs)
{
	char *tmp,*p;
	int len,limit;

	if( !bs ) return NULL;
	limit=SIZE_STEP;
	tmp=(char *)FB_MALLOC(sizeof(char)*SIZE_STEP);
	len = 0;

	if( bs->bs_type == 3 )
		tmp[0]='\0';
	else// if( bs->bs_type == 1,2 )
	{
		addStr( &tmp, bs->bs_colname,  &len, &limit );
		addStr( &tmp, " ",  &len, &limit );
		addStr( &tmp, bs->bs_oper,  &len, &limit );
		if( bs->bs_type == 1 )
		{
			addStr( &tmp, " ",  &len, &limit );
			addStr( &tmp, (char*)bs->bs_value,  &len, &limit );
		}
		else
		{
			addStr( &tmp, " (",  &len, &limit );
			p=getQueryStr((SQLPATTERN*)bs->bs_value);
			addStr( &tmp, p,  &len, &limit );
			FB_FREE(p);
			addStr( &tmp, ") ",  &len, &limit );
		}
	}
	return tmp;
}
char *getConstr( COMBISTRU   *rr )
{
	char *tmp,*p;
	int len,limit;

	if(rr==NULL) return tmp;
	limit=SIZE_STEP;
	tmp=(char *)FB_MALLOC(sizeof(char)*SIZE_STEP);
	len=0;

	if( rr->cs_combi == 0 )
	{
		FB_FREE(tmp);
		tmp=getBasicstr( rr->cs_leftbs );
	}
	else if( rr->cs_combi == 1 )
	{
		p=getConstr( rr->cs_leftcs ); //unimporsible that as 'leftbs'
		if(p[0]=='\0')
		{
			FB_FREE(tmp);
			tmp = p;
		}
		else
		{
			addStr( &tmp, " (",  &len, &limit );
			addStr( &tmp, p,  &len, &limit );
			addStr( &tmp, " )",  &len, &limit );
			FB_FREE(p);
		}
	}
	else if( rr->cs_combi == 2 )
	{
		addStr( &tmp, " NOT ",  &len, &limit );
		if( rr->cs_lefttype == 0 )
			p=getBasicstr( rr->cs_leftbs );
		else
			p=getConstr( rr->cs_leftcs );
		addStr( &tmp, p,  &len, &limit );
		FB_FREE(p);
	}
	else
	{
		if( rr->cs_lefttype == 0 )
			p=getBasicstr( rr->cs_leftbs );
		else
			p=getConstr( rr->cs_leftcs );
		if( p[0]=='\0' )
		{
			if( rr->cs_righttype == 0 )
				p=getBasicstr( rr->cs_rightbs );
			else if( rr->cs_righttype == 1 )
				p=getConstr( rr->cs_rightcs );

			FB_FREE(tmp);
			tmp = p;
		}
		else
		{
			addStr( &tmp, p,  &len, &limit );
			FB_FREE(p);
			if( rr->cs_righttype == 0 )
				p=getBasicstr( rr->cs_rightbs );
			else if( rr->cs_righttype == 1 )
				p=getConstr( rr->cs_rightcs );
			if( p[0]!='\0' )
			{
				if( rr->cs_combi == 3 )
					addStr( &tmp, " AND ",  &len, &limit );
				else
					addStr( &tmp, " OR ",  &len, &limit );
				addStr( &tmp, p,  &len, &limit );
			}
			FB_FREE(p);
		}
	}
	return tmp;
}

//去除条件的外层括号
char *getConditionString( COMBISTRU   *rr )
{
	char *tmp, *p;

	if( rr->cs_combi == 0 )
	{
		tmp = getConstr( rr );
	}
	else if( rr->cs_combi == 1 )
		tmp = getConditionString( rr->cs_leftcs );
	else if( rr->cs_combi == 2 )
		tmp = getConstr( rr );
	else
	{
		if( rr->cs_lefttype == 1 && rr->cs_leftcs->cs_combi == 1
		&& rr->cs_righttype == 1 && rr->cs_rightcs->cs_combi == 1 )
		{
			p=getConstr( rr->cs_leftcs );
			if( p[0]=='\0' )
			{
				tmp=getConditionString( rr->cs_rightcs );
				FB_FREE(p);
			}
			else
			{
				FB_FREE(p);
				p=getConstr( rr->cs_rightcs );
				if( p[0] == '\0' )
				{
					FB_FREE(p);
					tmp=getConditionString( rr->cs_leftcs );
				}
				else
					tmp = getConstr(rr);
			}
		}
		else if( rr->cs_lefttype == 1 && rr->cs_leftcs->cs_combi == 1 )
		{
			if(  rr->cs_righttype == 1 )
			{
				p=getConstr(rr->cs_rightcs );
				if( p[0]=='\0' )
					tmp=getConditionString(rr->cs_leftcs );
				else
					tmp=getConstr( rr );
				FB_FREE(p);
			}
			else
			{
				p=getBasicstr( rr->cs_rightbs );
				if( p[0]=='\0' )
					tmp=getConditionString( rr->cs_leftcs );
				else
					tmp=getConstr( rr );
				FB_FREE( p );
			}
		}
		else if( rr->cs_righttype == 1 && rr->cs_rightcs->cs_combi == 1 )
		{
			if( rr->cs_lefttype == 1 )
			{
				p=getConstr( rr->cs_leftcs);
				if( p[0] == '\0' )
					tmp=getConditionString( rr->cs_rightcs );
				else
					tmp=getConstr(rr);
				FB_FREE(p);
			}
			else
			{
				p=getBasicstr( rr->cs_leftbs );
				if( p[0]=='\0' )
					tmp=getConditionString( rr->cs_rightcs );
				else
					tmp=getConstr( rr );
				FB_FREE( p );
			}
		}
		else
			tmp=getConstr( rr);
	}
	return tmp;
}

/*
 * 根据SQLPATTERN模板生成查询SQL语句
 */
char* getQueryStr(SQLPATTERN *r)
{
	char *tmp,*p;
	int i,len,limit;
	char *seltype[4]={" ","DISTINCT","UNIQUE","ALL"};
	//COMBISTRU *cs;

	limit=SIZE_STEP;
	tmp=(char *)FB_MALLOC(sizeof(char)*SIZE_STEP);
	/*bzero(tmp,SIZE_STEP);*/
	len=0;

	if(r==NULL) return tmp;

	/*with subquery*/
	if(r->sp_with)
	{
		addStr(&tmp,"WITH ",&len,&limit);
		for( i=0; i<r->sp_with->w_count-1; i++)
		{
			addStr(&tmp,r->sp_with->w_subArray[i].ws_id,&len,&limit);
			addStr(&tmp," AS ( ",&len,&limit);
			p=getQueryStr((SQLPATTERN *)(r->sp_with->w_subArray[i].ws_subquery));
			addStr(&tmp,p,&len,&limit);
			FB_FREE(p);
			addStr(&tmp," ), ",&len,&limit);
		}
		addStr(&tmp,r->sp_with->w_subArray[i].ws_id,&len,&limit);
		addStr(&tmp," AS ( ",&len,&limit);
		p=getQueryStr((SQLPATTERN *)(r->sp_with->w_subArray[i].ws_subquery));
		addStr(&tmp,p,&len,&limit);
		FB_FREE(p);
		addStr(&tmp," ) ",&len,&limit);
	}
	/*select list*/
	addStr( &tmp, "SELECT ", &len, &limit);
	addStr( &tmp, seltype[r->sp_selectlist->sel_type], &len, &limit);
	addStr( &tmp, " ", &len, &limit);
	for( i=0; i<r->sp_selectlist->sel_count-1; i++)
	{
		addStr( &tmp, r->sp_selectlist->sel_list[i].sb_value, &len, &limit);
		addStr( &tmp, " ", &len, &limit);
		addStr( &tmp, r->sp_selectlist->sel_list[i].sb_alias, &len, &limit);
		addStr( &tmp, ", ", &len, &limit);
	}
	addStr( &tmp, r->sp_selectlist->sel_list[i].sb_value, &len, &limit);
	addStr( &tmp, " ", &len, &limit);
	addStr( &tmp, r->sp_selectlist->sel_list[i].sb_alias, &len, &limit);
	//printf("get a seleclist %s\n",tmp);
	/*from clause*/
	addStr( &tmp, " FROM ", &len, &limit);
	/*printf("get a seleclist %s\n",tmp);*/
	p=getTBRefStr(r->sp_fromlist);
	addStr( &tmp, p, &len, &limit);
	FB_FREE(p);
	//printf("get a from list %s\n",tmp);
	/*where clause*/
	if( r->sp_whereclause != NULL )
	{
		p=getConditionString( r->sp_whereclause->qt_cs );
		if( p[0]!='\0' )
		{
			addStr( &tmp, " WHERE ", &len, &limit);
	 		//p=getConstr(r->sp_whereclause->qt_cs);
	 		//printf(" condition string:%s\n", p );
	 		addStr( &tmp, p, &len, &limit);
	 	}
	 	FB_FREE(p);
	}

	/*sp_connectclause**/
	if( r->sp_connectclause != NULL )
	{
		if(r->sp_connectclause->hq_start!=NULL)
		{
			addStr( &tmp, " START WITH ", &len, &limit);
			p=getConstr(r->sp_connectclause->hq_start);
			addStr( &tmp, p, &len, &limit);
			FB_FREE(p);
		}
		if(r->sp_connectclause->hq_type==0)	addStr( &tmp, " CONNECT BY ", &len, &limit);
		else	addStr( &tmp, " CONNECT BY NOCYCLE ", &len, &limit);
		p=getConstr(r->sp_connectclause->hq_connect);
		addStr( &tmp, p, &len, &limit);
		FB_FREE(p);
	}
	/*group by*/
	if( r->sp_groupbyclause!=NULL )
	{
		GroupField *gf = r->sp_groupbyclause;
		addStr( &tmp, " GROUP BY ", &len, &limit);
		//addStr( &tmp, r->sp_groupbyclause->sf_value, &len, &limit);
		addStr( &tmp, gf->gcol, &len, &limit);
		gf = gf->gnext;
		while( gf )
		{
			addStr( &tmp, ", ", &len, &limit);
			addStr( &tmp, gf->gcol, &len, &limit);
			gf = gf->gnext;
		}
	}
	/*having*/
	if( r->sp_havingclause!=NULL )
	{
		addStr( &tmp, " HAVING ", &len, &limit);
		p=getConstr(r->sp_havingclause->qt_cs);
		addStr( &tmp, p, &len, &limit);
		FB_FREE(p);
	}
	/*order by clause*/
	if( r->sp_orderbyclause!=NULL )
	{
		OrderField *ofield;
		if( r->sp_orderbyclause->oflag == 0 )
		{
			addStr( &tmp, " ORDER BY ", &len, &limit);
		}
		else
		{
			addStr( &tmp, " ORDER SIBLINGS BY ", &len, &limit);
		}
		//addStr( &tmp, r->sp_orderbyclause->sf_value, &len, &limit);
		ofield = r->sp_orderbyclause->ofield;
		addStr( &tmp, ofield->ocol, &len, &limit);
		if( ofield->oasc == 1 )
			addStr( &tmp, " ASC", &len, &limit);
		else if( ofield->oasc == 2 )
			addStr( &tmp, " DESC", &len, &limit);

		if( ofield->onull == 1 )
			addStr( &tmp, " NULLS FIRST", &len, &limit);
		else if( ofield->onull == 2 )
			addStr( &tmp, " NULLS LAST", &len, &limit);

		ofield = ofield->onext;
		while( ofield )
		{
			addStr( &tmp, ", ", &len, &limit);
			addStr( &tmp, ofield->ocol, &len, &limit);
			if( ofield->oasc == 1 )
				addStr( &tmp, " ASC", &len, &limit);
			else if( ofield->oasc == 2 )
				addStr( &tmp, " DESC", &len, &limit);

			if( ofield->onull == 1 )
				addStr( &tmp, " NULLS FIRST", &len, &limit);
			else if( ofield->onull == 2 )
				addStr( &tmp, " NULLS LAST", &len, &limit);

			ofield = ofield->onext;
		}
		/*printf("order_cla: %s\n",r->sp_orderbyclause->sf_value);*/
	}
	/*for update clause*/
	if(r->sp_updateclause != NULL )
	{
		addStr( &tmp, r->sp_updateclause, &len, &limit);
	}

	return tmp;
}
/*
 * 根据描述表list（from 之后）语法树结构，生成相应字符串
 */
char *getTBRefStr(TBFS *tb)
{
	char *tmp,*p;
	int i,len,limit;

	/*printf("total fields: %d\n",tb->ts_count);*/
	limit=SIZE_STEP;
	tmp=(char *)FB_MALLOC(sizeof(char)*SIZE_STEP);
	if( tmp==NULL )  printf("FB_MALLOC  fail\n");
	len=0;

	if( tb==NULL ) return tmp;

	for( i=0; i<tb->ts_count-1; i++ )
	{
		p=getTBfield( &( tb->ts_field[i] ) );
		addStr( &tmp, p, &len, &limit);
		FB_FREE(p);/*free*/
		addStr( &tmp, ", ", &len, &limit);
	}
	p=getTBfield( &( tb->ts_field[i] ) );
	addStr( &tmp, p, &len, &limit);
	FB_FREE(p);
	/*printf("ddtb_ref:%s\tlenth:%d\n",tmp,strlen(tmp));*/
	return tmp;
}
char *getTBfield(TBfield *r)
{
	char *tmp,*p;
	int i,len,limit;

	limit=SIZE_STEP;
	tmp=(char *)FB_MALLOC(sizeof(char)*SIZE_STEP);
	len=0;

	if( r==NULL ) return tmp;

	if( r->tf_type == 0 )/*meat atom*/
	{
		if( r->tf_atom->tb_type == 0)/* no 'ONLY'*/
		{
			if( r->tf_atom->tb_head->qt_subquery != NULL )
			{
				addStr( &tmp, " (", &len, &limit);
				p=getQueryStr((SQLPATTERN*)(r->tf_atom->tb_head->qt_subquery));
				addStr( &tmp, p, &len, &limit);
				FB_FREE(p);
				addStr( &tmp, " ", &len, &limit);
				addStr( &tmp, r->tf_atom->tb_head->qt_value, &len, &limit);
				addStr( &tmp, ") ", &len, &limit);
			}
			else
			{
				addStr( &tmp, r->tf_atom->tb_head->qt_value, &len, &limit);
				addStr( &tmp, " ", &len, &limit);
			}
		}
		else /*with 'ONLY'*/
		{
			addStr( &tmp, " ONLY ( ", &len, &limit);
			if( r->tf_atom->tb_head->qt_subquery != NULL )
			{
				p=getQueryStr((SQLPATTERN*)(r->tf_atom->tb_head->qt_subquery));
				addStr( &tmp, p, &len, &limit);
				FB_FREE(p);
				addStr( &tmp, " ", &len, &limit);
			}

			addStr( &tmp, r->tf_atom->tb_head->qt_value, &len, &limit);
			addStr( &tmp, ") ", &len, &limit);
		}
		addStr( &tmp, r->tf_atom->tb_tail, &len, &limit);
	}
	else/*join */
	{
		p= getTBfield( r->tf_join->jt_left );
		addStr( &tmp, p, &len, &limit);
		FB_FREE(p);/*free*/
		addStr( &tmp, " ", &len, &limit);
		addStr( &tmp, r->tf_join->jt_op, &len, &limit);
		addStr( &tmp, " ", &len, &limit);
		p = getTBfield( r->tf_join->jt_right ) ;
		addStr( &tmp, p, &len, &limit);
		FB_FREE(p);/*free*/
		if( r->tf_join->jt_condition != NULL )
		{
			addStr( &tmp, "ON ", &len, &limit);
			p=getConstr( r->tf_join->jt_condition );
			addStr( &tmp, p, &len, &limit);
			FB_FREE(p);
		}
		addStr( &tmp, r->tf_join->jt_using, &len, &limit);
	}
	/*printf("ddfield:%s\tlen:%d\n",tmp,strlen(tmp));*/
	return tmp;
}
/*=========================== 释放内存 ===============================*/
/*free memory*/
void freePattern(SQLPATTERN *r)
{
	int i;
	/*printf("now free\n");*/
	if( r->sp_with == NULL ) goto fr1;
	/*free the withsub*/
	for( i=0; i<r->sp_with->w_count; i++)
	{
		if( r->sp_with->w_subArray[i].ws_subquery!=NULL )
			freePattern( ( SQLPATTERN *)( r->sp_with->w_subArray[i].ws_subquery ) );
	}
	FB_FREE( r->sp_with->w_subArray );
	FB_FREE( r->sp_with );
	r->sp_with = NULL;

	fr1:
	/*free select list*/
	/*printf("free select list\n");*/
	FB_FREE( r->sp_selectlist->sel_list );
	FB_FREE( r->sp_selectlist );
	r->sp_selectlist = NULL;

	/*printf("free select list\n");*/
	/*free from clasue*/
	/*printf("form list has %d fields\n",r->sp_fromlist->ts_count);*/
	for( i=0; i<r->sp_fromlist->ts_count; i++ )
	{
		/*printf("free tb field\n");*/
		freeTBfield( &(r->sp_fromlist->ts_field[i]) );
		/*printf("yes free it\n");	*/
	}

	FB_FREE( r->sp_fromlist->ts_field );
	/*printf("have free all the field\n");*/

	FB_FREE( r->sp_fromlist );
	/*printf("succes free from list\n");*/
	if( r->sp_whereclause == NULL ) goto fr2;
	/*free where clause*/
	freeCondition( r->sp_whereclause->qt_cs );
	FB_FREE( r->sp_whereclause );
	fr2:
	if( r->sp_connectclause == NULL ) goto fr3;
	/*free connection clause*/
	freeCondition( r->sp_connectclause->hq_start );
	freeCondition( r->sp_connectclause->hq_connect );
	FB_FREE( r->sp_connectclause );

	fr3:
	if( r->sp_groupbyclause == NULL ) goto fr4;
	/*free group*/
	{
		GroupField *bf, *gf = r->sp_groupbyclause;
		while( gf )
		{
			bf = gf;
			gf = gf->gnext;
			if( bf )
				FB_FREE( bf );
		}
		r->sp_groupbyclause = NULL;
	}

	if( r->sp_havingclause == NULL ) goto fr4;
	/*free having*/
	freeCondition( r->sp_havingclause->qt_cs );
	FB_FREE( r->sp_havingclause );
	r->sp_havingclause = NULL;

	fr4:
	if( r->sp_orderbyclause == NULL ) goto fr5;
	/*free order*/
	{
		OrderField *ofield, *bf;

		ofield = r->sp_orderbyclause->ofield;
		while( ofield )
		{
			bf = ofield;
			ofield = ofield ->onext;
			FB_FREE( bf );
		}
		FB_FREE( r->sp_orderbyclause );
		r->sp_orderbyclause = NULL;
	}
	fr5:
	if( r->sp_updateclause == NULL ) goto fend;
	/*free update*/
	FB_FREE( r->sp_updateclause );
	r->sp_updateclause = NULL;

	fend:
	/*free sql string*/
	if( r->sp_usqlstatement )
	{
		FB_FREE( r->sp_usqlstatement );
		r->sp_usqlstatement = NULL;
	}
	if( r->sp_osqlstatement )
	{
		FB_FREE( r->sp_osqlstatement );
		r->sp_osqlstatement = NULL;
	}
	if( r->time_col )
	{
		FB_FREE( r->time_col );
		r->time_col = NULL;
	}
	if( r->query_time )
	{
		FB_FREE( r->query_time );
		r->query_time = NULL;
	}
	/*free root*/
	FB_FREE( r );
}
/* free tbfield structure*/
void freeTBfield( TBfield *r )
{
	if( r->tf_type == 0 )
	{
		if( r->tf_atom->tb_head->qt_subquery != NULL )
		{
			freePattern( (SQLPATTERN *)(r->tf_atom->tb_head->qt_subquery) );
		}
		FB_FREE( r->tf_atom->tb_head );
		FB_FREE( r->tf_atom );
	}
	else/*join*/
	{
		freeTBfield( r->tf_join->jt_left );
		FB_FREE( r->tf_join->jt_left );
		freeTBfield( r->tf_join->jt_right );
		FB_FREE( r->tf_join->jt_right );
		if( r->tf_join->jt_condition != NULL )
		{
			freeCondition( (COMBISTRU *)(r->tf_join->jt_condition) );
		}
		FB_FREE( r->tf_join );
	}
}
/*free combistru struct*/
void freeCondition( COMBISTRU *r )
{
	if( r== NULL ) return;
	if( r->cs_combi == 0 )/*as a basic*/
	{
		freeBasic( r->cs_leftbs );
		/*printf("have free the basic\n");*/
	}
	else if( r->cs_combi < 3 )/* () combi */
	{
		/*printf("the combi spe:%d\n",r->cs_combi);*/
		if( r->cs_lefttype == 0 )
		{
			/* printf("free basic here\n");*/
			 freeBasic( r->cs_leftbs );
		}
		else freeCondition( r->cs_leftcs );
	}
	else
	{
		/*printf("the combi spe:%d\n",r->cs_combi);*/
		if( r->cs_lefttype == 0 ) freeBasic( r->cs_leftbs );
		else 	freeCondition( r->cs_leftcs );
		if( r->cs_righttype == 0 ) freeBasic( r->cs_rightbs );
		else		freeCondition( r->cs_rightcs );
	}
	FB_FREE( r );
}
void freeBasic( BASICSTRU *r )
{
	if( r== NULL ) return;
	/*
	 * 增加了 条件类型， 一定要记得对应释放处理类型
	 			——2005/11/21 17:03
	 */
	if( r->bs_type == 1 ) {
		/*printf("will free string:%s\n",(char *)r->bs_value);*/
		FB_FREE( (char *)(r->bs_value) );
	}
	else if( r->bs_type == 2 )
		freePattern( (SQLPATTERN *)(r->bs_value) );
	else if( r->bs_type > 99 )
	{
		FB_FREE( ((KeyNode*)(r->bs_value))->words );
		FB_FREE( r->bs_value );
	}
	else
		FB_FREE( r->bs_value );

	FB_FREE( r );
}
//
//用户调用接口：各种需要的sql语句格式生成
//
char *jn_getUniqueSQL( SQLPATTERN *r )
{
	char *tmp,*p;
	int i,len,limit;
	char *seltype[4]={" ","DISTINCT","UNIQUE","ALL"};

	limit=SIZE_STEP;
	tmp=(char *)FB_MALLOC(sizeof(char)*SIZE_STEP);
	/*bzero(tmp,SIZE_STEP);*/
	len=0;

	if(r==NULL) return tmp;
	/*with subquery*/
	if(r->sp_with)
	{
		addStr(&tmp,"WITH ",&len,&limit);
		for( i=0; i<r->sp_with->w_count-1; i++)
		{
			addStr(&tmp,r->sp_with->w_subArray[i].ws_id,&len,&limit);
			addStr(&tmp," AS ( ",&len,&limit);
			p=getQueryStr((SQLPATTERN *)(r->sp_with->w_subArray[i].ws_subquery));
			addStr(&tmp,p,&len,&limit);
			FB_FREE(p);
			addStr(&tmp," ), ",&len,&limit);
		}
		addStr(&tmp,r->sp_with->w_subArray[i].ws_id,&len,&limit);
		addStr(&tmp," AS ( ",&len,&limit);
		p=getQueryStr((SQLPATTERN *)(r->sp_with->w_subArray[i].ws_subquery));
		addStr(&tmp,p,&len,&limit);
		FB_FREE(p);
		addStr(&tmp," ) ",&len,&limit);
	}
	/*--------------------------------------------*/
	/* here the select_list was instead of rowid */
	/*-------------------------------------------*/
	addStr( &tmp, "SELECT ROWID ", &len, &limit);
	/*from clause*/
	addStr( &tmp, " FROM ", &len, &limit);
	/*printf("get a seleclist %s\n",tmp);*/
	p=getTBRefStr(r->sp_fromlist);
	addStr( &tmp, p, &len, &limit);
	FB_FREE(p);
	/*where clause*/
	if( r->sp_whereclause != NULL )
	{
		p=getConditionString( r->sp_whereclause->qt_cs );
		if( p[0]!='\0' )
		{
			addStr( &tmp, " WHERE ", &len, &limit);
	 		addStr( &tmp, p, &len, &limit);
	 	}
	 	FB_FREE(p);
	}

	return tmp;
}
char *jn_db_groupSql( SQLPATTERN *r )
{
	char *tmp,*p;
	int i,len,limit;
	char *seltype[4]={" ","DISTINCT","UNIQUE","ALL"};

	limit=SIZE_STEP;
	tmp=(char *)FB_MALLOC(sizeof(char)*SIZE_STEP);
	/*bzero(tmp,SIZE_STEP);*/
	len=0;

	if(r==NULL) return tmp;
	/*with subquery*/
	if(r->sp_with)
	{
		addStr(&tmp,"WITH ",&len,&limit);
		for( i=0; i<r->sp_with->w_count-1; i++)
		{
			addStr(&tmp,r->sp_with->w_subArray[i].ws_id,&len,&limit);
			addStr(&tmp," AS ( ",&len,&limit);
			p=getQueryStr((SQLPATTERN *)(r->sp_with->w_subArray[i].ws_subquery));
			addStr(&tmp,p,&len,&limit);
			FB_FREE(p);
			addStr(&tmp," ), ",&len,&limit);
		}
		addStr(&tmp,r->sp_with->w_subArray[i].ws_id,&len,&limit);
		addStr(&tmp," AS ( ",&len,&limit);
		p=getQueryStr((SQLPATTERN *)(r->sp_with->w_subArray[i].ws_subquery));
		addStr(&tmp,p,&len,&limit);
		FB_FREE(p);
		addStr(&tmp," ) ",&len,&limit);
	}
	/*--------------------------------------------*/
	/* here the select_list was instead of rowid */
	/*-------------------------------------------*/
	addStr( &tmp, "SELECT ", &len, &limit);
	//add group list
	if( r->sp_groupbyclause!=NULL )
	{
		GroupField *gf = r->sp_groupbyclause;
		SELECTLIST *slist = r->sp_selectlist;
		int hav;
		//addStr( &tmp, " GROUP BY ", &len, &limit);
		while( gf )
		{
			int i;
			//find the string if exist in select list
			hav = 0;
			for( i=0; i < slist->sel_count; i++ )
			{
				if( !strcmp( gf->gcol, slist->sel_list[i].sb_value )
				||  !strcmp( gf->gcol, slist->sel_list[i].sb_alias )
				)
				{
					hav = 1;
					break;
				}
			}
			if( hav == 0 )
			{
				addStr( &tmp, gf->gcol, &len, &limit);
				addStr( &tmp, ", ", &len, &limit);
			}
			gf = gf->gnext;
		}
	}
	//add select list
	addStr( &tmp, seltype[r->sp_selectlist->sel_type], &len, &limit);
	addStr( &tmp, " ", &len, &limit);
	for( i=0; i<r->sp_selectlist->sel_count-1; i++)
	{
		addStr( &tmp, r->sp_selectlist->sel_list[i].sb_value, &len, &limit);
		addStr( &tmp, " ", &len, &limit);
		addStr( &tmp, r->sp_selectlist->sel_list[i].sb_alias, &len, &limit);
		addStr( &tmp, ", ", &len, &limit);
	}
	addStr( &tmp, r->sp_selectlist->sel_list[i].sb_value, &len, &limit);
	addStr( &tmp, " ", &len, &limit);
	addStr( &tmp, r->sp_selectlist->sel_list[i].sb_alias, &len, &limit);
	//printf("get a seleclist %s\n",tmp);
	/*from clause*/
	addStr( &tmp, " FROM ", &len, &limit);
	/*printf("get a seleclist %s\n",tmp);*/
	p=getTBRefStr(r->sp_fromlist);
	addStr( &tmp, p, &len, &limit);
	FB_FREE(p);
	/*where clause*/
	if( r->sp_whereclause != NULL )
	{
		p=getConditionString( r->sp_whereclause->qt_cs );
		if( p[0]!='\0' )
		{
			addStr( &tmp, " WHERE ", &len, &limit);
	 		addStr( &tmp, p, &len, &limit);
	 	}
	 	FB_FREE(p);
	}
	/*group by*/
	if( r->sp_groupbyclause!=NULL )
	{
		GroupField *gf = r->sp_groupbyclause;
		addStr( &tmp, " GROUP BY ", &len, &limit);
		//addStr( &tmp, r->sp_groupbyclause->sf_value, &len, &limit);
		addStr( &tmp, gf->gcol, &len, &limit);
		gf = gf->gnext;
		while( gf )
		{
			addStr( &tmp, ", ", &len, &limit);
			addStr( &tmp, gf->gcol, &len, &limit);
			gf = gf->gnext;
		}
	}
	/*having*/
	if( r->sp_havingclause!=NULL )
	{
		addStr( &tmp, " HAVING ", &len, &limit);
		p=getConstr(r->sp_havingclause->qt_cs);
		addStr( &tmp, p, &len, &limit);
		FB_FREE(p);
	}

	return tmp;
}
/*----------new add 2005/9/15 by szy----------*/
/* 1- build get rowid query sql string        */
/* 2- build query sql string with built rowid */
/*--------------------------------------------*/
//exa: select rowid from .. where .. group .. order ..
char *jn_getRowidSQL( SQLPATTERN *r )
{
	char *tmp,*p;
	int i,len,limit;
	char *seltype[4]={" ","DISTINCT","UNIQUE","ALL"};

	limit=SIZE_STEP;
	tmp=(char *)FB_MALLOC(sizeof(char)*SIZE_STEP);
	/*bzero(tmp,SIZE_STEP);*/
	len=0;

	if(r==NULL) return tmp;
	/*with subquery*/

	if(r->sp_with)
	{
		addStr(&tmp,"WITH ",&len,&limit);
		for( i=0; i<r->sp_with->w_count-1; i++)
		{
			addStr(&tmp,r->sp_with->w_subArray[i].ws_id,&len,&limit);
			addStr(&tmp," AS ( ",&len,&limit);
			p=getQueryStr((SQLPATTERN *)(r->sp_with->w_subArray[i].ws_subquery));
			addStr(&tmp,p,&len,&limit);
			FB_FREE(p);
			addStr(&tmp," ), ",&len,&limit);
		}
		addStr(&tmp,r->sp_with->w_subArray[i].ws_id,&len,&limit);
		addStr(&tmp," AS ( ",&len,&limit);
		p=getQueryStr((SQLPATTERN *)(r->sp_with->w_subArray[i].ws_subquery));
		addStr(&tmp,p,&len,&limit);
		FB_FREE(p);
		addStr(&tmp," ) ",&len,&limit);
	}
	/*--------------------------------------------*/
	/* here the select_list was instead of rowid */
	/*-------------------------------------------*/
	addStr( &tmp, "SELECT ROWID ", &len, &limit);
	/*from clause*/
	addStr( &tmp, " FROM ", &len, &limit);
	/*printf("get a seleclist %s\n",tmp);*/
	p=getTBRefStr(r->sp_fromlist);
	addStr( &tmp, p, &len, &limit);
	FB_FREE(p);
	/*printf("get a from list %s\n",tmp);*/
	/*where clause*/
	if( r->sp_whereclause != NULL )
	{
		p=getConditionString( r->sp_whereclause->qt_cs );
		if( p[0]!='\0' )
		{
			addStr( &tmp, " WHERE ", &len, &limit);
	 		addStr( &tmp, p, &len, &limit);
	 	}
	 	FB_FREE(p);
	}
	/*sp_connectclause**/
	if( r->sp_connectclause != NULL )
	{
		if(r->sp_connectclause->hq_start!=NULL)
		{
			addStr( &tmp, " START WITH ", &len, &limit);
			p=getConstr(r->sp_connectclause->hq_start);
			addStr( &tmp, p, &len, &limit);
			FB_FREE(p);
		}
		if(r->sp_connectclause->hq_type==0)	addStr( &tmp, " CONNECT BY ", &len, &limit);
		else	addStr( &tmp, " CONNECT BY NOCYCLE ", &len, &limit);
		p=getConstr(r->sp_connectclause->hq_connect);
		addStr( &tmp, p, &len, &limit);
		FB_FREE(p);
	}
	/*group by*/
	if( r->sp_groupbyclause!=NULL )
	{
		GroupField *gf = r->sp_groupbyclause;
		addStr( &tmp, " GROUP BY ", &len, &limit);
		//addStr( &tmp, r->sp_groupbyclause->sf_value, &len, &limit);
		addStr( &tmp, gf->gcol, &len, &limit);
		gf = gf->gnext;
		while( gf )
		{
			addStr( &tmp, ", ", &len, &limit);
			addStr( &tmp, gf->gcol, &len, &limit);
			gf = gf->gnext;
		}
	}
	/*having*/
	if( r->sp_havingclause!=NULL )
	{
		addStr( &tmp, " HAVING ", &len, &limit);
		p=getConstr(r->sp_havingclause->qt_cs);
		addStr( &tmp, p, &len, &limit);
		FB_FREE(p);
	}
	/*order by clause*/
	if( r->sp_orderbyclause!=NULL )
	{
		OrderField *ofield;
		if( r->sp_orderbyclause->oflag == 0 )
		{
			addStr( &tmp, " ORDER BY ", &len, &limit);
		}
		else
		{
			addStr( &tmp, " ORDER SIBLINGS BY ", &len, &limit);
		}
		//addStr( &tmp, r->sp_orderbyclause->sf_value, &len, &limit);
		ofield = r->sp_orderbyclause->ofield;
		addStr( &tmp, ofield->ocol, &len, &limit);
		if( ofield->oasc == 1 )
			addStr( &tmp, " ASC", &len, &limit);
		else if( ofield->oasc == 2 )
			addStr( &tmp, " DESC", &len, &limit);

		if( ofield->onull == 1 )
			addStr( &tmp, " NULLS FIRST", &len, &limit);
		else if( ofield->onull == 2 )
			addStr( &tmp, " NULLS LAST", &len, &limit);

		ofield = ofield->onext;
		while( ofield )
		{
			addStr( &tmp, ", ", &len, &limit);
			addStr( &tmp, ofield->ocol, &len, &limit);
			if( ofield->oasc == 1 )
				addStr( &tmp, " ASC", &len, &limit);
			else if( ofield->oasc == 2 )
				addStr( &tmp, " DESC", &len, &limit);

			if( ofield->onull == 1 )
				addStr( &tmp, " NULLS FIRST", &len, &limit);
			else if( ofield->onull == 2 )
				addStr( &tmp, " NULLS LAST", &len, &limit);

			ofield = ofield->onext;
		}
		/*printf("order_cla: %s\n",r->sp_orderbyclause->sf_value);*/
	}

	return tmp;
}

/*
 * info: Generate a sql as: ... where rowid:=1 or rowid:=2 ....
 * version:
 *	use the newest string generating functions
 *				-check by szy, 2006/01/20
 */
//select ... from .. where rowid := 1 and rowid := ....
char *jn_db_querySQL( const int batsize, SQLPATTERN *sp )
{
	char *out, tmp[48], *atom;
	int baselen_fac=0, len, i, *baselen;

	atom = (char*)FB_MALLOC(sizeof(char)*MAX_SQL_SIZE);
	out = atom;
	if( out == NULL )
		return NULL;
	baselen = &baselen_fac;
	//with subquery
	if( sp->sp_with )
	{
		if( with2str( sp->sp_with, out, baselen )<0 )
		{
			FB_FREE( atom );
			return NULL;
		}
		out += *baselen;
	}
	//select list
	len = *baselen;
	if( sellist2str( sp->sp_selectlist, out, baselen )<0 )
	{
		//FB_FREE( atom );
		return atom;
	}
	out += *baselen-len;
	//from list
	len = *baselen;
	if( tbfs2str( sp->sp_fromlist, out, baselen )<0 )
	{
		return atom;
	}
	out += *baselen-len;
	//where
	//ADD_LEN( baselen, 15 );
	if( *baselen + 15 >= MAX_SQL_SIZE )
	{
		return atom;
	}
	else
	{
		*baselen += 15;
	}
	strcpy( out, "WHERE ROWID=:1 ");
	out += 15;
	for( i=2; i<=batsize; i++)
	{
		sprintf( tmp, "OR ROWID=:%d ", i);
		len = strlen( tmp );
	//	ADD_LEN( baselen, len );
		if( *baselen + len >= MAX_SQL_SIZE )
		{
			return atom;
		}
		else
		{
			*baselen += len;
		}
		strcpy( out, tmp );
		out += len;
	}//end for
	//for update clause
	if( sp->sp_updateclause != NULL )
	{
		len = strlen( sp->sp_updateclause);
		if( *baselen + len >= MAX_SQL_SIZE )
		{
			return atom;
		}
		else
		{
			*baselen += len;
		}
		strcpy( out, sp->sp_updateclause);
	}

	return atom;
}
char *jn_db_groupQuerySQL( const int batsize, SQLPATTERN *r )
{
	char *tmp,*p;
	int i,len,limit;
	char *seltype[4]={" ","DISTINCT","UNIQUE","ALL"};

	limit=SIZE_STEP;
	tmp=(char *)FB_MALLOC(sizeof(char)*SIZE_STEP);
	/*bzero(tmp,SIZE_STEP);*/
	len=0;

	if(r==NULL) return tmp;
	/*with subquery*/

	if(r->sp_with)
	{
		addStr(&tmp,"WITH ",&len,&limit);
		for( i=0; i<r->sp_with->w_count-1; i++)
		{
			addStr(&tmp,r->sp_with->w_subArray[i].ws_id,&len,&limit);
			addStr(&tmp," AS ( ",&len,&limit);
			p=getQueryStr((SQLPATTERN *)(r->sp_with->w_subArray[i].ws_subquery));
			addStr(&tmp,p,&len,&limit);
			FB_FREE(p);
			addStr(&tmp," ), ",&len,&limit);
		}
		addStr(&tmp,r->sp_with->w_subArray[i].ws_id,&len,&limit);
		addStr(&tmp," AS ( ",&len,&limit);
		p=getQueryStr((SQLPATTERN *)(r->sp_with->w_subArray[i].ws_subquery));
		addStr(&tmp,p,&len,&limit);
		FB_FREE(p);
		addStr(&tmp," ) ",&len,&limit);
	}
	/*select list*/
	addStr( &tmp, "SELECT ", &len, &limit);
		//add group list
	if( r->sp_groupbyclause!=NULL )
	{
		GroupField *gf = r->sp_groupbyclause;
		SELECTLIST *slist = r->sp_selectlist;
		int hav;
		//addStr( &tmp, " GROUP BY ", &len, &limit);
		while( gf )
		{
			int i;
			//find the string if exist in select list
			hav = 0;
			for( i=0; i < slist->sel_count; i++ )
			{
				if( !strcmp( gf->gcol, slist->sel_list[i].sb_value )
				||  !strcmp( gf->gcol, slist->sel_list[i].sb_alias )
				)
				{
					hav = 1;
					break;
				}
			}
			if( hav == 0 )
			{
				addStr( &tmp, gf->gcol, &len, &limit);
				addStr( &tmp, ", ", &len, &limit);
			}
			gf = gf->gnext;
		}
	}
	//select list
	addStr( &tmp, seltype[r->sp_selectlist->sel_type], &len, &limit);
	addStr( &tmp, " ", &len, &limit);
	for( i=0; i<r->sp_selectlist->sel_count-1; i++)
	{
		addStr( &tmp, r->sp_selectlist->sel_list[i].sb_value, &len, &limit);
		addStr( &tmp, " ", &len, &limit);
		addStr( &tmp, r->sp_selectlist->sel_list[i].sb_alias, &len, &limit);
		addStr( &tmp, ", ", &len, &limit);
	}
	addStr( &tmp, r->sp_selectlist->sel_list[i].sb_value, &len, &limit);
	addStr( &tmp, " ", &len, &limit);
	addStr( &tmp, r->sp_selectlist->sel_list[i].sb_alias, &len, &limit);
	/*printf("get a seleclist %s\n",tmp);*/
	/*from clause*/
	addStr( &tmp, " FROM ", &len, &limit);
	/*printf("get a seleclist %s\n",tmp);*/
	p=getTBRefStr(r->sp_fromlist);
	addStr( &tmp, p, &len, &limit);
	FB_FREE(p);
	//where
	addStr( &tmp, " WHERE ROWID=:1 ", &len, &limit);
	p=(char *)FB_MALLOC(sizeof(char)*40);
	for( i=2; i<=batsize; i++)
	{
		sprintf( p, "OR ROWID=:%d ", i);
		addStr( &tmp, p, &len, &limit);
	}//end for
	FB_FREE(p);
	/*group by*/
	if( r->sp_groupbyclause!=NULL )
	{
		GroupField *gf = r->sp_groupbyclause;
		addStr( &tmp, " GROUP BY ", &len, &limit);
		//addStr( &tmp, r->sp_groupbyclause->sf_value, &len, &limit);
		addStr( &tmp, gf->gcol, &len, &limit);
		gf = gf->gnext;
		while( gf )
		{
			addStr( &tmp, ", ", &len, &limit);
			addStr( &tmp, gf->gcol, &len, &limit);
			gf = gf->gnext;
		}
	}
	/*having*/
	if( r->sp_havingclause!=NULL )
	{
		addStr( &tmp, " HAVING ", &len, &limit);
		p=getConstr(r->sp_havingclause->qt_cs);
		addStr( &tmp, p, &len, &limit);
		FB_FREE(p);
	}
	/*for update clause*/
	if(r->sp_updateclause != NULL )
	{
		addStr( &tmp, r->sp_updateclause, &len, &limit);
	}

	return tmp;
}
/*
 * no where query sql string just for select list describe only use
 */
char *jn_db_noWhereSQL( SQLPATTERN *r )
{
	char *tmp,*p;
	int i,len,limit;
	char *seltype[4]={" ","DISTINCT","UNIQUE","ALL"};

	limit=SIZE_STEP;
	tmp=(char *)FB_MALLOC(sizeof(char)*SIZE_STEP);
	/*bzero(tmp,SIZE_STEP);*/
	len=0;

	if(r==NULL) return tmp;
	/*with subquery*/

	if(r->sp_with)
	{
		addStr(&tmp,"WITH ",&len,&limit);
		for( i=0; i<r->sp_with->w_count-1; i++)
		{
			addStr(&tmp,r->sp_with->w_subArray[i].ws_id,&len,&limit);
			addStr(&tmp," AS ( ",&len,&limit);
			p=getQueryStr((SQLPATTERN *)(r->sp_with->w_subArray[i].ws_subquery));
			addStr(&tmp,p,&len,&limit);
			FB_FREE(p);
			addStr(&tmp," ), ",&len,&limit);
		}
		addStr(&tmp,r->sp_with->w_subArray[i].ws_id,&len,&limit);
		addStr(&tmp," AS ( ",&len,&limit);
		p=getQueryStr((SQLPATTERN *)(r->sp_with->w_subArray[i].ws_subquery));
		addStr(&tmp,p,&len,&limit);
		FB_FREE(p);
		addStr(&tmp," ) ",&len,&limit);
	}
	/*select list*/
	addStr( &tmp, "SELECT ", &len, &limit);
	addStr( &tmp, seltype[r->sp_selectlist->sel_type], &len, &limit);
	addStr( &tmp, " ", &len, &limit);
	for( i=0; i<r->sp_selectlist->sel_count-1; i++)
	{
		addStr( &tmp, r->sp_selectlist->sel_list[i].sb_value, &len, &limit);
		addStr( &tmp, " ", &len, &limit);
		addStr( &tmp, r->sp_selectlist->sel_list[i].sb_alias, &len, &limit);
		addStr( &tmp, ", ", &len, &limit);
	}
	addStr( &tmp, r->sp_selectlist->sel_list[i].sb_value, &len, &limit);
	addStr( &tmp, " ", &len, &limit);
	addStr( &tmp, r->sp_selectlist->sel_list[i].sb_alias, &len, &limit);
	/*printf("get a seleclist %s\n",tmp);*/
	/*from clause*/
	addStr( &tmp, " FROM ", &len, &limit);
	/*printf("get a seleclist %s\n",tmp);*/
	p=getTBRefStr(r->sp_fromlist);
	addStr( &tmp, p, &len, &limit);
	FB_FREE(p);

	return tmp;
}
/*
 * acquire the total number of table_ref
 */
int sql_parse_getTabNum( SQLPATTERN *r )
{
	return (r->sp_fromlist->ts_count);
}
/*---------------------------
  |加入时间限制的sql语句    |
  \------------------------*/
/*
 * 输入：
 * 	start_time, end_time - 如果start_time[0]=='\0'或end_time[0]=='\0',
 	表明对应的时刻是没有要求的，即：在where后不需要加入相应项
 	不能同时满足，因为同时满足，就是调用
 			jn_db_getRowidSQL()函数了；
 */
/*
 * Info:
 	Generate the sql as: select rowid ...
 * Version:
 	use the newst sql generating functions
 			-check by Szy. 2006/01/20
 */
char *jn_db_timeRowidSQL(  SQLPATTERN *sp, char *start_time, char *end_time )
{
	char *out, *atom;
	int baselen, len;

	if( sp == NULL )
		return NULL;
	atom = (char *)FB_MALLOC(sizeof(char)*MAX_SQL_SIZE);
	out = atom;
	if( out == NULL )
		return out;
	baselen = 0;
	//with subquery
	if( sp->sp_with )
	{
		if( with2str( sp->sp_with, out, &baselen )<0 )
			return atom;
		out += baselen;
	}
	//select list
	/*--------------------------------------------*/
	/* here the select_list was instead of rowid */
	/*-------------------------------------------*/
	if( baselen + 13 >= MAX_SQL_SIZE )
		return atom;
	baselen += 13;
	strcpy( out, "SELECT ROWID ");
	out += 13;
	//from list
	len = baselen;
	if( tbfs2str( sp->sp_fromlist, out, &baselen )<0 )
		return atom;
	out += baselen-len;
	//where condition
	if( strlen( sp->time_col->col_name ) == 0 )
	{
		if( sp->sp_whereclause )
		{
			if( baselen + 6 >= MAX_SQL_SIZE )
			return atom;
			baselen += 6;
			strcpy( out, "WHERE " );
			out += 6;
			len = baselen;
			if( stem_con2str( sp->sp_whereclause->qt_cs, out, &baselen ) < 0 )
				return atom;
			if( baselen == len )
			{
				baselen -= 6;
				out -= 6;
				memset( out, 0, 6 );
			}
			else
			{
				out += baselen - len;
			}
		}
		goto CONN;
	}
	if( sp->sp_whereclause )
	{
		if( baselen + 8 >= MAX_SQL_SIZE )
			return atom;
		baselen += 8;
		strcpy( out, "WHERE ( " );
		out += 8;
		len = baselen;
		if( stem_con2str( sp->sp_whereclause->qt_cs, out, &baselen ) < 0 )
			return atom;
		if( baselen == len )
		{
			baselen -= 2;
			out -= 2;
			memset( out, 0, 2 );
		}
		else
		{
			out += baselen - len;
			if( baselen+7 >MAX_SQL_SIZE )
				return atom;
			baselen += 7;
			strcpy( out, " ) AND " );
			out += 7;
		}
	}
	else if( start_time[0] != '\0' || end_time[0] != '\0' )
	{
		if( baselen + 6 >= MAX_SQL_SIZE )
			return atom;
		baselen += 6;
		strcpy( out, "WHERE " );
		out += 6;
	}
	//加入时间条件
	if( start_time[0] != '\0' )
	{
		len = strlen( sp->time_col->col_name );
		if( baselen + len >= MAX_SQL_SIZE )
			return atom;
		baselen += len;
		strcpy( out, sp->time_col->col_name);
		out += len;
		if( !strcmp( sp->query_time->stime, start_time ) )
		{
			len = strlen( sp->query_time->scompare );
			if( baselen + len >= MAX_SQL_SIZE )
				return atom;
			baselen += len;
			strcpy( out, sp->query_time->scompare);
			out += len;
		}
		else
		{
			if( baselen + 2 >= MAX_SQL_SIZE )
				return atom;
			baselen += 2;
			strcpy( out, ">=");
			out += 2;
		}

		len = strlen( start_time );
		if( baselen + len + 9 >= MAX_SQL_SIZE )
			return atom;
		baselen += len + 9;
		strcpy( out, "TO_DATE(\'");
		out += 9;
		strcpy( out, start_time);
		out += len;

		if( end_time[0] != '\0' )
		{
			len = strlen( "\', \'YYYY-MM-DD hh24:mi:ss\') AND " );
			if( baselen +  len >= MAX_SQL_SIZE )
				return atom;
			baselen += len;
			strcpy( out, "\', \'YYYY-MM-DD hh24:mi:ss\') AND ");
			out += len;
		}
		else
		{
			len = strlen( "\', \'YYYY-MM-DD hh24:mi:ss\') " );
			if( baselen +  len >= MAX_SQL_SIZE )
				return atom;
			baselen += len;
			strcpy( out, "\', \'YYYY-MM-DD hh24:mi:ss\') ");
			out += len;
		}
	}
	if( end_time[0] != '\0' )
	{
		len = strlen( sp->time_col->col_name );
		if( baselen + len >= MAX_SQL_SIZE )
			return atom;
		baselen += len;
		strcpy( out, sp->time_col->col_name);
		out += len;
		if( !strcmp( sp->query_time->etime, end_time ) )
		{
			len = strlen( sp->query_time->ecompare );
			if( baselen + len >= MAX_SQL_SIZE )
				return atom;
			baselen += len;
			strcpy( out, sp->query_time->ecompare );
			out += len;
		}
		else
		{
			if( baselen + 1 >= MAX_SQL_SIZE )
				return atom;
			baselen ++;
			strcpy( out, "<");
			out ++;
		}

		len = strlen( end_time );
		if( baselen + len + 9 + strlen( "\', \'YYYY-MM-DD hh24:mi:ss\') " )>= MAX_SQL_SIZE )
			return atom;
		baselen += len + 9 + strlen( "\', \'YYYY-MM-DD hh24:mi:ss\') " );
		strcpy( out, "TO_DATE(\'");
		out += 9;
		strcpy( out, end_time);
		out += len;
		strcpy( out, "\', \'YYYY-MM-DD hh24:mi:ss\') ");
		out += strlen( "\', \'YYYY-MM-DD hh24:mi:ss\') ");
	}
	CONN:
	//connect clause
	if( sp->sp_connectclause != NULL )
	{
		if( sp->sp_connectclause->hq_start!=NULL )
		{
			if( baselen + 11 >= MAX_SQL_SIZE )
				return atom;
			baselen += 11;
			strcpy( out, "START WITH ");
			out += 11;
			len = baselen;
			if( condition2str( sp->sp_connectclause->hq_start, out, &baselen ) < 0 )
				return atom;
			out += baselen-len;
		}
		if( sp->sp_connectclause->hq_type==0)
		{
			if( baselen + 12 >= MAX_SQL_SIZE )
				return atom;
			baselen += 12;
			strcpy( out, " CONNECT BY ");
			out += 12;
		}
		else
		{
			if( baselen + 20 >= MAX_SQL_SIZE )
				return atom;
			baselen += 20;
			strcpy( out, " CONNECT BY NOCYCLE ");
			out += 20;
		}

		len = baselen;
		if( condition2str( sp->sp_connectclause->hq_connect, out, &baselen ) < 0 )
			return atom;
		out += baselen - len;
		if( baselen + 1 >= MAX_SQL_SIZE )
			return atom;
		baselen += 1;
		strcpy( out, " ");
		out ++;
	}
	//group clause
	if( sp->sp_groupbyclause )
	{
		len = baselen;
		if( group2str( sp->sp_groupbyclause, out, &baselen ) < 0 )
			return atom;
		out += baselen-len;
	}
	//having
	if( sp->sp_havingclause!=NULL )
	{
		if( baselen + 7 >= MAX_SQL_SIZE )
			return atom;
		baselen += 7;
		strcpy( out, "HAVING ");
		out += 7;
		len = baselen;
		if( condition2str( sp->sp_havingclause->qt_cs, out, &baselen )<0 )
			return atom;
		out += baselen-len;
	}
	//order by clause
	if( sp->sp_orderbyclause )
	{
		len = baselen;
		if( order2str( sp->sp_orderbyclause, out, &baselen )<0 )
			return atom;
		out += baselen-len;
	}
	/*for update clause*/
	if( sp->sp_updateclause != NULL )
	{
		strcpy( out, sp->sp_updateclause );
	}

	return atom;
}
/*--------------------------------------
 *  计算受到sql命令最大长度的限制，条件中 §
 * rowid:=xxx的最大个数                   §
 *----------------------------------------*/
int lenOfGroupStr( GroupField *gf)
{
	int len;

	if( !gf )
		return 0;

	len = strlen(" GROUP BY ");
	len += strlen( gf->gcol );

	gf = gf->gnext;
	while( gf )
	{
		len += 2 + strlen( gf->gcol ); //strlen(", ")
	}

	return len;
}
int lenOfOrderStr( OrderCla *ocla )
{
	int len;
	OrderField *ofield;

	if( !ocla )
		return 0;

	if( ocla->oflag == 0 )
	{
		len = strlen(" ORDER BY ");
	}
	else
	{
		len = strlen(" ORDER SIBLINGS BY ");
	}

	ofield = ocla->ofield;
	len += strlen( ofield->ocol);
	if( ofield->oasc == 1 )
		len += strlen( " ASC" );
	else if( ofield->oasc == 2 )
		len += strlen( " DESC" );

	if( ofield->onull == 1 )
		len += strlen( " NULLS FIRST" );
	else if( ofield->onull == 2 )
		len += strlen( " NULLS LAST" );
	ofield = ofield->onext;
	while( ofield )
	{
		len += 2 + strlen( ofield->ocol);
		if( ofield->oasc == 1 )
			len += strlen( " ASC" );
		else if( ofield->oasc == 2 )
			len += strlen( " DESC" );

		if( ofield->onull == 1 )
			len += strlen( " NULLS FIRST" );
		else if( ofield->onull == 2 )
			len += strlen( " NULLS LAST" );
		ofield = ofield->onext;
	}

	return len;
}

int lenOfExceptWhere (  SQLPATTERN *r )
{
	int len;
	char *p;
	char *seltype[4]={" ","DISTINCT","UNIQUE","ALL"};
	int i;

	/*with subquery*/
	len = 0;
	if(r->sp_with)
	{
		len += strlen( "WITH ");
		for( i=0; i<r->sp_with->w_count-1; i++)
		{
			len += strlen( r->sp_with->w_subArray[i].ws_id );
			len += strlen( " AS ( " );
			p=getQueryStr((SQLPATTERN *)(r->sp_with->w_subArray[i].ws_subquery));
			len += strlen( p );
			FB_FREE(p);
			len += strlen( " ), " );
		}
		len += strlen( r->sp_with->w_subArray[i].ws_id );
		len += strlen( " AS ( " );
		p=getQueryStr((SQLPATTERN *)(r->sp_with->w_subArray[i].ws_subquery));
		len += strlen( p );
		FB_FREE(p);
		len += strlen( " ) " );
	}
	/*select list*/
	len += strlen( "SELECT " );
	len += strlen( seltype[r->sp_selectlist->sel_type] ) + 1;
	for( i=0; i<r->sp_selectlist->sel_count-1; i++)
	{
		len += strlen( r->sp_selectlist->sel_list[i].sb_value ) + 1;
		len += strlen( r->sp_selectlist->sel_list[i].sb_alias ) + 2;//", "
	}
	len += strlen( r->sp_selectlist->sel_list[i].sb_value ) + 1;
	len += strlen( r->sp_selectlist->sel_list[i].sb_alias );
	/*printf("get a seleclist %s\n",tmp);*/
	/*from clause*/
	len += strlen( " FROM " );
	/*printf("get a seleclist %s\n",tmp);*/
	p=getTBRefStr(r->sp_fromlist);
	len += strlen(p);
	FB_FREE(p);
	//group by
	len += lenOfGroupStr( r->sp_groupbyclause );
	//having clause
	if( r->sp_havingclause!=NULL )
	{
		len += strlen( " HAVING " );
		p=getConstr(r->sp_havingclause->qt_cs);
		len += strlen(p);
		FB_FREE(p);
	}
	//order clause
	len += lenOfOrderStr( r->sp_orderbyclause );

	return len;
}
//#define MAX_SQL_SIZE		4000
//#define ROWID_LEN		18
int bindRowidNums( SQLPATTERN *r )
{
	int batsize, rlen;

	//strlen( " AND ROWID=:xxx" )=15, strlen(" Where ") = 7
	batsize =( MAX_SQL_SIZE - lenOfExceptWhere( r ) - 8 )/( 16 );

	return batsize;
}
/*----------------------------------------------------------------
 * 函数：从配置文件中读取关于表和对应时间列的信息
 *-----------------------------------------------------------------
 * 输入：
 *	file_name - 配置文件名字
 * 输出：
 *	tcol - 传出一个生成的TimeCol数组
 * 返回：
 *	读取配置项的总数目
 */
int readTimeCol( const char *file_name, TimeCol **tcol )
{
	int counter;
	TimeCol *ltcol, *tmp;
	FILE *fp;
	char buf[128],w1[64],w2[64];
	int i, j, wordnum;

	counter = 0;
	ltcol = NULL;
	fp = fopen( file_name, "r" );
	if( !fp ) return counter;
	//printf("open file %s\n", file_name );
	while( !feof( fp ) )
	{
		fgets( buf, 128, fp );
		//printf("read %s\n", buf);
		wordnum = 0;
		//scan the 1st word in row
		for( i=0; (buf[i] == ' '||buf[i] == '\t') && buf[i]!='\n'; i++ );
		for( j=0; buf[i] != ' ' && buf[i]!='\n'&& buf[i]!='\t'; j++, i++ )
			w1[j]=buf[i];
		w1[j]='\0';
		//一行之中只有一项
		if( buf[i] == '\n' )	continue;
		else wordnum=1;
		//scan 2nd word in row
		for( ; (buf[i] == ' '||buf[i] == '\t') && buf[i]!='\n'; i++ );
		for( j=0; buf[i] != ' ' && buf[i]!='\n'&& buf[i]!='\t'; j++, i++ )
			w2[j]=buf[i];
		w2[j]='\0';
		if( j == 0 )	continue;
		else wordnum=2;
		counter++;
		//new a TimeCol and save it;
		if( counter == 1 )
		{
			ltcol = (TimeCol *)FB_MALLOC(sizeof(TimeCol));
		}
		else
		{
			tmp = ltcol;
			ltcol =( TimeCol *) FB_REALLOC( tmp, counter*sizeof(TimeCol) );
		}
		strcpy( ltcol[counter-1].table_name, w1 );
		strcpy( ltcol[counter-1].col_name, w2 );
		//printf(" table: %s\tcolumn: %s\n", ltcol[counter-1].table_name, ltcol[counter-1].col_name);
	}
	*tcol = ltcol;
	fclose(fp);
	return counter;
}
void sql_parse_init( char *file_name )
{
	tcol_nums = readTimeCol( file_name, &tcol );
}
void sql_parse_destroy( )
{
	if( tcol )
		FB_FREE( tcol );
}
/*
 * 另一种获得整理后字符串的方法：
 *	与以前比较，减少了malloc和free的次数，提高了效率
 * 	update by szy
 		2005-12-13
 *
 */
int basic2str( BASICSTRU *bs, char out[], int *baselen )
{
	int len;

	//为空或者是剥离的时间条件
	if( !bs || bs->bs_type == 3 )
		return 0;
	if(  bs->bs_type > 99 )
	{
		KeyNode *kn;
		//CONTAINS( colname, '' )
		kn = (KeyNode *)(bs->bs_value);
		len = 19 +strlen( bs->bs_colname )+strlen(bs->bs_oper)+ strlen( kn->words )+strlen(kn->value);
		//存储容量不够
		ADD_LEN( baselen, len );
		sprintf( out, " CONTAINS( %s,'%s' ) %s %s ",
			bs->bs_colname,
			kn->words,
			bs->bs_oper,
			kn->value );
		return 0;
	}
	// if( bs->bs_type == 1,2 )
	len = strlen( bs->bs_colname ) + 1;
	//存储容量不够
	ADD_LEN( baselen, len );
	strcpy( out, bs->bs_colname );
	out[len-1] = ' ';
	out += len;
	//add oper
	len = strlen( bs->bs_oper );
	//存储容量不够
	ADD_LEN( baselen, len );
	strcpy( out, bs->bs_oper );
	out += len;

	//add value
	if( bs->bs_type == 1 )
	{
		len = strlen( bs->bs_value ) + 2;
		//存储容量不够
		ADD_LEN( baselen, len );
		out[0] = ' ';
		strcpy( out+1, (char*)bs->bs_value );
		*(out+len-1) = ' ';
	}
	else//subquery exist
	{
		//存储容量不够
		ADD_LEN( baselen, 2 );
		strcpy( out, " (" );
		out += 2;
		//subquery str
		if( sqlpat2str( (SQLPATTERN*)bs->bs_value, out, baselen ) < 0 )
			return -1;
		//存储容量不够
		ADD_LEN( baselen, 2 );
		strcpy( out, ") " );
	}

	return 0;
}
//
//这是用来从条件树结构中获得字符串命令的一个递归遍历程序
//
int condition2str( COMBISTRU   *rr, char out[], int *baselen )
{
	if(rr==NULL)
		return 0;

	if( rr->cs_combi == 0 )		//原子结构
	{
		if( basic2str( rr->cs_leftbs, out, baselen ) < 0 )
			return -1;
	}
	else if( rr->cs_combi == 1 )	//括号（）
	{
		int tlen;

		ADD_LEN( baselen, 3 );
		strcpy( out, " ( " );
		tlen = *baselen;
		out += 3;
		if( condition2str( rr->cs_leftcs, out, baselen ) < 0 )
			return -1;
		if( *baselen == tlen )
		{
			out -= 2;
			out[0]='\0';
			out[1]='\0';
			*baselen -= 2;
		}
		else
		{
			out += *baselen - tlen;
			ADD_LEN( baselen, 2 );
			strcpy( out, ") " );
		}
	}
	else if( rr->cs_combi == 2 )	//取反
	{
		int tlen1;

		ADD_LEN( baselen, 4 );
		strcpy( out, "NOT " );
		out += 4;
		tlen1 = *baselen;

		if( rr->cs_lefttype == 0 )
		{
			if( basic2str( rr->cs_leftbs, out, baselen ) < 0 )
				return -1;
		}
		else
		{
			if( condition2str( rr->cs_leftcs, out, baselen ) < 0 )
				return -1;
		}
		if( *baselen == tlen1 )
		{
			out -= 4;
			memset( out, 0, 4 );
			*baselen -= 4;
		}
	}
	else	//and or
	{
		int tlen2;

		tlen2 = *baselen;
		if( ( rr->cs_lefttype == 0
			&& basic2str( rr->cs_leftbs, out, baselen ) < 0 )
		|| ( rr->cs_lefttype == 1
			&& condition2str( rr->cs_leftcs, out, baselen ) < 0 )
		)
			return -1;
		if( tlen2 == *baselen )		//left child EQ to "" string
		{
			if( ( rr->cs_righttype == 0
				&& basic2str( rr->cs_rightbs, out, baselen ) < 0 )
			|| ( rr->cs_righttype == 1
				&& condition2str( rr->cs_rightcs, out, baselen ) < 0 )
			)
				return -1;
		}
		else
		{
			out = out + ( *baselen - tlen2 );

			switch( rr->cs_combi )
			{
				case 3:
					ADD_LEN( baselen, 5 );
					strcpy( out, " AND " );
					out += 5;
					break;
				case 4:
					ADD_LEN( baselen, 4 );
					strcpy( out, " OR " );
					out += 4;
					break;
				default:
					break;
			}
			tlen2 = *baselen;
			if( ( rr->cs_righttype == 0
				&& basic2str( rr->cs_rightbs, out, baselen ) < 0 )
			|| ( rr->cs_righttype == 1
				&& condition2str( rr->cs_rightcs, out, baselen ) < 0 )
			)
				return -1;
			if( tlen2 == *baselen )
			{
				if( rr->cs_combi == 3 )
				{
					out -= 5;
					memset( out, 0, 5 );
					*baselen -= 5;
				}
				else if( rr->cs_combi == 4 )
				{
					out -= 4;
					memset( out, 0, 4 );
					*baselen -= 4;
				}
			}
		}
	}
	return 0;
}
//
//去除条件字符串的外层括号（因为这层括号是无意义的）
//
int stem_con2str( COMBISTRU   *rr, char out[], int *baselen )
{
	if( rr == NULL )
		return -1;

	if( ( rr->cs_combi == 0
		&& condition2str( rr, out, baselen ) < 0 )
	||  ( rr->cs_combi == 1
		&& stem_con2str( rr->cs_leftcs, out, baselen) < 0 )
	||  ( rr->cs_combi == 2
		&& condition2str( rr, out, baselen ) < 0 )
	||   rr->cs_combi < 0
	)
		return -1;
	else if( rr->cs_combi >= 3 )	//AND OR
	{
		int tlen;

		if( rr->cs_lefttype == 1 && rr->cs_leftcs->cs_combi == 1
		&& rr->cs_righttype == 1 && rr->cs_rightcs->cs_combi == 1 )
		{
			tlen = *baselen;
			if( condition2str( rr->cs_leftcs, out, baselen ) < 0 )
				return -1;
			if( tlen == *baselen )
			{
				if( stem_con2str( rr->cs_rightcs, out, baselen) < 0 )
					return -1;
			}
			else
			{
				int tlen1;

				out += *baselen - tlen;
				if( rr->cs_combi == 3 )
				{
					ADD_LEN( baselen, 5 );
					strcpy( out, " AND " );
					out += 5;
				}
				else
				{
					ADD_LEN( baselen, 4 );
					strcpy( out, " OR " );
					out += 4;
				}
				tlen1 = *baselen;
				if( condition2str( rr->cs_rightcs, out, baselen ) < 0 )
					return -1;
				if( tlen1 == *baselen )
				{
					out -= ( tlen1 - tlen );
					memset( out, 0, tlen1-tlen );
					*baselen = tlen;
					//if 右子树对应空串，重新组织左子树串
					if( stem_con2str( rr->cs_leftcs, out, baselen) < 0 )
						return -1;
				}
			}
		}
		else if( rr->cs_lefttype == 1 && rr->cs_leftcs->cs_combi == 1 )
		{
			tlen = *baselen;
			if( ( rr->cs_righttype == 1
				&& condition2str( rr->cs_rightcs, out, baselen ) < 0 )
			|| ( rr->cs_righttype == 0
				&& basic2str( rr->cs_rightbs, out, baselen ) < 0 )
			)
				return -1;

			if( tlen == *baselen )
			{
				if( stem_con2str( rr->cs_leftcs, out, baselen ) < 0 )
					return -1;
			}
			else
			{
				//重新遍历整个树
				memset( out, 0, *baselen - tlen );
				*baselen = tlen;
				if( condition2str( rr, out, baselen ) < 0 )
					return -1;
			}
		}
		else if( rr->cs_righttype == 1 && rr->cs_rightcs->cs_combi == 1 )
		{
			tlen = *baselen;
			if( ( rr->cs_lefttype == 1
				&& condition2str( rr->cs_leftcs, out, baselen ) < 0 )
			|| ( rr->cs_lefttype == 0
				&& basic2str( rr->cs_leftbs, out, baselen ) < 0 )
			)
				return -1;
			if( tlen == *baselen )
			{
				if( stem_con2str( rr->cs_rightcs, out, baselen ) < 0 )
					return -1;
			}
			else
			{
				out += *baselen - tlen;
				if( rr->cs_combi == 3 )
				{
					ADD_LEN( baselen, 5 );
					strcpy( out, " AND " );
					out += 5;
				}
				else
				{
					ADD_LEN( baselen, 4 );
					strcpy( out, " OR " );
					out += 4;
				}
				tlen = *baselen;
				if( condition2str( rr->cs_rightcs, out, baselen ) < 0 )
					return -1;
				if( tlen == *baselen )
				{
					if( rr->cs_combi == 3 )
					{
						out -= 5;
						*baselen -= 5;
						memset( out, 0, 5 );
					}
					else
					{
						out -= 4;
						*baselen -= 4;
						memset( out, 0, 4 );
					}
				}
			}
		}
		else if( condition2str( rr, out, baselen ) < 0 )
			return -1;
	}
	return 0;
}
//
//转换获得with子句
int with2str( WITHSTRU *ws, char out[], int *baselen )
{
	int i, len;

	ADD_LEN( baselen, 5 );
	strcpy( out, "WITH " );
	out += 5;

	for( i=0; i<ws->w_count-1; i++)
	{
		len = strlen( ws->w_subArray[i].ws_id );
		ADD_LEN( baselen, len+6 );
		strcpy( out, ws->w_subArray[i].ws_id );
		out += len;
		strcpy( out, " AS ( " );
		out += 6;
		len = *baselen;
		if( sqlpat2str( (SQLPATTERN *)(ws->w_subArray[i].ws_subquery), out, baselen ) < 0 )
			return -1;
		out += *baselen - len;
		ADD_LEN( baselen, 4 );
		strcpy( out," ), ");
	}
	len = strlen( ws->w_subArray[i].ws_id );
	ADD_LEN( baselen, len+6 );
	strcpy( out, ws->w_subArray[i].ws_id );
	out += len;
	strcpy( out, " AS ( " );
	out += 6;
	len = *baselen;
	if( sqlpat2str( (SQLPATTERN *)(ws->w_subArray[i].ws_subquery), out, baselen ) < 0 )
		return -1;
	out += *baselen - len;
	ADD_LEN( baselen, 3 );
	strcpy( out," ) ");

	return 0;
}
int sellist2str( SELECTLIST *sel, char out[], int *baselen )
{
	char *seltype[4]={"","DISTINCT ","UNIQUE ","ALL "};
	int len, i;

	ADD_LEN( baselen, 7 );
	strcpy( out, "SELECT " );
	out += 7;

	len = strlen( seltype[sel->sel_type] );
	ADD_LEN( baselen, len );
	strcpy( out, seltype[sel->sel_type] );
	out += len;
	for( i=0; i<sel->sel_count-1; i++)
	{
		len = strlen( sel->sel_list[i].sb_value );
		ADD_LEN( baselen, len );
		strcpy( out, sel->sel_list[i].sb_value );
		out += len;
		len = strlen( sel->sel_list[i].sb_alias );
		if( len > 0 )
		{
			ADD_LEN( baselen, len+1 );
			strcpy( out, " ");
			out ++;
			strcpy( out, sel->sel_list[i].sb_alias );
			out += len;
		}
		ADD_LEN( baselen, 2 );
		strcpy( out, ", ");
		out += 2;
	}

	len = strlen( sel->sel_list[i].sb_value );
	ADD_LEN( baselen, len+1 );
	strcpy( out, sel->sel_list[i].sb_value );
	out += len;
	strcpy( out, " ");
	out ++;

	len = strlen( sel->sel_list[i].sb_alias );
	ADD_LEN( baselen, len+1 );
	strcpy( out, sel->sel_list[i].sb_alias );
	out += len;
	strcpy( out, " ");

	return 0;
}
int tbfield2str( TBfield *r, char out[], int *baselen )
{
	int len;

	if( r==NULL )
		return -1;

	if( r->tf_type == 0 )/*meat atom*/
	{
		if( r->tf_atom->tb_type == 0)/* no 'ONLY'*/
		{
			if( r->tf_atom->tb_head->qt_subquery != NULL )
			{
				ADD_LEN( baselen, 2 );
				strcpy( out, " (" );
				out += 2;

				len = *baselen;
				if( 0 > sqlpat2str( (SQLPATTERN*)(r->tf_atom->tb_head->qt_subquery), out, baselen ) )
					return -1;
				out += *baselen - len;

				len = strlen( r->tf_atom->tb_head->qt_value );
				ADD_LEN( baselen, len + 3 );
				strcpy( out, " ");
				out ++;
				strcpy( out, r->tf_atom->tb_head->qt_value);
				out += len;
				strcpy( out, ") " );
				out += 2;
			}
			else
			{
				len = strlen( r->tf_atom->tb_head->qt_value );
				ADD_LEN( baselen, len + 1 );
				strcpy( out, r->tf_atom->tb_head->qt_value);
				out += len;
				strcpy( out, " " );
				out += 1;
			}
		}
		else /*with 'ONLY'*/
		{
			ADD_LEN( baselen, 8 );
			strcpy( out, " ONLY ( ");
			out += 8;
			if( r->tf_atom->tb_head->qt_subquery != NULL )
			{
				len = *baselen;
				if( 0 > sqlpat2str( (SQLPATTERN*)(r->tf_atom->tb_head->qt_subquery), out, baselen ) )
					return -1;
				out += *baselen - len;
				ADD_LEN( baselen, 1 );
				strcpy( out, " ");
			}
			len = strlen( r->tf_atom->tb_head->qt_value );
			ADD_LEN( baselen, len + 2 );
			strcpy( out, r->tf_atom->tb_head->qt_value);
			out += len;
			strcpy( out, ") " );
			out += 2;
		}
		len = strlen( r->tf_atom->tb_tail );
		ADD_LEN( baselen, len);
		strcpy( out, r->tf_atom->tb_tail);
	}
	else/*join */
	{
		len = *baselen;
		if( tbfield2str( r->tf_join->jt_left, out, baselen ) < 0 )
			return -1;
		out += *baselen - len;
		len = strlen( r->tf_join->jt_op );
		ADD_LEN( baselen, len +2 );
		strcpy( out, " ");
		++ out;
		strcpy( out, r->tf_join->jt_op);
		out += len;
		strcpy( out, " ");
		++ out;

		len = *baselen;
		if( tbfield2str( r->tf_join->jt_right, out, baselen ) < 0 )
			return -1;
		out += *baselen -len;
		if( r->tf_join->jt_condition != NULL )
		{
			ADD_LEN( baselen, 3 );
			strcpy( out, "ON ");
			out += 3;
			len = *baselen;
			if( condition2str( r->tf_join->jt_condition, out, baselen )<0)
				return -1;
			out += *baselen - len;
			ADD_LEN( baselen, 1 );
			strcpy( out, " ");
			out ++;
		}
		len = strlen( r->tf_join->jt_using );
		ADD_LEN( baselen, len );
		strcpy( out, r->tf_join->jt_using );
	}
	/*printf("ddfield:%s\tlen:%d\n",tmp,strlen(tmp));*/
	return 0;
}
int tbfs2str( TBFS *tb, char out[], int *baselen )
{
	int i, len;

	if( tb==NULL )
		return -1;
	ADD_LEN( baselen, 5 );
	strcpy( out, "FROM " );
	out += 5;
	for( i=0; i<tb->ts_count-1; i++ )
	{
		len = *baselen;
		if( tbfield2str( &(tb->ts_field[i]), out, baselen ) < 0 )
			return -1;
		out += *baselen-len;
		ADD_LEN( baselen, 2 );
		strcpy( out, ", " );
		out += 2;
	}
	len = *baselen;
	if( tbfield2str( &(tb->ts_field[i]), out, baselen ) < 0 )
		return -1;
	out += *baselen-len;
	ADD_LEN( baselen, 1 );
	strcpy( out, " " );

	return 0;
}
int group2str( GroupField *gf, char out[], int *baselen )
{
	int len;

	if( gf == NULL )
		return -1;
	len = strlen(gf->gcol);
	ADD_LEN( baselen, 9+len );
	strcpy( out, "GROUP BY ");
	out += 9;
	strcpy( out, gf->gcol );
	out += len;
	gf = gf->gnext;
	while( gf )
	{
		len = strlen( gf->gcol );
		ADD_LEN( baselen, len+2 );
		strcpy( out, ", ");
		out += 2;
		strcpy( out, gf->gcol);
		out += len;
		gf = gf->gnext;
	}
	ADD_LEN( baselen, 1 );
	strcpy( out, " ");

	return 0;
}
int order2str( OrderCla *ord, char out[], int *baselen )
{
	OrderField *ofield;
	int len;

	if( ord == NULL )
		return -1;

	if( ord->oflag == 0 )
	{
		ADD_LEN( baselen, 9 );
		strcpy( out, "ORDER BY ");
		out += 9;
	}
	else
	{
		ADD_LEN( baselen, 18 );
		strcpy( out, "ORDER SIBLINGS BY ");
		out += 18;
	}

	ofield = ord->ofield;
	len = strlen( ofield->ocol );
	strcpy( out, ofield->ocol );
	out += len;
	if( ofield->oasc == 1 )
	{
		ADD_LEN( baselen, 4 );
		strcpy( out, " ASC");
		out += 4;
	}
	else if( ofield->oasc == 2 )
	{
		ADD_LEN( baselen, 5 );
		strcpy( out, " DESC");
		out += 5;
	}

	if( ofield->onull == 1 )
	{
		ADD_LEN( baselen, 12 );
		strcpy( out, " NULLS FIRST");
		out += 12;
	}
	else if( ofield->onull == 2 )
	{
		ADD_LEN( baselen, 11 );
		strcpy( out, " NULLS LAST");
		out += 11;
	}

	ofield = ofield->onext;
	while( ofield )
	{
		len = strlen( ofield->ocol );
		ADD_LEN( baselen, len +2 );
		strcpy( out, ", ");
		out += 2;
		strcpy( out, ofield->ocol );
		out += len;
		if( ofield->oasc == 1 )
		{
			ADD_LEN( baselen, 4 );
			strcpy( out, " ASC");
			out += 4;
		}
		else if( ofield->oasc == 2 )
		{
			ADD_LEN( baselen, 5 );
			strcpy( out, " DESC");
			out += 5;
		}

		if( ofield->onull == 1 )
		{
			ADD_LEN( baselen, 12 );
			strcpy( out, " NULLS FIRST");
			out += 12;
		}
		else if( ofield->onull == 2 )
		{
			ADD_LEN( baselen, 11 );
			strcpy( out, " NULLS LAST");
			out += 11;
		}

		ofield = ofield->onext;
	}
	ADD_LEN( baselen, 1 );
	strcpy( out, " ");

	return 0;
}
int sql_noselect( SQLPATTERN *sp, char out[], int *baselen )
{
	int len;
	//from list
	len = *baselen;
	if( tbfs2str( sp->sp_fromlist, out, baselen )<0 )
		return -1;
	out += *baselen-len;
	//where condition
	if( sp->sp_whereclause )
	{
		ADD_LEN( baselen, 6 );
		strcpy( out, "WHERE " );
		out += 6;
		len =*baselen;
		if( stem_con2str( sp->sp_whereclause->qt_cs, out, baselen ) < 0 )
			return -1;
		if( *baselen == len )
		{
			out-=6;
			memset( out, 0, 6 );
			*baselen -= 6;
		} 
		else
			out += *baselen - len;
	}
	//connect clause
	if( sp->sp_connectclause != NULL )
	{
		if( sp->sp_connectclause->hq_start!=NULL )
		{
			ADD_LEN( baselen, 11 );
			strcpy( out, "START WITH ");
			out += 11;
			len = *baselen;
			if( condition2str( sp->sp_connectclause->hq_start, out, baselen ) < 0 )
				return -1;
			out += *baselen-len;
		}
		if( sp->sp_connectclause->hq_type==0)
		{
			ADD_LEN( baselen, 12 );
			strcpy( out, " CONNECT BY ");
			out += 12;
		}
		else
		{
			ADD_LEN( baselen, 20 );
			strcpy( out, " CONNECT BY NOCYCLE ");
			out += 20;
		}

		len = *baselen;
		if( condition2str( sp->sp_connectclause->hq_connect, out, baselen ) < 0 )
			return -1;
		out += *baselen - len;
		ADD_LEN( baselen, 1 );
		strcpy( out, " ");
		out ++;
	}
	//group clause
	if( sp->sp_groupbyclause )
	{
		len = *baselen;
		if( group2str( sp->sp_groupbyclause, out, baselen ) < 0 )
			return -1;
		out += *baselen-len;
	}
	//having
	if( sp->sp_havingclause!=NULL )
	{
		ADD_LEN( baselen, 7 );
		strcpy( out, "HAVING ");
		out += 7;
		len = *baselen;
		if( condition2str( sp->sp_havingclause->qt_cs, out, baselen )<0 )
			return -1;
		out += *baselen-len;
	}
	//order by clause
	if( sp->sp_orderbyclause )
	{
		len = *baselen;
		if( order2str( sp->sp_orderbyclause, out, baselen )<0 )
			return -1;
		out += *baselen-len;
	}
	/*for update clause*/
	if( sp->sp_updateclause != NULL )
	{
		strcpy( out, sp->sp_updateclause );
	}
	return 0;	
}
/*
 * convert SQLPATTERN to a unique string
 */
int sqlpat2str( SQLPATTERN *sp, char out[], int *baselen )
{
	int len;

	if( sp == NULL )
		return -1;
	//with subquery
	if( sp->sp_with )
	{
		len = *baselen;
		if( with2str( sp->sp_with, out, baselen )<0 )
			return -1;
		out += *baselen-len;
	}
	//select list
	len = *baselen;
	if( sellist2str( sp->sp_selectlist, out, baselen )<0 )
		return -1;
	out += *baselen-len;
	
	if( sql_noselect( sp, out, baselen ) <0 )
		return -1;

	return 0;
}
//select rowid from .. where ... group ..
int sql_selectROWID( SQLPATTERN *sp, char out[], int *baselen )
{
	int len;
	if( sp == NULL )
		return -1;
	//with subquery
	if( sp->sp_with )
	{
		len = *baselen;
		if( with2str( sp->sp_with, out, baselen )<0 )
			return -1;
		out += *baselen-len;
	}
	//select list
	//len = *baselen;
	strcpy( out, "SELECT ROWID ");
	out += 13;
	*baselen += 13;
	
	if( sql_noselect( sp, out, baselen ) <0 )
		return -1;

	return 0;
}

//select any you set
int sqlField( SQLPATTERN *sp, const char *field, char out[], int *baselen )
{
	int len, len1;
	
	if( sp == NULL )
		return -1;
	//with subquery
	if( sp->sp_with )
	{
		len = *baselen;
		if( with2str( sp->sp_with, out, baselen )<0 )
			return -1;
		out += *baselen-len;
	}
	//select list
	//len = *baselen;
	//strcpy( out, "SELECT ROWID ");
	strcpy( out, field );
	len1 = strlen( field );
	out += len1;
	*baselen += len1;
	
	if( sql_noselect( sp, out, baselen ) <0 )
		return -1;

	return 0;
}

char *sql_with_time(  SQLPATTERN *sp, char *selist, char *start_time, char *end_time )
{
	char *out, *atom;
	int baselen, len;

	if( sp == NULL )
		return NULL;
	atom = (char *)FB_MALLOC(sizeof(char)*MAX_SQL_SIZE);
	out = atom;
	if( out == NULL )
		return out;
	baselen = 0;
	//with subquery
	if( sp->sp_with )
	{
		if( with2str( sp->sp_with, out, &baselen )<0 )
			return atom;
		out += baselen;
	}
	//select list
	if( selist == NULL || selist[0] == '\0' )
	{
		len = baselen;
		if( sellist2str( sp->sp_selectlist, out, &baselen )<0 )
			return atom;
		out += baselen-len;
	}
	else
	{
		strcpy( out, "SELECT ");
		out += 7;
		baselen += 7;
		strcpy( out, selist );
		len = strlen( selist );
		out += len;
		baselen += len;
	}
	
	//from list
	len = baselen;
	if( tbfs2str( sp->sp_fromlist, out, &baselen )<0 )
		return atom;
	out += baselen-len;
	//where condition
	if( strlen( sp->time_col->col_name ) == 0 )
	{
		if( sp->sp_whereclause )
		{
			if( baselen + 6 >= MAX_SQL_SIZE )
			return atom;
			baselen += 6;
			strcpy( out, "WHERE " );
			out += 6;
			len = baselen;
			if( stem_con2str( sp->sp_whereclause->qt_cs, out, &baselen ) < 0 )
				return atom;
			if( baselen == len )
			{
				baselen -= 6;
				out -= 6;
				memset( out, 0, 6 );
			}
			else
			{
				out += baselen - len;
			}
		}
		goto CONN;
	}
	if( sp->sp_whereclause )
	{
		if( baselen + 8 >= MAX_SQL_SIZE )
			return atom;
		baselen += 8;
		strcpy( out, "WHERE ( " );
		out += 8;
		len = baselen;
		if( stem_con2str( sp->sp_whereclause->qt_cs, out, &baselen ) < 0 )
			return atom;
		if( baselen == len )
		{
			baselen -= 2;
			out -= 2;
			memset( out, 0, 2 );
		}
		else
		{
			out += baselen - len;
			if( baselen+7 >MAX_SQL_SIZE )
				return atom;
			baselen += 7;
			strcpy( out, " ) AND " );
			out += 7;
		}
	}
	else if( start_time[0] != '\0' || end_time[0] != '\0' )
	{
		if( baselen + 6 >= MAX_SQL_SIZE )
			return atom;
		baselen += 6;
		strcpy( out, "WHERE " );
		out += 6;
	}
	//加入时间条件
	if( start_time[0] != '\0' )
	{
		len = strlen( sp->time_col->col_name );
		if( baselen + len >= MAX_SQL_SIZE )
			return atom;
		baselen += len;
		strcpy( out, sp->time_col->col_name);
		out += len;
		if( !strcmp( sp->query_time->stime, start_time ) )
		{
			len = strlen( sp->query_time->scompare );
			if( baselen + len >= MAX_SQL_SIZE )
				return atom;
			baselen += len;
			strcpy( out, sp->query_time->scompare);
			out += len;
		}
		else
		{
			if( baselen + 2 >= MAX_SQL_SIZE )
				return atom;
			baselen += 2;
			strcpy( out, ">=");
			out += 2;
		}

		len = strlen( start_time );
		if( baselen + len + 9 >= MAX_SQL_SIZE )
			return atom;
		baselen += len + 9;
		strcpy( out, "TO_DATE(\'");
		out += 9;
		strcpy( out, start_time);
		out += len;

		if( end_time[0] != '\0' )
		{
			len = strlen( "\', \'YYYY-MM-DD hh24:mi:ss\') AND " );
			if( baselen +  len >= MAX_SQL_SIZE )
				return atom;
			baselen += len;
			strcpy( out, "\', \'YYYY-MM-DD hh24:mi:ss\') AND ");
			out += len;
		}
		else
		{
			len = strlen( "\', \'YYYY-MM-DD hh24:mi:ss\') " );
			if( baselen +  len >= MAX_SQL_SIZE )
				return atom;
			baselen += len;
			strcpy( out, "\', \'YYYY-MM-DD hh24:mi:ss\') ");
			out += len;
		}
	}
	if( end_time[0] != '\0' )
	{
		len = strlen( sp->time_col->col_name );
		if( baselen + len >= MAX_SQL_SIZE )
			return atom;
		baselen += len;
		strcpy( out, sp->time_col->col_name);
		out += len;
		if( !strcmp( sp->query_time->etime, end_time ) )
		{
			len = strlen( sp->query_time->ecompare );
			if( baselen + len >= MAX_SQL_SIZE )
				return atom;
			baselen += len;
			strcpy( out, sp->query_time->ecompare );
			out += len;
		}
		else
		{
			if( baselen + 1 >= MAX_SQL_SIZE )
				return atom;
			baselen ++;
			strcpy( out, "<");
			out ++;
		}

		len = strlen( end_time );
		if( baselen + len + 9 + strlen( "\', \'YYYY-MM-DD hh24:mi:ss\') " )>= MAX_SQL_SIZE )
			return atom;
		baselen += len + 9 + strlen( "\', \'YYYY-MM-DD hh24:mi:ss\') " );
		strcpy( out, "TO_DATE(\'");
		out += 9;
		strcpy( out, end_time);
		out += len;
		strcpy( out, "\', \'YYYY-MM-DD hh24:mi:ss\') ");
		out += strlen( "\', \'YYYY-MM-DD hh24:mi:ss\') ");
	}
	CONN:
	//connect clause
	if( sp->sp_connectclause != NULL )
	{
		if( sp->sp_connectclause->hq_start!=NULL )
		{
			if( baselen + 11 >= MAX_SQL_SIZE )
				return atom;
			baselen += 11;
			strcpy( out, "START WITH ");
			out += 11;
			len = baselen;
			if( condition2str( sp->sp_connectclause->hq_start, out, &baselen ) < 0 )
				return atom;
			out += baselen-len;
		}
		if( sp->sp_connectclause->hq_type==0)
		{
			if( baselen + 12 >= MAX_SQL_SIZE )
				return atom;
			baselen += 12;
			strcpy( out, " CONNECT BY ");
			out += 12;
		}
		else
		{
			if( baselen + 20 >= MAX_SQL_SIZE )
				return atom;
			baselen += 20;
			strcpy( out, " CONNECT BY NOCYCLE ");
			out += 20;
		}

		len = baselen;
		if( condition2str( sp->sp_connectclause->hq_connect, out, &baselen ) < 0 )
			return atom;
		out += baselen - len;
		if( baselen + 1 >= MAX_SQL_SIZE )
			return atom;
		baselen += 1;
		strcpy( out, " ");
		out ++;
	}
	//group clause
	if( sp->sp_groupbyclause )
	{
		len = baselen;
		if( group2str( sp->sp_groupbyclause, out, &baselen ) < 0 )
			return atom;
		out += baselen-len;
	}
	//having
	if( sp->sp_havingclause!=NULL )
	{
		if( baselen + 7 >= MAX_SQL_SIZE )
			return atom;
		baselen += 7;
		strcpy( out, "HAVING ");
		out += 7;
		len = baselen;
		if( condition2str( sp->sp_havingclause->qt_cs, out, &baselen )<0 )
			return atom;
		out += baselen-len;
	}
	//order by clause
	if( sp->sp_orderbyclause )
	{
		len = baselen;
		if( order2str( sp->sp_orderbyclause, out, &baselen )<0 )
			return atom;
		out += baselen-len;
	}
	/*for update clause*/
	if( sp->sp_updateclause != NULL )
	{
		strcpy( out, sp->sp_updateclause );
	}

	return atom;
}

