/* A Bison parser, made by GNU Bison 1.875c.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NAME = 258,
     AMMSC = 259,
     INTNUM = 260,
     STRING = 261,
     APPROXNUM = 262,
     SELECT = 263,
     UNIQUE = 264,
     DISTINCT = 265,
     SUBPARTITION = 266,
     BLOCK = 267,
     SEED = 268,
     ON = 269,
     USING = 270,
     INNER = 271,
     CROSS = 272,
     NATURAL = 273,
     LEFT = 274,
     RIGHT = 275,
     OUTER = 276,
     LIKE = 277,
     ESCAPE = 278,
     FULL = 279,
     FOR = 280,
     UPDATE = 281,
     OF = 282,
     NOWAIT = 283,
     AS = 284,
     TIMESTAMP = 285,
     MINVALUE = 286,
     MAXVALUE = 287,
     READ = 288,
     ONLY = 289,
     TABLE = 290,
     WITH = 291,
     CHECK = 292,
     OPTION = 293,
     CONSTRAINT = 294,
     VERSIONS = 295,
     SCN = 296,
     BETWEEN = 297,
     AND = 298,
     IS = 299,
     NOT = 300,
     NULLX = 301,
     NULLS = 302,
     ASC = 303,
     DESC = 304,
     FIRST = 305,
     LAST = 306,
     ORDER = 307,
     SIBLINGS = 308,
     ALL = 309,
     ANY = 310,
     SOME = 311,
     EXISTS = 312,
     NOCYCLE = 313,
     START = 314,
     HAVING = 315,
     CUBE = 316,
     IN = 317,
     BY = 318,
     JOIN = 319,
     PARTITION_BY = 320,
     FROM = 321,
     PARTITION = 322,
     SAMPLE = 323,
     WHERE = 324,
     CONNECT = 325,
     GROUP = 326,
     ROLLUP = 327,
     GROUPING_SETS = 328,
     CONTAINS = 329,
     OR = 330,
     COMPARISON = 331,
     UMINUS = 332
   };
#endif
#define NAME 258
#define AMMSC 259
#define INTNUM 260
#define STRING 261
#define APPROXNUM 262
#define SELECT 263
#define UNIQUE 264
#define DISTINCT 265
#define SUBPARTITION 266
#define BLOCK 267
#define SEED 268
#define ON 269
#define USING 270
#define INNER 271
#define CROSS 272
#define NATURAL 273
#define LEFT 274
#define RIGHT 275
#define OUTER 276
#define LIKE 277
#define ESCAPE 278
#define FULL 279
#define FOR 280
#define UPDATE 281
#define OF 282
#define NOWAIT 283
#define AS 284
#define TIMESTAMP 285
#define MINVALUE 286
#define MAXVALUE 287
#define READ 288
#define ONLY 289
#define TABLE 290
#define WITH 291
#define CHECK 292
#define OPTION 293
#define CONSTRAINT 294
#define VERSIONS 295
#define SCN 296
#define BETWEEN 297
#define AND 298
#define IS 299
#define NOT 300
#define NULLX 301
#define NULLS 302
#define ASC 303
#define DESC 304
#define FIRST 305
#define LAST 306
#define ORDER 307
#define SIBLINGS 308
#define ALL 309
#define ANY 310
#define SOME 311
#define EXISTS 312
#define NOCYCLE 313
#define START 314
#define HAVING 315
#define CUBE 316
#define IN 317
#define BY 318
#define JOIN 319
#define PARTITION_BY 320
#define FROM 321
#define PARTITION 322
#define SAMPLE 323
#define WHERE 324
#define CONNECT 325
#define GROUP 326
#define ROLLUP 327
#define GROUPING_SETS 328
#define CONTAINS 329
#define OR 330
#define COMPARISON 331
#define UMINUS 332




/* Copy the first part of user declarations.  */
#line 1 "speed.y"

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


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 24 "speed.y"
typedef union YYSTYPE {
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
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 274 "speed.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 286 "speed.tab.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

# ifndef YYFREE
#  define YYFREE free
# endif
# ifndef YYMALLOC
#  define YYMALLOC malloc
# endif

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   define YYSTACK_ALLOC alloca
#  endif
# else
#  if defined (alloca) || defined (_ALLOCA_H)
#   define YYSTACK_ALLOC alloca
#  else
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   590

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  87
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  56
/* YYNRULES -- Number of rules. */
#define YYNRULES  183
/* YYNRULES -- Number of states. */
#define YYNSTATES  406

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   332

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      82,    83,    79,    77,    84,    78,    85,    80,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    86,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    81
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     6,    15,    26,    27,    30,    36,    44,
      47,    50,    51,    53,    55,    57,    59,    63,    67,    73,
      76,    77,    79,    82,    84,    86,    90,    94,    98,   102,
     105,   108,   113,   119,   125,   130,   135,   139,   141,   143,
     145,   146,   148,   150,   152,   156,   162,   164,   168,   170,
     174,   176,   182,   189,   192,   196,   198,   203,   208,   216,
     222,   226,   229,   234,   238,   244,   247,   252,   257,   262,
     267,   273,   282,   292,   293,   297,   301,   307,   308,   313,
     318,   329,   340,   342,   344,   346,   348,   354,   362,   369,
     378,   383,   388,   394,   399,   406,   415,   421,   429,   439,
     441,   445,   447,   449,   451,   454,   457,   460,   464,   468,
     471,   475,   477,   479,   481,   483,   485,   487,   489,   491,
     498,   502,   506,   512,   519,   525,   531,   536,   537,   540,
     545,   549,   556,   562,   569,   575,   577,   581,   588,   590,
     592,   594,   599,   600,   603,   604,   608,   613,   620,   628,
     632,   634,   638,   640,   642,   644,   649,   654,   656,   660,
     664,   670,   674,   678,   683,   688,   689,   692,   693,   697,
     702,   706,   712,   713,   715,   717,   718,   721,   724,   725,
     729,   730,   733,   737
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
      88,     0,    -1,    89,   141,    -1,    90,     8,    92,    66,
     101,   127,   128,   137,    -1,    90,     8,    92,    66,   101,
     127,   128,   129,   136,   137,    -1,    -1,    36,    91,    -1,
       3,    29,    82,    89,    83,    -1,    91,    84,     3,    29,
      82,    89,    83,    -1,    93,    79,    -1,    93,    94,    -1,
      -1,    54,    -1,     9,    -1,    10,    -1,    95,    -1,    94,
      84,    95,    -1,     3,    85,    79,    -1,     3,    85,     3,
      85,    79,    -1,    97,    96,    -1,    -1,     3,    -1,    29,
       3,    -1,    98,    -1,   100,    -1,    97,    77,    97,    -1,
      97,    78,    97,    -1,    97,    79,    97,    -1,    97,    80,
      97,    -1,    77,    97,    -1,    78,    97,    -1,     4,    82,
      79,    83,    -1,     4,    82,    10,   100,    83,    -1,     4,
      82,    54,    97,    83,    -1,     4,    82,    97,    83,    -1,
       3,    82,    99,    83,    -1,    82,    97,    83,    -1,     6,
      -1,     5,    -1,     7,    -1,    -1,    97,    -1,   134,    -1,
       3,    -1,     3,    85,     3,    -1,     3,    85,     3,    85,
       3,    -1,   102,    -1,   101,    84,   102,    -1,   103,    -1,
      82,   111,    83,    -1,   111,    -1,    34,    82,   104,    83,
     108,    -1,    34,    82,   104,    83,   108,     3,    -1,   104,
     108,    -1,   104,   108,     3,    -1,     3,    -1,    82,    89,
     107,    83,    -1,    35,    82,     3,    83,    -1,    35,    82,
       3,    83,    82,    77,    83,    -1,     3,    85,     3,    86,
       3,    -1,     3,    86,     3,    -1,     3,   105,    -1,     3,
      85,     3,   105,    -1,     3,   105,   106,    -1,     3,    85,
       3,   105,   106,    -1,     3,   106,    -1,     3,    85,     3,
     106,    -1,    67,    82,     3,    83,    -1,    11,    82,     3,
      83,    -1,    68,    82,     5,    83,    -1,    68,    12,    82,
       5,    83,    -1,    68,    82,     5,    83,    13,    82,     5,
      83,    -1,    68,    12,    82,     5,    83,    13,    82,     5,
      83,    -1,    -1,    36,    33,    34,    -1,    36,    37,    38,
      -1,    36,    37,    38,    39,     3,    -1,    -1,    29,    27,
      41,    97,    -1,    29,    27,    30,    97,    -1,    40,    42,
      41,   109,    43,   110,    29,    27,    41,    97,    -1,    40,
      42,    30,   109,    43,   110,    29,    27,    30,    97,    -1,
      97,    -1,    31,    -1,    97,    -1,    32,    -1,   102,    64,
     103,    14,   114,    -1,   102,    64,   103,    15,    82,   112,
      83,    -1,   102,    16,    64,   103,    14,   114,    -1,   102,
      16,    64,   103,    15,    82,   112,    83,    -1,   102,    17,
      64,   103,    -1,   102,    18,    64,   103,    -1,   102,    18,
      16,    64,   103,    -1,   102,   113,    64,   103,    -1,   102,
     113,    64,   103,    14,   114,    -1,   102,   113,    64,   103,
      15,    82,   112,    83,    -1,   102,    18,   113,    64,   103,
      -1,   102,    18,   113,    64,   103,    14,   114,    -1,   102,
      18,   113,    64,   103,    15,    82,   112,    83,    -1,   100,
      -1,   112,    84,   100,    -1,    24,    -1,    19,    -1,    20,
      -1,    24,    21,    -1,    19,    21,    -1,    20,    21,    -1,
     114,    75,   114,    -1,   114,    43,   114,    -1,    45,   114,
      -1,    82,   114,    83,    -1,   115,    -1,   117,    -1,   118,
      -1,   119,    -1,   121,    -1,   122,    -1,   124,    -1,   126,
      -1,    74,    82,   100,    84,     6,    83,    -1,   116,    76,
       5,    -1,    97,    76,    97,    -1,    97,    76,    82,    89,
      83,    -1,    97,    45,    42,    97,    43,    97,    -1,    97,
      42,    97,    43,    97,    -1,    97,    45,    22,    98,   120,
      -1,    97,    22,    98,   120,    -1,    -1,    23,    98,    -1,
     100,    44,    45,    46,    -1,   100,    44,    46,    -1,    97,
      45,    62,    82,    89,    83,    -1,    97,    62,    82,    89,
      83,    -1,    97,    45,    62,    82,   123,    83,    -1,    97,
      62,    82,   123,    83,    -1,    98,    -1,   123,    84,    98,
      -1,    97,    76,   125,    82,    89,    83,    -1,    55,    -1,
      54,    -1,    56,    -1,    57,    82,    89,    83,    -1,    -1,
      69,   114,    -1,    -1,    70,    63,   114,    -1,    70,    63,
      58,   114,    -1,    59,    36,   114,    70,    63,   114,    -1,
      59,    36,   114,    70,    63,    58,   114,    -1,    71,    63,
     130,    -1,   131,    -1,   130,    84,   131,    -1,    97,    -1,
     132,    -1,   135,    -1,    72,    82,   133,    83,    -1,    61,
      82,   133,    83,    -1,    97,    -1,    82,   134,    83,    -1,
     133,    84,    97,    -1,   133,    84,    82,   134,    83,    -1,
      97,    84,    97,    -1,   134,    84,    97,    -1,    73,    82,
     133,    83,    -1,    73,    82,   132,    83,    -1,    -1,    60,
     114,    -1,    -1,    52,    63,   138,    -1,    52,    53,    63,
     138,    -1,    97,   139,   140,    -1,   138,    84,    97,   139,
     140,    -1,    -1,    48,    -1,    49,    -1,    -1,    47,    50,
      -1,    47,    51,    -1,    -1,    25,    26,   142,    -1,    -1,
      27,   112,    -1,    27,   112,    28,    -1,    27,   112,     5,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,    87,    87,    96,   120,   149,   150,   156,   173,   200,
     225,   247,   250,   254,   258,   264,   273,   307,   319,   331,
     347,   348,   350,   354,   355,   356,   363,   370,   377,   384,
     390,   396,   407,   415,   423,   431,   439,   447,   448,   449,
     453,   454,   455,   458,   462,   470,   483,   491,   515,   523,
     531,   541,   552,   565,   576,   592,   617,   627,   638,   649,
     662,   674,   686,   699,   712,   727,   739,   755,   764,   775,
     781,   787,   795,   806,   807,   809,   811,   822,   826,   833,
     840,   848,   858,   859,   863,   864,   869,   886,   905,   923,
     942,   960,   978,   996,  1015,  1034,  1054,  1073,  1092,  1114,
    1115,  1160,  1162,  1164,  1166,  1168,  1170,  1175,  1180,  1184,
    1218,  1239,  1285,  1286,  1287,  1288,  1289,  1290,  1291,  1294,
    1317,  1347,  1382,  1399,  1414,  1460,  1475,  1494,  1498,  1506,
    1519,  1535,  1548,  1561,  1576,  1593,  1594,  1627,  1645,  1649,
    1653,  1659,  1675,  1676,  1698,  1701,  1710,  1719,  1728,  1740,
    1747,  1762,  1788,  1789,  1790,  1793,  1799,  1807,  1808,  1814,
    1829,  1847,  1862,  1879,  1886,  1897,  1898,  1910,  1911,  1923,
    1939,  1973,  2013,  2017,  2021,  2028,  2032,  2036,  2044,  2045,
    2054,  2058,  2064,  2070
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NAME", "AMMSC", "INTNUM", "STRING",
  "APPROXNUM", "SELECT", "UNIQUE", "DISTINCT", "SUBPARTITION", "BLOCK",
  "SEED", "ON", "USING", "INNER", "CROSS", "NATURAL", "LEFT", "RIGHT",
  "OUTER", "LIKE", "ESCAPE", "FULL", "FOR", "UPDATE", "OF", "NOWAIT", "AS",
  "TIMESTAMP", "MINVALUE", "MAXVALUE", "READ", "ONLY", "TABLE", "WITH",
  "CHECK", "OPTION", "CONSTRAINT", "VERSIONS", "SCN", "BETWEEN", "AND",
  "IS", "NOT", "NULLX", "NULLS", "ASC", "DESC", "FIRST", "LAST", "ORDER",
  "SIBLINGS", "ALL", "ANY", "SOME", "EXISTS", "NOCYCLE", "START", "HAVING",
  "CUBE", "IN", "BY", "JOIN", "PARTITION_BY", "FROM", "PARTITION",
  "SAMPLE", "WHERE", "CONNECT", "GROUP", "ROLLUP", "GROUPING_SETS",
  "CONTAINS", "OR", "COMPARISON", "'+'", "'-'", "'*'", "'/'", "UMINUS",
  "'('", "')'", "','", "'.'", "'@'", "$accept", "oraselect", "subquery",
  "sub_factor", "head_query", "select_list", "sign", "sub_list",
  "list_atom", "as_alias", "expr", "atom", "funclist", "col_ref",
  "table_ref", "tb_ref", "tb_atom", "query_tb_expr", "part_sub",
  "sample_cla", "sub_restrict", "flashback_query", "expr_min", "expr_max",
  "join_cla", "collist", "outer_type", "condition", "predicate",
  "contain_key", "comparison_predicate", "between_predicate",
  "like_predicate", "opt_escape", "test_for_null", "in_predicate",
  "atom_commalist", "all_or_any_predicate", "any_all_some",
  "existence_test", "where_cla", "hierarch_query", "group_cla", "gfields",
  "gfield", "rollup_cube_cla", "grouping_expr_list", "expr_list",
  "grouping_sets_cla", "having_cla", "order_cla", "order_tail", "otail2",
  "otail3", "for_update", "up_of_col", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,    43,    45,    42,
      47,   332,    40,    41,    44,    46,    64
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    87,    88,    89,    89,    90,    90,    91,    91,    92,
      92,    93,    93,    93,    93,    94,    94,    95,    95,    95,
      96,    96,    96,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    98,    98,    98,
      99,    99,    99,   100,   100,   100,   101,   101,   102,   102,
     102,   103,   103,   103,   103,   104,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   105,   105,   106,
     106,   106,   106,   107,   107,   107,   107,   108,   108,   108,
     108,   108,   109,   109,   110,   110,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   111,   111,   112,
     112,   113,   113,   113,   113,   113,   113,   114,   114,   114,
     114,   114,   115,   115,   115,   115,   115,   115,   115,   116,
     117,   117,   117,   118,   118,   119,   119,   120,   120,   121,
     121,   122,   122,   122,   122,   123,   123,   124,   125,   125,
     125,   126,   127,   127,   128,   128,   128,   128,   128,   129,
     130,   130,   131,   131,   131,   132,   132,   133,   133,   133,
     133,   134,   134,   135,   135,   136,   136,   137,   137,   137,
     138,   138,   139,   139,   139,   140,   140,   140,   141,   141,
     142,   142,   142,   142
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     2,     8,    10,     0,     2,     5,     7,     2,
       2,     0,     1,     1,     1,     1,     3,     3,     5,     2,
       0,     1,     2,     1,     1,     3,     3,     3,     3,     2,
       2,     4,     5,     5,     4,     4,     3,     1,     1,     1,
       0,     1,     1,     1,     3,     5,     1,     3,     1,     3,
       1,     5,     6,     2,     3,     1,     4,     4,     7,     5,
       3,     2,     4,     3,     5,     2,     4,     4,     4,     4,
       5,     8,     9,     0,     3,     3,     5,     0,     4,     4,
      10,    10,     1,     1,     1,     1,     5,     7,     6,     8,
       4,     4,     5,     4,     6,     8,     5,     7,     9,     1,
       3,     1,     1,     1,     2,     2,     2,     3,     3,     2,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     6,
       3,     3,     5,     6,     5,     5,     4,     0,     2,     4,
       3,     6,     5,     6,     5,     1,     3,     6,     1,     1,
       1,     4,     0,     2,     0,     3,     4,     6,     7,     3,
       1,     3,     1,     1,     1,     4,     4,     1,     3,     3,
       5,     3,     3,     4,     4,     0,     2,     0,     3,     4,
       3,     5,     0,     1,     1,     0,     2,     2,     0,     3,
       0,     2,     3,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       5,     0,     0,   178,     0,     0,     6,     1,     0,     2,
      11,     0,     0,   180,    13,    14,    12,     0,     0,     5,
       0,     0,   179,     0,    43,     0,    38,    37,    39,     0,
       0,     9,     0,    10,    15,    20,    23,    24,     0,     0,
      43,    99,   181,    55,     0,     0,     5,   142,    46,    48,
      77,    50,    40,     0,     0,    43,    29,    30,     0,     0,
      21,     0,     0,     0,     0,     0,    19,     7,     5,     0,
     183,   182,     0,     0,     0,     0,     0,     0,    61,    65,
       0,     0,    73,     0,    50,     0,     0,   144,     0,     0,
       0,   102,   103,   101,     0,     0,     0,     0,    53,    41,
       0,    42,    44,    17,     0,     0,     0,     0,    36,    16,
      22,    25,    26,    27,    28,     0,    44,   100,     0,     0,
       0,     0,     0,    60,    63,     5,     0,     0,     0,     0,
      49,     0,     0,     0,     0,     0,    24,   143,   111,     0,
     112,   113,   114,   115,   116,   117,   118,    47,     0,     0,
     167,     0,     0,     0,     0,     0,   105,   106,   104,     0,
       0,     0,     0,    54,     0,    35,     0,     0,     0,     0,
      31,    34,     8,     0,     0,     0,     0,     0,     0,    62,
      66,    77,    57,     0,     0,    56,   109,     5,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   165,     3,     0,    90,     0,    91,
       0,     0,     0,    93,     0,     0,     0,     0,   161,   162,
      45,    18,    32,    33,    68,    67,     0,    69,    59,    64,
      51,     0,    74,    75,     0,     0,   110,   127,     0,     0,
       0,     0,     5,   139,   138,   140,     5,   121,     0,     0,
     130,   108,   107,   120,     0,     0,   145,     0,     0,     0,
       0,   167,     0,     0,    92,    96,    86,     0,     0,     0,
      79,    78,    83,    82,     0,     0,    70,     0,    52,     0,
       0,   141,     0,     0,   126,     0,   127,     0,     5,     0,
     135,     0,     0,     5,   129,     0,   146,     0,   172,   168,
       0,     0,     0,   152,   149,   150,   153,   154,   166,     4,
      88,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,    58,    76,     0,   128,   124,   125,     0,     0,     0,
     132,   134,     0,   122,     0,     0,   169,   173,   174,   175,
       0,     0,     0,     0,     0,     0,    97,     0,    87,     0,
      85,    84,     0,     0,     0,     0,   119,   123,   131,   133,
     136,   137,     0,   147,     0,   170,   172,     0,   157,     0,
       0,     0,     0,   151,    89,     0,    95,     0,     0,     0,
      71,   148,   176,   177,   175,     0,     0,   156,     0,   155,
     164,   163,    98,     0,     0,    72,   171,   158,     0,   159,
       0,     0,     0,    81,    80,   160
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,     2,    82,     4,     6,    17,    18,    33,    34,    66,
     135,    36,   100,    37,    47,    48,    49,    50,    78,    79,
     129,    98,   274,   352,    51,    42,    95,   137,   138,   139,
     140,   141,   142,   284,   143,   144,   291,   145,   248,   146,
      87,   150,   204,   304,   305,   306,   369,   101,   307,   261,
     205,   299,   339,   365,     9,    22
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -311
static const short yypact[] =
{
     -29,    34,   109,    60,    82,    98,    61,  -311,   142,  -311,
       8,    91,   178,   173,  -311,  -311,  -311,   120,   160,   -29,
     198,   227,  -311,    39,    65,   154,  -311,  -311,  -311,   352,
     352,  -311,   352,   118,  -311,   347,  -311,  -311,   168,   171,
     170,  -311,    15,   140,   175,   184,    32,    51,   385,  -311,
     -13,  -311,   352,     7,   133,   139,  -311,  -311,   412,   358,
    -311,   275,   352,   352,   352,   352,  -311,  -311,   -29,   293,
    -311,  -311,   227,   216,   219,    -7,   304,   315,   269,  -311,
      19,   335,   317,   385,   268,   213,    39,     6,   296,   322,
     159,   360,   370,   379,    94,   350,   384,   337,   414,   404,
     336,   353,   354,  -311,   227,   352,   374,   419,  -311,  -311,
    -311,   115,   115,  -311,  -311,   375,   362,  -311,   448,   457,
     387,   456,   102,  -311,  -311,   -29,   388,   394,    85,   395,
    -311,   213,   403,   405,   213,   386,   442,    -5,  -311,   417,
    -311,  -311,  -311,  -311,  -311,  -311,  -311,   385,   434,   431,
     -50,    94,    94,   436,    94,   437,  -311,  -311,  -311,   199,
      94,   114,   141,  -311,   352,  -311,   352,    23,   424,   426,
    -311,  -311,  -311,   505,   432,   435,   509,   438,   513,   269,
    -311,   -13,   440,   483,   481,  -311,  -311,   -29,   227,   376,
     -35,   238,   352,    -3,   441,   228,   203,   213,   213,   515,
     213,    26,    62,   461,   465,  -311,   254,  -311,    94,  -311,
      94,   213,   444,   257,   352,   352,    75,    75,   433,   433,
    -311,  -311,  -311,  -311,  -311,  -311,   445,   514,  -311,  -311,
     526,   453,  -311,   492,   449,   450,  -311,   510,    81,   238,
     352,   454,   105,  -311,  -311,  -311,   310,   433,   455,   489,
    -311,  -311,   495,  -311,   -19,   213,    -5,   476,   352,   258,
     213,   488,   213,   459,  -311,   266,    -5,   227,   213,   460,
     433,   433,  -311,   433,   500,   501,   532,   464,  -311,   466,
     544,  -311,   542,   238,  -311,   352,   510,   289,   105,   467,
    -311,   202,   468,   -29,  -311,   490,    -5,   352,   364,   470,
     473,   474,   475,   433,   477,  -311,  -311,  -311,    -5,  -311,
      -5,   227,   213,   478,   205,    -5,   227,   270,   270,   480,
     547,  -311,  -311,   482,  -311,   433,  -311,   352,   484,   210,
    -311,  -311,   238,  -311,   485,   127,   470,  -311,  -311,   511,
     352,   368,   368,   338,   258,   237,    -5,   227,  -311,   245,
    -311,   433,   530,   534,   559,   486,  -311,   433,  -311,  -311,
    -311,  -311,   213,    -5,   261,  -311,   364,   352,   433,   250,
     294,   487,   301,  -311,  -311,   306,  -311,   539,   545,   491,
    -311,    -5,  -311,  -311,   511,   396,   323,  -311,   390,  -311,
    -311,  -311,  -311,   541,   535,  -311,  -311,  -311,   352,   433,
     352,   352,   339,   433,   433,  -311
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
    -311,  -311,     4,  -311,  -311,  -311,  -311,  -311,   516,  -311,
     -18,  -190,  -311,    -8,  -311,   -31,   -91,   493,   458,   -72,
    -311,   397,   365,   259,   533,  -258,   494,  -106,  -311,  -311,
    -311,  -311,  -311,   295,  -311,  -311,   297,  -311,  -311,  -311,
    -311,  -311,  -311,  -311,   239,   243,    90,  -310,  -311,  -311,
     326,   291,   223,   206,  -311,  -311
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned short yytable[] =
{
      35,   237,   202,   159,     3,   120,   124,     1,   197,   314,
     102,    56,    57,    41,    58,    83,    96,    14,    15,   239,
      70,   203,    43,    38,   197,   186,   220,    97,   190,    55,
      25,    26,    27,    28,    99,    43,   107,     5,   197,   240,
     198,    35,    43,    71,   111,   112,   113,   114,   236,   286,
     180,   295,   290,   345,    45,   147,   198,   386,   349,   241,
     206,   207,    16,   209,   117,   148,    44,    45,     1,   213,
     198,   131,   115,    44,    45,   121,   149,   136,    55,    25,
      26,    27,    28,   132,   255,     8,   103,   169,   402,   375,
      10,   251,   252,   324,   254,   256,   168,    43,   290,    72,
     133,   125,   221,    29,    30,   266,   272,   229,   134,     7,
      26,    27,    28,    73,    46,   257,   189,   264,   183,   265,
      85,    46,   184,   136,   285,   258,   136,    11,    44,    45,
      55,    25,    26,    27,    28,    86,    55,    25,    26,    27,
      28,     1,   360,   104,   214,    12,   218,    52,   219,   296,
      53,    73,    29,    30,   308,   215,   310,    32,    62,    63,
      64,    65,   315,    24,    25,    26,    27,    28,    13,    74,
      75,   216,   131,    19,   238,   153,   125,   247,    91,    92,
     235,    20,   217,    93,   132,   362,    23,   105,   178,   136,
     136,   234,   136,   136,    64,    65,   270,   271,   273,   273,
      21,   133,    59,   136,    29,    30,   346,    74,    75,   134,
      29,    30,   106,   211,   212,    32,    55,    25,    26,    27,
      28,    52,   287,   154,    69,    76,    77,    39,    58,   363,
      40,    55,    25,    26,    27,    28,    54,    29,    30,    31,
     298,   303,    32,    26,    27,    28,   289,   136,   249,   250,
     292,    67,   136,    68,   136,    69,   381,    80,   131,    41,
     136,    55,    25,    26,    27,    28,    81,   325,   262,   263,
     132,   268,   269,    55,    25,    26,    27,    28,   110,   298,
     312,   313,   243,   244,   245,   331,   332,   133,   348,    72,
      29,    30,   328,   359,   332,   134,   116,   334,   118,   351,
     351,   119,   350,    41,   136,    29,    30,   122,    41,   357,
     246,   382,   383,    55,    25,    26,    27,    28,   123,   300,
     374,    72,   366,   368,   368,   368,   303,   136,   376,    72,
     301,   302,   327,   387,   388,    29,    30,    75,   127,    41,
      32,    55,    25,    26,    27,    28,     1,    29,    30,   385,
      60,   130,    32,   128,   136,    55,    25,    26,    27,    28,
     151,    24,    25,    26,    27,    28,    62,    63,    64,    65,
     399,    55,    25,    26,    27,    28,    61,   389,   388,   162,
     385,   156,   403,   404,   391,   388,   152,    29,    30,   392,
      72,   157,    32,    55,    25,    26,    27,    28,   191,   300,
     158,    88,    89,    90,    91,    92,   397,   166,   191,    93,
     301,   161,   337,   338,   160,    29,    30,   163,   192,   165,
     367,   193,   405,   166,    62,    63,    64,    65,   192,    29,
      30,   193,   370,   372,    32,    29,    30,   166,   194,   167,
      32,    62,    63,    64,    65,    29,    30,   173,   194,    94,
     367,   174,   195,    62,    63,    64,    65,   170,   172,   108,
     175,   177,   195,    62,    63,    64,    65,    29,    30,   176,
     200,   181,   398,    62,    63,    64,    65,   182,   185,   108,
     164,    62,    63,    64,    65,   187,   196,   188,   164,    62,
      63,    64,    65,   199,   201,   108,    62,    63,    64,    65,
     208,   210,   171,    62,    63,    64,    65,   222,   220,   223,
      62,    63,    64,    65,   226,   224,   228,   232,   225,   233,
     253,   227,   231,   242,   259,   260,   267,   277,   276,   278,
     279,   280,   281,   283,   282,   294,   288,   293,   197,   297,
     202,   311,   316,   317,   318,   319,   320,   322,   323,   321,
     330,   333,   355,   335,   340,   341,   342,   343,   364,   377,
     347,   344,   354,   378,   379,   356,   393,   358,   361,   380,
     390,   400,   394,   126,   395,   109,   401,   353,   230,    84,
     179,   326,   275,   373,   155,   329,   371,   309,   336,   384,
     396
};

static const unsigned short yycheck[] =
{
      18,   191,    52,    94,     0,    12,    78,    36,    43,   267,
       3,    29,    30,    21,    32,    46,    29,     9,    10,    22,
       5,    71,     3,    19,    43,   131,     3,    40,   134,     3,
       4,     5,     6,     7,    52,     3,    54,     3,    43,    42,
      75,    59,     3,    28,    62,    63,    64,    65,    83,   239,
     122,    70,   242,   311,    35,    86,    75,   367,   316,    62,
     151,   152,    54,   154,    72,    59,    34,    35,    36,   160,
      75,    45,    68,    34,    35,    82,    70,    85,     3,     4,
       5,     6,     7,    57,    58,    25,    79,   105,   398,   347,
       8,   197,   198,   283,   200,   201,   104,     3,   288,    84,
      74,    82,    79,    77,    78,   211,    31,   179,    82,     0,
       5,     6,     7,    11,    82,    53,   134,   208,    33,   210,
      69,    82,    37,   131,    43,    63,   134,    29,    34,    35,
       3,     4,     5,     6,     7,    84,     3,     4,     5,     6,
       7,    36,   332,    10,    30,    84,   164,    82,   166,   255,
      85,    11,    77,    78,   260,    41,   262,    82,    77,    78,
      79,    80,   268,     3,     4,     5,     6,     7,    26,    67,
      68,    30,    45,    82,   192,    16,    82,   195,    19,    20,
     188,     3,    41,    24,    57,    58,    66,    54,    86,   197,
     198,   187,   200,   201,    79,    80,   214,   215,   216,   217,
      27,    74,    84,   211,    77,    78,   312,    67,    68,    82,
      77,    78,    79,    14,    15,    82,     3,     4,     5,     6,
       7,    82,   240,    64,    85,    85,    86,    29,   246,   335,
       3,     3,     4,     5,     6,     7,    82,    77,    78,    79,
     258,   259,    82,     5,     6,     7,   242,   255,    45,    46,
     246,    83,   260,    82,   262,    85,   362,    82,    45,   267,
     268,     3,     4,     5,     6,     7,    82,   285,    14,    15,
      57,    14,    15,     3,     4,     5,     6,     7,     3,   297,
      14,    15,    54,    55,    56,    83,    84,    74,    83,    84,
      77,    78,   288,    83,    84,    82,     3,   293,    82,   317,
     318,    82,    32,   311,   312,    77,    78,     3,   316,   327,
      82,    50,    51,     3,     4,     5,     6,     7,     3,    61,
      83,    84,   340,   341,   342,   343,   344,   335,    83,    84,
      72,    73,    43,    83,    84,    77,    78,    68,     3,   347,
      82,     3,     4,     5,     6,     7,    36,    77,    78,   367,
       3,    83,    82,    36,   362,     3,     4,     5,     6,     7,
      64,     3,     4,     5,     6,     7,    77,    78,    79,    80,
     388,     3,     4,     5,     6,     7,    29,    83,    84,    42,
     398,    21,   400,   401,    83,    84,    64,    77,    78,    83,
      84,    21,    82,     3,     4,     5,     6,     7,    22,    61,
      21,    16,    17,    18,    19,    20,    83,    84,    22,    24,
      72,    27,    48,    49,    64,    77,    78,     3,    42,    83,
      82,    45,    83,    84,    77,    78,    79,    80,    42,    77,
      78,    45,   342,   343,    82,    77,    78,    84,    62,    85,
      82,    77,    78,    79,    80,    77,    78,    85,    62,    64,
      82,     3,    76,    77,    78,    79,    80,    83,    83,    83,
       3,     5,    76,    77,    78,    79,    80,    77,    78,    82,
      36,    83,    82,    77,    78,    79,    80,    83,    83,    83,
      84,    77,    78,    79,    80,    82,    44,    82,    84,    77,
      78,    79,    80,    76,    63,    83,    77,    78,    79,    80,
      64,    64,    83,    77,    78,    79,    80,    83,     3,    83,
      77,    78,    79,    80,     5,    83,     3,    34,    83,    38,
       5,    83,    82,    82,    63,    60,    82,    13,    83,     3,
      77,    39,    83,    23,    84,    46,    82,    82,    43,    63,
      52,    82,    82,    43,    43,    13,    82,     3,     6,    83,
      83,    83,     5,    63,    84,    82,    82,    82,    47,    29,
      82,    84,    82,    29,     5,    83,    27,    83,    83,    83,
      83,    30,    27,    80,    83,    59,    41,   318,   181,    46,
     122,   286,   217,   344,    90,   288,   343,   261,   297,   366,
     384
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    36,    88,    89,    90,     3,    91,     0,    25,   141,
       8,    29,    84,    26,     9,    10,    54,    92,    93,    82,
       3,    27,   142,    66,     3,     4,     5,     6,     7,    77,
      78,    79,    82,    94,    95,    97,    98,   100,    89,    29,
       3,   100,   112,     3,    34,    35,    82,   101,   102,   103,
     104,   111,    82,    85,    82,     3,    97,    97,    97,    84,
       3,    29,    77,    78,    79,    80,    96,    83,    82,    85,
       5,    28,    84,    11,    67,    68,    85,    86,   105,   106,
      82,    82,    89,   102,   111,    69,    84,   127,    16,    17,
      18,    19,    20,    24,    64,   113,    29,    40,   108,    97,
      99,   134,     3,    79,    10,    54,    79,    97,    83,    95,
       3,    97,    97,    97,    97,    89,     3,   100,    82,    82,
      12,    82,     3,     3,   106,    82,   104,     3,    36,   107,
      83,    45,    57,    74,    82,    97,   100,   114,   115,   116,
     117,   118,   119,   121,   122,   124,   126,   102,    59,    70,
     128,    64,    64,    16,    64,   113,    21,    21,    21,   103,
      64,    27,    42,     3,    84,    83,    84,    85,   100,    97,
      83,    83,    83,    85,     3,     3,    82,     5,    86,   105,
     106,    83,    83,    33,    37,    83,   114,    82,    82,    97,
     114,    22,    42,    45,    62,    76,    44,    43,    75,    76,
      36,    63,    52,    71,   129,   137,   103,   103,    64,   103,
      64,    14,    15,   103,    30,    41,    30,    41,    97,    97,
       3,    79,    83,    83,    83,    83,     5,    83,     3,   106,
     108,    82,    34,    38,    89,   100,    83,    98,    97,    22,
      42,    62,    82,    54,    55,    56,    82,    97,   125,    45,
      46,   114,   114,     5,   114,    58,   114,    53,    63,    63,
      60,   136,    14,    15,   103,   103,   114,    82,    14,    15,
      97,    97,    31,    97,   109,   109,    83,    13,     3,    77,
      39,    83,    84,    23,   120,    43,    98,    97,    82,    89,
      98,   123,    89,    82,    46,    70,   114,    63,    97,   138,
      61,    72,    73,    97,   130,   131,   132,   135,   114,   137,
     114,    82,    14,    15,   112,   114,    82,    43,    43,    13,
      82,    83,     3,     6,    98,    97,   120,    43,    89,   123,
      83,    83,    84,    83,    89,    63,   138,    48,    49,   139,
      84,    82,    82,    82,    84,   112,   114,    82,    83,   112,
      32,    97,   110,   110,    82,     5,    83,    97,    83,    83,
      98,    83,    58,   114,    47,   140,    97,    82,    97,   133,
     133,   132,   133,   131,    83,   112,    83,    29,    29,     5,
      83,   114,    50,    51,   139,    97,   134,    83,    84,    83,
      83,    83,    83,    27,    27,    83,   140,    83,    82,    97,
      30,    41,   134,    97,    97,    83
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)		\
   ((Current).first_line   = (Rhs)[1].first_line,	\
    (Current).first_column = (Rhs)[1].first_column,	\
    (Current).last_line    = (Rhs)[N].last_line,	\
    (Current).last_column  = (Rhs)[N].last_column)
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if defined (YYMAXDEPTH) && YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  /* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 89 "speed.y"
    {
			yyvsp[-1].u_qs->sp_updateclause=yyvsp[0].u_str;
			/* $1->sp_usqlstatement=getQueryStr($$); */
			QUERY_STRU=yyvsp[-1].u_qs;
		;}
    break;

  case 3:
#line 102 "speed.y"
    {
			SQLPATTERN *tmp;

			tmp=(SQLPATTERN *)FB_MALLOC(sizeof(SQLPATTERN));
			memset( tmp, 0, sizeof(SQLPATTERN) );
			tmp->sp_with=yyvsp[-7].u_withsub;
			tmp->sp_selectlist=yyvsp[-5].u_selfield;
			tmp->sp_fromlist=yyvsp[-3].u_tb;
			tmp->sp_whereclause=yyvsp[-2].u_etree;
			tmp->sp_connectclause=yyvsp[-1].u_connect;
			tmp->sp_orderbyclause=yyvsp[0].u_oc;
			//tmp->sp_updateclause=NULL;
			//tmp->sp_groupbyclause=NULL;
			//tmp->sp_havingclause=NULL;
			yyval.u_qs=tmp;
			//$$->sp_osqlstatement=NULL;
			//$$->sp_usqlstatement=NULL;
		;}
    break;

  case 4:
#line 127 "speed.y"
    {
			SQLPATTERN *tmp;

			tmp=(SQLPATTERN *)FB_MALLOC(sizeof(SQLPATTERN));
			memset( tmp, 0, sizeof(SQLPATTERN) );
			tmp->sp_with=yyvsp[-9].u_withsub;
			tmp->sp_selectlist=yyvsp[-7].u_selfield;
			tmp->sp_fromlist=yyvsp[-5].u_tb;
			tmp->sp_whereclause=yyvsp[-4].u_etree;
			tmp->sp_connectclause=yyvsp[-3].u_connect;
			tmp->sp_groupbyclause=yyvsp[-2].u_gr;
			tmp->sp_havingclause=yyvsp[-1].u_etree;
			tmp->sp_orderbyclause=yyvsp[0].u_oc;
			//tmp->sp_updateclause= NULL;
			yyval.u_qs=tmp;
			//$$->sp_osqlstatement=NULL;
			//$$->sp_usqlstatement=NULL;
		;}
    break;

  case 5:
#line 149 "speed.y"
    {	yyval.u_withsub=NULL;	  ;}
    break;

  case 6:
#line 151 "speed.y"
    {
	  		yyval.u_withsub=yyvsp[0].u_withsub;
	  	;}
    break;

  case 7:
#line 157 "speed.y"
    {
	  		WITHSTRU *wsub;
	  		WITHSUB *tmp;

	  		tmp=(WITHSUB *)FB_MALLOC(sizeof(WITHSUB));
	  		upStr(yyvsp[-4].u_str);/*convert to upcase,'a'->'A'*/
	  		strcpy(tmp->ws_id,yyvsp[-4].u_str);
	  		tmp->ws_subquery=yyvsp[-1].u_qs;

	  		wsub=(WITHSTRU *)FB_MALLOC(sizeof(WITHSTRU));
	  		wsub->w_count=1;
	  		wsub->w_subArray=tmp;

	  		FB_FREE(yyvsp[-4].u_str);
	  		yyval.u_withsub=wsub;
	  	;}
    break;

  case 8:
#line 174 "speed.y"
    {
	  		int i;
	  		WITHSUB *tmp;

	  		yyvsp[-6].u_withsub->w_count++;
	  		tmp=(WITHSUB *)FB_MALLOC(sizeof(WITHSUB)*yyvsp[-6].u_withsub->w_count);
	  		upStr(yyvsp[-4].u_str);
	  		for( i=0; (i<yyvsp[-6].u_withsub->w_count-1)&&( compWith( &(yyvsp[-6].u_withsub->w_subArray[i]), yyvsp[-4].u_str, yyvsp[-1].u_qs ) <= 0 ) ; i++)
	  		{
	  			memcpy(&(tmp[i]),&(yyvsp[-6].u_withsub->w_subArray[i]),sizeof(WITHSUB));
	  		}
	  		strcpy(tmp[i].ws_id,yyvsp[-4].u_str);
	  		tmp[i].ws_subquery=yyvsp[-1].u_qs;
	  		for(i++ ; i<yyvsp[-6].u_withsub->w_count ; i++ )
	  		{
	  			memcpy(&(tmp[i]),&(yyvsp[-6].u_withsub->w_subArray[i-1]),sizeof(WITHSUB));
	  		}
	  		FB_FREE(yyvsp[-6].u_withsub->w_subArray);
	  		yyvsp[-6].u_withsub->w_subArray=tmp;

	  		FB_FREE(yyvsp[-4].u_str);
	  		yyval.u_withsub=yyvsp[-6].u_withsub;
	  	;}
    break;

  case 9:
#line 201 "speed.y"
    {	SELECTLIST *tmp;
	  		tmp=(SELECTLIST *)FB_MALLOC(sizeof(SELECTLIST));
	  		tmp->sel_list=(STRBUF *)FB_MALLOC(sizeof(STRBUF));
	  		tmp->sel_count=1;
	  		strcpy(tmp->sel_list->sb_value,"*");
	  		tmp->sel_list->sb_alias[0]='\0';

			if(yyvsp[-1].u_str[0]=='\0')	tmp->sel_type=0;
			else if(strcmp(yyvsp[-1].u_str,"DISTINCT")==0)
			{
				tmp->sel_type=1;
			}
			else if( strcmp(yyvsp[-1].u_str,"UNIQUE")==0 )
			{
				tmp->sel_type=2;
			}
			else if( strcmp(yyvsp[-1].u_str,"ALL")==0 )
			{
				tmp->sel_type=3;
			}

			FB_FREE(yyvsp[-1].u_str);
	  		yyval.u_selfield=tmp;
	  	;}
    break;

  case 10:
#line 226 "speed.y"
    {
	  		if(yyvsp[-1].u_str[0]=='\0')		yyvsp[0].u_selfield->sel_type=0;
			else if(strcmp(yyvsp[-1].u_str,"DISTINCT")==0)
			{
				yyvsp[0].u_selfield->sel_type=1;
			}
			else if( strcmp(yyvsp[-1].u_str,"UNIQUE")==0 )
			{
				yyvsp[0].u_selfield->sel_type=2;
			}
			else if( strcmp(yyvsp[-1].u_str,"ALL")==0 )
			{
				yyvsp[0].u_selfield->sel_type=3;
			}

			FB_FREE(yyvsp[-1].u_str);
	  		yyval.u_selfield=yyvsp[0].u_selfield;
	  	;}
    break;

  case 11:
#line 247 "speed.y"
    {
	  		yyval.u_str=strdup(" ");	yyval.u_str[0]='\0';
	  	;}
    break;

  case 12:
#line 251 "speed.y"
    {
	  		yyval.u_str=strdup("ALL");
	 	;}
    break;

  case 13:
#line 255 "speed.y"
    {
	  		yyval.u_str=strdup("UNIQUE");
	  	;}
    break;

  case 14:
#line 259 "speed.y"
    {
		  	yyval.u_str=strdup("DISTINCT");
	  	;}
    break;

  case 15:
#line 265 "speed.y"
    {
	  	SELECTLIST *tmp;

	  	tmp=(SELECTLIST *)FB_MALLOC(sizeof(SELECTLIST));
		tmp->sel_list=yyvsp[0].u_slist;
	  	tmp->sel_count=1;
	  	yyval.u_selfield=tmp;
	  ;}
    break;

  case 16:
#line 274 "speed.y"
    {
	  	int i;
		STRBUF *tmp;

	  	yyvsp[-2].u_selfield->sel_count++;
	  	
		tmp=(STRBUF*)FB_MALLOC(sizeof(STRBUF)*(yyvsp[-2].u_selfield->sel_count));
		for(i=0; i<yyvsp[-2].u_selfield->sel_count-1; i++)
	  	{
	  		if( (strcmp(yyvsp[-2].u_selfield->sel_list[i].sb_value,yyvsp[0].u_slist->sb_value)>0) ||
	  			(strcmp(yyvsp[-2].u_selfield->sel_list[i].sb_value,yyvsp[0].u_slist->sb_value)==0) &&
	  			(strcmp(yyvsp[-2].u_selfield->sel_list[i].sb_alias,yyvsp[0].u_slist->sb_alias)>0 ) )
	  		break;
	  		memcpy(&(tmp[i]),&(yyvsp[-2].u_selfield->sel_list[i]),sizeof(STRBUF));
	  	}
	  	memcpy(&(tmp[i]),yyvsp[0].u_slist,sizeof(STRBUF));
	  	for(i++;i<yyvsp[-2].u_selfield->sel_count;i++)
	  	{
	  		memcpy(&(tmp[i]),&(yyvsp[-2].u_selfield->sel_list[i-1]),sizeof(STRBUF));
	  	}
		FB_FREE(yyvsp[-2].u_selfield->sel_list);
		/*
		tmp = (STRBUF *)FB_REALLOC( $1->sel_list, sizeof(STRBUF)*($1->sel_count) );
		memcpy( &tmp[$1->sel_count-1], $3, sizeof(STRBUF) );
		*/
		
		FB_FREE(yyvsp[0].u_slist);
		yyvsp[-2].u_selfield->sel_list=tmp;

		yyval.u_selfield=yyvsp[-2].u_selfield;
	  ;}
    break;

  case 17:
#line 308 "speed.y"
    {
	    	STRBUF *tmp;
	  	tmp=(STRBUF *)FB_MALLOC(sizeof(STRBUF));
	  	upStr(yyvsp[-2].u_str);
	  	/*tmp->sb_value=(char *)FB_MALLOC(sizeof(char)*(strlen($1)+3));*/
	  	sprintf(tmp->sb_value, "%s.*",yyvsp[-2].u_str);
	  	tmp->sb_alias[0]='\0';

	  	FB_FREE(yyvsp[-2].u_str);
	  	yyval.u_slist=tmp;
	  ;}
    break;

  case 18:
#line 320 "speed.y"
    {
	  	STRBUF *tmp;
	  	tmp=(STRBUF *)FB_MALLOC(sizeof(STRBUF));
	  	upStr(yyvsp[-4].u_str);  	upStr(yyvsp[-2].u_str);
	  	sprintf(tmp->sb_value, "%s.%s.*",yyvsp[-4].u_str,yyvsp[-2].u_str);
	  	tmp->sb_alias[0]='\0';

	  	FB_FREE(yyvsp[-4].u_str);
	  	FB_FREE(yyvsp[-2].u_str);
	  	yyval.u_slist=tmp;
	  ;}
    break;

  case 19:
#line 332 "speed.y"
    {
	  	STRBUF *tmp;
	  	tmp=(STRBUF *)FB_MALLOC(sizeof(STRBUF));
	  	//memset( tmp, 0, sizeof(STRBUF) );
	  	strcpy(tmp->sb_value,yyvsp[-1].u_str);
	  	strcpy(tmp->sb_alias,yyvsp[0].u_str);

	  	FB_FREE(yyvsp[-1].u_str);
	  	FB_FREE(yyvsp[0].u_str);

	  	yyval.u_slist=tmp;
	  ;}
    break;

  case 20:
#line 347 "speed.y"
    { 	yyval.u_str=strdup(" ");	yyval.u_str[0]='\0';	  ;}
    break;

  case 21:
#line 349 "speed.y"
    { upStr(yyvsp[0].u_str); ;}
    break;

  case 22:
#line 351 "speed.y"
    { upStr(yyvsp[0].u_str);	yyval.u_str=yyvsp[0].u_str;  ;}
    break;

  case 25:
#line 357 "speed.y"
    {
	  		yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-2].u_str)+strlen(yyvsp[0].u_str)+2));
	  		sprintf(yyval.u_str,"%s+%s",yyvsp[-2].u_str,yyvsp[0].u_str);
	  		FB_FREE(yyvsp[-2].u_str);
	  		FB_FREE(yyvsp[0].u_str);
	  	;}
    break;

  case 26:
#line 364 "speed.y"
    {
	  		yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-2].u_str)+strlen(yyvsp[0].u_str)+2));
	  		sprintf(yyval.u_str,"%s-%s",yyvsp[-2].u_str,yyvsp[0].u_str);
	  		FB_FREE(yyvsp[-2].u_str);
	  		FB_FREE(yyvsp[0].u_str);
	  	;}
    break;

  case 27:
#line 371 "speed.y"
    {
	  		yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-2].u_str)+strlen(yyvsp[0].u_str)+2));
	  		sprintf(yyval.u_str,"%s*%s",yyvsp[-2].u_str,yyvsp[0].u_str);
	  		FB_FREE(yyvsp[-2].u_str);
	  		FB_FREE(yyvsp[0].u_str);
	  	;}
    break;

  case 28:
#line 378 "speed.y"
    {
	  		yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-2].u_str)+strlen(yyvsp[0].u_str)+2));
	  		sprintf(yyval.u_str,"%s/%s",yyvsp[-2].u_str,yyvsp[0].u_str);
	  		FB_FREE(yyvsp[-2].u_str);
	  		FB_FREE(yyvsp[0].u_str);
	  	;}
    break;

  case 29:
#line 385 "speed.y"
    {
	  		yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[0].u_str)+2));
	  		sprintf(yyval.u_str,"+%s",yyvsp[0].u_str);
	  		FB_FREE(yyvsp[0].u_str);
	  	;}
    break;

  case 30:
#line 391 "speed.y"
    {
	  		yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[0].u_str)+2));
	  		sprintf(yyval.u_str,"-%s",yyvsp[0].u_str);
	  		FB_FREE(yyvsp[0].u_str);
	  	;}
    break;

  case 31:
#line 397 "speed.y"
    {
	  		//fprintf(stderr,"start FB_MALLOC for (*) \n");
	  		yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-3].u_str)+5));
	  		//memset( $$,0, strlen($1)+5 );
	  		//if( $$ ) fprintf(stderr,"succeed FB_MALLOC for (*) \n");
	  		upStr( yyvsp[-3].u_str );
	  		sprintf(yyval.u_str,"%s( * )",yyvsp[-3].u_str);
	  		//fprintf(stderr,"after sprintf for (*) \n");
	  		FB_FREE(yyvsp[-3].u_str);
	  	;}
    break;

  case 32:
#line 408 "speed.y"
    {
	  		yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-4].u_str)+strlen(yyvsp[-1].u_str)+15));
	  		upStr( yyvsp[-4].u_str );
	  		sprintf(yyval.u_str,"%s( DISTINCT %s )",yyvsp[-4].u_str,yyvsp[-1].u_str);
	  		FB_FREE(yyvsp[-1].u_str);
	  		FB_FREE(yyvsp[-4].u_str);
	  	;}
    break;

  case 33:
#line 416 "speed.y"
    {
	  		yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-4].u_str)+strlen(yyvsp[-1].u_str)+10));
	  		upStr( yyvsp[-4].u_str );
	  		sprintf(yyval.u_str,"%s( ALL %s )",yyvsp[-4].u_str,yyvsp[-1].u_str);
	  		FB_FREE(yyvsp[-1].u_str);
	  		FB_FREE(yyvsp[-4].u_str);
	  	;}
    break;

  case 34:
#line 424 "speed.y"
    {
	  		yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-3].u_str)+strlen(yyvsp[-1].u_str)+5));
	  		upStr( yyvsp[-3].u_str );
	  		sprintf(yyval.u_str,"%s( %s )",yyvsp[-3].u_str,yyvsp[-1].u_str);
	  		FB_FREE(yyvsp[-1].u_str);
	  		FB_FREE(yyvsp[-3].u_str);
	  	;}
    break;

  case 35:
#line 432 "speed.y"
    {
			yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-3].u_str)+strlen(yyvsp[-1].u_str)+5));
			sprintf(yyval.u_str,"%s( %s )",yyvsp[-3].u_str,yyvsp[-1].u_str);
	  		FB_FREE(yyvsp[-1].u_str);
	  		FB_FREE(yyvsp[-3].u_str);
	  		/*printf("function: %s\n", $$);*/
		;}
    break;

  case 36:
#line 440 "speed.y"
    {
	  		yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-1].u_str)+5));
	  		sprintf(yyval.u_str,"( %s )",yyvsp[-1].u_str);
	  		FB_FREE(yyvsp[-1].u_str);
	  	;}
    break;

  case 40:
#line 453 "speed.y"
    {  yyval.u_str=strdup(" ");	;}
    break;

  case 43:
#line 459 "speed.y"
    {
	  	upStr(yyvsp[0].u_str);
	  ;}
    break;

  case 44:
#line 463 "speed.y"
    {
	  	yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-2].u_str)+strlen(yyvsp[0].u_str)+2));
	  	upStr(yyvsp[-2].u_str); 	upStr(yyvsp[0].u_str);
	  	sprintf(yyval.u_str,"%s.%s",yyvsp[-2].u_str,yyvsp[0].u_str);
	  	FB_FREE(yyvsp[-2].u_str);
	  	FB_FREE(yyvsp[0].u_str);
	  ;}
    break;

  case 45:
#line 471 "speed.y"
    {
	  	upStr(yyvsp[-4].u_str); 	upStr(yyvsp[-2].u_str); 	upStr(yyvsp[0].u_str);
	  	yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-4].u_str)+strlen(yyvsp[-2].u_str)+strlen(yyvsp[0].u_str)+3));
	  	sprintf(yyval.u_str,"%s.%s.%s",yyvsp[-4].u_str,yyvsp[-2].u_str,yyvsp[0].u_str);

	  	FB_FREE(yyvsp[-4].u_str);
	  	FB_FREE(yyvsp[-2].u_str);
	  	FB_FREE(yyvsp[0].u_str);
	  ;}
    break;

  case 46:
#line 484 "speed.y"
    {
			TBFS *tmp;
			tmp=(TBFS*)FB_MALLOC(sizeof(TBFS));
			tmp->ts_count=1;
			tmp->ts_field=yyvsp[0].u_tfield;
			yyval.u_tb=tmp;
		;}
    break;

  case 47:
#line 492 "speed.y"
    {
	  		int i;
	  		TBfield *tmp;
	  		/*TBatom *toms;*/
	  		yyvsp[-2].u_tb->ts_count++;
	  		tmp=(TBfield *)FB_MALLOC(sizeof(TBfield)*yyvsp[-2].u_tb->ts_count);
	  		for(i=0; (i<yyvsp[-2].u_tb->ts_count-1)&&(compTBfield(&(yyvsp[-2].u_tb->ts_field[i]),yyvsp[0].u_tfield)<0); i++)
	  		{
	  			memcpy(&(tmp[i]),&(yyvsp[-2].u_tb->ts_field[i]),sizeof(TBfield));
	  		}
	  		memcpy(&(tmp[i]),yyvsp[0].u_tfield,sizeof(TBfield));
	 		i++;
	  		for( ;i<yyvsp[-2].u_tb->ts_count;i++)
	  		{
	  			memcpy(&(tmp[i]),&(yyvsp[-2].u_tb->ts_field[i-1]),sizeof(TBfield));
	  		}
	  		FB_FREE(yyvsp[0].u_tfield);
	  		FB_FREE(yyvsp[-2].u_tb->ts_field);
	  		yyvsp[-2].u_tb->ts_field=tmp;
	  		yyval.u_tb=yyvsp[-2].u_tb;
	  	;}
    break;

  case 48:
#line 516 "speed.y"
    {
	  		TBfield *tmp;
	  		tmp=(TBfield *)FB_MALLOC(sizeof(TBfield));
	  		tmp->tf_type=0;
	  		tmp->tf_atom=yyvsp[0].u_tatom;
	  		yyval.u_tfield=tmp;
	  	;}
    break;

  case 49:
#line 524 "speed.y"
    {
	  		TBfield *tmp;
	  		tmp=(TBfield *)FB_MALLOC(sizeof(TBfield));
	  		tmp->tf_type=1;
	  		tmp->tf_join=yyvsp[-1].u_join;
	  		yyval.u_tfield=tmp;
	  	;}
    break;

  case 50:
#line 532 "speed.y"
    {
	  		TBfield *tmp;
	  		tmp=(TBfield *)FB_MALLOC(sizeof(TBfield));
	  		tmp->tf_type=1;
	  		tmp->tf_join=yyvsp[0].u_join;
	  		yyval.u_tfield=tmp;
	  	;}
    break;

  case 51:
#line 542 "speed.y"
    {
	  		TBatom *tmp;
	  		tmp=(TBatom *)FB_MALLOC(sizeof(TBatom));
	  		tmp->tb_type=1;
	  		tmp->tb_head=yyvsp[-2].u_qtb;	/*struct QueryTB*/
	  		strcpy(tmp->tb_tail,yyvsp[0].u_str);

	  		FB_FREE(yyvsp[0].u_str);
	  		yyval.u_tatom=tmp;
	  	;}
    break;

  case 52:
#line 553 "speed.y"
    {
	 		TBatom *tmp;
	  		tmp=(TBatom *)FB_MALLOC(sizeof(TBatom));
	  		tmp->tb_type=1;
	  		tmp->tb_head=yyvsp[-3].u_qtb;	/*struct QueryTB*/
	  		upStr(yyvsp[0].u_str);
	  		sprintf(tmp->tb_tail,"%s %s",yyvsp[-1].u_str,yyvsp[0].u_str);

	  		FB_FREE(yyvsp[-1].u_str);
	  		FB_FREE(yyvsp[0].u_str);
	  		yyval.u_tatom=tmp;
	 	;}
    break;

  case 53:
#line 566 "speed.y"
    {
		  	TBatom *tmp;
	  		tmp=(TBatom *)FB_MALLOC(sizeof(TBatom));
	  		tmp->tb_type=0;
	  		tmp->tb_head=yyvsp[-1].u_qtb;	/*struct QueryTB*/
	  		strcpy(tmp->tb_tail,yyvsp[0].u_str);

	  		FB_FREE(yyvsp[0].u_str);
	  		yyval.u_tatom=tmp;
		;}
    break;

  case 54:
#line 577 "speed.y"
    {
		  	TBatom *tmp;

	  		tmp=(TBatom *)FB_MALLOC(sizeof(TBatom));
	  		tmp->tb_type=0;
	  		tmp->tb_head=yyvsp[-2].u_qtb;	/*struct QueryTB*/
	  		upStr(yyvsp[0].u_str);
	  		sprintf(tmp->tb_tail,"%s %s",yyvsp[-1].u_str,yyvsp[0].u_str);

	  		FB_FREE(yyvsp[-1].u_str);
	  		FB_FREE(yyvsp[0].u_str);
	  		yyval.u_tatom=tmp;
		;}
    break;

  case 55:
#line 593 "speed.y"
    {
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			upStr(yyvsp[0].u_str);
			strcpy(tmp->qt_value,yyvsp[0].u_str);
			tmp->qt_subquery=NULL;
			if( !PAT_TCOL )
			{
				PAT_TCOL=(TimeCol *)FB_MALLOC(sizeof(TimeCol));
				strcpy( PAT_TCOL->table_name, yyvsp[0].u_str );
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

			yyval.u_qtb=tmp;
			FB_FREE(yyvsp[0].u_str);
		;}
    break;

  case 56:
#line 618 "speed.y"
    {
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			strcpy(tmp->qt_value,yyvsp[-1].u_str);
			tmp->qt_subquery=yyvsp[-2].u_qs;

			FB_FREE(yyvsp[-1].u_str);
			yyval.u_qtb=tmp;
		;}
    break;

  case 57:
#line 628 "speed.y"
    {
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			tmp->qt_subquery=NULL;
			upStr(yyvsp[-1].u_str);
			sprintf(tmp->qt_value,"TABLE ( %s )",yyvsp[-1].u_str);

			yyval.u_qtb=tmp;
			FB_FREE(yyvsp[-1].u_str);
		;}
    break;

  case 58:
#line 639 "speed.y"
    {
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			tmp->qt_subquery=NULL;
			upStr(yyvsp[-4].u_str);
			sprintf(tmp->qt_value,"TABLE ( %s ) (+)",yyvsp[-4].u_str);

			yyval.u_qtb=tmp;
			FB_FREE(yyvsp[-4].u_str);
		;}
    break;

  case 59:
#line 650 "speed.y"
    {
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			tmp->qt_subquery=NULL;
			upStr(yyvsp[-4].u_str);	upStr(yyvsp[-2].u_str); 	upStr(yyvsp[0].u_str);
			sprintf(tmp->qt_value,"%s.%s@%s",yyvsp[-4].u_str,yyvsp[-2].u_str,yyvsp[0].u_str);

			yyval.u_qtb=tmp;
			FB_FREE(yyvsp[-4].u_str);
			FB_FREE(yyvsp[-2].u_str);
			FB_FREE(yyvsp[0].u_str);
		;}
    break;

  case 60:
#line 663 "speed.y"
    {
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			tmp->qt_subquery=NULL;
			upStr(yyvsp[-2].u_str); 	upStr(yyvsp[0].u_str);
			sprintf(tmp->qt_value,"%s@%s",yyvsp[-2].u_str,yyvsp[0].u_str);

			yyval.u_qtb=tmp;
			FB_FREE(yyvsp[-2].u_str);
			FB_FREE(yyvsp[0].u_str);
		;}
    break;

  case 61:
#line 675 "speed.y"
    {
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			tmp->qt_subquery=NULL;
			upStr(yyvsp[-1].u_str);
			sprintf(tmp->qt_value,"%s %s",yyvsp[-1].u_str,yyvsp[0].u_str);

			FB_FREE(yyvsp[0].u_str);
			yyval.u_qtb=tmp;
			FB_FREE(yyvsp[-1].u_str);
		;}
    break;

  case 62:
#line 687 "speed.y"
    {
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			tmp->qt_subquery=NULL;
			upStr(yyvsp[-3].u_str);	upStr(yyvsp[-1].u_str);
			sprintf(tmp->qt_value,"%s.%s %s",yyvsp[-3].u_str,yyvsp[-1].u_str,yyvsp[0].u_str);

			FB_FREE(yyvsp[0].u_str);
			yyval.u_qtb=tmp;
			FB_FREE(yyvsp[-3].u_str);
			FB_FREE(yyvsp[-1].u_str);
		;}
    break;

  case 63:
#line 700 "speed.y"
    {
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			tmp->qt_subquery=NULL;
			upStr(yyvsp[-2].u_str);
			sprintf(tmp->qt_value,"%s %s %s",yyvsp[-2].u_str,yyvsp[-1].u_str,yyvsp[0].u_str);

			FB_FREE(yyvsp[-1].u_str);
			FB_FREE(yyvsp[0].u_str);
			yyval.u_qtb=tmp;
			FB_FREE(yyvsp[-2].u_str);
		;}
    break;

  case 64:
#line 713 "speed.y"
    {
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			tmp->qt_subquery=NULL;
			upStr(yyvsp[-4].u_str);
			upStr(yyvsp[-2].u_str);
			sprintf(tmp->qt_value,"%s.%s %s %s",yyvsp[-4].u_str,yyvsp[-2].u_str,yyvsp[-1].u_str,yyvsp[0].u_str);

			FB_FREE(yyvsp[-1].u_str);
			FB_FREE(yyvsp[0].u_str);
			yyval.u_qtb=tmp;
			FB_FREE(yyvsp[-4].u_str);
			FB_FREE(yyvsp[-2].u_str);
		;}
    break;

  case 65:
#line 728 "speed.y"
    {
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			tmp->qt_subquery=NULL;
			upStr(yyvsp[-1].u_str);
			sprintf(tmp->qt_value,"%s %s",yyvsp[-1].u_str,yyvsp[0].u_str);

			FB_FREE(yyvsp[0].u_str);
			yyval.u_qtb=tmp;
			FB_FREE(yyvsp[-1].u_str);
		;}
    break;

  case 66:
#line 740 "speed.y"
    {
			QueryTB *tmp;
			tmp=(QueryTB *)FB_MALLOC(sizeof(QueryTB));
			tmp->qt_subquery=NULL;
			upStr(yyvsp[-3].u_str);
			upStr(yyvsp[-1].u_str);
			sprintf(tmp->qt_value,"%s.%s %s",yyvsp[-3].u_str,yyvsp[-1].u_str,yyvsp[0].u_str);

			FB_FREE(yyvsp[0].u_str);
			yyval.u_qtb=tmp;
			FB_FREE(yyvsp[-3].u_str);
			FB_FREE(yyvsp[-1].u_str);
		;}
    break;

  case 67:
#line 756 "speed.y"
    {
			char *tmp;
			tmp=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-1].u_str)+15));
			upStr(yyvsp[-1].u_str);
			sprintf(tmp,"PARTITION ( %s )",yyvsp[-1].u_str);
			yyval.u_str=tmp;
			FB_FREE(yyvsp[-1].u_str);
		;}
    break;

  case 68:
#line 765 "speed.y"
    {
			char *tmp;
			tmp=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-1].u_str)+18));
			upStr(yyvsp[-1].u_str);
			sprintf(tmp,"PARTITION ( %s )",yyvsp[-1].u_str);
			yyval.u_str=tmp;
			FB_FREE(yyvsp[-1].u_str);
		;}
    break;

  case 69:
#line 776 "speed.y"
    {
	  		yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-1].u_str)+12));
	  		sprintf(yyval.u_str,"SAMPLE ( %s )",yyvsp[-1].u_str);
	  		FB_FREE(yyvsp[-1].u_str);
	  	;}
    break;

  case 70:
#line 782 "speed.y"
    {
	  		yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-1].u_str)+18));
	  		sprintf(yyval.u_str,"SAMPLE BLOCK ( %s )",yyvsp[-1].u_str);
	  		FB_FREE(yyvsp[-1].u_str);
	  	;}
    break;

  case 71:
#line 788 "speed.y"
    {
	  		yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-5].u_str)+21+strlen(yyvsp[-1].u_str)));
	  		sprintf(yyval.u_str,"SAMPLE ( %s ) SEED ( %s )",yyvsp[-5].u_str,yyvsp[-1].u_str);

	  		FB_FREE(yyvsp[-5].u_str);
	  		FB_FREE(yyvsp[-1].u_str);
	  	;}
    break;

  case 72:
#line 796 "speed.y"
    {
	  		yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-5].u_str)+27+strlen(yyvsp[-1].u_str)));
	  		sprintf(yyval.u_str,"SAMPLE BLOCK ( %s ) SEED ( %s )",yyvsp[-5].u_str,yyvsp[-1].u_str);

	  		FB_FREE(yyvsp[-5].u_str);
	  		FB_FREE(yyvsp[-1].u_str);
		;}
    break;

  case 73:
#line 806 "speed.y"
    {	yyval.u_str=strdup(" ");	yyval.u_str[0]='\0';	;}
    break;

  case 74:
#line 808 "speed.y"
    {	yyval.u_str=strdup("WITH READ ONLY");	;}
    break;

  case 75:
#line 810 "speed.y"
    {	yyval.u_str=strdup("WITH CHECK OPTION");	;}
    break;

  case 76:
#line 812 "speed.y"
    {
	  		yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(30+strlen(yyvsp[0].u_str)));
	  		upStr(yyvsp[0].u_str);
	  		sprintf(yyval.u_str,"WITH CHECK OPTION CONSTRAINT %s", yyvsp[0].u_str);

	  		FB_FREE(yyvsp[0].u_str);
	  	;}
    break;

  case 77:
#line 822 "speed.y"
    {
			yyval.u_str=strdup(" ");
			yyval.u_str[0]='\0';
		;}
    break;

  case 78:
#line 827 "speed.y"
    {
	  		yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(10+strlen(yyvsp[0].u_str)));
	  		sprintf(yyval.u_str,"AS OF SCN %s", yyvsp[0].u_str);

	  		FB_FREE(yyvsp[0].u_str);
	  	;}
    break;

  case 79:
#line 834 "speed.y"
    {
	  		yyval.u_str=( char *) FB_MALLOC( sizeof(char)*(17+strlen(yyvsp[0].u_str)));
	  		sprintf(yyval.u_str,"AS OF TIMESTAMP %s", yyvsp[0].u_str);

	  		FB_FREE(yyvsp[0].u_str);
	  	;}
    break;

  case 80:
#line 841 "speed.y"
    {
		  	yyval.u_str=(char *) FB_MALLOC( sizeof(char)*(strlen(yyvsp[-6].u_str)+strlen(yyvsp[-4].u_str)+strlen(yyvsp[0].u_str)+35));
		  	sprintf(yyval.u_str,"VERSIONS BETWEEN SCN %s AND %s AS OF SCN %s", yyvsp[-6].u_str, yyvsp[-4].u_str, yyvsp[0].u_str );
		  	FB_FREE(yyvsp[-6].u_str);
		  	FB_FREE(yyvsp[-4].u_str);
		  	FB_FREE(yyvsp[0].u_str);
		;}
    break;

  case 81:
#line 849 "speed.y"
    {
		  	yyval.u_str=(char *) FB_MALLOC( sizeof(char)*(strlen(yyvsp[-6].u_str)+strlen(yyvsp[-4].u_str)+strlen(yyvsp[0].u_str)+48));
		  	sprintf(yyval.u_str,"VERSIONS BETWEEN TIMESTAMP %s AND %s AS OF TIMESTAMP %s", yyvsp[-6].u_str, yyvsp[-4].u_str, yyvsp[0].u_str );
		  	FB_FREE(yyvsp[-6].u_str);
		  	FB_FREE(yyvsp[-4].u_str);
		  	FB_FREE(yyvsp[0].u_str);
		;}
    break;

  case 83:
#line 860 "speed.y"
    {	yyval.u_str=strdup("MINVALUE");	;}
    break;

  case 85:
#line 865 "speed.y"
    {	yyval.u_str=strdup("MAXVALUE");	;}
    break;

  case 86:
#line 870 "speed.y"
    {
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=yyvsp[-2].u_tatom;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=yyvsp[-4].u_tfield;
			strcpy(tmp->jt_op,"JOIN");
			tmp->jt_right=tf;
			tmp->jt_using[0]='\0';
			tmp->jt_condition=yyvsp[0].u_combi;
			yyval.u_join=tmp;
		;}
    break;

  case 87:
#line 887 "speed.y"
    {
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=yyvsp[-4].u_tatom;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=yyvsp[-6].u_tfield;
			strcpy(tmp->jt_op,"JOIN");
			tmp->jt_right=tf;
			sprintf(tmp->jt_using,"USING ( %s )",yyvsp[-1].u_str);
			tmp->jt_condition=NULL;

			FB_FREE(yyvsp[-1].u_str);
			yyval.u_join=tmp;
		;}
    break;

  case 88:
#line 906 "speed.y"
    {
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=yyvsp[-2].u_tatom;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=yyvsp[-5].u_tfield;
			strcpy(tmp->jt_op,"INNER JOIN");
			tmp->jt_right=tf;
			tmp->jt_using[0]='\0';
			tmp->jt_condition=yyvsp[0].u_combi;

			yyval.u_join=tmp;
		;}
    break;

  case 89:
#line 924 "speed.y"
    {
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=yyvsp[-4].u_tatom;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=yyvsp[-7].u_tfield;
			strcpy(tmp->jt_op,"INNER JOIN");
			tmp->jt_right=tf;
			sprintf(tmp->jt_using,"USING ( %s )",yyvsp[-1].u_str);
			tmp->jt_condition=NULL;

			FB_FREE(yyvsp[-1].u_str);
			yyval.u_join=tmp;
		;}
    break;

  case 90:
#line 943 "speed.y"
    {
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=yyvsp[0].u_tatom;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=yyvsp[-3].u_tfield;
			strcpy(tmp->jt_op,"CROSS JOIN");
			tmp->jt_right=tf;
			tmp->jt_using[0]='\0';
			tmp->jt_condition=NULL;

			yyval.u_join=tmp;
		;}
    break;

  case 91:
#line 961 "speed.y"
    {
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=yyvsp[0].u_tatom;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=yyvsp[-3].u_tfield;
			strcpy(tmp->jt_op,"NATURAL JOIN");
			tmp->jt_right=tf;
			tmp->jt_using[0]='\0';
			tmp->jt_condition=NULL;

			yyval.u_join=tmp;
		;}
    break;

  case 92:
#line 979 "speed.y"
    {
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=yyvsp[0].u_tatom;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=yyvsp[-4].u_tfield;
			strcpy(tmp->jt_op,"NATURAL INNER JOIN");
			tmp->jt_right=tf;
			tmp->jt_using[0]='\0';
			tmp->jt_condition=NULL;

			yyval.u_join=tmp;
		;}
    break;

  case 93:
#line 997 "speed.y"
    {
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=yyvsp[0].u_tatom;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=yyvsp[-3].u_tfield;
			sprintf(tmp->jt_op,"%s JOIN",yyvsp[-2].u_str);
			tmp->jt_right=tf;
			tmp->jt_using[0]='\0';
			tmp->jt_condition=NULL;

			FB_FREE(yyvsp[-2].u_str);
			yyval.u_join=tmp;
		;}
    break;

  case 94:
#line 1016 "speed.y"
    {
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=yyvsp[-2].u_tatom;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=yyvsp[-5].u_tfield;
			sprintf(tmp->jt_op,"%s JOIN",yyvsp[-4].u_str);
			tmp->jt_right=tf;
			tmp->jt_using[0]='\0';
			tmp->jt_condition=yyvsp[0].u_combi;

			FB_FREE(yyvsp[-4].u_str);
			yyval.u_join=tmp;
		;}
    break;

  case 95:
#line 1035 "speed.y"
    {
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=yyvsp[-4].u_tatom;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=yyvsp[-7].u_tfield;
			sprintf(tmp->jt_op,"%s JOIN",yyvsp[-6].u_str);
			tmp->jt_right=tf;
			sprintf(tmp->jt_using,"USING ( %s )",yyvsp[-1].u_str);
			tmp->jt_condition=NULL;

			FB_FREE(yyvsp[-6].u_str);
			FB_FREE(yyvsp[-1].u_str);
			yyval.u_join=tmp;
		;}
    break;

  case 96:
#line 1055 "speed.y"
    {
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=yyvsp[0].u_tatom;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=yyvsp[-4].u_tfield;
			sprintf(tmp->jt_op,"NATURAL %s JOIN",yyvsp[-2].u_str);
			tmp->jt_right=tf;
			tmp->jt_using[0]='\0';
			tmp->jt_condition=NULL;

			FB_FREE(yyvsp[-2].u_str);
			yyval.u_join=tmp;
		;}
    break;

  case 97:
#line 1074 "speed.y"
    {
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=yyvsp[-2].u_tatom;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=yyvsp[-6].u_tfield;
			sprintf(tmp->jt_op,"NATURAL %s JOIN",yyvsp[-4].u_str);
			tmp->jt_right=tf;
			tmp->jt_using[0]='\0';
			tmp->jt_condition=yyvsp[0].u_combi;

			FB_FREE(yyvsp[-4].u_str);
			yyval.u_join=tmp;
		;}
    break;

  case 98:
#line 1093 "speed.y"
    {
			JoinTB *tmp;
			TBfield *tf;

			tf=(TBfield *)FB_MALLOC(sizeof(TBfield));
			tf->tf_type=0;
			tf->tf_atom=yyvsp[-4].u_tatom;

			tmp=(JoinTB *)FB_MALLOC(sizeof(JoinTB));
			tmp->jt_left=yyvsp[-8].u_tfield;
			sprintf(tmp->jt_op,"NATURAL %s JOIN",yyvsp[-6].u_str);
			tmp->jt_right=tf;
			sprintf(tmp->jt_using,"USING ( %s )",yyvsp[-1].u_str);
			tmp->jt_condition=NULL;

			FB_FREE(yyvsp[-6].u_str);
			FB_FREE(yyvsp[-1].u_str);
			yyval.u_join=tmp;
		;}
    break;

  case 100:
#line 1116 "speed.y"
    {/*here need sort*/
		char *tmp,*p;
		int i=0,l1,l3;
		l1 = strlen(yyvsp[-2].u_str);
		l3 = strlen(yyvsp[0].u_str);
		tmp = (char *)FB_MALLOC( sizeof(char)*( l1+l3+3 ) );
		while(1)
		{
			p=strchr(yyvsp[-2].u_str+i,',');
			if(!p)
			{
				sprintf(tmp,"%s, %s",yyvsp[-2].u_str,yyvsp[0].u_str);
				break;
			}
			if( myStrncmp( yyvsp[-2].u_str+i, yyvsp[0].u_str, p-yyvsp[-2].u_str-i, l3 )>0 )
			{
				memcpy( tmp, yyvsp[-2].u_str, i );
				memcpy( tmp + i, yyvsp[0].u_str, l3 );
				*( tmp + i + l3 ) = ',';
				*( tmp + i + l3 + 1 ) = ' ';
				memcpy( tmp + i + l3 + 2, p + 1, yyvsp[-2].u_str + l1 - p );
				tmp[l1+l3+2]='\0';
				break;
			}
			i=p-yyvsp[-2].u_str+1;
		}

		FB_FREE(yyvsp[-2].u_str);	FB_FREE(yyvsp[0].u_str);
		yyval.u_str=tmp;
	  ;}
    break;

  case 101:
#line 1161 "speed.y"
    { yyval.u_str=strdup("FULL"); ;}
    break;

  case 102:
#line 1163 "speed.y"
    { yyval.u_str=strdup("LEFT");  ;}
    break;

  case 103:
#line 1165 "speed.y"
    { yyval.u_str=strdup("RIGHT");  ;}
    break;

  case 104:
#line 1167 "speed.y"
    { yyval.u_str=strdup("FULL OUTER"); ;}
    break;

  case 105:
#line 1169 "speed.y"
    { yyval.u_str=strdup("LEFT OUTER");  ;}
    break;

  case 106:
#line 1171 "speed.y"
    { yyval.u_str=strdup("RIGHT OUTER"); ;}
    break;

  case 107:
#line 1176 "speed.y"
    {
	  	/*NONE(即：一个basic条件表达式)=0,'()'=1,NOT=2,AND=3,OR=4*/
			yyval.u_combi = ( COMBISTRU * )combi_condition( yyvsp[-2].u_combi, yyvsp[0].u_combi, 4 );
		;}
    break;

  case 108:
#line 1181 "speed.y"
    {
			yyval.u_combi = ( COMBISTRU * )combi_condition( yyvsp[-2].u_combi, yyvsp[0].u_combi, 3 );
		;}
    break;

  case 109:
#line 1185 "speed.y"
    {
			COMBISTRU *tmp;

			if( yyvsp[0].u_combi->cs_combi==0 )
			{
			/*NONE(即：一个basic条件表达式)=0,'()'=1,NOT=2,AND=3,OR=4*/
				yyvsp[0].u_combi->cs_combi=2;
				yyval.u_combi=yyvsp[0].u_combi;
			}
			else if( yyvsp[0].u_combi->cs_combi==2 )/*not(not)=old*/
			{
				if(yyvsp[0].u_combi->cs_lefttype==0)
				{
					yyvsp[0].u_combi->cs_combi=0; /*not(not)basic=basic*/
					yyval.u_combi=yyvsp[0].u_combi;
				}
				else /*left child is combined node*/
				{
					yyval.u_combi=yyvsp[0].u_combi->cs_leftcs;
					yyvsp[0].u_combi->cs_leftcs=NULL;
					FB_FREE(yyvsp[0].u_combi); /*free the left child*/
				}
			}
			else
			{
				tmp=(COMBISTRU *)FB_MALLOC(sizeof(COMBISTRU));
				tmp->cs_combi=2; /*show as 'NOT'*/
				tmp->cs_lefttype=1;/*say that is a combine condition node*/
				tmp->cs_leftcs=yyvsp[0].u_combi;
				tmp->cs_righttype=-1;
				yyval.u_combi=tmp;
			}
		;}
    break;

  case 110:
#line 1219 "speed.y"
    {
			COMBISTRU *tmp;
			 /*为另一层括号，或者本身符号在逻辑表达式中就比AND和OR优先级高*/
			if( yyvsp[-1].u_combi->cs_combi <2 )
			{/*n层括号等效于一层括号*/
				yyval.u_combi=yyvsp[-1].u_combi;
			}
			else
			{
				tmp=(COMBISTRU *)FB_MALLOC(sizeof(COMBISTRU));
				tmp->cs_combi=1; /*show as '()'*/

				tmp->cs_lefttype=1;/*say that is a combine condition node*/
				tmp->cs_leftcs=yyvsp[-1].u_combi;

				tmp->cs_righttype=-1; /*say there is not a child*/

				yyval.u_combi=tmp;
			}
		;}
    break;

  case 111:
#line 1240 "speed.y"
    {
			COMBISTRU *tmp;

			tmp=(COMBISTRU *)FB_MALLOC(sizeof(COMBISTRU));
			if( yyvsp[0].u_basic->bs_type == 4 )
			{
				BetweenTime *betw;
				BASICSTRU *l_basic, *r_basic;
				
				betw = ( BetweenTime *)(yyvsp[0].u_basic->bs_value);
				tmp->cs_combi = 3;
				//left basic child-node
				tmp->cs_lefttype = 0;
				tmp->cs_leftbs = (BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
				memset( tmp->cs_leftbs, 0, sizeof(BASICSTRU));
				tmp->cs_leftbs->bs_type = 3;
				strcpy( tmp->cs_leftbs->bs_colname, yyvsp[0].u_basic->bs_colname );
				strcpy( tmp->cs_leftbs->bs_oper,">=" );
				tmp->cs_leftbs->bs_value = (char *)FB_MALLOC(strlen(betw->low_t)+1);
				memset( tmp->cs_leftbs->bs_value, 0, strlen(betw->low_t)+1);
				strcpy( tmp->cs_leftbs->bs_value, betw->low_t );
				//right basic child-node
				tmp->cs_righttype = 0;
				tmp->cs_rightbs = yyvsp[0].u_basic;	//(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
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
				tmp->cs_leftbs=yyvsp[0].u_basic;
				tmp->cs_righttype=-1;/*that say havn't a right child */
			}
			yyval.u_combi=tmp;
		;}
    break;

  case 119:
#line 1295 "speed.y"
    {
			BASICSTRU *tmp;
			KeyNode *kt;
			char *bp, *ep;
			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			memset( tmp, 0, sizeof(BASICSTRU) );
			strcpy( tmp->bs_colname, yyvsp[-3].u_str );
			kt = (KeyNode *)FB_MALLOC( sizeof(KeyNode) );
			kt->words = (char*)FB_MALLOC( strlen(yyvsp[-1].u_str)+1 );
			memset( kt->words, 0, strlen(yyvsp[-1].u_str)+1 );
			bp = strchr( yyvsp[-1].u_str, '\'' );
			++bp;
			ep = strrchr( yyvsp[-1].u_str, '\'' );
			strncpy( kt->words, bp, ep-bp );
			tmp->bs_value = kt;
			FB_FREE(yyvsp[-1].u_str);
			FB_FREE( yyvsp[-3].u_str );
			yyval.u_basic=tmp;
		;}
    break;

  case 120:
#line 1318 "speed.y"
    {
			/*  COMPARISON    bs_type * flag( AND = 1, OR = -1 )
			 *  =		100
			 *  <>		101
			 *  <		102
			 *  >		103
			 *  <=		104
			 *  >=		105
			 */
			strcpy( yyvsp[-2].u_basic->bs_oper, yyvsp[-1].u_str );
			if( !strncmp( yyvsp[-1].u_str, "=", 1 ) )
				yyvsp[-2].u_basic->bs_type = 100;
			else if( !strcmp( yyvsp[-1].u_str, "<>" ) )
				yyvsp[-2].u_basic->bs_type = 101;
			else if( yyvsp[-1].u_str[0] == '<' && yyvsp[-1].u_str[1] != '='  )
				yyvsp[-2].u_basic->bs_type = 102;
			else if( yyvsp[-1].u_str[0]=='>' && yyvsp[-1].u_str[1] !='=' )
				yyvsp[-2].u_basic->bs_type = 103;
			else if( !strncmp( yyvsp[-1].u_str, "<=", 2 ) )
				yyvsp[-2].u_basic->bs_type = 104;
			else if( !strncmp( yyvsp[-1].u_str, ">=", 2 ) )
				yyvsp[-2].u_basic->bs_type = 105;
			else
				yyvsp[-2].u_basic->bs_type = 1;
			FB_FREE( yyvsp[-1].u_str );
			strcpy( ((KeyNode*)(yyvsp[-2].u_basic->bs_value))->value, yyvsp[0].u_str );
			//printf("$2: %s\tbs_type: %d\tintnum: %s\n", $1->bs_oper, $1->bs_type, $1->bs_value );
			yyval.u_basic = yyvsp[-2].u_basic;
		;}
    break;

  case 121:
#line 1348 "speed.y"
    {
			BASICSTRU *tmp;

			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,yyvsp[-2].u_str);
			strcpy(tmp->bs_oper,yyvsp[-1].u_str);
			tmp->bs_value=yyvsp[0].u_str;
			/*--------modified by szy at 2005/9/23--------*/
			/*tmp->bs_type =1 mean a string
			 *		3 mean a time value (this is a time condition)
			 */
			//printf(" col name: %s:%s\n", $1, PAT_TCOL->col_name );
			if( !strcasecmp( PAT_TCOL->col_name, yyvsp[-2].u_str ) && (yyvsp[-1].u_str[0]=='<' || yyvsp[-1].u_str[0]=='>') )
			{
				tmp->bs_type=3;

				if( yyvsp[-1].u_str[0]=='<' )
				{
					recognizeTime( yyvsp[0].u_str, QUERY_TIME->etime );
					strcpy( QUERY_TIME->ecompare, yyvsp[-1].u_str );
				}
				else /*if( !strcmp( $2, ">" ) || !strcmp( $2, ">=" ) )*/
				{
					recognizeTime( yyvsp[0].u_str, QUERY_TIME->stime );
					strcpy( QUERY_TIME->scompare, yyvsp[-1].u_str );
				}
			}
			else
				tmp->bs_type=1;			/*means a string*/

			FB_FREE(yyvsp[-2].u_str);
			FB_FREE(yyvsp[-1].u_str);
			yyval.u_basic=tmp;
		;}
    break;

  case 122:
#line 1383 "speed.y"
    {
			BASICSTRU *tmp;

			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,yyvsp[-4].u_str);
			strcpy(tmp->bs_oper,yyvsp[-3].u_str);
			tmp->bs_type=2;	/*means a subquery*/
			tmp->bs_value=yyvsp[-1].u_qs;

			FB_FREE(yyvsp[-4].u_str);
			FB_FREE(yyvsp[-3].u_str);
			yyval.u_basic=tmp;
		;}
    break;

  case 123:
#line 1400 "speed.y"
    {
			BASICSTRU *tmp;
			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,yyvsp[-5].u_str);
			strcpy(tmp->bs_oper,"NOT BETWEEN");
			tmp->bs_type=1;
			tmp->bs_value=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-2].u_str)+strlen(yyvsp[0].u_str)+6));
			sprintf(tmp->bs_value,"%s AND %s",yyvsp[-2].u_str,yyvsp[0].u_str);

			FB_FREE(yyvsp[-5].u_str);
			FB_FREE(yyvsp[-2].u_str);
			FB_FREE(yyvsp[0].u_str);
			yyval.u_basic=tmp;
		;}
    break;

  case 124:
#line 1415 "speed.y"
    {
			BASICSTRU *tmp;
			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,yyvsp[-4].u_str);
			strcpy(tmp->bs_oper,"BETWEEN");
			tmp->bs_type=1;
			//printf("Parse Between now\n");
			//convert between to >= | <=
			if( !strcasecmp( PAT_TCOL->col_name, yyvsp[-4].u_str ) )
			{
				//printf("time-col: %s\n", PAT_TCOL->col_name );
				if( !strncasecmp( yyvsp[-2].u_str, "to_date(", 8 ) && !strncasecmp( yyvsp[0].u_str, "to_date(", 8) )
				{
					recognizeTime( yyvsp[-2].u_str, QUERY_TIME->stime );
					strcpy( QUERY_TIME->scompare, ">=" );
					recognizeTime( yyvsp[0].u_str, QUERY_TIME->etime );
					strcpy( QUERY_TIME->ecompare, "<=" );
					tmp->bs_type = 4;
				}

			}

			if( tmp->bs_type == 1 )
			{
				tmp->bs_value=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-2].u_str)+strlen(yyvsp[0].u_str)+6));
				sprintf(tmp->bs_value,"%s AND %s",yyvsp[-2].u_str,yyvsp[0].u_str);
				/*printf("between cla: %s\n",tmp->bs_value);*/
			}
			else//bs_type == 4
			{
				BetweenTime *betw=(BetweenTime *)FB_MALLOC(sizeof(BetweenTime));
				memset( betw, 0, sizeof(BetweenTime) );
				strcpy( betw->low_t, yyvsp[-2].u_str );
				strcpy( betw->high_t, yyvsp[0].u_str );
				tmp->bs_value = betw;
			}

			FB_FREE(yyvsp[-4].u_str);
			FB_FREE(yyvsp[-2].u_str);
			FB_FREE(yyvsp[0].u_str);
			yyval.u_basic=tmp;
		;}
    break;

  case 125:
#line 1461 "speed.y"
    {
			BASICSTRU *tmp;
			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,yyvsp[-4].u_str);
			strcpy(tmp->bs_oper,"NOT LIKE");
			tmp->bs_type=1;
			tmp->bs_value=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-1].u_str)+strlen(yyvsp[0].u_str)+3));
			sprintf(tmp->bs_value," %s %s",yyvsp[-1].u_str,yyvsp[0].u_str);

			FB_FREE(yyvsp[-4].u_str);
			FB_FREE(yyvsp[-1].u_str);
			FB_FREE(yyvsp[0].u_str);
			yyval.u_basic=tmp;
		;}
    break;

  case 126:
#line 1476 "speed.y"
    {
			BASICSTRU *tmp;
			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,yyvsp[-3].u_str);
			strcpy(tmp->bs_oper,"LIKE");
			tmp->bs_type=1;
			tmp->bs_value=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-1].u_str)+strlen(yyvsp[0].u_str)+3));
			sprintf(tmp->bs_value," %s %s",yyvsp[-1].u_str,yyvsp[0].u_str);

			FB_FREE(yyvsp[-3].u_str);
			FB_FREE(yyvsp[-1].u_str);
			FB_FREE(yyvsp[0].u_str);
			yyval.u_basic=tmp;
		;}
    break;

  case 127:
#line 1494 "speed.y"
    {
			yyval.u_str=strdup(" ");
			yyval.u_str[0]='\0';
		;}
    break;

  case 128:
#line 1499 "speed.y"
    {
			yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[0].u_str)+8));
			sprintf(yyval.u_str,"ESCAPE %s", yyvsp[0].u_str);
			FB_FREE(yyvsp[0].u_str);
		;}
    break;

  case 129:
#line 1507 "speed.y"
    {
			BASICSTRU *tmp;
			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,yyvsp[-3].u_str);
			strcpy(tmp->bs_oper,"IS NOT NULL");
			tmp->bs_type=1;
			tmp->bs_value=(char *)FB_MALLOC(sizeof(char));
			*((char *)tmp->bs_value)='\0';

			FB_FREE(yyvsp[-3].u_str);
			yyval.u_basic=tmp;
		;}
    break;

  case 130:
#line 1520 "speed.y"
    {
			BASICSTRU *tmp;
			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,yyvsp[-2].u_str);
			strcpy(tmp->bs_oper,"IS NULL");
			tmp->bs_type=1;
			tmp->bs_value=(char *)FB_MALLOC(sizeof(char));
			*((char*)tmp->bs_value)='\0';

			FB_FREE(yyvsp[-2].u_str);
			yyval.u_basic=tmp;
		;}
    break;

  case 131:
#line 1536 "speed.y"
    {
			BASICSTRU *tmp;

			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,yyvsp[-5].u_str);
			strcpy(tmp->bs_oper,"NOT IN");
			tmp->bs_type=2;	/*means a subquery*/
			tmp->bs_value=yyvsp[-1].u_qs;

			FB_FREE(yyvsp[-5].u_str);
			yyval.u_basic=tmp;
		;}
    break;

  case 132:
#line 1549 "speed.y"
    {
			BASICSTRU *tmp;

			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,yyvsp[-4].u_str);
			strcpy(tmp->bs_oper,"IN");
			tmp->bs_type=2;	/*means a subquery*/
			tmp->bs_value=yyvsp[-1].u_qs;

			FB_FREE(yyvsp[-4].u_str);
			yyval.u_basic=tmp;
		;}
    break;

  case 133:
#line 1562 "speed.y"
    {
			int i,j;
			BASICSTRU *tmp;
			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,yyvsp[-5].u_str);
			strcpy(tmp->bs_oper,"NOt IN");
			tmp->bs_type=1;
			tmp->bs_value=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-1].u_str)+5));
			sprintf(tmp->bs_value,"( %s )",yyvsp[-1].u_str);

			FB_FREE(yyvsp[-5].u_str);
			FB_FREE(yyvsp[-1].u_str);
			yyval.u_basic=tmp;
		;}
    break;

  case 134:
#line 1577 "speed.y"
    {
			int i,j;
			BASICSTRU *tmp;
			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,yyvsp[-4].u_str);
			strcpy(tmp->bs_oper,"IN");
			tmp->bs_type=1;
			tmp->bs_value=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-1].u_str)+5));
			sprintf(tmp->bs_value,"( %s )",yyvsp[-1].u_str);

			FB_FREE(yyvsp[-4].u_str);
			FB_FREE(yyvsp[-1].u_str);
			yyval.u_basic=tmp;
		;}
    break;

  case 136:
#line 1595 "speed.y"
    {/*here need sort*/
			char *tmp,*p;
			int i=0;
			int l1,l3;
			l1 = strlen( yyvsp[-2].u_str );
			l3 = strlen( yyvsp[0].u_str );
			tmp = (char *)FB_MALLOC(sizeof(char)*( l1+l3+3 ));
			while(1)
			{
				p=strchr(yyvsp[-2].u_str+i,',');
				if(!p)
				{
					sprintf(tmp,"%s, %s", yyvsp[-2].u_str, yyvsp[0].u_str);
					break;
				}
				if(myStrncmp( yyvsp[-2].u_str+i, yyvsp[0].u_str, p-yyvsp[-2].u_str-i, l3)>0)
				{
					memcpy( tmp, yyvsp[-2].u_str, i);
					memcpy( tmp+i, yyvsp[0].u_str, l3);
					tmp[i+l3] = ',';
					tmp[i+l3+1] = ' ';
					memcpy( tmp+i+l3+2, p+1, yyvsp[-2].u_str+l1-p);
					tmp[l1+l3+2]='\0';
					break;
				}
				i=p-yyvsp[-2].u_str+1;
			}
			FB_FREE(yyvsp[-2].u_str);	FB_FREE(yyvsp[0].u_str);
			yyval.u_str=tmp;
	  	;}
    break;

  case 137:
#line 1628 "speed.y"
    {
			BASICSTRU *tmp;

			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			strcpy(tmp->bs_colname,yyvsp[-5].u_str);
			strcpy(tmp->bs_oper,yyvsp[-4].u_str);
			sprintf(tmp->bs_oper," %s",yyvsp[-3].u_str);
			tmp->bs_type=2;	/*means a subquery*/
			tmp->bs_value=yyvsp[-1].u_qs;

			FB_FREE(yyvsp[-5].u_str);
			FB_FREE(yyvsp[-4].u_str);
			FB_FREE(yyvsp[-3].u_str);
			yyval.u_basic=tmp;
		;}
    break;

  case 138:
#line 1646 "speed.y"
    {
			yyval.u_str=strdup("ANY");
		;}
    break;

  case 139:
#line 1650 "speed.y"
    {
			yyval.u_str=strdup("ALL");
		;}
    break;

  case 140:
#line 1654 "speed.y"
    {
			yyval.u_str=strdup("SOME");
		;}
    break;

  case 141:
#line 1660 "speed.y"
    {
			BASICSTRU *tmp;

			tmp=(BASICSTRU *)FB_MALLOC(sizeof(BASICSTRU));
			tmp->bs_colname[0]='\0';
			strcpy(tmp->bs_oper,"EXISTS");
			tmp->bs_type=2;	/*means a subquery*/
			tmp->bs_value=yyvsp[-1].u_qs;

			yyval.u_basic=tmp;
		;}
    break;

  case 142:
#line 1675 "speed.y"
    {	yyval.u_etree=NULL;	;}
    break;

  case 143:
#line 1677 "speed.y"
    {
			EXPRTREE *tmp;
			tmp=(EXPRTREE *)FB_MALLOC(sizeof(EXPRTREE));
			tmp->qt_count=countCondition(yyvsp[0].u_combi);
			/*
			 * 修枝排序树：主要是contains组合方面
			 */
			tmp->qt_cs=yyvsp[0].u_combi;
			//合并其中的同类contains条件
			unite_contains( tmp->qt_cs );
			if( unique_deal( tmp->qt_cs ) == -1 )
			{
				//printf("where clause error\n");
				YYERROR;
			}
			yyval.u_etree=tmp;
		;}
    break;

  case 144:
#line 1698 "speed.y"
    {
			yyval.u_connect=NULL;
		;}
    break;

  case 145:
#line 1702 "speed.y"
    {
			HIERARCH *tmp;
			tmp=(HIERARCH *)FB_MALLOC(sizeof(HIERARCH));
			tmp->hq_type=0;/*none*/
			tmp->hq_start=NULL;	/*not have*/
			tmp->hq_connect=yyvsp[0].u_combi;
			yyval.u_connect=tmp;
		;}
    break;

  case 146:
#line 1711 "speed.y"
    {
			HIERARCH *tmp;
			tmp=(HIERARCH *)FB_MALLOC(sizeof(HIERARCH));
			tmp->hq_type=1;/*nocycle*/
			tmp->hq_start=NULL;	/*not have*/
			tmp->hq_connect=yyvsp[0].u_combi;
			yyval.u_connect=tmp;
		;}
    break;

  case 147:
#line 1720 "speed.y"
    {
			HIERARCH *tmp;
			tmp=(HIERARCH *)FB_MALLOC(sizeof(HIERARCH));
			tmp->hq_type=0;/*none*/
			tmp->hq_start=yyvsp[-3].u_combi;/* have*/
			tmp->hq_connect=yyvsp[0].u_combi;
			yyval.u_connect=tmp;
		;}
    break;

  case 148:
#line 1729 "speed.y"
    {
			HIERARCH *tmp;
			tmp=(HIERARCH *)FB_MALLOC(sizeof(HIERARCH));
			tmp->hq_type=1;/*nocycle*/
			tmp->hq_start=yyvsp[-4].u_combi;/* have*/
			tmp->hq_connect=yyvsp[0].u_combi;
			yyval.u_connect=tmp;
		;}
    break;

  case 149:
#line 1741 "speed.y"
    {
			yyval.u_gr=yyvsp[0].u_gr;
			//$3->sf_type=-1; 	/**havn't used such a sign*/
		;}
    break;

  case 150:
#line 1748 "speed.y"
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
	  		strcpy( tmp->gcol, yyvsp[0].u_str );
	  		yyval.u_gr=tmp;
	  		FB_FREE(yyvsp[0].u_str);
	  	;}
    break;

  case 151:
#line 1763 "speed.y"
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
	  		strcpy( tmp->gcol, yyvsp[0].u_str );
 			FB_FREE(yyvsp[0].u_str);
			yyvsp[-2].u_gr->gnext = tmp;
			yyval.u_gr=yyvsp[-2].u_gr;
	  	;}
    break;

  case 155:
#line 1794 "speed.y"
    {
	  		yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-1].u_str)+11));
	  		sprintf(yyval.u_str,"ROLLUP( %s )",yyvsp[-1].u_str);
	  		FB_FREE(yyvsp[-1].u_str);
	  	;}
    break;

  case 156:
#line 1800 "speed.y"
    {
			yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-1].u_str)+9));
	  		sprintf(yyval.u_str,"CUBE( %s )",yyvsp[-1].u_str);
	  		FB_FREE(yyvsp[-1].u_str);
		;}
    break;

  case 158:
#line 1809 "speed.y"
    {
			yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-1].u_str)+5));
			sprintf(yyval.u_str,"( %s )",yyvsp[-1].u_str);
			FB_FREE(yyvsp[-1].u_str);
		;}
    break;

  case 159:
#line 1815 "speed.y"
    {
			char *tmp;
			/*
			tmp=(char *)realloc($1, sizeof(char)*(strlen($1)+strlen($3)+2));
			$$=tmp;
			strcat($$,",");
			strcat($$,$3);
			*/
			tmp=(char *)FB_MALLOC( sizeof(char)*(strlen(yyvsp[-2].u_str)+strlen(yyvsp[0].u_str)+3));
			sprintf(tmp,"%s, %s",yyvsp[-2].u_str, yyvsp[0].u_str);
			yyval.u_str=tmp;
			FB_FREE(yyvsp[-2].u_str);
			FB_FREE(yyvsp[0].u_str);
		;}
    break;

  case 160:
#line 1830 "speed.y"
    {
			char *tmp;
			/*
			tmp=(char *)realloc($1, sizeof(char)*(strlen($1)+strlen($4)+7));
			$$=tmp;
			strcat($$,", ( ");
			strcat($$,$4);
			strcat($$," )");
			*/
			tmp=(char *)FB_MALLOC( sizeof(char)*(strlen(yyvsp[-4].u_str)+strlen(yyvsp[-1].u_str)+7));
			sprintf(tmp, "%s, (%s)", yyvsp[-4].u_str, yyvsp[-1].u_str);
			yyval.u_str=tmp;
			FB_FREE(yyvsp[-4].u_str);
			FB_FREE(yyvsp[-1].u_str);
		;}
    break;

  case 161:
#line 1848 "speed.y"
    {
	  		char *tmp;
			/*
			tmp=(char *)realloc($1, sizeof(char)*(strlen($1)+strlen($3)+2));
			$$=tmp;
	  		strcat($$,",");
	  		strcat($$,$3);
	  		*/
	  		tmp=(char *)FB_MALLOC( sizeof(char)*(strlen(yyvsp[-2].u_str)+strlen(yyvsp[0].u_str)+3));
	  		sprintf( tmp, "%s, %s", yyvsp[-2].u_str, yyvsp[0].u_str);
	  		yyval.u_str=tmp;
	  		FB_FREE(yyvsp[-2].u_str);
	  		FB_FREE(yyvsp[0].u_str);
	  	;}
    break;

  case 162:
#line 1863 "speed.y"
    {
	  		char *tmp;
			/*
			tmp=(char *)realloc($1, sizeof(char)*(strlen($1)+strlen($3)+2));
			$$=tmp;
	  		strcat($$,",");
	  		strcat($$,$3);
	  		*/
	  		tmp=(char *)FB_MALLOC( sizeof(char)*(strlen(yyvsp[-2].u_str)+strlen(yyvsp[0].u_str)+3));
	  		sprintf( tmp, "%s, %s", yyvsp[-2].u_str, yyvsp[0].u_str);
	  		yyval.u_str=tmp;
	  		FB_FREE(yyvsp[-2].u_str);
	  		FB_FREE(yyvsp[0].u_str);
	  	;}
    break;

  case 163:
#line 1880 "speed.y"
    {
	  		yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-1].u_str)+17));
	  		sprintf(yyval.u_str,"GROUPING SETS( %s )",yyvsp[-1].u_str);

	  		FB_FREE(yyvsp[-1].u_str);
	  	;}
    break;

  case 164:
#line 1887 "speed.y"
    {
	  		yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-1].u_str)+17));
	  		sprintf(yyval.u_str,"GROUPING SETS( %s )",yyvsp[-1].u_str);

	  		FB_FREE(yyvsp[-1].u_str);
	  	;}
    break;

  case 165:
#line 1897 "speed.y"
    {	yyval.u_etree=NULL;	;}
    break;

  case 166:
#line 1899 "speed.y"
    {
			EXPRTREE *tmp;
			tmp=(EXPRTREE *)FB_MALLOC(sizeof(EXPRTREE));
			tmp->qt_count=countCondition(yyvsp[0].u_combi);
			tmp->qt_cs=yyvsp[0].u_combi;
			yyval.u_etree=tmp;
		;}
    break;

  case 167:
#line 1910 "speed.y"
    {	yyval.u_oc=NULL;	;}
    break;

  case 168:
#line 1912 "speed.y"
    {
			//$3->sf_type=0;
			//$$=$3;

			OrderCla *tmp;
			tmp = (OrderCla *) FB_MALLOC( sizeof( OrderCla ) );
			tmp->oflag = 0;
			tmp->ofield = yyvsp[0].u_of;

			yyval.u_oc=tmp;
		;}
    break;

  case 169:
#line 1924 "speed.y"
    {
			/*
			$4->sf_type=1;
			$$=$4;
			*/

			OrderCla *tmp;
			tmp = (OrderCla *) FB_MALLOC( sizeof( OrderCla ) );
			tmp->oflag = 1;
			tmp->ofield = yyvsp[0].u_of;

			yyval.u_oc=tmp;
		;}
    break;

  case 170:
#line 1940 "speed.y"
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
			strcpy( tmp->ocol, yyvsp[-2].u_str );

			if( yyvsp[-1].u_str[0] == '\0' )
				tmp->oasc = 0;
			else if( !strcmp( yyvsp[-1].u_str, "ASC" ) )
				tmp->oasc = 1;
			else
				tmp->oasc = 2;

			if( yyvsp[0].u_str[0] == '\0' )
				tmp->onull = 0;
			else if( !strcmp( yyvsp[0].u_str, "NULLS FIRST") )
				tmp->onull = 1;
			else
				tmp->onull = 2;

			tmp->onext = NULL;

			FB_FREE(yyvsp[-2].u_str);
			FB_FREE(yyvsp[-1].u_str);
			FB_FREE(yyvsp[0].u_str);
			yyval.u_of=tmp;
	  	;}
    break;

  case 171:
#line 1974 "speed.y"
    {
			/*
			int i,j;
			$1->sf_count++;
			sprintf($1->sf_value+strlen($1->sf_value),", %s %s %s",$3,$4,$5);

			FB_FREE($3);	FB_FREE($4);	FB_FREE($5);
			*/
						OrderField *tmp;
			tmp = (OrderField *)FB_MALLOC( sizeof( OrderField ) );
			strcpy( tmp->ocol, yyvsp[-2].u_str );

			if( yyvsp[-1].u_str[0] == '\0' )
				tmp->oasc = 0;
			else if( !strcmp( yyvsp[-1].u_str, "ASC" ) )
				tmp->oasc = 1;
			else
				tmp->oasc = 2;

			if( yyvsp[0].u_str[0] == '\0' )
				tmp->onull = 0;
			else if( !strcmp( yyvsp[0].u_str, "NULLS FIRST") )
				tmp->onull = 1;
			else
				tmp->onull = 2;

			tmp->onext = NULL;

			yyvsp[-4].u_of->onext = tmp;

			FB_FREE(yyvsp[-2].u_str);
			FB_FREE(yyvsp[-1].u_str);
			FB_FREE(yyvsp[0].u_str);

			yyval.u_of=yyvsp[-4].u_of;
	  	;}
    break;

  case 172:
#line 2013 "speed.y"
    {
	  		yyval.u_str=strdup(" ");
	  		yyval.u_str[0]='\0';
	  	;}
    break;

  case 173:
#line 2018 "speed.y"
    {
			yyval.u_str=strdup("ASC");
		;}
    break;

  case 174:
#line 2022 "speed.y"
    {
			yyval.u_str=strdup("DESC");
		;}
    break;

  case 175:
#line 2028 "speed.y"
    {
	  		yyval.u_str=strdup(" ");
	  		yyval.u_str[0]='\0';
	  	;}
    break;

  case 176:
#line 2033 "speed.y"
    {
			yyval.u_str=strdup("NULLS FIRST");
		;}
    break;

  case 177:
#line 2037 "speed.y"
    {
			yyval.u_str=strdup("NULLS LAST");
		;}
    break;

  case 178:
#line 2044 "speed.y"
    {	yyval.u_str=NULL;	;}
    break;

  case 179:
#line 2046 "speed.y"
    {
			yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(10+strlen(yyvsp[0].u_str)));
			sprintf(yyval.u_str," FOR UPDATE %s",yyvsp[0].u_str);
			FB_FREE(yyvsp[0].u_str);
		;}
    break;

  case 180:
#line 2054 "speed.y"
    {
	  		yyval.u_str=strdup("  ");
	  		yyval.u_str[1]='\0';
	  	;}
    break;

  case 181:
#line 2059 "speed.y"
    {
			yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[0].u_str)+4));
			sprintf(yyval.u_str,"OF %s",yyvsp[0].u_str);
			FB_FREE(yyvsp[0].u_str);
		;}
    break;

  case 182:
#line 2065 "speed.y"
    {
			yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-1].u_str)+11));
			sprintf(yyval.u_str,"OF %s NOWAIT",yyvsp[-1].u_str);
			FB_FREE(yyvsp[-1].u_str);
		;}
    break;

  case 183:
#line 2071 "speed.y"
    {
			yyval.u_str=(char *)FB_MALLOC(sizeof(char)*(strlen(yyvsp[-1].u_str)+11));
			sprintf(yyval.u_str,"OF %s %s",yyvsp[-1].u_str,yyvsp[0].u_str);
			FB_FREE(yyvsp[-1].u_str);
			FB_FREE(yyvsp[0].u_str);
		;}
    break;


    }

/* Line 1000 of yacc.c.  */
#line 3886 "speed.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  const char* yyprefix;
	  char *yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
	      }
	  yysize += (sizeof ("syntax error, unexpected ")
		     + yystrlen (yytname[yytype]));
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			yyp = yystpcpy (yyp, yyprefix);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yyprefix = " or ";
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (yychar == YYEOF)
	     for (;;)
	       {
		 YYPOPSTACK;
		 if (yyssp == yyss)
		   YYABORT;
		 YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
		 yydestruct (yystos[*yyssp], yyvsp);
	       }
        }
      else
	{
	  YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
	  yydestruct (yytoken, &yylval);
	  yychar = YYEMPTY;

	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes YYERROR and the label
     yyerrorlab therefore never appears in user code.  */
  if (0)
     goto yyerrorlab;
#endif

  yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 2079 "speed.y"

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

