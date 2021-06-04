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
/* Line 1275 of yacc.c.  */
#line 211 "speed.tab.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif





