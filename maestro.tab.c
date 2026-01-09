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
#line 1 "maestro.y"

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

class Value {
public:
    std::string type;
    int iVal;
    float fVal;
    std::string sVal;
    bool bVal;

    Value() : type("silenzio"), iVal(0), fVal(0.0), bVal(false) {}
};

class SymbolTable; 

class Symbol { 
    public: 
        std::string name, type, kind; 
        std::vector <std::string> paramTypes; 
        SymbolTable* nestedScope = nullptr; 
        
        Value value; 

        Symbol(std::string _name, std::string _type, std::string _kind) { 
            name = _name; type = _type; kind = _kind; 
            if (type == "basso") value.type = "basso", value.iVal = 0;
            else if (type == "soprano") value.type = "soprano", value.fVal = 0.0;
            else if (type == "verita") value.type = "verita", value.bVal = false;
        }
}; 

class SymbolTable { 
    public: 
        std::string scope; 
        SymbolTable* parent; 
        std::map<std::string, Symbol> symbols; 
        Symbol* lastAddedFunction = nullptr; 

        SymbolTable(std::string _scope, SymbolTable* _parent = nullptr) : scope(_scope), parent(_parent) {}

        bool addSymbol(std::string _name, std::string _type, std::string _kind) { 
            if(symbols.find(_name) != symbols.end()) return false; 
            symbols.insert({ _name, Symbol(_name, _type, _kind)});
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

        void printTable(std::ofstream& fout) { 
            fout << "Symbol Table: " << scope << "\n____________________________________________\n";
            for(auto const &[key, val] : symbols) fout << val.name << "\t" << val.type << "\t" << val.kind << "\n";
            fout << "\n"; 
        }
}; 

SymbolTable* globalScope = nullptr; 
SymbolTable* currentScope = nullptr;
std::ofstream tableFile("tables.txt"); 

enum NodeType { NODE_LITERAL, NODE_ID, NODE_OP, NODE_ASSIGN, NODE_PRINT, NODE_OTHER };

class ASTNode {
public:
    NodeType nodeType;
    std::string op;
    Value literalValue;
    std::string idName;
    std::string exprType;
    ASTNode *left, *right;

    std::vector<ASTNode*> arguments;

    ASTNode(NodeType t) : nodeType(t), left(nullptr), right(nullptr) {}

    Value evaluate() {
        Value res; 

        switch(nodeType) {
            case NODE_LITERAL: {
                return literalValue;
            }

            case NODE_ID: {
                Symbol* symbol = currentScope->findSymbol(idName);
                if(symbol == nullptr) { 
                    std::cerr << "[EROARE SEMANTICA] Variabila " + idName + " nu este definita!\n";
                    exit(1); 
                }
                return symbol->value;
            }

            case NODE_ASSIGN: {
                Value val = right->evaluate(); 
                Symbol* symbol = currentScope->findSymbol(idName); 
                
                if(symbol == nullptr) {
                    size_t dotPos = idName.find('.');
                    if (dotPos != std::string::npos) {
                        std::string objName = idName.substr(0, dotPos);
                        if (currentScope->findSymbol(objName) != nullptr) {
                            currentScope->addSymbol(idName, val.type, "field_instance");
                            symbol = currentScope->findSymbol(idName);
                        }
                    }
                }

                if(symbol == nullptr) { 
                    std::cerr << "[EROARE SEMANTICA] Variabila " + idName + " nu este definita!\n";
                    exit(1); 
                }

                if(symbol->type != val.type) { 
                    std::cerr << "[EROARE SEMANTICA] Nu poti atribui un " + val.type + " la o variabila de tip " + symbol->type << "\n";
                    exit(1); 
                }

                symbol->value = val; 
                return val;
            }

            case NODE_PRINT: {
                Value val = left->evaluate();
                if (val.type == "basso") std::cout << val.iVal << "\n";
                else if (val.type == "soprano") std::cout << val.fVal << "\n";
                else if (val.type == "libretto") std::cout << val.sVal << "\n";
                else if (val.type == "verita") std::cout << (val.bVal ? "vero" : "falso") << "\n";
                return val;
            }

            case NODE_OP: {
                if (op == "UMINUS") {
                    Value val = left->evaluate();
                    if (val.type == "basso") val.iVal = -val.iVal;
                    else if (val.type == "soprano") val.fVal = -val.fVal;
                    return val;
                }
                
                Value l = left->evaluate(); 
                Value r = right->evaluate();
                
                if (op == "+") {
                    if (l.type != r.type) {
                        std::cerr << "[EROARE SEMANTICA] Nu poti aduna " + l.type + " cu " + r.type << "\n";
                        exit(1);
                    }
                    res.type = l.type;
                    if (l.type == "basso") res.iVal = l.iVal + r.iVal;
                    else if (l.type == "soprano") res.fVal = l.fVal + r.fVal;
                } 
                else if (op == "-") {
                    if (l.type != r.type) {
                        std::cerr << "[EROARE SEMANTICA] Nu poti face scadere intre un " + l.type + " si un " + r.type << "\n";
                        exit(1);
                    }
                    res.type = l.type;
                    if (l.type == "basso") res.iVal = l.iVal - r.iVal;
                    else if (l.type == "soprano") res.fVal = l.fVal - r.fVal;
                }
                else if (op == "*") {
                    if (l.type != r.type) {
                         std::cerr << "[EROARE SEMANTICA] Nu poti inmulti " + l.type + " cu " + r.type << "\n";
                         exit(1);
                    }
                    res.type = l.type;
                    if (l.type == "basso") res.iVal = l.iVal * r.iVal;
                    else if (l.type == "soprano") res.fVal = l.fVal * r.fVal;
                }
                else if (op == "/") {
                     if (l.type != r.type) {
                         std::cerr << "[EROARE SEMANTICA] Nu poti imparti " + l.type + " cu " + r.type << "\n";
                         exit(1);
                    }
                    res.type = l.type;
                    if (l.type == "basso") {
                        if (r.iVal == 0) { std::cerr << "Eroare: Impartire la zero!\n"; exit(1); }
                        res.iVal = l.iVal / r.iVal;
                    }
                    else if (l.type == "soprano") {
                        if (r.fVal == 0.0) { std::cerr << "Eroare: Impartire la zero!\n"; exit(1); }
                        res.fVal = l.fVal / r.fVal;
                    }
                }
                else {
                    res.type = "verita";
                    if (op == "==") {
                        if (l.type == "basso") res.bVal = (l.iVal == r.iVal);
                        else res.bVal = (l.fVal == r.fVal);
                    }
                    else if (op == "!=") {
                        if (l.type == "basso") res.bVal = (l.iVal != r.iVal);
                        else res.bVal = (l.fVal != r.fVal);
                    }
                    else if (op == "<") {
                        if (l.type == "basso") res.bVal = (l.iVal < r.iVal);
                        else res.bVal = (l.fVal < r.fVal);
                    }
                    else if (op == ">") {
                        if (l.type == "basso") res.bVal = (l.iVal > r.iVal);
                        else res.bVal = (l.fVal > r.fVal);
                    }
                    else if (op == "<=") {
                        if (l.type == "basso") res.bVal = (l.iVal <= r.iVal);
                        else res.bVal = (l.fVal <= r.fVal);
                    }
                    else if (op == ">=") {
                        if (l.type == "basso") res.bVal = (l.iVal >= r.iVal);
                        else res.bVal = (l.fVal >= r.fVal);
                    }
                }
                return res;
            }

            case NODE_OTHER: {
                Symbol* function = currentScope->findSymbol(idName);
                
                if(!function) { 
                    std::cerr << "[EROARE SEMANTICA] Functia " + idName + " nu este definita!\n"; 
                    exit(1); 
                }

                if(function->paramTypes.size() != arguments.size()) { 
                    std::cerr << "[EROARE SINTACTICA] Functia " + idName + " asteapta " 
                              << std::to_string(function->paramTypes.size()) + " argumente, dar i s-au dat " 
                              << std::to_string(arguments.size()) << "\n";
                    exit(1); 
                }

                for(size_t i = 0; i < arguments.size(); i++) { 
                    Value argVal = arguments[i]->evaluate();

                    if(function->paramTypes[i] != argVal.type) { 
                        std::cerr << "[EROARE SINTACTICA] La functia " + idName + " , argumentul " 
                                  << std::to_string(i + 1) + " trebuie sa fie " + function->paramTypes[i] 
                                  << ", dar a primti" + argVal.type << "\n";
                        exit(1);  
                    }
                }

                res.type = function->type; 
                return res;
            }
        }
        return res;
    }
};

std::string currentType, currentName; 

#line 343 "maestro.tab.c"

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
  YYSYMBOL_LIT_INT = 9,                    /* LIT_INT  */
  YYSYMBOL_LIT_FLOAT = 10,                 /* LIT_FLOAT  */
  YYSYMBOL_LIT_STRING = 11,                /* LIT_STRING  */
  YYSYMBOL_TOK_TRUE = 12,                  /* TOK_TRUE  */
  YYSYMBOL_TOK_FALSE = 13,                 /* TOK_FALSE  */
  YYSYMBOL_TOK_CLASS = 14,                 /* TOK_CLASS  */
  YYSYMBOL_TOK_MAIN = 15,                  /* TOK_MAIN  */
  YYSYMBOL_TOK_IF = 16,                    /* TOK_IF  */
  YYSYMBOL_TOK_WHILE = 17,                 /* TOK_WHILE  */
  YYSYMBOL_TOK_PRINT = 18,                 /* TOK_PRINT  */
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
  YYSYMBOL_TOK_MINUS_ASSIGN = 29,          /* TOK_MINUS_ASSIGN  */
  YYSYMBOL_TOK_MUL_ASSIGN = 30,            /* TOK_MUL_ASSIGN  */
  YYSYMBOL_TOK_DIV_ASSIGN = 31,            /* TOK_DIV_ASSIGN  */
  YYSYMBOL_32_ = 32,                       /* '<'  */
  YYSYMBOL_33_ = 33,                       /* '>'  */
  YYSYMBOL_34_ = 34,                       /* '+'  */
  YYSYMBOL_35_ = 35,                       /* '-'  */
  YYSYMBOL_36_ = 36,                       /* '*'  */
  YYSYMBOL_37_ = 37,                       /* '/'  */
  YYSYMBOL_UMINUS = 38,                    /* UMINUS  */
  YYSYMBOL_39_ = 39,                       /* '('  */
  YYSYMBOL_40_ = 40,                       /* ')'  */
  YYSYMBOL_41_ = 41,                       /* '{'  */
  YYSYMBOL_42_ = 42,                       /* '}'  */
  YYSYMBOL_43_ = 43,                       /* ';'  */
  YYSYMBOL_44_ = 44,                       /* '.'  */
  YYSYMBOL_45_ = 45,                       /* ','  */
  YYSYMBOL_YYACCEPT = 46,                  /* $accept  */
  YYSYMBOL_program = 47,                   /* program  */
  YYSYMBOL_48_1 = 48,                      /* $@1  */
  YYSYMBOL_global_elements = 49,           /* global_elements  */
  YYSYMBOL_global_decl = 50,               /* global_decl  */
  YYSYMBOL_51_2 = 51,                      /* $@2  */
  YYSYMBOL_global_decl_suffix = 52,        /* global_decl_suffix  */
  YYSYMBOL_53_3 = 53,                      /* $@3  */
  YYSYMBOL_finale_block = 54,              /* finale_block  */
  YYSYMBOL_55_4 = 55,                      /* $@4  */
  YYSYMBOL_stmt_list_pure = 56,            /* stmt_list_pure  */
  YYSYMBOL_statement = 57,                 /* statement  */
  YYSYMBOL_assignment_stmt = 58,           /* assignment_stmt  */
  YYSYMBOL_print_stmt = 59,                /* print_stmt  */
  YYSYMBOL_expression = 60,                /* expression  */
  YYSYMBOL_func_call = 61,                 /* func_call  */
  YYSYMBOL_args_list = 62,                 /* args_list  */
  YYSYMBOL_non_empty_args = 63,            /* non_empty_args  */
  YYSYMBOL_control_stmt = 64,              /* control_stmt  */
  YYSYMBOL_if_stmt = 65,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 66,                /* while_stmt  */
  YYSYMBOL_block_pure = 67,                /* block_pure  */
  YYSYMBOL_class_decl = 68,                /* class_decl  */
  YYSYMBOL_class_body = 69,                /* class_body  */
  YYSYMBOL_class_member = 70,              /* class_member  */
  YYSYMBOL_class_member_suffix = 71,       /* class_member_suffix  */
  YYSYMBOL_param_list = 72,                /* param_list  */
  YYSYMBOL_non_empty_params = 73,          /* non_empty_params  */
  YYSYMBOL_func_body = 74,                 /* func_body  */
  YYSYMBOL_var_decl = 75,                  /* var_decl  */
  YYSYMBOL_var_decl_suffix = 76,           /* var_decl_suffix  */
  YYSYMBOL_return_type = 77,               /* return_type  */
  YYSYMBOL_data_type = 78                  /* data_type  */
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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

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
#define YYLAST   424

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  33
/* YYNRULES -- Number of rules.  */
#define YYNRULES  83
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  162

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   287


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
      39,    40,    36,    34,    45,    35,    44,    37,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    43,
      32,     2,    33,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    41,     2,    42,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    38
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   301,   301,   301,   305,   305,   307,   307,   307,   309,
     309,   313,   316,   316,   323,   323,   326,   327,   328,   329,
     330,   331,   342,   353,   363,   373,   383,   395,   398,   405,
     410,   411,   412,   413,   415,   421,   426,   427,   428,   429,
     430,   431,   433,   434,   436,   437,   438,   439,   440,   441,
     442,   443,   447,   457,   468,   469,   471,   472,   474,   474,
     476,   478,   480,   482,   484,   484,   486,   488,   488,   490,
     490,   493,   497,   503,   505,   507,   507,   509,   509,   511,
     511,   511,   511,   511
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TOK_TYPE_INT",
  "TOK_TYPE_FLOAT", "TOK_TYPE_STRING", "TOK_TYPE_BOOL", "TOK_TYPE_VOID",
  "TOK_ID", "LIT_INT", "LIT_FLOAT", "LIT_STRING", "TOK_TRUE", "TOK_FALSE",
  "TOK_CLASS", "TOK_MAIN", "TOK_IF", "TOK_WHILE", "TOK_PRINT",
  "TOK_ASSIGN", "TOK_EQ", "TOK_NEQ", "TOK_LEQ", "TOK_GEQ", "TOK_AND",
  "TOK_OR", "TOK_INC", "TOK_DEC", "TOK_PLUS_ASSIGN", "TOK_MINUS_ASSIGN",
  "TOK_MUL_ASSIGN", "TOK_DIV_ASSIGN", "'<'", "'>'", "'+'", "'-'", "'*'",
  "'/'", "UMINUS", "'('", "')'", "'{'", "'}'", "';'", "'.'", "','",
  "$accept", "program", "$@1", "global_elements", "global_decl", "$@2",
  "global_decl_suffix", "$@3", "finale_block", "$@4", "stmt_list_pure",
  "statement", "assignment_stmt", "print_stmt", "expression", "func_call",
  "args_list", "non_empty_args", "control_stmt", "if_stmt", "while_stmt",
  "block_pure", "class_decl", "class_body", "class_member",
  "class_member_suffix", "param_list", "non_empty_params", "func_body",
  "var_decl", "var_decl_suffix", "return_type", "data_type", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-29)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -29,    20,   -29,   -29,   409,   -29,   -29,   -29,   -29,   -29,
     -29,    14,   -29,   -29,   -29,   -29,    21,   -29,   -10,    -9,
     -29,   -29,   -29,   -15,    71,     9,    83,   -29,   -29,   -29,
     -29,   -29,   -29,    33,   109,     7,     8,    13,   -29,   -29,
     -29,   -29,    10,   -29,   -29,   -29,   -29,    46,   -28,   -29,
     -29,   -29,   -29,   -29,    83,    83,   122,   -29,    32,    41,
      83,    29,    42,    83,    83,    83,    83,    83,    89,    83,
      83,    83,   -29,   -13,    96,   -29,   290,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,   -29,
      68,    60,    98,    32,   -29,   -29,   146,   -29,   -29,   170,
     194,   218,   242,   374,    69,    67,   -16,   311,   332,   353,
     -29,    72,   -29,   164,   164,   -27,   -27,    66,   140,   -27,
     -27,   -18,   -18,   -29,   -29,    75,    32,   -29,    70,   -29,
     -29,   -29,   -29,   -29,   -29,    83,    83,    83,    76,    76,
      77,   -29,   111,    80,   374,   266,    84,   -29,   -29,   -29,
     -29,   217,    87,   -29,   -29,   -29,   -29,    65,   -29,    88,
     -29,   -29
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     5,     1,     0,    79,    80,    81,    82,    78,
      83,     0,    12,     4,     3,     6,     0,    77,     0,     0,
       7,    65,    15,     0,     0,     0,     0,     9,    75,     8,
      11,    63,    64,     0,    83,     0,     0,     0,    13,    14,
      16,    17,     0,    19,    58,    59,    20,     0,    49,    44,
      45,    46,    47,    48,     0,     0,     0,    50,    70,     0,
       0,     0,     0,     0,     0,     0,     0,    55,     0,     0,
       0,     0,    18,     0,     0,    34,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    76,
       0,    69,     0,    70,    67,    66,     0,    21,    22,     0,
       0,     0,     0,    57,     0,    54,     0,     0,     0,     0,
      74,    35,    51,    36,    37,    40,    41,    42,    43,    38,
      39,    30,    31,    32,    33,     0,     0,    72,     0,    27,
      23,    24,    25,    26,    52,     0,     0,    55,     0,     0,
       0,    15,     0,     0,    56,     0,     0,    15,    60,    61,
      29,    73,     0,    71,    15,    28,    53,     0,    10,     0,
      62,    68
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -29,   -29,   -29,   -29,   -29,   -29,   -29,   -29,   -29,   -29,
     -20,   -29,   -29,   -29,   -21,   -25,   -12,   -29,   -29,   -29,
     -29,    -8,   -29,   -29,   -29,   -29,    40,   -29,    -5,   -29,
      61,   117,    -3
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,     4,    13,    23,    29,    58,    14,    19,
     151,    39,    40,    41,   103,    57,   104,   105,    43,    44,
      45,   148,    15,    24,    32,    95,    90,    91,   152,    46,
      30,    16,    47
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      42,    17,    25,   136,    26,    56,    26,    85,    86,    87,
      88,    67,     5,     6,     7,     8,    74,    34,    87,    88,
       3,    17,    18,   137,    27,    35,    36,    37,    28,    20,
      28,    21,    22,    75,    76,     5,     6,     7,     8,    96,
      10,    59,    99,   100,   101,   102,    69,    70,   107,   108,
     109,    38,    71,    72,    73,    92,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,     5,     6,
       7,     8,    97,    34,     5,     6,     7,     8,     9,    10,
      93,    35,    36,    37,    94,    98,    77,    78,    79,    80,
      92,    48,    49,    50,    51,    52,    53,   106,    83,    84,
      85,    86,    87,    88,   111,   126,   127,   160,   125,   134,
     143,   137,   135,    31,   144,   145,   141,   147,    54,   153,
     150,   154,    55,   142,   156,   146,    42,   157,    60,   158,
     161,   149,    42,   128,   110,    61,    62,    63,    64,    65,
      66,    33,    77,    78,    79,    80,    81,    82,    67,   159,
       0,     0,     0,    68,    83,    84,    85,    86,    87,    88,
      77,    78,    79,    80,    81,    89,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    83,    84,
      85,    86,    87,    88,     0,     0,    79,    80,     0,   129,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    83,    84,    85,    86,    87,    88,     0,     0,
       0,     0,     0,   130,    77,    78,    79,    80,    81,    82,
       5,     6,     7,     8,     0,    34,    83,    84,    85,    86,
      87,    88,     0,    35,    36,    37,     0,   131,    77,    78,
      79,    80,    81,    82,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,    87,    88,     0,     0,     0,     0,
       0,   132,    77,    78,    79,    80,    81,    82,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,    87,    88,
       0,     0,     0,     0,     0,   133,    77,    78,    79,    80,
      81,    82,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,    87,    88,     0,     0,     0,     0,     0,   155,
      77,    78,    79,    80,    81,    82,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,    87,    88,     0,     0,
     112,    77,    78,    79,    80,    81,    82,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,    87,    88,     0,
       0,   138,    77,    78,    79,    80,    81,    82,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,    87,    88,
       0,     0,   139,    77,    78,    79,    80,    81,    82,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,    87,
      88,     0,     0,   140,    77,    78,    79,    80,    81,    82,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
      87,    88,     5,     6,     7,     8,     9,    10,     0,     0,
       0,     0,     0,    11,    12
};

static const yytype_int16 yycheck[] =
{
      25,     4,    22,    19,    19,    26,    19,    34,    35,    36,
      37,    39,     3,     4,     5,     6,    44,     8,    36,    37,
       0,    24,     8,    39,    39,    16,    17,    18,    43,     8,
      43,    41,    41,    54,    55,     3,     4,     5,     6,    60,
       8,     8,    63,    64,    65,    66,    39,    39,    69,    70,
      71,    42,    39,    43,     8,    58,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,     3,     4,
       5,     6,    43,     8,     3,     4,     5,     6,     7,     8,
      39,    16,    17,    18,    43,    43,    20,    21,    22,    23,
      93,     8,     9,    10,    11,    12,    13,     8,    32,    33,
      34,    35,    36,    37,     8,    45,     8,    42,    40,    40,
      40,    39,    45,    42,   135,   136,    41,    41,    35,     8,
      43,    41,    39,   126,    40,   137,   151,   147,    19,    42,
      42,   139,   157,    93,    73,    26,    27,    28,    29,    30,
      31,    24,    20,    21,    22,    23,    24,    25,    39,   154,
      -1,    -1,    -1,    44,    32,    33,    34,    35,    36,    37,
      20,    21,    22,    23,    24,    43,    20,    21,    22,    23,
      24,    25,    32,    33,    34,    35,    36,    37,    32,    33,
      34,    35,    36,    37,    -1,    -1,    22,    23,    -1,    43,
      20,    21,    22,    23,    24,    25,    32,    33,    34,    35,
      36,    37,    32,    33,    34,    35,    36,    37,    -1,    -1,
      -1,    -1,    -1,    43,    20,    21,    22,    23,    24,    25,
       3,     4,     5,     6,    -1,     8,    32,    33,    34,    35,
      36,    37,    -1,    16,    17,    18,    -1,    43,    20,    21,
      22,    23,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    34,    35,    36,    37,    -1,    -1,    -1,    -1,
      -1,    43,    20,    21,    22,    23,    24,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    34,    35,    36,    37,
      -1,    -1,    -1,    -1,    -1,    43,    20,    21,    22,    23,
      24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      34,    35,    36,    37,    -1,    -1,    -1,    -1,    -1,    43,
      20,    21,    22,    23,    24,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    34,    35,    36,    37,    -1,    -1,
      40,    20,    21,    22,    23,    24,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    34,    35,    36,    37,    -1,
      -1,    40,    20,    21,    22,    23,    24,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    34,    35,    36,    37,
      -1,    -1,    40,    20,    21,    22,    23,    24,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    34,    35,    36,
      37,    -1,    -1,    40,    20,    21,    22,    23,    24,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    34,    35,
      36,    37,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    -1,    -1,    14,    15
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    47,    48,     0,    49,     3,     4,     5,     6,     7,
       8,    14,    15,    50,    54,    68,    77,    78,     8,    55,
       8,    41,    41,    51,    69,    56,    19,    39,    43,    52,
      76,    42,    70,    77,     8,    16,    17,    18,    42,    57,
      58,    59,    61,    64,    65,    66,    75,    78,     8,     9,
      10,    11,    12,    13,    35,    39,    60,    61,    53,     8,
      19,    26,    27,    28,    29,    30,    31,    39,    44,    39,
      39,    39,    43,     8,    44,    60,    60,    20,    21,    22,
      23,    24,    25,    32,    33,    34,    35,    36,    37,    43,
      72,    73,    78,    39,    43,    71,    60,    43,    43,    60,
      60,    60,    60,    60,    62,    63,     8,    60,    60,    60,
      76,     8,    40,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    40,    45,     8,    72,    43,
      43,    43,    43,    43,    40,    45,    19,    39,    40,    40,
      40,    41,    78,    40,    60,    60,    62,    41,    67,    67,
      43,    56,    74,     8,    41,    43,    40,    56,    42,    74,
      42,    42
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    46,    48,    47,    49,    49,    50,    51,    50,    53,
      52,    52,    55,    54,    56,    56,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    57,    58,    58,    59,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    61,    61,    62,    62,    63,    63,    64,    64,
      65,    66,    67,    68,    69,    69,    70,    71,    71,    72,
      72,    73,    73,    74,    75,    76,    76,    77,    77,    78,
      78,    78,    78,    78
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     3,     2,     0,     1,     0,     4,     0,
       7,     1,     0,     5,     2,     0,     1,     1,     2,     1,
       1,     3,     3,     4,     4,     4,     4,     4,     6,     5,
       3,     3,     3,     3,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     3,     4,     6,     1,     0,     3,     1,     1,     1,
       5,     5,     3,     5,     2,     0,     3,     1,     6,     1,
       0,     4,     2,     1,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1
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
#line 301 "maestro.y"
         { globalScope = new SymbolTable("Global Scope"); currentScope = globalScope; }
#line 1572 "maestro.tab.c"
    break;

  case 3: /* program: $@1 global_elements finale_block  */
#line 302 "maestro.y"
                                 { currentScope->printTable(tableFile); std::cout << "Sintassi Corretta!\n"; }
#line 1578 "maestro.tab.c"
    break;

  case 7: /* $@2: %empty  */
#line 307 "maestro.y"
                                             { currentType = (yyvsp[-1].stringValue); currentName = (yyvsp[0].stringValue); }
#line 1584 "maestro.tab.c"
    break;

  case 9: /* $@3: %empty  */
#line 309 "maestro.y"
                        { 
        currentScope->addSymbol(currentName, currentType, "function");
        currentScope = new SymbolTable("Function: " + currentName, currentScope); 
    }
#line 1593 "maestro.tab.c"
    break;

  case 10: /* global_decl_suffix: '(' $@3 param_list ')' '{' func_body '}'  */
#line 312 "maestro.y"
                                       { currentScope->printTable(tableFile); currentScope = currentScope->parent; }
#line 1599 "maestro.tab.c"
    break;

  case 11: /* global_decl_suffix: var_decl_suffix  */
#line 313 "maestro.y"
                      { currentScope->addSymbol(currentName, currentType, "global_var"); }
#line 1605 "maestro.tab.c"
    break;

  case 12: /* $@4: %empty  */
#line 316 "maestro.y"
                       { currentScope = new SymbolTable("Main Function", currentScope); }
#line 1611 "maestro.tab.c"
    break;

  case 13: /* finale_block: TOK_MAIN $@4 '{' stmt_list_pure '}'  */
#line 317 "maestro.y"
                           { 
        for (ASTNode* node : *(yyvsp[-1].astList)) if (node) node->evaluate(); // Evaluate Main Block [cite: 1, 48]
        currentScope->printTable(tableFile); currentScope = currentScope->parent; 
    }
#line 1620 "maestro.tab.c"
    break;

  case 14: /* stmt_list_pure: stmt_list_pure statement  */
#line 323 "maestro.y"
                                         { (yyvsp[-1].astList)->push_back((yyvsp[0].astNode)); (yyval.astList) = (yyvsp[-1].astList); }
#line 1626 "maestro.tab.c"
    break;

  case 15: /* stmt_list_pure: %empty  */
#line 323 "maestro.y"
                                                                           { (yyval.astList) = new std::vector<ASTNode*>(); }
#line 1632 "maestro.tab.c"
    break;

  case 16: /* statement: assignment_stmt  */
#line 326 "maestro.y"
                      { (yyval.astNode) = (yyvsp[0].astNode); }
#line 1638 "maestro.tab.c"
    break;

  case 17: /* statement: print_stmt  */
#line 327 "maestro.y"
                 { (yyval.astNode) = (yyvsp[0].astNode); }
#line 1644 "maestro.tab.c"
    break;

  case 18: /* statement: func_call ';'  */
#line 328 "maestro.y"
                    { (yyval.astNode) = nullptr; }
#line 1650 "maestro.tab.c"
    break;

  case 19: /* statement: control_stmt  */
#line 329 "maestro.y"
                   { (yyval.astNode) = nullptr; }
#line 1656 "maestro.tab.c"
    break;

  case 20: /* statement: var_decl  */
#line 330 "maestro.y"
               { (yyval.astNode) = nullptr; }
#line 1662 "maestro.tab.c"
    break;

  case 21: /* statement: TOK_ID TOK_INC ';'  */
#line 331 "maestro.y"
                         { 
        (yyval.astNode) = new ASTNode(NODE_ASSIGN); (yyval.astNode)->idName = (yyvsp[-2].stringValue); 
        
        ASTNode* opNode = new ASTNode(NODE_OP); opNode->op = "+";
        ASTNode* idNode = new ASTNode(NODE_ID); idNode->idName = (yyvsp[-2].stringValue);
        ASTNode* litNode = new ASTNode(NODE_LITERAL); litNode->literalValue.type = "basso"; litNode->literalValue.iVal = 1;

        opNode->left = idNode; opNode->right = litNode;
        (yyval.astNode)->right = opNode;
    }
#line 1677 "maestro.tab.c"
    break;

  case 22: /* statement: TOK_ID TOK_DEC ';'  */
#line 342 "maestro.y"
                         { 
        (yyval.astNode) = new ASTNode(NODE_ASSIGN); (yyval.astNode)->idName = (yyvsp[-2].stringValue); 
        
        ASTNode* opNode = new ASTNode(NODE_OP); opNode->op = "-";
        ASTNode* idNode = new ASTNode(NODE_ID); idNode->idName = (yyvsp[-2].stringValue);
        ASTNode* litNode = new ASTNode(NODE_LITERAL); litNode->literalValue.type = "basso"; litNode->literalValue.iVal = 1;

        opNode->left = idNode; opNode->right = litNode;
        (yyval.astNode)->right = opNode;
    }
#line 1692 "maestro.tab.c"
    break;

  case 23: /* statement: TOK_ID TOK_PLUS_ASSIGN expression ';'  */
#line 353 "maestro.y"
                                            {
        (yyval.astNode) = new ASTNode(NODE_ASSIGN); (yyval.astNode)->idName = (yyvsp[-3].stringValue);

        ASTNode* opNode = new ASTNode(NODE_OP); opNode->op = "+";
        ASTNode* idNode = new ASTNode(NODE_ID); idNode->idName = (yyvsp[-3].stringValue);

        opNode->left = idNode; opNode->right = (yyvsp[-1].astNode); 
        (yyval.astNode)->right = opNode;
    }
#line 1706 "maestro.tab.c"
    break;

  case 24: /* statement: TOK_ID TOK_MINUS_ASSIGN expression ';'  */
#line 363 "maestro.y"
                                             {
        (yyval.astNode) = new ASTNode(NODE_ASSIGN); (yyval.astNode)->idName = (yyvsp[-3].stringValue);

        ASTNode* opNode = new ASTNode(NODE_OP); opNode->op = "-";
        ASTNode* idNode = new ASTNode(NODE_ID); idNode->idName = (yyvsp[-3].stringValue);

        opNode->left = idNode; opNode->right = (yyvsp[-1].astNode); 
        (yyval.astNode)->right = opNode;
    }
#line 1720 "maestro.tab.c"
    break;

  case 25: /* statement: TOK_ID TOK_MUL_ASSIGN expression ';'  */
#line 373 "maestro.y"
                                           {
        (yyval.astNode) = new ASTNode(NODE_ASSIGN); (yyval.astNode)->idName = (yyvsp[-3].stringValue);

        ASTNode* opNode = new ASTNode(NODE_OP); opNode->op = "*";
        ASTNode* idNode = new ASTNode(NODE_ID); idNode->idName = (yyvsp[-3].stringValue);

        opNode->left = idNode; opNode->right = (yyvsp[-1].astNode); 
        (yyval.astNode)->right = opNode;
    }
#line 1734 "maestro.tab.c"
    break;

  case 26: /* statement: TOK_ID TOK_DIV_ASSIGN expression ';'  */
#line 383 "maestro.y"
                                           {
        (yyval.astNode) = new ASTNode(NODE_ASSIGN); (yyval.astNode)->idName = (yyvsp[-3].stringValue);

        ASTNode* opNode = new ASTNode(NODE_OP); opNode->op = "/";
        ASTNode* idNode = new ASTNode(NODE_ID); idNode->idName = (yyvsp[-3].stringValue);

        opNode->left = idNode; opNode->right = (yyvsp[-1].astNode); 
        (yyval.astNode)->right = opNode;
    }
#line 1748 "maestro.tab.c"
    break;

  case 27: /* assignment_stmt: TOK_ID TOK_ASSIGN expression ';'  */
#line 395 "maestro.y"
                                     { 
        (yyval.astNode) = new ASTNode(NODE_ASSIGN); (yyval.astNode)->idName = (yyvsp[-3].stringValue); (yyval.astNode)->right = (yyvsp[-1].astNode); 
    }
#line 1756 "maestro.tab.c"
    break;

  case 28: /* assignment_stmt: TOK_ID '.' TOK_ID TOK_ASSIGN expression ';'  */
#line 398 "maestro.y"
                                                  {
        (yyval.astNode) = new ASTNode(NODE_ASSIGN);
        (yyval.astNode)->idName = std::string((yyvsp[-5].stringValue)) + "." + std::string((yyvsp[-3].stringValue));
        (yyval.astNode)->right = (yyvsp[-1].astNode);
    }
#line 1766 "maestro.tab.c"
    break;

  case 29: /* print_stmt: TOK_PRINT '(' expression ')' ';'  */
#line 405 "maestro.y"
                                             { 
        (yyval.astNode) = new ASTNode(NODE_PRINT); (yyval.astNode)->left = (yyvsp[-2].astNode);
    }
#line 1774 "maestro.tab.c"
    break;

  case 30: /* expression: expression '+' expression  */
#line 410 "maestro.y"
                              { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = "+"; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1780 "maestro.tab.c"
    break;

  case 31: /* expression: expression '-' expression  */
#line 411 "maestro.y"
                                { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = "-"; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1786 "maestro.tab.c"
    break;

  case 32: /* expression: expression '*' expression  */
#line 412 "maestro.y"
                                { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = "*"; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1792 "maestro.tab.c"
    break;

  case 33: /* expression: expression '/' expression  */
#line 413 "maestro.y"
                                { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = "/"; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1798 "maestro.tab.c"
    break;

  case 34: /* expression: '-' expression  */
#line 415 "maestro.y"
                                  { 
            (yyval.astNode) = new ASTNode(NODE_OP); 
            (yyval.astNode)->op = "UMINUS";  /* Nume special ca sa stim la evaluare */
            (yyval.astNode)->left = (yyvsp[0].astNode); 
            (yyval.astNode)->right = nullptr; 
        }
#line 1809 "maestro.tab.c"
    break;

  case 35: /* expression: TOK_ID '.' TOK_ID  */
#line 421 "maestro.y"
                        { 
        (yyval.astNode) = new ASTNode(NODE_ID);
        (yyval.astNode)->idName = std::string((yyvsp[-2].stringValue)) + "." + std::string((yyvsp[0].stringValue));
    }
#line 1818 "maestro.tab.c"
    break;

  case 36: /* expression: expression TOK_EQ expression  */
#line 426 "maestro.y"
                                   { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = "=="; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1824 "maestro.tab.c"
    break;

  case 37: /* expression: expression TOK_NEQ expression  */
#line 427 "maestro.y"
                                    { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = "!="; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1830 "maestro.tab.c"
    break;

  case 38: /* expression: expression '<' expression  */
#line 428 "maestro.y"
                                { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = "<"; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1836 "maestro.tab.c"
    break;

  case 39: /* expression: expression '>' expression  */
#line 429 "maestro.y"
                                { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = ">"; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1842 "maestro.tab.c"
    break;

  case 40: /* expression: expression TOK_LEQ expression  */
#line 430 "maestro.y"
                                    { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = "<="; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1848 "maestro.tab.c"
    break;

  case 41: /* expression: expression TOK_GEQ expression  */
#line 431 "maestro.y"
                                    { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = ">="; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1854 "maestro.tab.c"
    break;

  case 42: /* expression: expression TOK_AND expression  */
#line 433 "maestro.y"
                                    { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = "&&"; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1860 "maestro.tab.c"
    break;

  case 43: /* expression: expression TOK_OR expression  */
#line 434 "maestro.y"
                                   { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = "||"; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1866 "maestro.tab.c"
    break;

  case 44: /* expression: LIT_INT  */
#line 436 "maestro.y"
              { (yyval.astNode) = new ASTNode(NODE_LITERAL); (yyval.astNode)->literalValue.type = "basso"; (yyval.astNode)->literalValue.iVal = atoi((yyvsp[0].stringValue)); }
#line 1872 "maestro.tab.c"
    break;

  case 45: /* expression: LIT_FLOAT  */
#line 437 "maestro.y"
                { (yyval.astNode) = new ASTNode(NODE_LITERAL); (yyval.astNode)->literalValue.type = "soprano"; (yyval.astNode)->literalValue.fVal = atof((yyvsp[0].stringValue)); }
#line 1878 "maestro.tab.c"
    break;

  case 46: /* expression: LIT_STRING  */
#line 438 "maestro.y"
                 { (yyval.astNode) = new ASTNode(NODE_LITERAL); (yyval.astNode)->literalValue.type = "libretto"; (yyval.astNode)->literalValue.sVal = (yyvsp[0].stringValue); }
#line 1884 "maestro.tab.c"
    break;

  case 47: /* expression: TOK_TRUE  */
#line 439 "maestro.y"
               { (yyval.astNode) = new ASTNode(NODE_LITERAL); (yyval.astNode)->literalValue.type = "verita"; (yyval.astNode)->literalValue.bVal = true; }
#line 1890 "maestro.tab.c"
    break;

  case 48: /* expression: TOK_FALSE  */
#line 440 "maestro.y"
                { (yyval.astNode) = new ASTNode(NODE_LITERAL); (yyval.astNode)->literalValue.type = "verita"; (yyval.astNode)->literalValue.bVal = false; }
#line 1896 "maestro.tab.c"
    break;

  case 49: /* expression: TOK_ID  */
#line 441 "maestro.y"
             { (yyval.astNode) = new ASTNode(NODE_ID); (yyval.astNode)->idName = (yyvsp[0].stringValue); }
#line 1902 "maestro.tab.c"
    break;

  case 50: /* expression: func_call  */
#line 442 "maestro.y"
                { (yyval.astNode) = new ASTNode(NODE_OTHER); (yyval.astNode)->exprType = "unknown"; }
#line 1908 "maestro.tab.c"
    break;

  case 51: /* expression: '(' expression ')'  */
#line 443 "maestro.y"
                         { (yyval.astNode) = (yyvsp[-1].astNode); }
#line 1914 "maestro.tab.c"
    break;

  case 52: /* func_call: TOK_ID '(' args_list ')'  */
#line 447 "maestro.y"
                             { 
        (yyval.astNode) = new ASTNode(NODE_OTHER); 
        (yyval.astNode)->idName = (yyvsp[-3].stringValue); 

        if ((yyvsp[-1].astList)) {
            (yyval.astNode)->arguments = *(yyvsp[-1].astList); 
            delete (yyvsp[-1].astList);          
        }
    }
#line 1928 "maestro.tab.c"
    break;

  case 53: /* func_call: TOK_ID '.' TOK_ID '(' args_list ')'  */
#line 457 "maestro.y"
                                          { 
        (yyval.astNode) = new ASTNode(NODE_OTHER); 
        (yyval.astNode)->idName = std::string((yyvsp[-5].stringValue)) + "." + std::string((yyvsp[-3].stringValue)); 
        
        if ((yyvsp[-1].astList)) {
            (yyval.astNode)->arguments = *(yyvsp[-1].astList); 
            delete (yyvsp[-1].astList); 
        }
    }
#line 1942 "maestro.tab.c"
    break;

  case 54: /* args_list: non_empty_args  */
#line 468 "maestro.y"
                          { (yyval.astList) = (yyvsp[0].astList); }
#line 1948 "maestro.tab.c"
    break;

  case 55: /* args_list: %empty  */
#line 469 "maestro.y"
      { (yyval.astList) = new std::vector<ASTNode*>(); }
#line 1954 "maestro.tab.c"
    break;

  case 56: /* non_empty_args: non_empty_args ',' expression  */
#line 471 "maestro.y"
                                              { (yyvsp[-2].astList)->push_back((yyvsp[0].astNode)); (yyval.astList) = (yyvsp[-2].astList); }
#line 1960 "maestro.tab.c"
    break;

  case 57: /* non_empty_args: expression  */
#line 472 "maestro.y"
                 { (yyval.astList) = new std::vector<ASTNode*>(); (yyval.astList)->push_back((yyvsp[0].astNode)); }
#line 1966 "maestro.tab.c"
    break;

  case 60: /* if_stmt: TOK_IF '(' expression ')' block_pure  */
#line 476 "maestro.y"
                                              { (yyval.astNode) = nullptr; }
#line 1972 "maestro.tab.c"
    break;

  case 61: /* while_stmt: TOK_WHILE '(' expression ')' block_pure  */
#line 478 "maestro.y"
                                                    { (yyval.astNode) = nullptr; }
#line 1978 "maestro.tab.c"
    break;

  case 62: /* block_pure: '{' stmt_list_pure '}'  */
#line 480 "maestro.y"
                                   { (yyval.astList) = (yyvsp[-1].astList); }
#line 1984 "maestro.tab.c"
    break;

  case 71: /* non_empty_params: non_empty_params ',' data_type TOK_ID  */
#line 493 "maestro.y"
                                          { 
        currentScope->addSymbol((yyvsp[0].stringValue), (yyvsp[-1].stringValue), "parameter"); 
        currentScope->addParamToLastFunction((yyvsp[-1].stringValue)); 
    }
#line 1993 "maestro.tab.c"
    break;

  case 72: /* non_empty_params: data_type TOK_ID  */
#line 497 "maestro.y"
                     { 
        currentScope->addSymbol((yyvsp[0].stringValue), (yyvsp[-1].stringValue), "parameter"); 
        currentScope->addParamToLastFunction((yyvsp[-1].stringValue)); 
    }
#line 2002 "maestro.tab.c"
    break;


#line 2006 "maestro.tab.c"

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
      yyerror (YY_("syntax error"));
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

  return yyresult;
}

#line 512 "maestro.y"


void yyerror(const char *s) { fprintf(stderr, "Error line %d: %s\n", yylineno, s); exit(1); }
int main() { yyparse(); return 0; }
