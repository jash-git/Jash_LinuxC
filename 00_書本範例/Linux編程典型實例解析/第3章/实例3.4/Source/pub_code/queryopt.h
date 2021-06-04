/*
 * Queryopt.h
 *
 * Version 1.0
 *
 * Belong to : Query Optimizing SubSystem
 *
 * History:
 *  2005-7-27		Zhu defined sp_type & DUAL_LINK
 *  2005-7-27		Yuan defined UserComm Interface
 *	2005-7-26		Create by Zhu HongYu & Yuan ZhiPing
 *
 */
#ifndef QUERYOPT_H
#define QUERYOPT_H
#include <pthread.h>
/*------------------------------------------
 * ����������ʱ�������ƽṹ ͷ�ļ�          *
 *------------------------------------------*/
#include "extractTime.h"
#include <time.h>
/* define malloc and free */
#define FB_MALLOC	malloc
#define FB_FREE 	free
#define FB_REALLOC	realloc
/*follow used in parser code*/
#define MAX_STR_LEN			1024
#define MAX_NAME_LEN 		128
#define MAX_ALIAS_LEN		128
#define MAX_OPER_LEN		40
#define MAX_COL_LEN			128

#define MAX_TAB_LEN			30

#define ROWID_LENGTH		19 /* Oracle rowid length + '\0' */

/* add by yuanzhp	0802*/
//#define	MD5_LENGTH			40
//��ѯ��������ص����������������ֵʱ�������Ľ��������
//��Ϊȱʡֵ�������û���̬���á�
#define	MAX_ROWID			100000
/*add by yuanzhp 20050729
*/
#define MAX_USERNAME_LEN	30
#define	MAX_USERPASS_LEN	30

#define TEMPLET_EXPIRE_PERIOD 	10*60*60

//��������ϣ���������rowcache�����������
//��ϣֵ�ļ��㷽����rs_id�ַ���(16�ֽ�)ÿ���ַ�ASCIIֵ�����ȡ
//ROWCACHE_MAX_HASH_ELEMENT��ģ��
#define	ROWCACHE_MAX_HASH_ELEMENT		100
//������漯�б���Ľ����������
#define	ROWCACHE_MAX_ELEMENT			1000
//SQLģ���ϣ�ĸ���
#define	SQLPATTERN_MAX_HASH_ELEMENT	100

//modify by szy at 2006-3-30
typedef struct _between_time {
	char	low_t[64];
	char	high_t[64];
}BetweenTime;

/* add by yuanzhp 0802
�ڴ����:
MD5_LENGTH*MAX_ROWID  *ROWCACHE_MAX_ELEMENT=1G����
*/
/*
#define 	DISTINCT		1
#define	UNIQUE			2
#define 	ALL				3
#define	NONE			0
*/
/*
 * Common Structure
 */
typedef	struct	_DUAL_LINK
{
	struct	_DUAL_LINK	*prev;
	struct	_DUAL_LINK	*next;
	pthread_mutex_t 		lock;
	void					*context;
} DUAL_LINK;
typedef	struct	_HASH_ELEMENT
{
	DUAL_LINK			*link_head;
	pthread_mutex_t 		lock;
} HASH_ELEMENT;


/*
 * Structure for SQL Pattern Lib
 */
//char *bs_operator[]={ ">", "<", ">=", "<=", "<>", "LIKE", "BETWEEN", "IN", "NOT IN", "IS NULLX", "IS NOT NULLX",
//with subquery clause
typedef struct _WITHSUB
{
	char 		ws_id[MAX_NAME_LEN];		//Ԥ��ѯ������
	void  		*ws_subquery;			//Ԥ�Ӳ�ѯSQLPATTERN����ָ��
}WITHSUB;
typedef struct _WITHSTRU
{
	int 	w_count;			//Ԥ�Ӳ�ѯ�ĸ���
	WITHSUB *w_subArray;			//Ԥ�Ӳ�ѯ����
}WITHSTRU;
//Select list struct
typedef struct _STRBUF
{
	char	sb_value[MAX_STR_LEN];		//�ַ���
	char	sb_alias[MAX_ALIAS_LEN];	//�����ַ���
}STRBUF;

typedef struct _SELECTLIST
{
	int		sel_type;	//Select ���� none:0; DISTINCT:1; UNIQUE:2; ALL:3;
	int		sel_count;	//���ص��ֶθ���
	STRBUF		*sel_list;	//��ѯ���ֶ�����
} SELECTLIST;
//condition;
//char *bs_operator[]={ ">", "<", ">=", "<=", "<>", "LIKE", "BETWEEN", "IN", "NOT IN", "IS NULLX", "IS NOT NULLX",
typedef struct _BASICSTRU
{
	char	bs_colname[MAX_COL_LEN];	//�ֶ��������ñ���.�ֶ����ĸ�ʽ
	char	bs_oper[MAX_OPER_LEN];		//�������š��ȽϷ�,like,between,in,�����ࣨis nullx,exists...)����
	int	bs_type;			//ֵ���ͣ��ַ�����SELECT�Ӿ�
	/*------------------------------------------------------------------*/
	/* bs_type �¼���һ����־���壬ֵ3��ʾ����ṹ��һ����ʾʱ��������,
	   1,string
	   2,subquery pattern
	   3,time structure
	   					9��23�����	by szy
	/*-------------------------------------------------------------------*/
	void	*bs_value;			//ֵָ��
} BASICSTRU;
typedef struct _COMBISTRU
{
	int			cs_combi;	//��Ϸ�ʽ��NONE(����һ��basic�������ʽ)=0,()=1,NOT=2,AND=3,OR=4
	int			cs_lefttype;	//������������ 0 �����ģ� 1 ��ϵ� -1��ʾ��
	union {
		BASICSTRU		*cs_leftbs;	//�������͵������ṹ
		struct _COMBISTRU	*cs_leftcs;	//������͵������ṹ
	};
	int			cs_righttype;	//������������
	union {
		BASICSTRU		*cs_rightbs;	//�������͵������ṹ
		struct _COMBISTRU	*cs_rightcs;	//������͵������ṹ
	};
}COMBISTRU;
typedef struct _EXPRTREE
{
	int		qt_count;	//��������
	COMBISTRU	*qt_cs;		//������Ͻṹָ��
}EXPRTREE;
//from clause;
typedef struct _queryTB
{
	char		qt_value[MAX_STR_LEN];	//�ַ���
	void	  	*qt_subquery;		//û���Ӳ�ѯʱ�����ָ��ΪNULL  SQLPATTERN����
}QueryTB;
typedef struct _tbAtom
{
	int 		tb_type;		//none: 0; only:1
	QueryTB		*tb_head;		//ONLY '(' query_tb_expr ')' flashback_query ���Ӿ��е�$3
	char		tb_tail[MAX_STR_LEN];	//������������ַ�����
}TBatom;

typedef struct _tbField
{
	int 		tf_type;		//0:tbAtom;	1:joinTB;
	union {
		struct _tbAtom	*tf_atom;
		struct _joinTB	*tf_join;
	};					//����ָ�����ֽṹ��ָ�룻
}TBfield;

typedef struct _joinTB
{
	TBfield		*jt_left;		//JOIN �Ӿ�֮��һ������table
	char		jt_op[MAX_OPER_LEN];	//����join�����Ͳ���
	TBfield		*jt_right;		//JOIN �Ӿ�֮�ڶ�������table
	char		jt_using[MAX_STR_LEN];	//using clause string
	COMBISTRU	*jt_condition;		//on condition pointer���û�о�ΪNULL
}JoinTB;

typedef struct _tbfieds
{
	int 		ts_count;		//�ܹ��ֶ�����
	TBfield		*ts_field;		//field����
}TBFS;

//hierarch_query clause;
typedef struct _HIERARCH
{
	int 		hq_type;	//none:0;	NOCYCLE:1;
	COMBISTRU	*hq_start;	//start with condition
	COMBISTRU	*hq_connect;	//connectted by condition
}HIERARCH;
//group by clause;
//having clause;just as the where clause;condition;
//order clause; as groupby clause;

typedef struct _STRFIELDS
{
	int	sf_count;	//total fields;
	int 	sf_type;	//-1:not use;	0:none(order);		1:SIBLINGS(order clause);
	char	sf_value[MAX_STR_LEN];
}STRFIELDS;

typedef struct _group_field
{
	char			gcol[MAX_COL_LEN];	//
	struct _group_field	*gnext;
}GroupField;

//order by clause
typedef struct _order_field
{
	int			oasc;	//0, ' '; 1,ASC; 2,DESC
	int			onull;	//0, ''; 1, NULLS FIRST; 2, NULLS LAST
	char			ocol[MAX_COL_LEN];	//
	struct _order_field	*onext;
}OrderField;
typedef struct _order_cla
{
	int		oflag;		//0,' '; 1, SIBLINGS
	OrderField	*ofield;
}OrderCla;
#define MD5_LENGTH	16
//total query patten struct;��ѯ�ṹģ��
typedef struct _SQLPATTERN
{
	unsigned char 		sp_id[MD5_LENGTH];	//SQL����ʶ��
	int				sp_type;		//��־��ģ������ͣ���ʱ����̬����̬
	WITHSTRU		*sp_with;		//��ѯ��俪ʼ��with�Ӿ䣻
	SELECTLIST	*sp_selectlist;		//��ѯ�ֶΣ�
	TBFS				*sp_fromlist;		//table
	EXPRTREE	*sp_whereclause;	//where;
	HIERARCH	*sp_connectclause;	//HIERARCH connect;
//	STRFIELDS	*sp_groupbyclause;	//groupby
	GroupField	*sp_groupbyclause;

	EXPRTREE	*sp_havingclause;	//having
//	STRFIELDS	*sp_orderbyclause;	//order by
	OrderCla	*sp_orderbyclause;

	char		*sp_updateclause;	//for update clause;

	char		*sp_osqlstatement;	//ԭʼ��sql���
	char		*sp_usqlstatement;	//��һ���Ժ��sql���

	time_t		sp_lastaccesstime;	//������ʱ��
	TimeCol		*time_col;		//������table name��ʱ����name
	QueryTime	*query_time;		//��ʼʱ�䣬��ֹʱ��
}SQLPATTERN;


typedef HASH_ELEMENT SQLPATTERNHASH;

//����sp_id����hashֵ������hashֵ���ʵ�����
SQLPATTERNHASH *sqlpattern_hash[SQLPATTERN_MAX_HASH_ELEMENT];

/*
 * Structure for user connect
 */
extern	int	listenport;	//�����Ķ˿ڡ�
typedef struct _USRCOMM
{
	int 		sqlid;			//�û�ִ�е�����ʶ��ͨ����ʶ��SQL������ȥ���.��ֵ��Ҫ���ݵ��ͻ��ˣ��ͻ����ݴ�ֵ��ȡ�������ݺ�ִ�к���������
	char		*sqlstr;		//���ʵ�����ݣ�����������
	int 		pagecount;		//��ҳʱ��ÿҳ������¼����
	int		sqlmode;		//���ִ�е�ģʽ���������Ƿ�����
	int		retrownum;		//��ǰ���صļ�¼��
	int		retpagenum;		//���ص�ҳ��
	int		resultcount;	//������еĽ������¼��
	int		matchratio;		//�����ģ�������ģ���ƥ��̶ȡ��ٷֱ�ֵ��
} USRSQLCOMM;

typedef struct _USR
{
	int		uid;				//�û�����ID��
	char 	username[MAX_USERNAME_LEN];
	char 	userpass[MAX_USERPASS_LEN];
	int		buf_maxrowcount;	//������������ļ�¼����ȱʡ��ȡȫ�ֲ�������ֵ
	USRSQLCOMM *usrsqlcommhead;	//�û�ִ�е��������ÿ��ִ�����һ���ṹ��Ϣ
} USR;

#define KEY_VALUE_LEN		64
typedef struct _key_node
{
	char 		*words;
	char		value[KEY_VALUE_LEN];
}KeyNode;

#define KEY_WORD_LEN		512
typedef struct _key_word
{
	char			k_word[KEY_WORD_LEN];
}KeyContain;
typedef struct _mid_key
{
	int		m_type;
	int		m_lefttype;
	union{
		KeyContain		*m_leftbs;
		struct _mid_key		*m_leftcs;
	};
	int		m_righttype;
	union{
		KeyContain		*m_rightbs;
		struct _mid_key		*m_rightcs;
	};
}MidKey;
SQLPATTERN *myParse(const char *sql );

#endif
