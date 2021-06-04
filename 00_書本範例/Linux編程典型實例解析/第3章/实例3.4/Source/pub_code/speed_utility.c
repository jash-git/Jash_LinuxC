#include <stdlib.h>
#include "queryopt.h"
#include "speed_sqlpat.h"
#include "speed_utility.h"

/*====================== 字符串基本功能 =======================================*/
/*获得统一大写的字符串*/
void upStr(char *src)
{
	int i,len;
	len = strlen(src);
	for( i=0; i<len; i++ )
	{
		if( src[i]>='a' && src[i]<='z' )
			src[i] = src[i] - 32;
	}
}
/*
 * compare string s1 and s2
 */
int myStrncmp( char *s1, char *s2, int len1, int len2)
{
	int shortlen,i,flag;
	flag = 0;
	shortlen = ( len1>len2 )?len2:len1;
	for( i=0; i<shortlen; i++ )
	{
		if( s1[i] != s2[i] )
			return (s1[i]-s2[i]);
	}
	return (len1-len2);
}
/*===================== 为归一化树结构提供比较函数 ===============================*/
/*
 * 按照规则比较条件condition两个基本结构，为归一化条件排序提供标志
 */
int compBasic(BASICSTRU *b1,BASICSTRU *b2)
{
	//两个contains条件的结点比较
	if( b1->bs_type > 99 && b2->bs_type > 99)
	{
		int ret;
		ret = strcmp( b1->bs_colname, b2->bs_colname );
		if( ret < 0 )
			return -1;
		else if( ret>0 )
			return 1;
		else//== 0, same
		{
			ret = b1->bs_type - b2->bs_type;
			if( ret < 0 ) return -1;
			else if( ret>0 ) return 1;
			else
			{
				KeyNode *tmp1, *tmp2;
				tmp1 = (KeyNode *)(b1->bs_value);
				tmp2 = (KeyNode *)(b2->bs_value);
				ret = strcmp( tmp1->value, tmp2->value );
				if( ret<0 )	return -1;
				else if( ret>0 )	return 1;
				else	return 0;
			}
		}
	}
	else if( b1->bs_type > 99 )
		return -1;
	else if( b2->bs_type > 99)
		return 1;

	if( strcmp(b1->bs_colname,b2->bs_colname) < 0) return -1;
	else if( strcmp(b1->bs_colname,b2->bs_colname) > 0) return 1;

	if( strcmp(b1->bs_oper,b2->bs_oper) < 0) return -1;
	else if( strcmp(b1->bs_oper,b2->bs_oper) > 0) return 1;

	if(b1->bs_type < b2->bs_type ) return -1;
	if(b1->bs_type > b2->bs_type ) return 1;

	if(b1->bs_type == 1) /* value is string */
	return (strlen(b1->bs_value)-strlen(b2->bs_value));

	return 0; /*both subquery clause,can't check*/
}
/*
 * 按照规则比较条件condition两个复合结构，为归一化条件排序提供标志
 */
int compCombi(COMBISTRU *r1,COMBISTRU *r2)
{
	int i=0;

	if( r1->cs_combi < r2->cs_combi ) return -1;
	else if ( (r1->cs_combi) > (r2->cs_combi) ) return 1;
	else{/* equal as:basic,(),NOT,AND,OR */
		if( r1->cs_combi > 2 ) /* and or*/
		{
			if(r1->cs_lefttype < r2->cs_lefttype ) return -1;
			if(r1->cs_lefttype > r2->cs_lefttype ) return 1;

			if(r1->cs_lefttype == 0)
				i=compBasic(r1->cs_leftbs,r2->cs_leftbs);
			else
				i=compCombi(r1->cs_leftcs,r2->cs_leftcs);

			if( i!= 0 ) return i;

			if(r1->cs_righttype < r2->cs_righttype ) return -1;
			if(r1->cs_righttype > r2->cs_righttype ) return 1;

			if(r1->cs_righttype == 0)
				i=compBasic(r1->cs_rightbs,r2->cs_rightbs);
			else
				i=compCombi(r1->cs_rightcs,r2->cs_rightcs);
			return i;
		}
		if( r1->cs_combi >0 )/*not (,)*/
		{
			if( r1->cs_lefttype == r2->cs_lefttype )
			{
				if(r1->cs_lefttype == 0)
					return compBasic(r1->cs_leftbs,r2->cs_leftbs);
				else
					return compCombi(r1->cs_leftcs,r2->cs_leftcs);
			}
			else
				return( r1->cs_lefttype < r2->cs_lefttype?-1:1 );
		}
		if( r1->cs_combi == 0) return compBasic(r1->cs_leftbs,r2->cs_leftbs);
	}

	return 0;
}
/*
 * 修枝条件树，合并其中的同层contains语句
 */
int unite_contains( COMBISTRU *r )
{
	if( r->cs_combi == 1 && r->cs_lefttype == 1 )
		return unite_contains( r->cs_leftcs );
	else if( r->cs_combi == 3 || r->cs_combi == 4 )
	{
		//left,right==both basic node
		if( r->cs_lefttype == 0 && r->cs_righttype == 0 )
		{
			//they can combied
			if( r->cs_leftbs->bs_type>99
			&& compBasic( r->cs_leftbs, r->cs_rightbs ) == 0 )
			{
				KeyNode *tmp1, *tmp2;
				char *buf;
				int i;

				tmp1 = (KeyNode *)(r->cs_leftbs->bs_value);
				tmp2 = (KeyNode *)(r->cs_rightbs->bs_value);
				i = strlen(tmp1->words)+strlen(tmp2->words)+10;
				buf = FB_MALLOC( sizeof(char)*i);
				memset( buf, 0, i );
				if( r->cs_combi == 3 )
					sprintf( buf, "(%s) AND (%s)",
						tmp1->words, tmp2->words );
				else
					sprintf( buf, "(%s) OR (%s)",
						tmp1->words, tmp2->words );
				FB_FREE( tmp1->words );
				FB_FREE( tmp2->words );
				tmp1->words = buf;
				r->cs_combi = 0;
				r->cs_righttype = -1;
				FB_FREE( r->cs_rightbs->bs_value );
				FB_FREE( r->cs_rightbs );
				r->cs_rightbs = NULL;
			}
		}
		//left basic; right combied
		else if( r->cs_lefttype == 0 && r->cs_righttype == 1 )
		{
			if( r->cs_leftbs->bs_type>99
			&& r->cs_rightcs->cs_combi == r->cs_combi )
			{
				if( r->cs_rightcs->cs_lefttype == 0 )
				{
					//right->left is a basic node
					if( compBasic( r->cs_leftbs, r->cs_rightcs->cs_leftbs ) == 0 )
					{
						BASICSTRU *tmp1;
						COMBISTRU *tmp2;
						KeyNode *kn1, *kn2;
						char *buf;
						int i;

						tmp1 = r->cs_leftbs;
						tmp2 = r->cs_rightcs;
						kn1 = (KeyNode *) (tmp1->bs_value);
						kn2 = (KeyNode *) (tmp2->cs_leftbs->bs_value);
						i = strlen(kn1->words)+strlen(kn2->words)+10;
						buf = FB_MALLOC( sizeof(char)*i);
						memset( buf, 0, i );
						if( r->cs_combi == 3 )
							sprintf( buf, "(%s) AND (%s)",
								kn1->words, kn2->words );
						else
							sprintf( buf, "(%s) OR (%s)",
								kn1->words, kn2->words );
						FB_FREE( kn1->words );
						FB_FREE( kn2->words );
						kn1->words = buf;
						//释放右孩子的左子结点
						FB_FREE( kn2 );
						FB_FREE( tmp2->cs_leftbs );
						r->cs_righttype = tmp2->cs_righttype;
						if( r->cs_righttype == 1 )
							r->cs_rightcs = tmp2->cs_rightcs;
						else
							r->cs_rightbs = tmp2->cs_rightbs;
						//释放r右孩子
						FB_FREE( tmp2 );
						return unite_contains( r );
					}
					else
						unite_contains( r->cs_rightcs );
				}
				else//右孩子的左子结点是个复合结点，这种情况在本程序中不存在第一个是包含
				{
					unite_contains( r->cs_rightcs->cs_leftcs );
					if( r->cs_rightcs->cs_righttype == 1 )
						unite_contains( r->cs_rightcs->cs_rightcs );
				}
			}
			else
				unite_contains( r->cs_rightcs );
		}
		else if( r->cs_lefttype == 1 && r->cs_righttype == 0 )
		{
			//左孩子为组合结点，右孩子为单纯结点，在本程序中不存在着，这种情况不处理
			BASICSTRU *btmp;
			btmp = r->cs_rightbs;
			r->cs_righttype = 1;
			r->cs_rightcs = r->cs_leftcs;
			r->cs_lefttype = 0;
			r->cs_leftbs = btmp;
			unite_contains( r );
		}
		else if( r->cs_lefttype == 1 && r->cs_righttype == 1 )
		{
			unite_contains( r->cs_leftcs );
			unite_contains( r->cs_rightcs );
			if( r->cs_leftcs->cs_combi == 0 )
			{
				if( r->cs_rightcs->cs_combi == 0 )
				{
					if( r->cs_rightcs->cs_leftbs->bs_type>99
					&& compBasic( r->cs_leftcs->cs_leftbs, r->cs_rightcs->cs_leftbs )==0 )
					{
						char *buf;
						COMBISTRU *cl_node, *cr_node;
						KeyNode *kn1, *kn2;
						int i;

						cl_node = r->cs_leftcs;
						cr_node = r->cs_rightcs;

						kn1 = (KeyNode *)( cl_node->cs_leftbs->bs_value );
						kn2 = (KeyNode *)( cr_node->cs_leftbs->bs_value );
						i = strlen( kn1->words ) + strlen( kn1->words ) + 10;
						buf = (char *)FB_MALLOC(sizeof(char)*i);
						memset( buf, 0, i );
						if( r->cs_combi == 3 )
							sprintf( buf, "(%s) AND (%s)",
								kn1->words, kn2->words );
						else
							sprintf( buf, "(%s) OR (%s)",
								kn1->words, kn2->words );
						FB_FREE( kn1->words );
						FB_FREE( kn2->words );
						kn2->words = buf;
						//释放左孩子
						FB_FREE( kn1 );
						FB_FREE( cl_node->cs_leftbs );
						FB_FREE( cl_node );
						//set left child
						r->cs_lefttype = 0;
						r->cs_leftbs = cr_node->cs_leftbs;
						r->cs_righttype = -1;
						r->cs_rightbs = NULL;
						//释放右孩子
						FB_FREE( cr_node );
						r->cs_combi = 0;
					}
					else
					{
						COMBISTRU *tmp_cs1;
						tmp_cs1 = r->cs_rightcs;
						r->cs_rightbs = tmp_cs1->cs_leftbs;
						r->cs_righttype = 0;
						FB_FREE( tmp_cs1 );
					}
				}
				else
				{
					COMBISTRU *tmp_cs2;
					tmp_cs2 = r->cs_leftcs;
					r->cs_lefttype = 0;
					r->cs_leftbs = tmp_cs2->cs_leftbs;
					FB_FREE( tmp_cs2 );
					unite_contains( r );
				}
			}
			else if( r->cs_rightcs->cs_combi == 0 )
			{
				COMBISTRU *tmp;

				tmp = r->cs_leftcs;
				r->cs_lefttype = 0;
				r->cs_leftbs = r->cs_rightcs->cs_leftbs;
				FB_FREE( r->cs_rightcs );
				r->cs_rightcs = tmp;

				unite_contains( r );
			}
		}
	}
	return 0;
}
/*
 * 排序条件语法结构树: 非面向用户接口，请勿随便使用^_^
 */
void takeSort(COMBISTRU *r)
{
	BASICSTRU *btmp;
	COMBISTRU *ctmp;
	/* right child is a basic node */
	if(r->cs_righttype==0)
	{
		if(r->cs_lefttype==0)
		{
			if(compBasic(r->cs_leftbs,r->cs_rightbs)<=0) return;

			btmp=r->cs_leftbs;
			r->cs_leftbs=r->cs_rightbs;
			r->cs_rightbs=btmp;
			btmp=NULL;
			return;
		}
		// a basic node and a combi compare
		else if( (r->cs_leftcs->cs_combi) < (r->cs_combi) )
		{
			btmp=r->cs_rightbs;
			r->cs_righttype=1;
			r->cs_rightcs=r->cs_leftcs;
			r->cs_lefttype=0;
			r->cs_leftbs=btmp;
			btmp=NULL;
			return;
		}
		// basic is as the same lever as right child's childs
		else//r->cs_combi == r->cs_leftcs->cs_combi
		{
			if( r->cs_leftcs->cs_righttype==0 )
			{
				if(compBasic(r->cs_leftcs->cs_rightbs,r->cs_rightbs)<=0)
					return;
				btmp=r->cs_leftcs->cs_rightbs;
				r->cs_leftcs->cs_rightbs=r->cs_rightbs;
				r->cs_rightbs=btmp;
			}
			else	//缺省basic结点在com结点左侧，即：前者是后者的前序
			{
				btmp=r->cs_rightbs;
				r->cs_righttype=1;
				r->cs_rightcs=r->cs_leftcs->cs_rightcs;
				r->cs_leftcs->cs_rightbs=btmp;
				r->cs_leftcs->cs_righttype=0;
			}
			takeSort(r->cs_leftcs);/*递归*/
			return;
		}
	}
	else if( r->cs_righttype != 1 )
		return;
	/*down is righttype==1*/
	/*now compare the right child as basic , not, () such */
	if(r->cs_lefttype==0)//r->cs_righttype == 1
	{	//可以作修改
		if( r->cs_rightcs->cs_combi == r->cs_combi )
		{
			if( r->cs_rightcs->cs_lefttype == 0 )
			{
				if( compBasic( r->cs_leftbs, r->cs_rightcs->cs_leftbs ) > 0 )
				{
					BASICSTRU *bt;
					bt = r->cs_rightcs->cs_leftbs;
					r->cs_rightcs->cs_leftbs = r->cs_leftbs;
					r->cs_leftbs = bt;
				}
				//对右孩子排序
				takeSort( r->cs_rightcs );
			}
			else
			{
				takeSort( r->cs_rightcs );
				if( r->cs_rightcs->cs_lefttype == 0 )
					takeSort( r );
			}
		}
	}
	else//both combi node
	{
		//both the : 不可能存在
		if( r->cs_leftcs->cs_combi == r->cs_combi
		&& r->cs_rightcs->cs_combi == r->cs_combi )
		{
			return;
		}
		else if( r->cs_leftcs->cs_combi == r->cs_combi )
		{
			if(r->cs_leftcs->cs_righttype == 0)
				return;
			else
			{
				if(compCombi(r->cs_leftcs->cs_rightcs,r->cs_rightcs)<=0)
					return;
				else
				{
					ctmp=r->cs_rightcs;
					r->cs_rightcs=r->cs_leftcs->cs_rightcs;
					r->cs_leftcs->cs_rightcs=ctmp;
					takeSort(r->cs_leftcs);
					return;
				}
			}
		}
		else //r->cs_righttcs->cs_combi == r->cs_combi
		{
			ctmp=r->cs_leftcs;
			if( r->cs_rightcs->cs_lefttype == 0 )
			{
				r->cs_lefttype = 0;
				r->cs_leftbs = r->cs_rightcs->cs_leftbs;
				r->cs_rightcs->cs_lefttype = 1;
				r->cs_rightcs->cs_leftcs = ctmp;
			}
			else
			{
				if(compCombi(r->cs_leftcs, r->cs_rightcs->cs_leftcs)<=0)
					return;
				r->cs_leftcs = r->cs_rightcs->cs_leftcs;
				r->cs_rightcs=ctmp;
			}
			takeSort(r->cs_rightcs);
		}
	}
	/*r->cs_lefttype==0 and righttype==1 just return;*/
	return;
}
/*
 * 比较TBfield
 */
int compTBfield( TBfield *r1, TBfield *r2 )
{
	int re;
	if( r1->tf_type - r2->tf_type != 0 )
	return ( r1->tf_type - r2->tf_type );

	if( r1->tf_type == 0 )
	{
		re=r1->tf_atom->tb_type - r2->tf_atom->tb_type;
		if( re != 0 )	return re;


		re=strcmp( r1->tf_atom->tb_head->qt_value, r2->tf_atom->tb_head->qt_value);
		if( re!=0 ) return re;
		return strcmp( r1->tf_atom->tb_tail, r2->tf_atom->tb_tail );
	}
	else/*join*/
	{
		re=r1->tf_join->jt_left->tf_type - r2->tf_join->jt_left->tf_type;
		if( re != 0 )	return re;

		re=strcmp( r1->tf_join->jt_op, r2->tf_join->jt_op );
		if( re!=0 ) return re;

		/*right is atom*/
		re=r1->tf_join->jt_right->tf_atom->tb_type - r2->tf_join->jt_right->tf_atom->tb_type ;
		if( re!=0 ) return re;

		re=strcmp( r1->tf_join->jt_right->tf_atom->tb_head->qt_value,
			r2->tf_join->jt_right->tf_atom->tb_head->qt_value);
		if( re!= 0 ) return re;

		return strcmp(r1->tf_join->jt_right->tf_atom->tb_tail, r2->tf_join->jt_right->tf_atom->tb_tail);
	}
}
/*compare subquery*/
int compWith(WITHSUB *w, char *as_name, SQLPATTERN *tmp )
{
	int i;
	TBatom *tb;
	for( i=0; i<tmp->sp_fromlist->ts_count; i++)
	{
		if( ( tmp->sp_fromlist->ts_field[i] ).tf_type == 0 )
		{
			tb=( tmp->sp_fromlist->ts_field[i] ).tf_atom;
			if( strcmp( tb->tb_head->qt_value, w->ws_id ) == 0 )
			return -1;
		}
	}

	return strcmp( w->ws_id, as_name );
}
/*=================================================================*/
/*
 * 查询条件语法树的外部条件个数
 */
int countCondition(COMBISTRU *r)
{
	int i;
	if(r->cs_combi<3) /*as basic,(),not**/
	{
		i=1;
	}
	else
	{
		i=0;
		if(r->cs_lefttype==0) i++;
		else
		{
			if(r->cs_leftcs->cs_combi == r->cs_combi) i+=countCondition(r->cs_leftcs);
			else i++;
		}
		if(r->cs_righttype==0) i++;
		else
		{
			if(r->cs_rightcs->cs_combi == r->cs_combi) i+= countCondition(r->cs_rightcs);
			else i++;
		}
	}
	return i;
}


/*-----------------------------------------
 *函数：从输入的sql string中抽取表名称
 *-----------------------------------------
 *输入：
 *	sql 查询命令string（针对单表查询）
 *输出：
 *	查询table名称
 */
void getTabName( char *tname, const char *sql )
{
	char *tmp;
	int i;

	tmp = strstr(sql,"from");
	if( !tmp )
	tmp = strstr(sql,"FROM");

	if( !tmp )
	{
		printf("the sql string dosn't contain keyword 'from' or 'FROM'\n");
		return ;
	}
	//tmp point to after the 'FROM' ..
	tmp += 4;
	//skip blank
	while(*tmp==' ') tmp++;
	//forward stop when meet the ' ';
	for( i=0; (*(tmp+i)!='\0') && (*(tmp+i)!=' '); i++ );
	strncpy( tname, tmp, i );
	tname[i] = '\0';
}
/*
 * get timecol from the TimeCol Array
 *
 */
void getTimeColName( TimeCol *pat_tcol )
{
	//char table_name[128];
	int i;
	//scan for col_name
	for( i=0; i<tcol_nums; i++ )
	{
		upStr( tcol[i].table_name );
		upStr( tcol[i].col_name );
		if( !strcmp( pat_tcol->table_name, tcol[i].table_name ) ) break;
	}
	if( i==tcol_nums )
	{
		pat_tcol->col_name[0]='\0';
	}
	else
	{
		strcpy( pat_tcol->col_name, tcol[i].col_name );
	}
}
/*
 * TO_DATE( '2000-01-12', '' );
 * '2000-01-12'
 */
void recognizeTime(const char *src, char *des )
{
	int i, j, len;

	len = strlen(src);
	for( i=0; i<len; i++ )
		if( src[i] == '\'' ) break;
	for( i++,j=0; src[i] != '\'' && i<len; i++,j++ )
		des[j]=src[i];
	des[j] = '\0';
}
/*
 * combi two condition nodes: AND, OR
 */
COMBISTRU *combi_condition( COMBISTRU *c1, COMBISTRU *c2, int type )
{
	COMBISTRU	*r;
	/*如何排序左右子树，根据。。。
	 * 条件树的结点有两种类型：COMBISTRU（中间的结点）、BASICSTRU（叶子）
	 * COMBISTRU结点由属性cs_combi标识，0－只有左孩子指针指向叶子结点；1－是'()'；
	 *  2－表示NOT组合；3－AND组合；4－OR组合
	 */
	/* c1 point to a basic struct node */
	if( c1->cs_combi == 0 )
	{
		/*combine two single basic struct*/
		if( c2->cs_combi == 0 )
		{
			/* sort ???*/
			if(compBasic(c1->cs_leftbs,c2->cs_leftbs)<=0)
			{
				c1->cs_righttype=0;
				c1->cs_rightbs=c2->cs_leftbs;
				c2->cs_leftbs=NULL;
				FB_FREE(c2);
				/*NONE(即：一个basic条件表达式)=0,'()'=1,NOT=2,AND=3,OR=4*/
				c1->cs_combi = type;
				r=c1;
			}
			else
			{
				c2->cs_righttype=0;
				c2->cs_rightbs=c1->cs_leftbs;
				c1->cs_leftbs=NULL;
				FB_FREE(c1);
				 /*NONE(即：一个basic条件表达式)=0,'()'=1,NOT=2,AND=3,OR=4*/
				c2->cs_combi = type;
				r=c2;
			}
		}
		/* c2 a combi-struct node */
		else
		{
			c1->cs_righttype = 1;
			c1->cs_rightcs = c2;
			c1->cs_combi = type;
			if( c2->cs_combi < type )/* 1-'()'; 2-NOT; 3-AND */
				return c1;
			//c2->cs_combi == type
			//重新排序
			takeSort(c1);
			r=c1;
		}
	}
	/* c1 a combi; c2 point to basic node */
	else if( c2->cs_combi == 0 )
		return combi_condition( c2, c1, type );
	/* both combi */
	else
	{
		COMBISTRU	*tmp;
		tmp=(COMBISTRU*)FB_MALLOC(sizeof(COMBISTRU));
		tmp->cs_combi=type;
		tmp->cs_lefttype=1;
		tmp->cs_righttype=1;
		if( c1->cs_combi<type && c2->cs_combi<type )/*of corse (c2->cs_combi<type)*/
		{
			if(compCombi(c1,c2)<0)
			{
				tmp->cs_leftcs=c1;
				tmp->cs_rightcs=c2;
			}
			else
			{
				tmp->cs_leftcs=c2;
				tmp->cs_rightcs=c1;
			}
		}
		else
		{
			tmp->cs_leftcs=c1;
			tmp->cs_rightcs=c2;
			takeSort(tmp);
		}
		r=tmp;
	}
	return r;
}

/*
 * 分析CONTAINS子句，为hanyu.y中的动作提供功能函数
 * 仅限内部使用
 */
void sort_key( MidKey *r )
{
	/*
	 * provide that: r->m_lefttype == 0|1, r->m_righttype == 1
	 * r->m_type == r->m_rightcs->m_type
	 */
	if( r->m_lefttype == 0 )
	{
		if( r->m_rightcs->m_lefttype == 0 )
		{
			if( strcmp( r->m_rightcs->m_leftbs->k_word, r->m_leftbs->k_word ) < 0 )
			{
				KeyContain *tmp;
				tmp = r->m_rightcs->m_leftbs;
				r->m_rightcs->m_leftbs = r->m_leftbs;
				r->m_leftbs = tmp;
			}
			sort_key( r->m_rightcs );
		}
	}
	else
	{
		if( r->m_rightcs->m_lefttype == 0 )
		{
			KeyContain *tmp1;
			tmp1 = r->m_rightcs->m_leftbs;
			r->m_rightcs->m_leftcs = r->m_leftcs;
			r->m_rightcs->m_type = 1;
			r->m_lefttype = 0;
			r->m_leftbs = tmp1;
			sort_key( r->m_rightcs );
		}
		else if( r->m_rightcs->m_leftcs->m_type <= r->m_leftcs->m_type
		&& comp_mid( r->m_rightcs->m_leftcs, r->m_leftcs)<0 )
		{
			MidKey *tmp2;
			tmp2 = r->m_rightcs->m_leftcs;
			r->m_rightcs->m_leftcs = r->m_leftcs;
			r->m_leftcs = tmp2;
			sort_key( r->m_rightcs );
		}
	}
}
//合并两个中间结点，当作AND运算时，type＝3；当OR时，type＝4
MidKey *combi_midKey( MidKey *m1, MidKey *m2, int type )
{
	MidKey	*r;
	if( m1->m_type == 0 )
	{
		//both are basic node
		if( m2->m_type == 0 )
		{
			if( strcmp( m1->m_leftbs->k_word, m2->m_leftbs->k_word ) > 0 )
			{
				m2->m_righttype = 0;
				m2->m_rightbs = m1->m_leftbs;
				FB_FREE( m1 );
				r = m2;
			}
			else
			{
				m1->m_righttype = 0;
				m1->m_rightbs = m2->m_leftbs;
				FB_FREE( m2 );
				r = m1;
			}
			r->m_type = type;
		}
		// m2 midle node
		else
		{
			if( m2->m_type == 1 && m2->m_leftcs->m_type <= type )
			{
				MidKey *mt;
				mt = m2->m_leftcs;
				FB_FREE( m2 );
				m2 = mt;
			}
			m1->m_type = type;
			m1->m_righttype = 1;
			m1->m_rightcs = m2;

			if( m2->m_type < type )
				return m1;

			//m2 type same as type
			if( m2->m_lefttype == 0 )
			{
				if( strcmp( m1->m_leftbs->k_word, m2->m_leftbs->k_word ) > 0 )
				{
					KeyContain	*tmp;
					tmp = m1->m_leftbs;
					m1->m_leftbs = m2->m_leftbs;
					m2->m_leftbs = tmp;
				}
				return m1;
			}
			//m2 as the same lever
			else
			{
				sort_key( m1 );
				return m1;
			}
		}
	}
	else if( m2->m_type == 0 )
	return combi_midKey( m2, m1, type );
	else
	{
		//这里()括号内不可能包括在basic node
		//剥除不必要括号
		if( m1->m_type == 1 && m1->m_leftcs->m_type <= type )
		{
			MidKey *mt;
			mt = m1->m_leftcs;
			FB_FREE( m1 );
			m1 = mt;
		}
		if( m2->m_type == 1 && m2->m_leftcs->m_type <= type )
		{
			MidKey *mt;
			mt = m2->m_leftcs;
			FB_FREE( m2 );
			m2 = mt;
		}
		//
		r = (MidKey *)FB_MALLOC( sizeof( MidKey ));
		r->m_type = type;
		r->m_lefttype = r->m_righttype = 1;
		if( m1->m_type < type )
		{
			if( m2->m_type < type )
			{
				if( comp_mid( m1, m2 ) > 0 )
				{
					r->m_leftcs = m2;
					r->m_rightcs = m1;
				}
				else
				{
					r->m_leftcs = m1;
					r->m_rightcs = m2;
				}
				return r;
			}
			else//==
			{
				r->m_leftcs = m1;
				r->m_rightcs = m2;
				sort_key( r );
				return r;
			}
		}
		else if( m2->m_type < type )
		{
			r->m_leftcs = m2;
			r->m_rightcs = m1;
			sort_key( r );
		}
		else
		{
			printf("Bad conditions accureeeeee\n");
		}
	}
	return r;
}
//比较两个中间结点的顺序
int comp_mid( MidKey *m1, MidKey *m2 )
{
	if( m1->m_type < m2->m_type )
		return -1;
	else if( m1->m_type == m2->m_type )
	{
		if( m1->m_lefttype == 0 && m2->m_lefttype == 0 )
		{
			int ret;
			ret=strcmp( m1->m_leftbs->k_word, m2->m_leftbs->k_word );
			if( ret < 0 )
				return -1;
			else  if( ret > 0 )
				return 1;
			else//compare right child
			{
		right_comp:
				if( m1->m_righttype == 0 && m2->m_righttype == 0 )
				{
					int ret1;
					ret=strcmp( m1->m_rightbs->k_word, m2->m_rightbs->k_word );
					if( ret>0 )
						return 1;
					else if( ret<0 )
						return -1;
					else
						return 0;
				}
				else if( m1->m_righttype < m2->m_righttype   )
					return -1;
				else if( m1->m_righttype > m2->m_righttype   )
					return 1;
				else
				{
					return comp_mid( m1->m_rightcs, m2->m_rightcs );
				}
			}
		}
		else if( m1->m_lefttype == 0 && m2->m_lefttype == 1 )
			return -1;
		else if( m1->m_lefttype == 1 && m2->m_lefttype == 0 )
			return 1;
		else
		{
			int ret2;
			ret2 = comp_mid( m1->m_leftcs, m2->m_leftcs );
			if( ret2!=0 )
				return ret2;
			else
				goto right_comp;
		}
	}
	else
		return 1;
}
//将一棵CONTAINS子句对应的逻辑树转换为归一划字符串
int mkey2str( MidKey *mr, char txt[], int *baselen )
{
	int i, len;
	//i为txt游标，len为本结点转换为字符传后的长度
	i = *baselen;
	len = i;
	switch( mr->m_type )
	{
		case 0:
			sprintf( txt+i, " %s", mr->m_leftbs->k_word );
			i += strlen( mr->m_leftbs->k_word ) + 1;
			break;
		case 1:
		{
			if( mr->m_lefttype == 0 )
			{
				sprintf( txt+i, " ( %s )", mr->m_leftbs->k_word );
				i+= 5+strlen( mr->m_leftbs->k_word );
			}
			else
			{
				sprintf( txt+i, " (" );
				i+=2;
				//递归遍历
				mkey2str( mr->m_leftcs, txt, &i );
				sprintf( txt+i, " )" );
				i+=2;
			}
			break;
		}
		case 3:
		case 4:
		{
			if( mr->m_lefttype == 0 )
			{
				sprintf( txt+i, " %s",  mr->m_leftbs->k_word );
				i+= strlen( mr->m_leftbs->k_word ) + 1;
			}
			else
				mkey2str( mr->m_leftcs, txt, &i );
			if( mr->m_type == 3 )
			{
				sprintf( txt+i, " AND" );
				i+=4;
			}
			else
			{
				sprintf( txt+i, " OR" );
				i+=3;
			}
			if( mr->m_righttype == 0 )
			{
				sprintf( txt+i, " %s", mr->m_rightbs->k_word );
				i+= strlen( mr->m_rightbs->k_word ) + 1;
			}
			else
				mkey2str( mr->m_rightcs, txt, &i );
			break;
		}
	}
	len = i-len;
	*baselen = i;
	return len;
}
//归一划contains: 在KeyNode结点的基础上计算
int unique_deal( COMBISTRU *r )
{
	if( r->cs_lefttype == 0 )
	{
		//convert
		if( r->cs_leftbs->bs_type > 99 )
		{
			KeyNode *kn;
			MidKey *tmp;
			char *buf;
			int blen;

			kn = (KeyNode *)(r->cs_leftbs->bs_value);
			buf = (char*)FB_MALLOC( ( strlen(kn->words)+128 ) );
			memset( buf, 0, strlen(kn->words)+128 );
			sprintf( buf, "%s", kn->words );
			//parse the buf
			tmp = (MidKey *) hanyu_parse( buf );
			if( tmp == NULL )
			{
				FB_FREE( kn->words );
				FB_FREE( buf );
				return -1;
			}
			//get unique string
			memset( buf, 0, strlen(kn->words)+128 );
			blen = 0;
			mkey2str( tmp, buf, &blen );
			//save to kn
			FB_FREE( kn->words );
			kn->words = buf;
			//printf("%s\n", buf);
			//释放树结构
			mkey_free( tmp );
		}
	}
	else if( r->cs_lefttype == 1 )
		return unique_deal( r->cs_leftcs );
	if( r->cs_righttype == 0 )
	{
		//convert
		if( r->cs_rightbs->bs_type > 99 )
		{
			KeyNode *kn;
			MidKey *tmp;
			char *buf;
			int blen;

			kn = (KeyNode *)(r->cs_rightbs->bs_value);
			buf = (char*)FB_MALLOC( ( strlen(kn->words)+128 ) );
			memset( buf, 0, strlen(kn->words)+128 );
			sprintf( buf, "%s", kn->words );
			//parse the buf
			tmp = (MidKey *) hanyu_parse( buf );
			if( tmp == NULL )
			{
				FB_FREE( kn->words );
				FB_FREE( buf );
				return -1;
			}
			//get unique string
			memset( buf, 0, strlen(kn->words)+128 );
			blen = 0;
			mkey2str( tmp, buf, &blen );
			//save to kn
			FB_FREE( kn->words );
			kn->words = buf;
			//printf("%s\n", buf);
			//释放树结构
			mkey_free( tmp );
		}
	}
	else if( r->cs_righttype == 1 )
		return unique_deal( r->cs_rightcs );

	return 0;
}
//释放CONTAINS子句对应的分析树
int mkey_free( MidKey *m )
{
	if( m->m_lefttype == 0 )
	{
		FB_FREE( m->m_leftbs );
	}
	else if( m->m_lefttype == 1 )
		mkey_free( m->m_leftcs );
	if( m->m_righttype == 0 )
		FB_FREE( m->m_rightbs );
	else if( m->m_righttype == 1 )
		mkey_free( m->m_rightcs );

	FB_FREE( m );
	return 0;
}
