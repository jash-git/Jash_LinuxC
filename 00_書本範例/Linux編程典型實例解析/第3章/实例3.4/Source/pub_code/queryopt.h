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
 * 描述表名和时间列名称结构 头文件          *
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
//查询语句允许返回的最大结果数，超过此值时，超过的结果不缓存
//此为缺省值，允许用户动态设置。
#define	MAX_ROWID			100000
/*add by yuanzhp 20050729
*/
#define MAX_USERNAME_LEN	30
#define	MAX_USERPASS_LEN	30

#define TEMPLET_EXPIRE_PERIOD 	10*60*60

//结果缓存哈希数组个数，rowcache的链表个数。
//哈希值的计算方法是rs_id字符串(16字节)每个字符ASCII值相加再取
//ROWCACHE_MAX_HASH_ELEMENT的模。
#define	ROWCACHE_MAX_HASH_ELEMENT		100
//结果缓存集中保存的结果集个数。
#define	ROWCACHE_MAX_ELEMENT			1000
//SQL模版哈希的个数
#define	SQLPATTERN_MAX_HASH_ELEMENT	100

//modify by szy at 2006-3-30
typedef struct _between_time {
	char	low_t[64];
	char	high_t[64];
}BetweenTime;

/* add by yuanzhp 0802
内存估算:
MD5_LENGTH*MAX_ROWID  *ROWCACHE_MAX_ELEMENT=1G左右
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
	char 		ws_id[MAX_NAME_LEN];		//预查询的名字
	void  		*ws_subquery;			//预子查询SQLPATTERN类型指针
}WITHSUB;
typedef struct _WITHSTRU
{
	int 	w_count;			//预子查询的个数
	WITHSUB *w_subArray;			//预子查询数组
}WITHSTRU;
//Select list struct
typedef struct _STRBUF
{
	char	sb_value[MAX_STR_LEN];		//字符串
	char	sb_alias[MAX_ALIAS_LEN];	//别名字符串
}STRBUF;

typedef struct _SELECTLIST
{
	int		sel_type;	//Select 类型 none:0; DISTINCT:1; UNIQUE:2; ALL:3;
	int		sel_count;	//返回的字段个数
	STRBUF		*sel_list;	//查询的字段数组
} SELECTLIST;
//condition;
//char *bs_operator[]={ ">", "<", ">=", "<=", "<>", "LIKE", "BETWEEN", "IN", "NOT IN", "IS NULLX", "IS NOT NULLX",
typedef struct _BASICSTRU
{
	char	bs_colname[MAX_COL_LEN];	//字段名，采用表名.字段名的格式
	char	bs_oper[MAX_OPER_LEN];		//操作符号“比较符,like,between,in,断言类（is nullx,exists...)”等
	int	bs_type;			//值类型，字符串或SELECT子句
	/*------------------------------------------------------------------*/
	/* bs_type 新加入一个标志意义，值3表示这个结构是一个表示时间条件的,
	   1,string
	   2,subquery pattern
	   3,time structure
	   					9月23日添加	by szy
	/*-------------------------------------------------------------------*/
	void	*bs_value;			//值指针
} BASICSTRU;
typedef struct _COMBISTRU
{
	int			cs_combi;	//组合方式，NONE(即：一个basic条件表达式)=0,()=1,NOT=2,AND=3,OR=4
	int			cs_lefttype;	//左子条件类型 0 基本的， 1 组合的 -1表示无
	union {
		BASICSTRU		*cs_leftbs;	//基本类型的条件结构
		struct _COMBISTRU	*cs_leftcs;	//组合类型的条件结构
	};
	int			cs_righttype;	//右子条件类型
	union {
		BASICSTRU		*cs_rightbs;	//基本类型的条件结构
		struct _COMBISTRU	*cs_rightcs;	//组合类型的条件结构
	};
}COMBISTRU;
typedef struct _EXPRTREE
{
	int		qt_count;	//条件个数
	COMBISTRU	*qt_cs;		//条件组合结构指针
}EXPRTREE;
//from clause;
typedef struct _queryTB
{
	char		qt_value[MAX_STR_LEN];	//字符串
	void	  	*qt_subquery;		//没有子查询时候，这个指针为NULL  SQLPATTERN类型
}QueryTB;
typedef struct _tbAtom
{
	int 		tb_type;		//none: 0; only:1
	QueryTB		*tb_head;		//ONLY '(' query_tb_expr ')' flashback_query 该子句中的$3
	char		tb_tail[MAX_STR_LEN];	//上述语句后跟的字符串；
}TBatom;

typedef struct _tbField
{
	int 		tf_type;		//0:tbAtom;	1:joinTB;
	union {
		struct _tbAtom	*tf_atom;
		struct _joinTB	*tf_join;
	};					//可以指向两种结构的指针；
}TBfield;

typedef struct _joinTB
{
	TBfield		*jt_left;		//JOIN 子句之第一操作数table
	char		jt_op[MAX_OPER_LEN];	//所作join的类型操作
	TBfield		*jt_right;		//JOIN 子句之第二操作数table
	char		jt_using[MAX_STR_LEN];	//using clause string
	COMBISTRU	*jt_condition;		//on condition pointer如果没有就为NULL
}JoinTB;

typedef struct _tbfieds
{
	int 		ts_count;		//总共字段数；
	TBfield		*ts_field;		//field数组
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
//total query patten struct;查询结构模板
typedef struct _SQLPATTERN
{
	unsigned char 		sp_id[MD5_LENGTH];	//SQL语句标识符
	int				sp_type;		//标志该模版的类型：临时、静态、动态
	WITHSTRU		*sp_with;		//查询语句开始的with子句；
	SELECTLIST	*sp_selectlist;		//查询字段；
	TBFS				*sp_fromlist;		//table
	EXPRTREE	*sp_whereclause;	//where;
	HIERARCH	*sp_connectclause;	//HIERARCH connect;
//	STRFIELDS	*sp_groupbyclause;	//groupby
	GroupField	*sp_groupbyclause;

	EXPRTREE	*sp_havingclause;	//having
//	STRFIELDS	*sp_orderbyclause;	//order by
	OrderCla	*sp_orderbyclause;

	char		*sp_updateclause;	//for update clause;

	char		*sp_osqlstatement;	//原始的sql语句
	char		*sp_usqlstatement;	//归一化以后的sql语句

	time_t		sp_lastaccesstime;	//最后访问时间
	TimeCol		*time_col;		//包含有table name和时间列name
	QueryTime	*query_time;		//起始时间，终止时间
}SQLPATTERN;


typedef HASH_ELEMENT SQLPATTERNHASH;

//根据sp_id计算hash值，根据hash值访问到链表
SQLPATTERNHASH *sqlpattern_hash[SQLPATTERN_MAX_HASH_ELEMENT];

/*
 * Structure for user connect
 */
extern	int	listenport;	//监听的端口。
typedef struct _USRCOMM
{
	int 		sqlid;			//用户执行的语句标识，通过标识在SQL缓冲中去语句.此值需要传递到客户端，客户根据此值获取后续数据和执行后续操作。
	char		*sqlstr;		//语句实际内容？？？？？？
	int 		pagecount;		//分页时，每页的最大记录数。
	int		sqlmode;		//语句执行的模式，阻塞还是非阻塞
	int		retrownum;		//当前返回的记录号
	int		retpagenum;		//返回的页号
	int		resultcount;	//语句命中的结果最大记录数
	int		matchratio;		//语句于模版库州中模版的匹配程度。百分比值。
} USRSQLCOMM;

typedef struct _USR
{
	int		uid;				//用户连接ID。
	char 	username[MAX_USERNAME_LEN];
	char 	userpass[MAX_USERPASS_LEN];
	int		buf_maxrowcount;	//缓冲区最大保留的记录数。缺省的取全局参数设置值
	USRSQLCOMM *usrsqlcommhead;	//用户执行的语句链表。每个执行语句一个结构信息
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
