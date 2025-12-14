/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_MAESTRO_TAB_H_INCLUDED
# define YY_YY_MAESTRO_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    TOK_TYPE_INT = 258,            /* TOK_TYPE_INT  */
    TOK_TYPE_FLOAT = 259,          /* TOK_TYPE_FLOAT  */
    TOK_TYPE_STRING = 260,         /* TOK_TYPE_STRING  */
    TOK_TYPE_BOOL = 261,           /* TOK_TYPE_BOOL  */
    TOK_TYPE_VOID = 262,           /* TOK_TYPE_VOID  */
    TOK_CLASS = 263,               /* TOK_CLASS  */
    TOK_MAIN = 264,                /* TOK_MAIN  */
    TOK_IF = 265,                  /* TOK_IF  */
    TOK_WHILE = 266,               /* TOK_WHILE  */
    TOK_PRINT = 267,               /* TOK_PRINT  */
    TOK_TRUE = 268,                /* TOK_TRUE  */
    TOK_FALSE = 269,               /* TOK_FALSE  */
    TOK_ID = 270,                  /* TOK_ID  */
    LIT_INT = 271,                 /* LIT_INT  */
    LIT_FLOAT = 272,               /* LIT_FLOAT  */
    LIT_STRING = 273,              /* LIT_STRING  */
    TOK_ASSIGN = 274,              /* TOK_ASSIGN  */
    TOK_EQ = 275,                  /* TOK_EQ  */
    TOK_NEQ = 276,                 /* TOK_NEQ  */
    TOK_LEQ = 277,                 /* TOK_LEQ  */
    TOK_GEQ = 278,                 /* TOK_GEQ  */
    TOK_AND = 279,                 /* TOK_AND  */
    TOK_OR = 280,                  /* TOK_OR  */
    TOK_INC = 281,                 /* TOK_INC  */
    TOK_DEC = 282,                 /* TOK_DEC  */
    TOK_PLUS_ASSIGN = 283,         /* TOK_PLUS_ASSIGN  */
    UMINUS = 284                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_MAESTRO_TAB_H_INCLUDED  */
