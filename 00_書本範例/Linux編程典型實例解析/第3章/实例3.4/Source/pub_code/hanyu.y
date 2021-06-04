%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queryopt.h"

typedef struct {
	MidKey	*m_key;
}KeyInfo;

#define YYLEX_PARAM  hy_scanner
#define YYPARSE_PARAM hy_scanner

#define	KEY_ROOT ((KeyInfo *)hyget_extra( hy_scanner ))->m_key

%}

%union	{
	MidKey		*u_key;
	char		*u_str;
};

%pure_parser

%start key_start
%type <u_key> 	and_or_expr key_start

%token <u_str>  KEY_INDEX

%left OR
%left AND

%%
key_start:
		and_or_expr 
		{
			KEY_ROOT = $1;
		}
		;
and_or_expr:/* 0:none, 1:(), 3:and, 4:or */
		and_or_expr AND and_or_expr
		{
			$$ = ( MidKey * )combi_midKey( $1, $3, 3 );
			if( !$$ )
				printf("and null\n");
		}
	|	and_or_expr OR and_or_expr
		{
			$$ = ( MidKey * )combi_midKey( $1, $3, 4 );
		}
	|	'(' and_or_expr ')'
		{
			if( $2->m_type < 2 )
				$$ = $2;
			else
			{
				MidKey		*mk;
				mk = (MidKey *)FB_MALLOC( sizeof(MidKey) );
				memset( mk, 0, sizeof(MidKey) );
				mk->m_type = 1;
				mk->m_lefttype = 1;
				mk->m_righttype = -1;
				mk->m_leftcs = $2;
				$$ = mk;
			}
		}
	|	KEY_INDEX
		{
			MidKey		*mk;
			KeyContain	*tmp;
			tmp = (KeyContain*)FB_MALLOC(sizeof(KeyContain));
			memset( tmp, 0, sizeof(KeyContain) );
			strcpy( tmp->k_word, $1 );
			mk = (MidKey *)FB_MALLOC( sizeof(MidKey) );
			memset( mk, 0, sizeof(MidKey) );
			mk->m_type = 0;
			mk->m_lefttype = 0;
			mk->m_leftbs = tmp;
			mk->m_righttype = -1;
			$$ = mk;
		}
	;
%%
MidKey *hanyu_parse( const char *cla )
{
	KeyInfo *ki;
	void *hy_scanner;

	ki = (KeyInfo*)FB_MALLOC(sizeof(KeyInfo) );
	memset( ki, 0, sizeof(KeyInfo) );
	hylex_init ( &hy_scanner );
	hyset_extra ( ki, hy_scanner );
	hy_scan_string( cla, hy_scanner );
	if(!yyparse( hy_scanner ))
	{
		MidKey *tmp;
		tmp = KEY_ROOT;
		FB_FREE( ki );
		hylex_destroy ( hy_scanner );
		return tmp;
	}

	FB_FREE( ki );
	hylex_destroy ( hy_scanner );
	return NULL;
}

