/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 3 "maestro.y"

#include <iostream> 
#include <vector> 
#include <string> 
#include <map> 
#include <fstream> 
#include <cstdio> 
#include <cstdlib> 
#include <cstring> 

extern int yylineno;
int yylex(void);
void yyerror(const char *s);

class SymbolTable; 

class Symbol { 
    public: 
        std::string name, type, kind; 
        std::vector <std::string> paramTypes; 
        SymbolTable* nestedScope = nullptr; 

        Symbol(std::string _name, std::string _type, std::string _kind) { 
            name = _name; 
            type = _type; 
            kind = _kind; 
        }
}; 

class SymbolTable { 
    public: 
        std::string scope; 
        SymbolTable* parent; 
        std::map<std::string, Symbol> symbols; 
        Symbol* lastAddedFunction = nullptr; 

        SymbolTable(std::string _scope, SymbolTable* _parent = nullptr) { 
            scope = _scope; 
            parent = _parent; 
        }

        bool addSymbol(std::string _name, std::string _type, std::string _kind) { 
            if(symbols.find(_name) != symbols.end()) return false; 

            Symbol currentSymbol = Symbol(_name, _type, _kind); 
            symbols.insert({ _name, currentSymbol});

            if(_kind == "method" || _kind == "function") lastAddedFunction = &symbols.at(_name);

            return true; 
        }

        void addParamToLastFunction (std::string paramType) { 
            if(lastAddedFunction != nullptr) lastAddedFunction->paramTypes.push_back(paramType); 
            else if (parent != nullptr) parent->addParamToLastFunction(paramType); 
        }

        Symbol* findSymbol(std::string name) { 
            if(symbols.find(name) != symbols.end()) return &symbols.at(name); 
            if(parent != nullptr) return parent->findSymbol(name); 

            return nullptr; 
        }

        void addParamToLastSymbol(std::string paramType) { 

        }

        void printTable(std::ofstream& fout) { 
            fout << "Symbol Table: " << scope << '\n'; 
            if(parent) fout << "Parent Scope: " << parent->scope << '\n'; 
            else fout << "Parent Scope: NULL" << '\n'; 

            fout << "____________________________________________" << '\n';
            fout << "1)Name\t\t\t2)Type\t\t\t3)Kind" << '\n'; 
            fout << "____________________________________________" << '\n';
            for(auto const &[key, val] : symbols) { 
                fout << val.name << "\t\t\t" << val.type << "\t\t\t" << val.kind << '\n';
            }

            fout << '\n'; 
        }
}; 

SymbolTable* globalScope = nullptr; 
SymbolTable* currentScope = nullptr;

std::string currentType, currentName, currentClassName; 

std::ofstream tableFile("tables.txt"); 

#line 163 "maestro.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "maestro.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TOK_TYPE_INT = 3,               /* TOK_TYPE_INT  */
  YYSYMBOL_TOK_TYPE_FLOAT = 4,             /* TOK_TYPE_FLOAT  */
  YYSYMBOL_TOK_TYPE_STRING = 5,            /* TOK_TYPE_STRING  */
  YYSYMBOL_TOK_TYPE_BOOL = 6,              /* TOK_TYPE_BOOL  */
  YYSYMBOL_TOK_TYPE_VOID = 7,              /* TOK_TYPE_VOID  */
  YYSYMBOL_TOK_ID = 8,                     /* TOK_ID  */
  YYSYMBOL_TOK_CLASS = 9,                  /* TOK_CLASS  */
  YYSYMBOL_TOK_MAIN = 10,                  /* TOK_MAIN  */
  YYSYMBOL_TOK_IF = 11,                    /* TOK_IF  */
  YYSYMBOL_TOK_WHILE = 12,                 /* TOK_WHILE  */
  YYSYMBOL_TOK_PRINT = 13,                 /* TOK_PRINT  */
  YYSYMBOL_TOK_TRUE = 14,                  /* TOK_TRUE  */
  YYSYMBOL_TOK_FALSE = 15,                 /* TOK_FALSE  */
  YYSYMBOL_LIT_INT = 16,                   /* LIT_INT  */
  YYSYMBOL_LIT_FLOAT = 17,                 /* LIT_FLOAT  */
  YYSYMBOL_LIT_STRING = 18,                /* LIT_STRING  */
  YYSYMBOL_TOK_ASSIGN = 19,                /* TOK_ASSIGN  */
  YYSYMBOL_TOK_EQ = 20,                    /* TOK_EQ  */
  YYSYMBOL_TOK_NEQ = 21,                   /* TOK_NEQ  */
  YYSYMBOL_TOK_LEQ = 22,                   /* TOK_LEQ  */
  YYSYMBOL_TOK_GEQ = 23,                   /* TOK_GEQ  */
  YYSYMBOL_TOK_AND = 24,                   /* TOK_AND  */
  YYSYMBOL_TOK_OR = 25,                    /* TOK_OR  */
  YYSYMBOL_TOK_INC = 26,                   /* TOK_INC  */
  YYSYMBOL_TOK_DEC = 27,                   /* TOK_DEC  */
  YYSYMBOL_TOK_PLUS_ASSIGN = 28,           /* TOK_PLUS_ASSIGN  */
  YYSYMBOL_29_ = 29,                       /* '<'  */
  YYSYMBOL_30_ = 30,                       /* '>'  */
  YYSYMBOL_31_ = 31,                       /* '+'  */
  YYSYMBOL_32_ = 32,                       /* '-'  */
  YYSYMBOL_33_ = 33,                       /* '*'  */
  YYSYMBOL_34_ = 34,                       /* '/'  */
  YYSYMBOL_UMINUS = 35,                    /* UMINUS  */
  YYSYMBOL_36_ = 36,                       /* '('  */
  YYSYMBOL_37_ = 37,                       /* ')'  */
  YYSYMBOL_38_ = 38,                       /* '{'  */
  YYSYMBOL_39_ = 39,                       /* '}'  */
  YYSYMBOL_40_ = 40,                       /* ';'  */
  YYSYMBOL_41_ = 41,                       /* ','  */
  YYSYMBOL_42_ = 42,                       /* '.'  */
  YYSYMBOL_YYACCEPT = 43,                  /* $accept  */
  YYSYMBOL_program = 44,                   /* program  */
  YYSYMBOL_45_1 = 45,                      /* $@1  */
  YYSYMBOL_global_elements = 46,           /* global_elements  */
  YYSYMBOL_global_decl = 47,               /* global_decl  */
  YYSYMBOL_48_2 = 48,                      /* $@2  */
  YYSYMBOL_global_decl_suffix = 49,        /* global_decl_suffix  */
  YYSYMBOL_50_3 = 50,                      /* $@3  */
  YYSYMBOL_data_type = 51,                 /* data_type  */
  YYSYMBOL_simple_type = 52,               /* simple_type  */
  YYSYMBOL_class_decl = 53,                /* class_decl  */
  YYSYMBOL_54_4 = 54,                      /* $@4  */
  YYSYMBOL_class_body = 55,                /* class_body  */
  YYSYMBOL_class_member = 56,              /* class_member  */
  YYSYMBOL_57_5 = 57,                      /* $@5  */
  YYSYMBOL_return_type = 58,               /* return_type  */
  YYSYMBOL_class_member_suffix = 59,       /* class_member_suffix  */
  YYSYMBOL_60_6 = 60,                      /* $@6  */
  YYSYMBOL_param_list = 61,                /* param_list  */
  YYSYMBOL_non_empty_params = 62,          /* non_empty_params  */
  YYSYMBOL_func_body = 63,                 /* func_body  */
  YYSYMBOL_var_decl_list = 64,             /* var_decl_list  */
  YYSYMBOL_var_decl = 65,                  /* var_decl  */
  YYSYMBOL_var_decl_suffix = 66,           /* var_decl_suffix  */
  YYSYMBOL_finale_block = 67,              /* finale_block  */
  YYSYMBOL_68_7 = 68,                      /* $@7  */
  YYSYMBOL_stmt_list_pure = 69,            /* stmt_list_pure  */
  YYSYMBOL_statement = 70,                 /* statement  */
  YYSYMBOL_assignment_stmt = 71,           /* assignment_stmt  */
  YYSYMBOL_unary_stmt = 72,                /* unary_stmt  */
  YYSYMBOL_print_stmt = 73,                /* print_stmt  */
  YYSYMBOL_control_stmt = 74,              /* control_stmt  */
  YYSYMBOL_if_stmt = 75,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 76,                /* while_stmt  */
  YYSYMBOL_block_pure = 77,                /* block_pure  */
  YYSYMBOL_expression = 78,                /* expression  */
  YYSYMBOL_lvalue = 79,                    /* lvalue  */
  YYSYMBOL_func_call = 80,                 /* func_call  */
  YYSYMBOL_args_list = 81,                 /* args_list  */
  YYSYMBOL_non_empty_args = 82,            /* non_empty_args  */
  YYSYMBOL_literal = 83                    /* literal  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   331

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  43
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  91
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  161

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   284


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      36,    37,    33,    31,    41,    32,    42,    34,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    40,
      29,     2,    30,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    38,     2,    39,     2,     2,     2,     2,
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
      25,    26,    27,    28,    35
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   133,   133,   133,   145,   145,   149,   149,   149,   157,
     157,   174,   185,   186,   187,   188,   189,   193,   194,   195,
     196,   200,   200,   218,   218,   222,   222,   230,   231,   235,
     238,   238,   255,   255,   259,   263,   270,   274,   274,   278,
     287,   287,   287,   293,   293,   306,   306,   310,   311,   312,
     313,   314,   318,   335,   353,   354,   358,   362,   362,   366,
     370,   374,   380,   388,   396,   404,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   422,   423,   432,   438,
     441,   470,   494,   546,   547,   551,   555,   562,   563,   564,
     565,   566
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TOK_TYPE_INT",
  "TOK_TYPE_FLOAT", "TOK_TYPE_STRING", "TOK_TYPE_BOOL", "TOK_TYPE_VOID",
  "TOK_ID", "TOK_CLASS", "TOK_MAIN", "TOK_IF", "TOK_WHILE", "TOK_PRINT",
  "TOK_TRUE", "TOK_FALSE", "LIT_INT", "LIT_FLOAT", "LIT_STRING",
  "TOK_ASSIGN", "TOK_EQ", "TOK_NEQ", "TOK_LEQ", "TOK_GEQ", "TOK_AND",
  "TOK_OR", "TOK_INC", "TOK_DEC", "TOK_PLUS_ASSIGN", "'<'", "'>'", "'+'",
  "'-'", "'*'", "'/'", "UMINUS", "'('", "')'", "'{'", "'}'", "';'", "','",
  "'.'", "$accept", "program", "$@1", "global_elements", "global_decl",
  "$@2", "global_decl_suffix", "$@3", "data_type", "simple_type",
  "class_decl", "$@4", "class_body", "class_member", "$@5", "return_type",
  "class_member_suffix", "$@6", "param_list", "non_empty_params",
  "func_body", "var_decl_list", "var_decl", "var_decl_suffix",
  "finale_block", "$@7", "stmt_list_pure", "statement", "assignment_stmt",
  "unary_stmt", "print_stmt", "control_stmt", "if_stmt", "while_stmt",
  "block_pure", "expression", "lvalue", "func_call", "args_list",
  "non_empty_args", "literal", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-119)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -119,     5,  -119,  -119,    76,  -119,  -119,  -119,  -119,  -119,
    -119,    15,  -119,  -119,  -119,  -119,    20,  -119,  -119,   -31,
    -119,    -7,  -119,    -4,  -119,     8,    59,    59,  -119,  -119,
    -119,  -119,     6,   -34,    -2,    12,    16,  -119,  -119,  -119,
       1,  -119,  -119,  -119,  -119,    -1,    28,  -119,  -119,  -119,
    -119,  -119,    59,    59,    81,  -119,  -119,  -119,   109,   319,
    -119,  -119,    62,    59,    63,    59,    59,    59,  -119,    59,
    -119,  -119,    59,  -119,  -119,   172,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,  -119,  -119,
      64,    51,    46,  -119,   244,    55,    52,    58,   190,   208,
     226,   130,   151,  -119,   287,   287,    91,    91,   274,   259,
      91,    91,    17,    17,  -119,  -119,  -119,    61,   319,    -3,
    -119,    59,    59,    69,    69,    56,  -119,  -119,  -119,    89,
    -119,  -119,  -119,   244,    71,  -119,  -119,  -119,  -119,    70,
     141,  -119,   319,  -119,    27,  -119,  -119,  -119,  -119,  -119,
      92,  -119,   318,    79,  -119,    50,    80,  -119,  -119,    78,
    -119
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     5,     1,     0,    12,    13,    14,    15,    28,
      16,     0,    43,     4,    27,     6,     0,     3,    21,     0,
       7,     0,    46,     0,    24,     0,     0,     0,     9,    40,
       8,    11,     0,    79,     0,     0,     0,    44,    45,    47,
       0,    49,    48,    57,    58,     0,     0,    90,    91,    87,
      88,    89,     0,     0,     0,    77,    76,    78,     0,    33,
      22,    23,     0,    84,     0,     0,     0,     0,    51,     0,
      54,    55,     0,    50,    75,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    41,    42,
       0,     0,    32,    25,    86,     0,    83,    80,     0,     0,
       0,     0,     0,    74,    70,    71,    68,    69,    72,    73,
      66,    67,    62,    63,    64,    65,    35,     0,     0,     0,
      81,     0,    84,     0,     0,     0,    52,    53,    38,     0,
      30,    29,    26,    85,     0,    46,    59,    60,    56,     0,
      46,    34,    33,    82,     0,    10,    17,    18,    19,    20,
       0,    37,    36,     0,    61,     0,     0,    39,    38,     0,
      31
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,   -53,  -119,
    -119,  -119,  -119,  -119,  -119,    94,  -119,  -119,    -6,  -119,
     -21,  -119,  -119,   -20,  -119,  -119,  -118,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,    24,   -23,   -25,   -24,    34,  -119,
    -119
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,     4,    13,    23,    30,    59,    14,   150,
      15,    21,    32,    61,   119,    16,   132,   142,    91,    92,
     139,   140,   151,    31,    17,    19,    25,    38,    39,    40,
      41,    42,    43,    44,   136,    94,    55,    56,    95,    96,
      57
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      45,    46,    63,    54,    58,     3,    90,    22,    64,     5,
       6,     7,     8,     9,    10,    26,    33,   144,    69,    34,
      35,    36,   152,    18,    27,    70,    71,    72,    20,    74,
      75,    24,    28,   130,    65,    33,    29,   131,    34,    35,
      36,    68,    98,    99,   100,    60,   101,    37,    66,   102,
      86,    87,    67,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   129,   154,    33,    73,    26,
      93,    97,   116,    47,    48,    49,    50,    51,    27,     5,
       6,     7,     8,     9,    10,    11,    12,   118,   117,    90,
      29,    52,   120,   121,   122,    53,   138,   141,   133,   128,
     155,    76,    77,    78,    79,    80,    81,   135,   143,   145,
      82,    83,    84,    85,    86,    87,   156,   160,   158,    45,
      46,    88,    84,    85,    86,    87,    62,    45,    46,    76,
      77,    78,    79,    80,    81,   157,   153,   159,    82,    83,
      84,    85,    86,    87,   146,   147,   148,   149,   137,    89,
      76,    77,    78,    79,    80,    81,   134,     0,     0,    82,
      83,    84,    85,    86,    87,     0,     0,     0,     0,     0,
     126,    76,    77,    78,    79,    80,    81,     0,     0,     0,
      82,    83,    84,    85,    86,    87,     0,     0,     0,     0,
       0,   127,    76,    77,    78,    79,    80,    81,     0,     0,
       0,    82,    83,    84,    85,    86,    87,     0,     0,   103,
      76,    77,    78,    79,    80,    81,     0,     0,     0,    82,
      83,    84,    85,    86,    87,     0,     0,   123,    76,    77,
      78,    79,    80,    81,     0,     0,     0,    82,    83,    84,
      85,    86,    87,     0,     0,   124,    76,    77,    78,    79,
      80,    81,     0,     0,     0,    82,    83,    84,    85,    86,
      87,     0,     0,   125,    76,    77,    78,    79,    80,    81,
       0,     0,     0,    82,    83,    84,    85,    86,    87,    76,
      77,    78,    79,    80,     0,     0,     0,     0,    82,    83,
      84,    85,    86,    87,    76,    77,    78,    79,     0,     0,
       0,     0,     0,    82,    83,    84,    85,    86,    87,    78,
      79,     0,     0,     0,     0,     0,    82,    83,    84,    85,
      86,    87,     5,     6,     7,     8,    33,    10,     0,    34,
      35,    36
};

static const yytype_int16 yycheck[] =
{
      25,    25,    36,    26,    27,     0,    59,    38,    42,     3,
       4,     5,     6,     7,     8,    19,     8,   135,    19,    11,
      12,    13,   140,     8,    28,    26,    27,    28,     8,    52,
      53,    38,    36,    36,    36,     8,    40,    40,    11,    12,
      13,    40,    65,    66,    67,    39,    69,    39,    36,    72,
      33,    34,    36,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,   118,    39,     8,    40,    19,
       8,     8,     8,    14,    15,    16,    17,    18,    28,     3,
       4,     5,     6,     7,     8,     9,    10,    41,    37,   142,
      40,    32,    37,    41,    36,    36,    40,     8,   121,    38,
       8,    20,    21,    22,    23,    24,    25,    38,    37,    39,
      29,    30,    31,    32,    33,    34,    37,    39,    38,   144,
     144,    40,    31,    32,    33,    34,    32,   152,   152,    20,
      21,    22,    23,    24,    25,   155,   142,   158,    29,    30,
      31,    32,    33,    34,     3,     4,     5,     6,   124,    40,
      20,    21,    22,    23,    24,    25,   122,    -1,    -1,    29,
      30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    20,    21,    22,    23,    24,    25,    -1,    -1,    -1,
      29,    30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    20,    21,    22,    23,    24,    25,    -1,    -1,
      -1,    29,    30,    31,    32,    33,    34,    -1,    -1,    37,
      20,    21,    22,    23,    24,    25,    -1,    -1,    -1,    29,
      30,    31,    32,    33,    34,    -1,    -1,    37,    20,    21,
      22,    23,    24,    25,    -1,    -1,    -1,    29,    30,    31,
      32,    33,    34,    -1,    -1,    37,    20,    21,    22,    23,
      24,    25,    -1,    -1,    -1,    29,    30,    31,    32,    33,
      34,    -1,    -1,    37,    20,    21,    22,    23,    24,    25,
      -1,    -1,    -1,    29,    30,    31,    32,    33,    34,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,
      31,    32,    33,    34,    20,    21,    22,    23,    -1,    -1,
      -1,    -1,    -1,    29,    30,    31,    32,    33,    34,    22,
      23,    -1,    -1,    -1,    -1,    -1,    29,    30,    31,    32,
      33,    34,     3,     4,     5,     6,     8,     8,    -1,    11,
      12,    13
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    44,    45,     0,    46,     3,     4,     5,     6,     7,
       8,     9,    10,    47,    51,    53,    58,    67,     8,    68,
       8,    54,    38,    48,    38,    69,    19,    28,    36,    40,
      49,    66,    55,     8,    11,    12,    13,    39,    70,    71,
      72,    73,    74,    75,    76,    79,    80,    14,    15,    16,
      17,    18,    32,    36,    78,    79,    80,    83,    78,    50,
      39,    56,    58,    36,    42,    36,    36,    36,    40,    19,
      26,    27,    28,    40,    78,    78,    20,    21,    22,    23,
      24,    25,    29,    30,    31,    32,    33,    34,    40,    40,
      51,    61,    62,     8,    78,    81,    82,     8,    78,    78,
      78,    78,    78,    37,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,     8,    37,    41,    57,
      37,    41,    36,    37,    37,    37,    40,    40,    38,    51,
      36,    40,    59,    78,    81,    38,    77,    77,    40,    63,
      64,     8,    60,    37,    69,    39,     3,     4,     5,     6,
      52,    65,    69,    61,    39,     8,    37,    66,    38,    63,
      39
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    43,    45,    44,    46,    46,    47,    48,    47,    50,
      49,    49,    51,    51,    51,    51,    51,    52,    52,    52,
      52,    54,    53,    55,    55,    57,    56,    58,    58,    59,
      60,    59,    61,    61,    62,    62,    63,    64,    64,    65,
      66,    66,    66,    68,    67,    69,    69,    70,    70,    70,
      70,    70,    71,    71,    72,    72,    73,    74,    74,    75,
      76,    77,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    79,
      79,    80,    80,    81,    81,    82,    82,    83,    83,    83,
      83,    83
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     3,     2,     0,     1,     0,     4,     0,
       7,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     6,     2,     0,     0,     4,     1,     1,     1,
       0,     7,     1,     0,     4,     2,     2,     2,     0,     3,
       1,     3,     3,     0,     5,     2,     0,     1,     1,     1,
       2,     2,     4,     4,     2,     2,     5,     1,     1,     5,
       5,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     1,     1,     1,     1,
       3,     4,     6,     1,     0,     3,     1,     1,     1,     1,
       1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* $@1: %empty  */
#line 133 "maestro.y"
    { 
        globalScope = new SymbolTable("Global Scope"); 
        currentScope = globalScope; 
    }
#line 1658 "maestro.tab.c"
    break;

  case 3: /* program: $@1 global_elements finale_block  */
#line 138 "maestro.y"
    { 
        currentScope->printTable(tableFile); 
        std::cout << "Sintassi Corretta!\n"; 
    }
#line 1667 "maestro.tab.c"
    break;

  case 7: /* $@2: %empty  */
#line 149 "maestro.y"
                                   { 
        currentType = (yyvsp[-1].stringValue); 
        currentName = (yyvsp[0].stringValue); 
    }
#line 1676 "maestro.tab.c"
    break;

  case 9: /* $@3: %empty  */
#line 157 "maestro.y"
        { 
        // Incepe o functie

        // Adaugam numele functiei in Global Scope
        if(!currentScope->addSymbol(currentName, currentType, "function")) { 
            yyerror(("Symbol" + currentName + "already defined.").c_str()); 
        }

        // Cream scope ul functiei
        SymbolTable* functionScope = new SymbolTable("Function: " + currentName, currentScope); 
        currentScope = functionScope; 
    }
#line 1693 "maestro.tab.c"
    break;

  case 10: /* global_decl_suffix: '(' $@3 param_list ')' '{' func_body '}'  */
#line 169 "maestro.y"
                                     { 
        // Finalul functiei - printam tabela si revenim la parinte 
        currentScope->printTable(tableFile); 
        currentScope = currentScope->parent; 
    }
#line 1703 "maestro.tab.c"
    break;

  case 11: /* global_decl_suffix: var_decl_suffix  */
#line 174 "maestro.y"
                    { 
        // Daca ajungem aici inseamna da declaratia este o variabila globala (o adaugam in scope)
        if(!currentScope->addSymbol(currentName, currentType, "global_var")) { 
            yyerror(("Symbol" + currentName + "already defined.").c_str()); 
        }
    }
#line 1714 "maestro.tab.c"
    break;

  case 12: /* data_type: TOK_TYPE_INT  */
#line 185 "maestro.y"
                 { (yyval.stringValue) = (yyvsp[0].stringValue); }
#line 1720 "maestro.tab.c"
    break;

  case 13: /* data_type: TOK_TYPE_FLOAT  */
#line 186 "maestro.y"
                   { (yyval.stringValue) = (yyvsp[0].stringValue); }
#line 1726 "maestro.tab.c"
    break;

  case 14: /* data_type: TOK_TYPE_STRING  */
#line 187 "maestro.y"
                    { (yyval.stringValue) = (yyvsp[0].stringValue); }
#line 1732 "maestro.tab.c"
    break;

  case 15: /* data_type: TOK_TYPE_BOOL  */
#line 188 "maestro.y"
                  { (yyval.stringValue) = (yyvsp[0].stringValue); }
#line 1738 "maestro.tab.c"
    break;

  case 16: /* data_type: TOK_ID  */
#line 189 "maestro.y"
           { (yyval.stringValue) = (yyvsp[0].stringValue); }
#line 1744 "maestro.tab.c"
    break;

  case 17: /* simple_type: TOK_TYPE_INT  */
#line 193 "maestro.y"
                 { (yyval.stringValue) = (yyvsp[0].stringValue); }
#line 1750 "maestro.tab.c"
    break;

  case 18: /* simple_type: TOK_TYPE_FLOAT  */
#line 194 "maestro.y"
                   { (yyval.stringValue) = (yyvsp[0].stringValue); }
#line 1756 "maestro.tab.c"
    break;

  case 19: /* simple_type: TOK_TYPE_STRING  */
#line 195 "maestro.y"
                    { (yyval.stringValue) = (yyvsp[0].stringValue); }
#line 1762 "maestro.tab.c"
    break;

  case 20: /* simple_type: TOK_TYPE_BOOL  */
#line 196 "maestro.y"
                  { (yyval.stringValue) = (yyvsp[0].stringValue); }
#line 1768 "maestro.tab.c"
    break;

  case 21: /* $@4: %empty  */
#line 200 "maestro.y"
                     { 
        std::string className = (yyvsp[0].stringValue); 
        currentScope->addSymbol(className, "class", "class_def"); 

        // Cream un nou scope pentru clasa
        SymbolTable* classScope = new SymbolTable("Class: " + className, currentScope); 
        Symbol* classSymbol = currentScope->findSymbol(className); 
        if(classSymbol != nullptr) classSymbol->nestedScope = classScope; 

        currentScope = classScope; 
    }
#line 1784 "maestro.tab.c"
    break;

  case 22: /* class_decl: TOK_CLASS TOK_ID $@4 '{' class_body '}'  */
#line 211 "maestro.y"
                       { 
        currentScope->printTable(tableFile); 
        currentScope = currentScope->parent; 
    }
#line 1793 "maestro.tab.c"
    break;

  case 25: /* $@5: %empty  */
#line 222 "maestro.y"
                       {
        currentType = (yyvsp[-1].stringValue); 
        currentName = (yyvsp[0].stringValue); 
    }
#line 1802 "maestro.tab.c"
    break;

  case 27: /* return_type: data_type  */
#line 230 "maestro.y"
              { (yyval.stringValue) = (yyvsp[0].stringValue); }
#line 1808 "maestro.tab.c"
    break;

  case 28: /* return_type: TOK_TYPE_VOID  */
#line 231 "maestro.y"
                  { (yyval.stringValue) = (yyvsp[0].stringValue); }
#line 1814 "maestro.tab.c"
    break;

  case 29: /* class_member_suffix: ';'  */
#line 235 "maestro.y"
        { 
        currentScope->addSymbol(currentName, currentType, "field"); 
    }
#line 1822 "maestro.tab.c"
    break;

  case 30: /* $@6: %empty  */
#line 238 "maestro.y"
        { 
        // Metoda a clasei
        currentScope->addSymbol(currentName, currentType, "method"); 

        // Cream scope ul metodei
        SymbolTable* methodScope = new SymbolTable("Method: " + currentName, currentScope); 
        currentScope = methodScope; 
    }
#line 1835 "maestro.tab.c"
    break;

  case 31: /* class_member_suffix: '(' $@6 param_list ')' '{' func_body '}'  */
#line 246 "maestro.y"
                                     { 
        currentScope->printTable(tableFile); 
        currentScope = currentScope->parent; 
    }
#line 1844 "maestro.tab.c"
    break;

  case 34: /* non_empty_params: non_empty_params ',' data_type TOK_ID  */
#line 259 "maestro.y"
                                          { 
        currentScope->addSymbol((yyvsp[0].stringValue), (yyvsp[-1].stringValue), "parameter"); 
        currentScope->addParamToLastFunction((yyvsp[-1].stringValue)); 
    }
#line 1853 "maestro.tab.c"
    break;

  case 35: /* non_empty_params: data_type TOK_ID  */
#line 263 "maestro.y"
                     { 
        currentScope->addSymbol((yyvsp[0].stringValue), (yyvsp[-1].stringValue), "parameter"); 
        currentScope->addParamToLastFunction((yyvsp[-1].stringValue)); 
    }
#line 1862 "maestro.tab.c"
    break;

  case 39: /* var_decl: simple_type TOK_ID var_decl_suffix  */
#line 278 "maestro.y"
                                       { 
        if(!currentScope->addSymbol((yyvsp[-1].stringValue), (yyvsp[-2].stringValue), "local_var")) { 
            yyerror(("Variable" + std::string((yyvsp[-1].stringValue)) + "redeclared!").c_str()); 
        }
    }
#line 1872 "maestro.tab.c"
    break;

  case 43: /* $@7: %empty  */
#line 293 "maestro.y"
             { 
        SymbolTable* mainScope = new SymbolTable("Main Function", currentScope);
        currentScope = mainScope; 
    }
#line 1881 "maestro.tab.c"
    break;

  case 44: /* finale_block: TOK_MAIN $@7 '{' stmt_list_pure '}'  */
#line 297 "maestro.y"
                           { 
        currentScope->printTable(tableFile); 
        currentScope = currentScope->parent; 
    }
#line 1890 "maestro.tab.c"
    break;

  case 52: /* assignment_stmt: lvalue TOK_ASSIGN expression ';'  */
#line 318 "maestro.y"
                                     { 
        // $1 = numele variabilei 
        // $3 = tipul expresiei

        // Verificam daca a fost declarata variabila 
        Symbol* symbol = currentScope->findSymbol((yyvsp[-3].stringValue));
        if(symbol == nullptr) { 
            yyerror(("[EROARE SEMANTICA] Variabila " + std::string((yyvsp[-3].stringValue)) + " nu este definita!").c_str());
            exit(1); 
        }

        // Verificam daca tipurile coincid 
        if(strcmp(symbol->type.c_str(), (yyvsp[-1].stringValue)) != 0) { 
            yyerror(("[EROARE SEMANTICA] Nu poti atribui un " + std::string((yyvsp[-1].stringValue)) + " la o variabila de tip" + symbol->type).c_str());
            exit(1); 
        }
    }
#line 1912 "maestro.tab.c"
    break;

  case 53: /* assignment_stmt: lvalue TOK_PLUS_ASSIGN expression ';'  */
#line 335 "maestro.y"
                                          { 
        // $1 = numele variabilei 
        // $3 = tipul expresiei

        // Verificam daca a fost declarata variabila 
        Symbol* symbol = currentScope->findSymbol((yyvsp[-3].stringValue));
        if(symbol == nullptr) { 
            yyerror(("[EROARE SEMANTICA] Variabila " + std::string((yyvsp[-3].stringValue)) + " nu este definita!").c_str());
            exit(1); 
        }

        // Verificam daca tipurile coincid 
        if(strcmp(symbol->type.c_str(), (yyvsp[-1].stringValue)) != 0) { 
            yyerror(("[EROARE SEMANTICA] Nu poti atribui un " + std::string((yyvsp[-1].stringValue)) + " la o variabila de tip" + symbol->type).c_str());
            exit(1); 
        }
    }
#line 1934 "maestro.tab.c"
    break;

  case 62: /* expression: expression '+' expression  */
#line 380 "maestro.y"
                              { 
        if(strcmp((yyvsp[-2].stringValue), (yyvsp[0].stringValue)) != 0) { 
            yyerror(("[EROARE SEMANTICA] Nu poti aduna " + std::string((yyvsp[-2].stringValue)) + " cu " + std::string((yyvsp[0].stringValue))).c_str());
            exit(1); 
        }

        (yyval.stringValue) = (yyvsp[-2].stringValue); 
    }
#line 1947 "maestro.tab.c"
    break;

  case 63: /* expression: expression '-' expression  */
#line 388 "maestro.y"
                                { 
        if(strcmp((yyvsp[-2].stringValue), (yyvsp[0].stringValue)) != 0) { 
            yyerror(("[EROARE SEMANTICA] Nu poti face scadere intre un " + std::string((yyvsp[-2].stringValue)) + " si un " + std::string((yyvsp[0].stringValue))).c_str());
            exit(1); 
        }

        (yyval.stringValue) = (yyvsp[-2].stringValue); 
    }
#line 1960 "maestro.tab.c"
    break;

  case 64: /* expression: expression '*' expression  */
#line 396 "maestro.y"
                                { 
        if(strcmp((yyvsp[-2].stringValue), (yyvsp[0].stringValue)) != 0) { 
            yyerror(("[EROARE SEMANTICA] Nu poti inmulti " + std::string((yyvsp[-2].stringValue)) + " cu " + std::string((yyvsp[0].stringValue))).c_str());
            exit(1); 
        }

        (yyval.stringValue) = (yyvsp[-2].stringValue); 
    }
#line 1973 "maestro.tab.c"
    break;

  case 65: /* expression: expression '/' expression  */
#line 404 "maestro.y"
                                { 
        if(strcmp((yyvsp[-2].stringValue), (yyvsp[0].stringValue)) != 0) { 
            yyerror(("[EROARE SEMANTICA] Nu poti imparti " + std::string((yyvsp[-2].stringValue)) + " cu " + std::string((yyvsp[0].stringValue))).c_str());
            exit(1); 
        }

        (yyval.stringValue) = (yyvsp[-2].stringValue); 
    }
#line 1986 "maestro.tab.c"
    break;

  case 66: /* expression: expression '<' expression  */
#line 412 "maestro.y"
                                { (yyval.stringValue) = strdup("verita"); }
#line 1992 "maestro.tab.c"
    break;

  case 67: /* expression: expression '>' expression  */
#line 413 "maestro.y"
                                { (yyval.stringValue) = strdup("verita"); }
#line 1998 "maestro.tab.c"
    break;

  case 68: /* expression: expression TOK_LEQ expression  */
#line 414 "maestro.y"
                                    { (yyval.stringValue) = strdup("verita"); }
#line 2004 "maestro.tab.c"
    break;

  case 69: /* expression: expression TOK_GEQ expression  */
#line 415 "maestro.y"
                                    { (yyval.stringValue) = strdup("verita"); }
#line 2010 "maestro.tab.c"
    break;

  case 70: /* expression: expression TOK_EQ expression  */
#line 416 "maestro.y"
                                   { (yyval.stringValue) = strdup("verita"); }
#line 2016 "maestro.tab.c"
    break;

  case 71: /* expression: expression TOK_NEQ expression  */
#line 417 "maestro.y"
                                    { (yyval.stringValue) = strdup("verita"); }
#line 2022 "maestro.tab.c"
    break;

  case 72: /* expression: expression TOK_AND expression  */
#line 418 "maestro.y"
                                    { (yyval.stringValue) = strdup("verita"); }
#line 2028 "maestro.tab.c"
    break;

  case 73: /* expression: expression TOK_OR expression  */
#line 419 "maestro.y"
                                   { (yyval.stringValue) = strdup("verita"); }
#line 2034 "maestro.tab.c"
    break;

  case 74: /* expression: '(' expression ')'  */
#line 420 "maestro.y"
                         { (yyval.stringValue) = (yyvsp[-1].stringValue); }
#line 2040 "maestro.tab.c"
    break;

  case 75: /* expression: '-' expression  */
#line 421 "maestro.y"
                                  { (yyval.stringValue) = (yyvsp[0].stringValue); }
#line 2046 "maestro.tab.c"
    break;

  case 76: /* expression: func_call  */
#line 422 "maestro.y"
                { (yyval.stringValue) = strdup("unknown"); }
#line 2052 "maestro.tab.c"
    break;

  case 77: /* expression: lvalue  */
#line 423 "maestro.y"
             { 
        Symbol* symbol = currentScope->findSymbol((yyvsp[0].stringValue));
        if(symbol == nullptr) { 
            yyerror(("[EROARE SEMANTICA] Variabila: " + std::string((yyvsp[0].stringValue)) + " nu este definita!").c_str());
            exit(1); 
        }

        (yyval.stringValue) = strdup(symbol->type.c_str()); 
    }
#line 2066 "maestro.tab.c"
    break;

  case 78: /* expression: literal  */
#line 432 "maestro.y"
            { 
        (yyval.stringValue) = (yyvsp[0].stringValue); 
    }
#line 2074 "maestro.tab.c"
    break;

  case 79: /* lvalue: TOK_ID  */
#line 438 "maestro.y"
           { 
        (yyval.stringValue) = (yyvsp[0].stringValue); 
    }
#line 2082 "maestro.tab.c"
    break;

  case 80: /* lvalue: TOK_ID '.' TOK_ID  */
#line 441 "maestro.y"
                      { 
        // Cautam obiectul 
        Symbol* object = currentScope->findSymbol((yyvsp[-2].stringValue)); 
        if(object == nullptr) { 
            yyerror(("[EROARE SEMANTICA] Obiectul " + std::string((yyvsp[-2].stringValue)) + " nu este definit!").c_str());
            exit(1); 
        } 

        std::string className = object->type; 
        Symbol* classSymbol = currentScope->findSymbol(className);

        // verificam daca exista clasa 
        if(classSymbol == nullptr || classSymbol->kind != "class_def") { 
            yyerror(("[EROARE SEMANTICA] Variabila " + std::string((yyvsp[-2].stringValue)) + " este de tip " + className + ", care nu este o clasa!").c_str());
            exit(1); 
        }

        SymbolTable* classScope = classSymbol->nestedScope; 
        if(classScope->symbols.find((yyvsp[0].stringValue)) == classScope->symbols.end()) { 
            yyerror(("[EROARE SEMANTICA] Clasa " + className +  " nu are membru " + std::string((yyvsp[0].stringValue))).c_str());
            exit(1); 
        }

        Symbol &member = classScope->symbols.at((yyvsp[0].stringValue)); 
        (yyval.stringValue) = strdup(member.type.c_str()); 
    }
#line 2113 "maestro.tab.c"
    break;

  case 81: /* func_call: TOK_ID '(' args_list ')'  */
#line 470 "maestro.y"
                             { 
        Symbol* function = currentScope->findSymbol((yyvsp[-3].stringValue)); 
        if(!function) { 
            yyerror(("[EROARE SEMANTICA] Functia " + std::string((yyvsp[-3].stringValue)) + " nu este definita!").c_str()); 
            exit(1); 
        }

        // Verifica numarul de parametri
        std::vector<std::string>* args = (yyvsp[-1].vectorValue); 
        if(function->paramTypes.size() != args->size()) { 
            yyerror(("[EROARE SINTACTICA] Functia " + std::string((yyvsp[-3].stringValue)) + " asteapta " + std::to_string(function->paramTypes.size()) + " argumente, dar i s-au dat " + std::to_string(args->size())).c_str());
            exit(1); 
        }

        // Verifica tipul fiecarui parametru
        for(size_t i = 0; i < args->size(); i++) { 
            if(function->paramTypes[i] != (*args)[i]) { 
                yyerror(("[EROARE SINTACTICA] La functia " + std::string((yyvsp[-3].stringValue)) + " , argumentul " + std::to_string(i + 1) + " trebuie sa fie " + function->paramTypes[i] + ", dar a primti" + (*args)[i]).c_str());
                exit(1);  
            }
        }

        (yyval.stringValue) = strdup(function->type.c_str()); 
        delete args; 
    }
#line 2143 "maestro.tab.c"
    break;

  case 82: /* func_call: TOK_ID '.' TOK_ID '(' args_list ')'  */
#line 494 "maestro.y"
                                            { 
        // Cautam obiectul 
        Symbol* object = currentScope->findSymbol((yyvsp[-5].stringValue)); 
        if(object == nullptr) { 
            yyerror(("[EROARE SEMANTICA] Obiectul " + std::string((yyvsp[-5].stringValue)) + " nu este definit!").c_str());
            exit(1); 
        } 

        std::string className = object->type; 
        Symbol* classSymbol = currentScope->findSymbol(className);

        // verificam daca exista clasa 
        if(classSymbol == nullptr || classSymbol->kind != "class_def") { 
            yyerror(("[EROARE SEMANTICA] Variabila " + std::string((yyvsp[-5].stringValue)) + " este de tip " + className + ", care nu este o clasa!").c_str());
            exit(1); 
        }
        
        SymbolTable* classScope = classSymbol->nestedScope; 
        if(classScope->symbols.find((yyvsp[-3].stringValue)) == classScope->symbols.end()) { 
            yyerror(("[EROARE SEMANTICA] Clasa " + className +  " nu are metoda " + std::string((yyvsp[-3].stringValue))).c_str());
            exit(1); 
        }

        Symbol& method = classScope->symbols.at((yyvsp[-3].stringValue));

        if(method.kind != "method") { 
            yyerror(("[EROARE SEMANTICA] " + std::string((yyvsp[-3].stringValue)) + " nu este o metoda a clasei, ci este un camp!").c_str()); 
            exit(1); 
        }

        std::vector<std::string>* args = (yyvsp[-1].vectorValue); 

        // Verificarea numarului de argumente
        if(method.paramTypes.size() != args->size()) { 
            yyerror(( "[EROARE SINTACTICA] Metoda " + std::string((yyvsp[-3].stringValue)) + " asteapta " + std::to_string(method.paramTypes.size()) + " argumente, dar i s-au dat " + std::to_string(args->size())).c_str());
            exit(1); 
        }

        // Verificarea tipului argumentelor
        for(size_t i = 0; i < args->size(); i++) { 
            if(method.paramTypes[i] != (*args)[i]) { 
                yyerror(("[EROARE SEMANTICA] La metoda " + std::string((yyvsp[-3].stringValue)) + ", argumentul " + std::to_string(i + 1) + " trebuie sa fie " + method.paramTypes[i] + ", dar a primit" + (*args)[i]).c_str()); 
                exit(1); 
            }
        }

        (yyval.stringValue) = strdup(method.type.c_str()); 
        delete args; 
    }
#line 2197 "maestro.tab.c"
    break;

  case 83: /* args_list: non_empty_args  */
#line 546 "maestro.y"
                   { (yyval.vectorValue) = (yyvsp[0].vectorValue); }
#line 2203 "maestro.tab.c"
    break;

  case 84: /* args_list: %empty  */
#line 547 "maestro.y"
        { (yyval.vectorValue) = new std::vector<std::string>(); }
#line 2209 "maestro.tab.c"
    break;

  case 85: /* non_empty_args: non_empty_args ',' expression  */
#line 551 "maestro.y"
                                  { 
        (yyvsp[-2].vectorValue)->push_back((yyvsp[0].stringValue)); 
        (yyval.vectorValue) = (yyvsp[-2].vectorValue); 
    }
#line 2218 "maestro.tab.c"
    break;

  case 86: /* non_empty_args: expression  */
#line 555 "maestro.y"
               { 
        (yyval.vectorValue) = new std::vector<std::string> (); 
        (yyval.vectorValue)->push_back((yyvsp[0].stringValue)); 
    }
#line 2227 "maestro.tab.c"
    break;

  case 87: /* literal: LIT_INT  */
#line 562 "maestro.y"
            { (yyval.stringValue) = strdup("basso"); }
#line 2233 "maestro.tab.c"
    break;

  case 88: /* literal: LIT_FLOAT  */
#line 563 "maestro.y"
              { (yyval.stringValue) = strdup("soprano"); }
#line 2239 "maestro.tab.c"
    break;

  case 89: /* literal: LIT_STRING  */
#line 564 "maestro.y"
               { (yyval.stringValue) = strdup("libretto"); }
#line 2245 "maestro.tab.c"
    break;

  case 90: /* literal: TOK_TRUE  */
#line 565 "maestro.y"
             { (yyval.stringValue) = strdup("verita"); }
#line 2251 "maestro.tab.c"
    break;

  case 91: /* literal: TOK_FALSE  */
#line 566 "maestro.y"
              { (yyval.stringValue) = strdup("verita"); }
#line 2257 "maestro.tab.c"
    break;


#line 2261 "maestro.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 567 "maestro.y"


void yyerror(const char *s) {
    fprintf(stderr, "Errore di Sintassi line %d: %s\n", yylineno, s);
    exit(1);
}

int main() {
    yyparse();
    return 0;
}
