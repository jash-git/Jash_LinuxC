%{
#include <stdio.h>
#include "queryopt.h"
#include "hanyu.tab.h"
//[-+*/:(),.;@]	{ return yytext[0]; }
%}
%option reentrant
%option bison-bridge
%%
AND 		return AND;

OR		return OR;	
[\(\)]		return yytext[0]; 
([^ \'\(\)\r\n]|\'\'|\\\(|\\\))*	{
			yylval->u_str=(char *)FB_MALLOC(yyleng+1);
			memset(yylval->u_str, 0, yyleng+1 );
			memcpy(yylval->u_str, yytext, yyleng);
			return KEY_INDEX;
		}
.		;
%%
void hyerror(char *s, yyscan_t yyscanner )
{
	//struct yyguts_t *yyg=(struct yyguts_t *) yyscanner;
	//printf("lexical error: %s at %s\n", s, yytext);
	printf("lexical error: %s \n", s );
}
int yywrap(yyscan_t yyscanner )
{
return 1;
}

