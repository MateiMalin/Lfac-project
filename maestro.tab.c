#define YYBISON 30802
#define YYBISON_VERSION "3.8.2"
#define YYSKELETON_NAME "yacc.c"
#define YYPURE 0
#define YYPUSH 0
#define YYPULL 1
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
void yyerror(const char* s);
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
    if (symbols.find(_name) != symbols.end()) return false;
    symbols.insert({ _name, Symbol(_name, _type, _kind) });
    if (_kind == "method" || _kind == "function") lastAddedFunction = &symbols.at(_name);
    return true;
  }
  void addParamToLastFunction(std::string paramType) {
    if (lastAddedFunction != nullptr) lastAddedFunction->paramTypes.push_back(paramType);
    else if (parent != nullptr) parent->addParamToLastFunction(paramType);
  }
  Symbol* findSymbol(std::string name) {
    if (symbols.find(name) != symbols.end()) return &symbols.at(name);
    if (parent != nullptr) return parent->findSymbol(name);
    return nullptr;
  }
  void printTable(std::ofstream& fout) {
    fout << "Symbol Table: " << scope << "\n____________________________________________\n";
    for (auto const& [key, val] : symbols) fout << val.name << "\t" << val.type << "\t" << val.kind << "\n";
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
  ASTNode* left, * right;
  std::vector<ASTNode*> arguments;
  ASTNode(NodeType t) : nodeType(t), left(nullptr), right(nullptr) {}
  Value evaluate() {
    Value res;
    switch (nodeType) {
    case NODE_LITERAL: {
      return literalValue;
    }
    case NODE_ID: {
      Symbol* symbol = currentScope->findSymbol(idName);
      if (symbol == nullptr) {
        std::cerr << "[EROARE SEMANTICA] Variabila " + idName + " nu este definita!\n";
        exit(1);
      }
      return symbol->value;
    }
    case NODE_ASSIGN: {
      Value val = right->evaluate();
      Symbol* symbol = currentScope->findSymbol(idName);
      if (symbol == nullptr) {
        size_t dotPos = idName.find('.');
        if (dotPos != std::string::npos) {
          std::string objName = idName.substr(0, dotPos);
          if (currentScope->findSymbol(objName) != nullptr) {
            currentScope->addSymbol(idName, val.type, "field_instance");
            symbol = currentScope->findSymbol(idName);
          }
        }
      }
      if (symbol == nullptr) {
        std::cerr << "[EROARE SEMANTICA] Variabila " + idName + " nu este definita!\n";
        exit(1);
      }
      if (symbol->type != val.type) {
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
      if (!function) {
        std::cerr << "[EROARE SEMANTICA] Functia " + idName + " nu este definita!\n";
        exit(1);
      }
      if (function->paramTypes.size() != arguments.size()) {
        std::cerr << "[EROARE SINTACTICA] Functia " + idName + " asteapta "
          << std::to_string(function->paramTypes.size()) + " argumente, dar i s-au dat "
          << std::to_string(arguments.size()) << "\n";
        exit(1);
      }
      for (size_t i = 0; i < arguments.size(); i++) {
        Value argVal = arguments[i]->evaluate();
        if (function->paramTypes[i] != argVal.type) {
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
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,
  YYSYMBOL_YYerror = 1,
  YYSYMBOL_YYUNDEF = 2,
  YYSYMBOL_TOK_TYPE_INT = 3,
  YYSYMBOL_TOK_TYPE_FLOAT = 4,
  YYSYMBOL_TOK_TYPE_STRING = 5,
  YYSYMBOL_TOK_TYPE_BOOL = 6,
  YYSYMBOL_TOK_TYPE_VOID = 7,
  YYSYMBOL_TOK_ID = 8,
  YYSYMBOL_LIT_INT = 9,
  YYSYMBOL_LIT_FLOAT = 10,
  YYSYMBOL_LIT_STRING = 11,
  YYSYMBOL_TOK_TRUE = 12,
  YYSYMBOL_TOK_FALSE = 13,
  YYSYMBOL_TOK_CLASS = 14,
  YYSYMBOL_TOK_MAIN = 15,
  YYSYMBOL_TOK_IF = 16,
  YYSYMBOL_TOK_WHILE = 17,
  YYSYMBOL_TOK_PRINT = 18,
  YYSYMBOL_TOK_ASSIGN = 19,
  YYSYMBOL_TOK_EQ = 20,
  YYSYMBOL_TOK_NEQ = 21,
  YYSYMBOL_TOK_LEQ = 22,
  YYSYMBOL_TOK_GEQ = 23,
  YYSYMBOL_TOK_AND = 24,
  YYSYMBOL_TOK_OR = 25,
  YYSYMBOL_TOK_INC = 26,
  YYSYMBOL_TOK_DEC = 27,
  YYSYMBOL_TOK_PLUS_ASSIGN = 28,
  YYSYMBOL_TOK_MINUS_ASSIGN = 29,
  YYSYMBOL_TOK_MUL_ASSIGN = 30,
  YYSYMBOL_TOK_DIV_ASSIGN = 31,
  YYSYMBOL_32_ = 32,
  YYSYMBOL_33_ = 33,
  YYSYMBOL_34_ = 34,
  YYSYMBOL_35_ = 35,
  YYSYMBOL_36_ = 36,
  YYSYMBOL_37_ = 37,
  YYSYMBOL_UMINUS = 38,
  YYSYMBOL_39_ = 39,
  YYSYMBOL_40_ = 40,
  YYSYMBOL_41_ = 41,
  YYSYMBOL_42_ = 42,
  YYSYMBOL_43_ = 43,
  YYSYMBOL_44_ = 44,
  YYSYMBOL_45_ = 45,
  YYSYMBOL_YYACCEPT = 46,
  YYSYMBOL_program = 47,
  YYSYMBOL_48_1 = 48,
  YYSYMBOL_global_elements = 49,
  YYSYMBOL_global_decl = 50,
  YYSYMBOL_51_2 = 51,
  YYSYMBOL_global_decl_suffix = 52,
  YYSYMBOL_53_3 = 53,
  YYSYMBOL_finale_block = 54,
  YYSYMBOL_55_4 = 55,
  YYSYMBOL_stmt_list_pure = 56,
  YYSYMBOL_statement = 57,
  YYSYMBOL_assignment_stmt = 58,
  YYSYMBOL_print_stmt = 59,
  YYSYMBOL_expression = 60,
  YYSYMBOL_func_call = 61,
  YYSYMBOL_args_list = 62,
  YYSYMBOL_non_empty_args = 63,
  YYSYMBOL_control_stmt = 64,
  YYSYMBOL_if_stmt = 65,
  YYSYMBOL_while_stmt = 66,
  YYSYMBOL_block_pure = 67,
  YYSYMBOL_class_decl = 68,
  YYSYMBOL_class_body = 69,
  YYSYMBOL_class_member = 70,
  YYSYMBOL_class_member_suffix = 71,
  YYSYMBOL_param_list = 72,
  YYSYMBOL_non_empty_params = 73,
  YYSYMBOL_func_body = 74,
  YYSYMBOL_var_decl = 75,
  YYSYMBOL_var_decl_suffix = 76,
  YYSYMBOL_return_type = 77,
  YYSYMBOL_data_type = 78
};
typedef enum yysymbol_kind_t yysymbol_kind_t;
#ifdef short
# undef short
#endif
#ifndef __PTRDIFF_MAX__
# include <limits.h> 
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> 
#  define YY_STDINT_H
# endif
#endif
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
#   include <stddef.h> 
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
#  include <stddef.h> 
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
typedef yytype_uint8 yy_state_t;
typedef int yy_state_fast_t;
#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> 
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
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) 
#endif
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
# define YY_INITIAL_VALUE(Value) 
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
# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> 
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> 
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> 
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif
# ifdef YYSTACK_ALLOC
#  define YYSTACK_FREE(Ptr) do { ; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM 4032 
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
#   include <stdlib.h> 
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void* malloc(YYSIZE_T);
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free(void*);
#   endif
#  endif
# endif
#endif 
#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)
# define YYCOPY_NEEDED 1
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
#endif 
#define YYFINAL  3
#define YYLAST   424
#define YYNTOKENS  46
#define YYNNTS  33
#define YYNRULES  83
#define YYNSTATES  162
#define YYMAXUTOK   287
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)
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
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])
#if YYDEBUG || 0
static const char* yysymbol_name(yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;
static const char* const yytname[] =
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
static const char*
yysymbol_name(yysymbol_kind_t yysymbol)
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
static const yytype_int8 yypgoto[] =
{
     -29,   -29,   -29,   -29,   -29,   -29,   -29,   -29,   -29,   -29,
     -20,   -29,   -29,   -29,   -21,   -25,   -12,   -29,   -29,   -29,
     -29,    -8,   -29,   -29,   -29,   -29,    40,   -29,    -5,   -29,
      61,   117,    -3
};
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,     4,    13,    23,    29,    58,    14,    19,
     151,    39,    40,    41,   103,    57,   104,   105,    43,    44,
      45,   148,    15,    24,    32,    95,    90,    91,   152,    46,
      30,    16,    47
};
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
#define YYERRCODE YYUNDEF
#if YYDEBUG
# ifndef YYFPRINTF
#  include <stdio.h> 
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
static void
yy_symbol_value_print(FILE* yyo,
  yysymbol_kind_t yykind, YYSTYPE const* const yyvaluep)
{
  FILE* yyoutput = yyo;
  YY_USE(yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
    YY_USE(yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}
static void
yy_symbol_print(FILE* yyo,
  yysymbol_kind_t yykind, YYSTYPE const* const yyvaluep)
{
  YYFPRINTF(yyo, "%s %s (",
    yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name(yykind));
  yy_symbol_value_print(yyo, yykind, yyvaluep);
  YYFPRINTF(yyo, ")");
}
static void
yy_stack_print(yy_state_t* yybottom, yy_state_t* yytop)
{
  YYFPRINTF(stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
  {
    int yybot = *yybottom;
    YYFPRINTF(stderr, " %d", yybot);
  }
  YYFPRINTF(stderr, "\n");
}
# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)
static void
yy_reduce_print(yy_state_t* yyssp, YYSTYPE* yyvsp,
  int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF(stderr, "Reducing stack by rule %d (line %d):\n",
    yyrule - 1, yylno);
  for (yyi = 0; yyi < yynrhs; yyi++)
  {
    YYFPRINTF(stderr, "   $%d = ", yyi + 1);
    yy_symbol_print(stderr,
      YY_ACCESSING_SYMBOL(+yyssp[yyi + 1 - yynrhs]),
      &yyvsp[(yyi + 1) - (yynrhs)]);
    YYFPRINTF(stderr, "\n");
  }
}
# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)
int yydebug;
#else 
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif 
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif
#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif
static void
yydestruct(const char* yymsg,
  yysymbol_kind_t yykind, YYSTYPE* yyvaluep)
{
  YY_USE(yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT(yymsg, yykind, yyvaluep, yylocationp);
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
    YY_USE(yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}
int yychar;
YYSTYPE yylval;
int yynerrs;
int
yyparse(void)
{
  yy_state_fast_t yystate = 0;
  int yyerrstatus = 0;
  YYPTRDIFF_T yystacksize = YYINITDEPTH;
  yy_state_t yyssa[YYINITDEPTH];
  yy_state_t* yyss = yyssa;
  yy_state_t* yyssp = yyss;
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE* yyvs = yyvsa;
  YYSTYPE* yyvsp = yyvs;
  int yyn;
  int yyresult;
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  YYSTYPE yyval;
#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))
  int yylen = 0;
  YYDPRINTF((stderr, "Starting parse\n"));
  yychar = YYEMPTY;
  goto yysetstate;
yynewstate:
  yyssp++;
yysetstate:
  YYDPRINTF((stderr, "Entering state %d\n", yystate));
  YY_ASSERT(0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
    * yyssp = YY_CAST(yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
    YY_STACK_PRINT(yyss, yyssp);
  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
  {
    YYPTRDIFF_T yysize = yyssp - yyss + 1;
# if defined yyoverflow
    {
      yy_state_t* yyss1 = yyss;
      YYSTYPE* yyvs1 = yyvs;
      yyoverflow(YY_("memory exhausted"),
        &yyss1, yysize * YYSIZEOF(*yyssp),
        &yyvs1, yysize * YYSIZEOF(*yyvsp),
        &yystacksize);
      yyss = yyss1;
      yyvs = yyvs1;
    }
# else 
    if (YYMAXDEPTH <= yystacksize)
      YYNOMEM;
    yystacksize *= 2;
    if (YYMAXDEPTH < yystacksize)
      yystacksize = YYMAXDEPTH;
    {
      yy_state_t* yyss1 = yyss;
      union yyalloc* yyptr =
        YY_CAST(union yyalloc*,
          YYSTACK_ALLOC(YY_CAST(YYSIZE_T, YYSTACK_BYTES(yystacksize))));
      if (!yyptr)
        YYNOMEM;
      YYSTACK_RELOCATE(yyss_alloc, yyss);
      YYSTACK_RELOCATE(yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
      if (yyss1 != yyssa)
        YYSTACK_FREE(yyss1);
    }
# endif
    yyssp = yyss + yysize - 1;
    yyvsp = yyvs + yysize - 1;
    YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF((stderr, "Stack size increased to %ld\n",
        YY_CAST(long, yystacksize)));
    YY_IGNORE_USELESS_CAST_END
      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
  }
#endif 
  if (yystate == YYFINAL)
    YYACCEPT;
  goto yybackup;
yybackup:
  yyn = yypact[yystate];
  if (yypact_value_is_default(yyn))
    goto yydefault;
  if (yychar == YYEMPTY)
  {
    YYDPRINTF((stderr, "Reading a token\n"));
    yychar = yylex();
  }
  if (yychar <= YYEOF)
  {
    yychar = YYEOF;
    yytoken = YYSYMBOL_YYEOF;
    YYDPRINTF((stderr, "Now at end of input.\n"));
  }
  else if (yychar == YYerror)
  {
    yychar = YYUNDEF;
    yytoken = YYSYMBOL_YYerror;
    goto yyerrlab1;
  }
  else
  {
    yytoken = YYTRANSLATE(yychar);
    YY_SYMBOL_PRINT("Next token is", yytoken, &yylval, &yylloc);
  }
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
  {
    if (yytable_value_is_error(yyn))
      goto yyerrlab;
    yyn = -yyn;
    goto yyreduce;
  }
  if (yyerrstatus)
    yyerrstatus--;
  YY_SYMBOL_PRINT("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
    * ++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
    yychar = YYEMPTY;
  goto yynewstate;
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;
yyreduce:
  yylen = yyr2[yyn];
  yyval = yyvsp[1 - yylen];
  YY_REDUCE_PRINT(yyn);
  switch (yyn)
  {
  case 2:
#line 301 "maestro.y"
  { globalScope = new SymbolTable("Global Scope"); currentScope = globalScope; }
#line 1572 "maestro.tab.c"
  break;
  case 3:
#line 302 "maestro.y"
  { currentScope->printTable(tableFile); std::cout << "Sintassi Corretta!\n"; }
#line 1578 "maestro.tab.c"
  break;
  case 7:
#line 307 "maestro.y"
  { currentType = (yyvsp[-1].stringValue); currentName = (yyvsp[0].stringValue); }
#line 1584 "maestro.tab.c"
  break;
  case 9:
#line 309 "maestro.y"
  {
    currentScope->addSymbol(currentName, currentType, "function");
    currentScope = new SymbolTable("Function: " + currentName, currentScope);
  }
#line 1593 "maestro.tab.c"
  break;
  case 10:
#line 312 "maestro.y"
  { currentScope->printTable(tableFile); currentScope = currentScope->parent; }
#line 1599 "maestro.tab.c"
  break;
  case 11:
#line 313 "maestro.y"
  { currentScope->addSymbol(currentName, currentType, "global_var"); }
#line 1605 "maestro.tab.c"
  break;
  case 12:
#line 316 "maestro.y"
  { currentScope = new SymbolTable("Main Function", currentScope); }
#line 1611 "maestro.tab.c"
  break;
  case 13:
#line 317 "maestro.y"
  {
    for (ASTNode* node : *(yyvsp[-1].astList)) if (node) node->evaluate();
    currentScope->printTable(tableFile); currentScope = currentScope->parent;
  }
#line 1620 "maestro.tab.c"
  break;
  case 14:
#line 323 "maestro.y"
  { (yyvsp[-1].astList)->push_back((yyvsp[0].astNode)); (yyval.astList) = (yyvsp[-1].astList); }
#line 1626 "maestro.tab.c"
  break;
  case 15:
#line 323 "maestro.y"
  { (yyval.astList) = new std::vector<ASTNode*>(); }
#line 1632 "maestro.tab.c"
  break;
  case 16:
#line 326 "maestro.y"
  { (yyval.astNode) = (yyvsp[0].astNode); }
#line 1638 "maestro.tab.c"
  break;
  case 17:
#line 327 "maestro.y"
  { (yyval.astNode) = (yyvsp[0].astNode); }
#line 1644 "maestro.tab.c"
  break;
  case 18:
#line 328 "maestro.y"
  { (yyval.astNode) = nullptr; }
#line 1650 "maestro.tab.c"
  break;
  case 19:
#line 329 "maestro.y"
  { (yyval.astNode) = nullptr; }
#line 1656 "maestro.tab.c"
  break;
  case 20:
#line 330 "maestro.y"
  { (yyval.astNode) = nullptr; }
#line 1662 "maestro.tab.c"
  break;
  case 21:
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
  case 22:
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
  case 23:
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
  case 24:
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
  case 25:
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
  case 26:
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
  case 27:
#line 395 "maestro.y"
  {
    (yyval.astNode) = new ASTNode(NODE_ASSIGN); (yyval.astNode)->idName = (yyvsp[-3].stringValue); (yyval.astNode)->right = (yyvsp[-1].astNode);
  }
#line 1756 "maestro.tab.c"
  break;
  case 28:
#line 398 "maestro.y"
  {
    (yyval.astNode) = new ASTNode(NODE_ASSIGN);
    (yyval.astNode)->idName = std::string((yyvsp[-5].stringValue)) + "." + std::string((yyvsp[-3].stringValue));
    (yyval.astNode)->right = (yyvsp[-1].astNode);
  }
#line 1766 "maestro.tab.c"
  break;
  case 29:
#line 405 "maestro.y"
  {
    (yyval.astNode) = new ASTNode(NODE_PRINT); (yyval.astNode)->left = (yyvsp[-2].astNode);
  }
#line 1774 "maestro.tab.c"
  break;
  case 30:
#line 410 "maestro.y"
  { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = "+"; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1780 "maestro.tab.c"
  break;
  case 31:
#line 411 "maestro.y"
  { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = "-"; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1786 "maestro.tab.c"
  break;
  case 32:
#line 412 "maestro.y"
  { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = "*"; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1792 "maestro.tab.c"
  break;
  case 33:
#line 413 "maestro.y"
  { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = "/"; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1798 "maestro.tab.c"
  break;
  case 34:
#line 415 "maestro.y"
  {
    (yyval.astNode) = new ASTNode(NODE_OP);
    (yyval.astNode)->op = "UMINUS";
    (yyval.astNode)->left = (yyvsp[0].astNode);
    (yyval.astNode)->right = nullptr;
  }
#line 1809 "maestro.tab.c"
  break;
  case 35:
#line 421 "maestro.y"
  {
    (yyval.astNode) = new ASTNode(NODE_ID);
    (yyval.astNode)->idName = std::string((yyvsp[-2].stringValue)) + "." + std::string((yyvsp[0].stringValue));
  }
#line 1818 "maestro.tab.c"
  break;
  case 36:
#line 426 "maestro.y"
  { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = "=="; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1824 "maestro.tab.c"
  break;
  case 37:
#line 427 "maestro.y"
  { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = "!="; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1830 "maestro.tab.c"
  break;
  case 38:
#line 428 "maestro.y"
  { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = "<"; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1836 "maestro.tab.c"
  break;
  case 39:
#line 429 "maestro.y"
  { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = ">"; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1842 "maestro.tab.c"
  break;
  case 40:
#line 430 "maestro.y"
  { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = "<="; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1848 "maestro.tab.c"
  break;
  case 41:
#line 431 "maestro.y"
  { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = ">="; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1854 "maestro.tab.c"
  break;
  case 42:
#line 433 "maestro.y"
  { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = "&&"; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1860 "maestro.tab.c"
  break;
  case 43:
#line 434 "maestro.y"
  { (yyval.astNode) = new ASTNode(NODE_OP); (yyval.astNode)->op = "||"; (yyval.astNode)->left = (yyvsp[-2].astNode); (yyval.astNode)->right = (yyvsp[0].astNode); }
#line 1866 "maestro.tab.c"
  break;
  case 44:
#line 436 "maestro.y"
  { (yyval.astNode) = new ASTNode(NODE_LITERAL); (yyval.astNode)->literalValue.type = "basso"; (yyval.astNode)->literalValue.iVal = atoi((yyvsp[0].stringValue)); }
#line 1872 "maestro.tab.c"
  break;
  case 45:
#line 437 "maestro.y"
  { (yyval.astNode) = new ASTNode(NODE_LITERAL); (yyval.astNode)->literalValue.type = "soprano"; (yyval.astNode)->literalValue.fVal = atof((yyvsp[0].stringValue)); }
#line 1878 "maestro.tab.c"
  break;
  case 46:
#line 438 "maestro.y"
  { (yyval.astNode) = new ASTNode(NODE_LITERAL); (yyval.astNode)->literalValue.type = "libretto"; (yyval.astNode)->literalValue.sVal = (yyvsp[0].stringValue); }
#line 1884 "maestro.tab.c"
  break;
  case 47:
#line 439 "maestro.y"
  { (yyval.astNode) = new ASTNode(NODE_LITERAL); (yyval.astNode)->literalValue.type = "verita"; (yyval.astNode)->literalValue.bVal = true; }
#line 1890 "maestro.tab.c"
  break;
  case 48:
#line 440 "maestro.y"
  { (yyval.astNode) = new ASTNode(NODE_LITERAL); (yyval.astNode)->literalValue.type = "verita"; (yyval.astNode)->literalValue.bVal = false; }
#line 1896 "maestro.tab.c"
  break;
  case 49:
#line 441 "maestro.y"
  { (yyval.astNode) = new ASTNode(NODE_ID); (yyval.astNode)->idName = (yyvsp[0].stringValue); }
#line 1902 "maestro.tab.c"
  break;
  case 50:
#line 442 "maestro.y"
  { (yyval.astNode) = new ASTNode(NODE_OTHER); (yyval.astNode)->exprType = "unknown"; }
#line 1908 "maestro.tab.c"
  break;
  case 51:
#line 443 "maestro.y"
  { (yyval.astNode) = (yyvsp[-1].astNode); }
#line 1914 "maestro.tab.c"
  break;
  case 52:
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
  case 53:
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
  case 54:
#line 468 "maestro.y"
  { (yyval.astList) = (yyvsp[0].astList); }
#line 1948 "maestro.tab.c"
  break;
  case 55:
#line 469 "maestro.y"
  { (yyval.astList) = new std::vector<ASTNode*>(); }
#line 1954 "maestro.tab.c"
  break;
  case 56:
#line 471 "maestro.y"
  { (yyvsp[-2].astList)->push_back((yyvsp[0].astNode)); (yyval.astList) = (yyvsp[-2].astList); }
#line 1960 "maestro.tab.c"
  break;
  case 57:
#line 472 "maestro.y"
  { (yyval.astList) = new std::vector<ASTNode*>(); (yyval.astList)->push_back((yyvsp[0].astNode)); }
#line 1966 "maestro.tab.c"
  break;
  case 60:
#line 476 "maestro.y"
  { (yyval.astNode) = nullptr; }
#line 1972 "maestro.tab.c"
  break;
  case 61:
#line 478 "maestro.y"
  { (yyval.astNode) = nullptr; }
#line 1978 "maestro.tab.c"
  break;
  case 62:
#line 480 "maestro.y"
  { (yyval.astList) = (yyvsp[-1].astList); }
#line 1984 "maestro.tab.c"
  break;
  case 71:
#line 493 "maestro.y"
  {
    currentScope->addSymbol((yyvsp[0].stringValue), (yyvsp[-1].stringValue), "parameter");
    currentScope->addParamToLastFunction((yyvsp[-1].stringValue));
  }
#line 1993 "maestro.tab.c"
  break;
  case 72:
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
  YY_SYMBOL_PRINT("-> $$ =", YY_CAST(yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);
  YYPOPSTACK(yylen);
  yylen = 0;
  *++yyvsp = yyval;
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
      ? yytable[yyi]
      : yydefgoto[yylhs]);
  }
  goto yynewstate;
yyerrlab:
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE(yychar);
  if (!yyerrstatus)
  {
    ++yynerrs;
    yyerror(YY_("syntax error"));
  }
  if (yyerrstatus == 3)
  {
    if (yychar <= YYEOF)
    {
      if (yychar == YYEOF)
        YYABORT;
    }
    else
    {
      yydestruct("Error: discarding",
        yytoken, &yylval);
      yychar = YYEMPTY;
    }
  }
  goto yyerrlab1;
yyerrorlab:
  if (0)
    YYERROR;
  ++yynerrs;
  YYPOPSTACK(yylen);
  yylen = 0;
  YY_STACK_PRINT(yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;
yyerrlab1:
  yyerrstatus = 3;
  for (;;)
  {
    yyn = yypact[yystate];
    if (!yypact_value_is_default(yyn))
    {
      yyn += YYSYMBOL_YYerror;
      if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
      {
        yyn = yytable[yyn];
        if (0 < yyn)
          break;
      }
    }
    if (yyssp == yyss)
      YYABORT;
    yydestruct("Error: popping",
      YY_ACCESSING_SYMBOL(yystate), yyvsp);
    YYPOPSTACK(1);
    yystate = *yyssp;
    YY_STACK_PRINT(yyss, yyssp);
  }
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
    * ++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
    YY_SYMBOL_PRINT("Shifting", YY_ACCESSING_SYMBOL(yyn), yyvsp, yylsp);
  yystate = yyn;
  goto yynewstate;
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;
yyexhaustedlab:
  yyerror(YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;
yyreturnlab:
  if (yychar != YYEMPTY)
  {
    yytoken = YYTRANSLATE(yychar);
    yydestruct("Cleanup: discarding lookahead",
      yytoken, &yylval);
  }
  YYPOPSTACK(yylen);
  YY_STACK_PRINT(yyss, yyssp);
  while (yyssp != yyss)
  {
    yydestruct("Cleanup: popping",
      YY_ACCESSING_SYMBOL(+*yyssp), yyvsp);
    YYPOPSTACK(1);
  }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE(yyss);
#endif
  return yyresult;
}
#line 512 "maestro.y"
void yyerror(const char* s) { fprintf(stderr, "Error line %d: %s\n", yylineno, s); exit(1); }
int main() { yyparse(); return 0; }