%{
/*
 *  by sunzhaoyu
 *  function: get token from the input sql query statement for parser's use
 */
#include "queryopt.h"
#include "speed.tab.h"
#include <string.h>
#include <stdlib.h>
void yyerror(char *s, yyscan_t yyscanner );
%}
%option bison-bridge
%option reentrant
%%

	/* literal keyword tokens */

ALL		{ return ALL; }
AND		{ return AND; }
AVG	|
MIN	|
MAX	|
SUM	|
COUNT		{ /* yylval->u_str=strdup("COUNT\0");*/
			yylval->u_str=(char *)FB_MALLOC(yyleng+1);
			memset(yylval->u_str, 0, yyleng+1 );
			memcpy(yylval->u_str, yytext, yyleng);

			return AMMSC;
		}
CONNECT		{ return CONNECT;	}
ANY		{ return ANY; }
AS		{ return AS; }
ASC		{ return ASC; }
BETWEEN		{ return BETWEEN; }
BLOCK		{ return BLOCK;	}
BY		{ return BY; }
CUBE		{ return CUBE;	}
CHECK		{ return CHECK; }
CONSTRAINT	{ return CONSTRAINT;	}
CROSS		{ return CROSS;	}
DESC		{ return DESC; }
DISTINCT	{ return DISTINCT; }
ESCAPE		{ return ESCAPE; }
EXISTS		{ return EXISTS; }
FOR		{ return FOR; }
FROM		{ return FROM; }
FIRST		{ return FIRST;	}
GROUP		{ return GROUP; }
GROUPING[ \t]+SETS		{ return GROUPING_SETS; }
HAVING		{ return HAVING; }
IN		{ return IN; }
INNER		{ return INNER;}
IS		{ return IS; }
LIKE		{ return LIKE; }
LAST		{ return LAST;	}
MAXVALUE	{ return MAXVALUE;	}
MINVALUE	{ return MINVALUE;	}
NATURAL		{ return NATURAL;	}
NOT		{ return NOT; }
NOCYCLE		{ return NOCYCLE;	}
NOWAIT		{ return NOWAIT;	}
NULL		{ return NULLX; }
NULLS		{ return NULLS;	}
OF		{ return OF; }
ON		{ return ON; }
ONLY		{ return ONLY;	}
OPTION		{ return OPTION; }
OR		{ return OR; }
ORDER		{ return ORDER; }
PARTITION	{ return PARTITION; }
READ		{ return READ;	}
ROLLUP		{ return ROLLUP;	}
SAMPLE		{ return SAMPLE;	}
START		{ return START;	}
SELECT		{ return SELECT; }
SIBLINGS	{ return SIBLINGS;	}
SOME		{ return SOME; }
SCN		{ return SCN;	}
TABLE		{ return TABLE; }
TIMESTAMP	{ return TIMESTAMP;	}
UNIQUE		{ return UNIQUE; }
UPDATE		{ return UPDATE; }
VERSIONS	{ return VERSIONS;	}
WHERE		{ return WHERE; }
WITH		{ return WITH; }
LEFT		{ return LEFT; }
RIGHT		{ return RIGHT; }
FULL   		{ return FULL;	}
OUTER		{ return OUTER;	}
JOIN		{ return JOIN;	}
SUBPARTITION	{ return SUBPARTITION;	}
SEED		{ return SEED;	}
USING		{ return USING;	}
CONTAINS	{ return CONTAINS; }

	/* punctuation */

"="	|
"<>" 	|
"<"	|
">"	|
"<="	|
">="		{
			/*yylval->u_str=strdup(yytext);*/
			yylval->u_str=(char *)FB_MALLOC(yyleng+1);
			memset(yylval->u_str, 0, yyleng+1 );
			memcpy(yylval->u_str, yytext, yyleng);
			return COMPARISON;
		}

[-+*/:(),.;@]	{ return yytext[0]; }

	/* names */

[A-Za-z][A-Za-z0-9_]*	{
				/*yylval->u_str=strdup(yytext); */
				yylval->u_str=(char *)FB_MALLOC(yyleng+1);
				memset(yylval->u_str, 0, yyleng+1 );
				memcpy(yylval->u_str, yytext, yyleng);

			  	return NAME;
			 }

	/* numbers */

[0-9]+	|
[0-9]+"."[0-9]* |
"."[0-9]*	{ /*yylval->u_str=strdup(yytext);*/
			yylval->u_str=(char *)FB_MALLOC(yyleng+1);
			memset(yylval->u_str, 0, yyleng+1 );
			memcpy(yylval->u_str, yytext, yyleng);
			return INTNUM;
		}

[0-9]+[eE][+-]?[0-9]+	|
[0-9]+"."[0-9]*[eE][+-]?[0-9]+ |
"."[0-9]*[eE][+-]?[0-9]+	{ /*yylval->u_str=strdup(yytext);*/
					yylval->u_str=(char *)FB_MALLOC(yyleng+1);
					memset(yylval->u_str, 0, yyleng+1 );
					memcpy(yylval->u_str, yytext, yyleng);
					return APPROXNUM;
				}
	/* strings */

'([^'\n]|\'\')*'	{
			/*yylval->u_str=strdup(yytext);*/
			yylval->u_str=(char *)FB_MALLOC(yyleng+1);
			memset(yylval->u_str, 0, yyleng+1 );
			memcpy(yylval->u_str, yytext, yyleng);
			return STRING;
	}

'([^'\n]|\'\')*$	{ yyerror("Unterminated string", yyg); return 256;}
[ \t\r]+	;	/* white space */
"--".*$		;	/* comment */
.		{
			/*/printf("%d\t", yytext[0] );
			if( yytext[0] >= 126 )
			{
				//printf("%d\t", yytext[0] );
				return 48;
			}
			else
			*/
			return yytext[0];	//yyerror("bad input", yyg);
		}
%%

void
yyerror(char *s, yyscan_t yyscanner )
{
	//struct yyguts_t *yyg=(struct yyguts_t *) yyscanner;
	//printf("lexical error: %s at %s\n", s, yytext);
	printf("lexical error: %s \n", s );
}
int yywrap(yyscan_t yyscanner )
{
return 1;
}

/*
main(int ac, char **av)
{
	//read from file
	if(ac > 1 && (yyin = fopen(av[1], "r")) == NULL) {
		perror(av[1]);
		exit(1);
	}
	//read from string
	yy_scan_string("SELECT * FROM dept");
	//another way to get from string;
	char buf[30];
	strcpy(buf,"SELECT * FROM dept");
	yy_scan_string(buf);
	buf[strlen("SELECT * FROM dept")]=YY_END_OF_BUFFER_CHAR;
	buf[strlen("SELECT * FROM dept")+1]=YY_END_OF_BUFFER_CHAR;
	if(yy_scan_buffer(buf,strlen(buf))==NULL)
	printf("can't build the buffer\n");

	//call yyparse();
	if(!yyparse())
		printf("SQL parse worked\n");
	else
		printf("SQL parse failed\n");

	printf("\n*****query string from structure***********\n%s\n***************\n\n",query_stru->sp_usqlstatement);
	//free structure;
	freePattern(query_stru);
}
*/
