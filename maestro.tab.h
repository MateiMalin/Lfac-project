
#ifndef YY_YY_MAESTRO_TAB_H_INCLUDED
# define YY_YY_MAESTRO_TAB_H_INCLUDED
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
#line 273 "maestro.y"
 
    #include <vector> 
    #include <string> 

#line 54 "maestro.tab.h"

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
    TOK_ID = 263,                  /* TOK_ID  */
    LIT_INT = 264,                 /* LIT_INT  */
    LIT_FLOAT = 265,               /* LIT_FLOAT  */
    LIT_STRING = 266,              /* LIT_STRING  */
    TOK_TRUE = 267,                /* TOK_TRUE  */
    TOK_FALSE = 268,               /* TOK_FALSE  */
    TOK_CLASS = 269,               /* TOK_CLASS  */
    TOK_MAIN = 270,                /* TOK_MAIN  */
    TOK_IF = 271,                  /* TOK_IF  */
    TOK_WHILE = 272,               /* TOK_WHILE  */
    TOK_PRINT = 273,               /* TOK_PRINT  */
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
    TOK_MINUS_ASSIGN = 284,        /* TOK_MINUS_ASSIGN  */
    TOK_MUL_ASSIGN = 285,          /* TOK_MUL_ASSIGN  */
    TOK_DIV_ASSIGN = 286,          /* TOK_DIV_ASSIGN  */
    UMINUS = 287                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 278 "maestro.y"
 
    char* stringValue; 
    class ASTNode* astNode;
    std::vector<ASTNode*>* astList;

#line 109 "maestro.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_MAESTRO_TAB_H_INCLUDED  */
