//asta o sa fie parserul, cu regulile de gramatica

%{
#include <iostream> 
#include <vector> 
#include <string> 
#include <map> 
#include <fstream> 
#include <cstdio> 
#include <cstdlib> 

extern int yylineno;
int yylex(void);
void yyerror(const char *s);

class Symbol { 
    public: 
        std::string name, type, kind; 
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

        SymbolTable(std::string _scope, SymbolTable* _parent = nullptr) { 
            scope = _scope; 
            parent = _parent; 
        }

        bool addSymbol(std::string _name, std::string _type, std::string _kind) { 
            if(symbols.find(_name) != symbols.end()) return false; 

            Symbol currentSymbol = Symbol(_name, _type, _kind); 
            symbols.insert({ _name, currentSymbol});

            return true; 
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
%}

%union { 
    char* strVal; 
}

//token definition
%token <strVal> TOK_TYPE_INT TOK_TYPE_FLOAT TOK_TYPE_STRING TOK_TYPE_BOOL TOK_TYPE_VOID
%token <strVal> TOK_ID 
%token TOK_CLASS TOK_MAIN TOK_IF TOK_WHILE TOK_PRINT
%token TOK_TRUE TOK_FALSE
%token LIT_INT LIT_FLOAT LIT_STRING
%token TOK_ASSIGN TOK_EQ TOK_NEQ TOK_LEQ TOK_GEQ TOK_AND TOK_OR
%token TOK_INC TOK_DEC TOK_PLUS_ASSIGN

%type <strVal> data_type simple_type return_type

//operator priority
%left TOK_OR
%left TOK_AND
%left TOK_EQ TOK_NEQ
%left '<' '>' TOK_LEQ TOK_GEQ
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%%
//structura programului

program:
    { 
        globalScope = new SymbolTable("Global Scope"); 
        currentScope = globalScope; 
    }
    global_elements finale_block 
    { 
        currentScope->printTable(tableFile); 
        std::cout << "Sintassi Corretta!\n"; 
    }
    ;

global_elements:
    global_elements global_decl |
    ;

global_decl:
    class_decl| return_type TOK_ID { 
        currentType = $1; 
        currentName = $2; 
    }
    global_decl_suffix
    ;

global_decl_suffix:
    '(' { 
        // Incepe o functie

        // Adaugam numele functiei in Global Scope
        if(!currentScope->addSymbol(currentName, currentType, "function")) { 
            yyerror(("Symbol" + currentName + "already defined.").c_str()); 
        }

        // Cream scope ul functiei
        SymbolTable* functionScope = new SymbolTable("Function: " + currentName, currentScope); 
        currentScope = functionScope; 
    } ')' '{' func_body '}' { 
        // Finalul functiei - printam tabela si revenim la parinte 
        currentScope->printTable(tableFile); 
        currentScope = currentScope->parent; 
    } | var_decl_suffix { 
        // Daca ajungem aici inseamna da declaratia este o variabila globala (o adaugam in scope)
        if(!currentScope->addSymbol(currentName, currentType, "global_var")) { 
            yyerror(("Symbol" + currentName + "already defined.").c_str()); 
        }
    }                      
    ;

//data types si clasele

data_type:
    TOK_TYPE_INT { $$ = $1; } |
    TOK_TYPE_FLOAT { $$ = $1; } |
    TOK_TYPE_STRING { $$ = $1; } |
    TOK_TYPE_BOOL { $$ = $1; } |
    TOK_ID { $$ = $1; }
    ;

simple_type:
    TOK_TYPE_INT { $$ = $1; } |
    TOK_TYPE_FLOAT { $$ = $1; } |
    TOK_TYPE_STRING { $$ = $1; } |
    TOK_TYPE_BOOL { $$ = $1; } 
    ;

class_decl:
    TOK_CLASS TOK_ID { 
        std::string className = $2; 
        currentScope->addSymbol(className, "class", "class_def"); 

        // Cream un nou scope pentru clasa
        SymbolTable* classScope = new SymbolTable("Class: " + className, currentScope); 
        currentScope = classScope; 
    }
    '{' class_body '}' { 
        currentScope->printTable(tableFile); 
        currentScope = currentScope->parent; 
    }
    ;

class_body:
    class_body class_member |   
    ;

class_member:
    return_type TOK_ID {
        currentType = $1; 
        currentName = $2; 
    }
    class_member_suffix
    ;

return_type:
    data_type { $$ = $1; } |
    TOK_TYPE_VOID { $$ = $1; }
    ;

class_member_suffix:
    ';' { 
        currentScope->addSymbol(currentName, currentType, "field"); 
    } | 
    '(' { 
        // Metoda a clasei
        currentScope->addSymbol(currentName, currentType, "method"); 

        // Cream scope ul metodei
        SymbolTable* methodScope = new SymbolTable("Method: " + currentName, currentScope); 
        currentScope = methodScope; 
    }
    param_list ')' '{' func_body '}' { 
        currentScope->printTable(tableFile); 
        currentScope = currentScope->parent; 
    }
    ;

//functi

param_list:
    non_empty_params |
    ;

non_empty_params:
    non_empty_params ',' data_type TOK_ID { 
        currentScope->addSymbol($4, $3, "parameter"); 
    } | 
    data_type TOK_ID { 
        currentScope->addSymbol($2, $1, "parameter"); 
    }
    ; 

func_body:
    var_decl_list stmt_list_pure
    ;

var_decl_list:
    var_decl_list var_decl|
    ;

var_decl:
    simple_type TOK_ID var_decl_suffix { 
        if(!currentScope->addSymbol($2, $1, "local_var")) { 
            yyerror(("Variable" + std::string($2) + "redeclared!").c_str()); 
        }
    }
    ;


var_decl_suffix:
    ';'| TOK_ASSIGN expression ';'| TOK_PLUS_ASSIGN expression ';'
    ;

//blocul main (ala cu grand finale )

finale_block:
    TOK_MAIN { 
        SymbolTable* mainScope = new SymbolTable("Main Function", currentScope);
        currentScope = mainScope; 
    }
    '{' stmt_list_pure '}' { 
        currentScope->printTable(tableFile); 
        currentScope = currentScope->parent; 
    }
    ;

//statements

stmt_list_pure:
    stmt_list_pure statement| 
    ;

statement:
    assignment_stmt
    | control_stmt
    | print_stmt
    | func_call ';'
    | unary_stmt ';'   
    ;

assignment_stmt:
    lvalue TOK_ASSIGN expression ';'
    | lvalue TOK_PLUS_ASSIGN expression ';'
    ;

unary_stmt:
    lvalue TOK_INC        
    | lvalue TOK_DEC      
    ;

print_stmt:
    TOK_PRINT '(' expression ')' ';'
    ;

control_stmt:
    if_stmt| while_stmt
    ;

if_stmt:
    TOK_IF '(' expression ')' block_pure
    ;

while_stmt:
    TOK_WHILE '(' expression ')' block_pure
    ;

block_pure:
    '{' stmt_list_pure '}'
    ;

//expression

expression:
    expression '+' expression
    | expression '-' expression
    | expression '*' expression
    | expression '/' expression
    | expression '<' expression
    | expression '>' expression
    | expression TOK_LEQ expression
    | expression TOK_GEQ expression
    | expression TOK_EQ expression
    | expression TOK_NEQ expression
    | expression TOK_AND expression
    | expression TOK_OR expression
    | '(' expression ')'
    | '-' expression %prec UMINUS
    | func_call
    | lvalue
    | literal
    ;

lvalue:
    TOK_ID| TOK_ID '.' TOK_ID
    ;

func_call:
    TOK_ID '(' args_list ')'| TOK_ID '.' TOK_ID '(' args_list ')'
    ;

args_list:
    non_empty_args |
    ;

non_empty_args:
    non_empty_args ',' expression| expression
    ;

literal:
    LIT_INT | LIT_FLOAT | LIT_STRING | TOK_TRUE | TOK_FALSE
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Errore di Sintassi line %d: %s\n", yylineno, s);
    exit(1);
}

int main() {
    yyparse();
    return 0;
}