//asta o sa fie parserul, cu regulile de gramatica

%{
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

class Symbol { 
    public: 
        std::string name, type, kind; 
        std::vector <std::string> paramTypes; 
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
%}

%code requires { 
    #include <vector> 
    #include <string> 
}

%union { 
    char* stringValue; 
    std::vector<std::string>* vectorValue; 
}

//token definition
%token <stringValue> TOK_TYPE_INT TOK_TYPE_FLOAT TOK_TYPE_STRING TOK_TYPE_BOOL TOK_TYPE_VOID
%token <stringValue> TOK_ID 
%token TOK_CLASS TOK_MAIN TOK_IF TOK_WHILE TOK_PRINT
%token TOK_TRUE TOK_FALSE
%token LIT_INT LIT_FLOAT LIT_STRING
%token TOK_ASSIGN TOK_EQ TOK_NEQ TOK_LEQ TOK_GEQ TOK_AND TOK_OR
%token TOK_INC TOK_DEC TOK_PLUS_ASSIGN

%type <stringValue> expression lvalue literal func_call
%type <stringValue> data_type simple_type return_type
%type <vectorValue> args_list non_empty_args

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
        currentScope->addParamToLastFunction($3); 
    } | 
    data_type TOK_ID { 
        currentScope->addSymbol($2, $1, "parameter"); 
        currentScope->addParamToLastFunction($1); 
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
    lvalue TOK_ASSIGN expression ';' { 
        // $1 = numele variabilei 
        // $3 = tipul expresiei

        // Verificam daca a fost declarata variabila 
        Symbol* symbol = currentScope->findSymbol($1);
        if(symbol == nullptr) { 
            yyerror(("[EROARE SEMANTICA] Variabila " + std::string($1) + " nu este definita!").c_str());
            exit(1); 
        }

        // Verificam daca tipurile coincid 
        if(strcmp(symbol->type.c_str(), $3) != 0) { 
            yyerror(("[EROARE SEMANTICA] Nu poti atribui un " + std::string($3) + " la o variabila de tip" + symbol->type).c_str());
            exit(1); 
        }
    } |
    lvalue TOK_PLUS_ASSIGN expression ';' { 
        // $1 = numele variabilei 
        // $3 = tipul expresiei

        // Verificam daca a fost declarata variabila 
        Symbol* symbol = currentScope->findSymbol($1);
        if(symbol == nullptr) { 
            yyerror(("[EROARE SEMANTICA] Variabila " + std::string($1) + " nu este definita!").c_str());
            exit(1); 
        }

        // Verificam daca tipurile coincid 
        if(strcmp(symbol->type.c_str(), $3) != 0) { 
            yyerror(("[EROARE SEMANTICA] Nu poti atribui un " + std::string($3) + " la o variabila de tip" + symbol->type).c_str());
            exit(1); 
        }
    }
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
    expression '+' expression { 
        if(strcmp($1, $3) != 0) { 
            yyerror(("[EROARE SEMANTICA] Nu poti aduna " + std::string($1) + " cu " + std::string($3)).c_str());
            exit(1); 
        }

        $$ = $1; 
    }
    | expression '-' expression { 
        if(strcmp($1, $3) != 0) { 
            yyerror(("[EROARE SEMANTICA] Nu poti face scadere intre un " + std::string($1) + " si un " + std::string($3)).c_str());
            exit(1); 
        }

        $$ = $1; 
    }
    | expression '*' expression { 
        if(strcmp($1, $3) != 0) { 
            yyerror(("[EROARE SEMANTICA] Nu poti inmulti " + std::string($1) + " cu " + std::string($3)).c_str());
            exit(1); 
        }

        $$ = $1; 
    }
    | expression '/' expression { 
        if(strcmp($1, $3) != 0) { 
            yyerror(("[EROARE SEMANTICA] Nu poti imparti " + std::string($1) + " cu " + std::string($3)).c_str());
            exit(1); 
        }

        $$ = $1; 
    }
    | expression '<' expression { $$ = strdup("verita"); }
    | expression '>' expression { $$ = strdup("verita"); }
    | expression TOK_LEQ expression { $$ = strdup("verita"); }
    | expression TOK_GEQ expression { $$ = strdup("verita"); }
    | expression TOK_EQ expression { $$ = strdup("verita"); }
    | expression TOK_NEQ expression { $$ = strdup("verita"); }
    | expression TOK_AND expression { $$ = strdup("verita"); }
    | expression TOK_OR expression { $$ = strdup("verita"); }
    | '(' expression ')' { $$ = $2; }
    | '-' expression %prec UMINUS { $$ = $2; }
    | func_call { $$ = strdup("unknown"); } // TODO
    | lvalue { 
        Symbol* symbol = currentScope->findSymbol($1);
        if(symbol == nullptr) { 
            yyerror(("[EROARE SEMANTICA] Variabila: " + std::string($1) + " nu este definita!").c_str());
            exit(1); 
        }

        $$ = strdup(symbol->type.c_str()); 
    } | 
    literal { 
        $$ = $1; 
    }
    ;

lvalue:
    TOK_ID { 
        $$ = $1; 
    } | 
    TOK_ID '.' TOK_ID { 
        $$ = $3; 
    }
    ;

func_call:
    TOK_ID '(' args_list ')' { 
        Symbol* function = currentScope->findSymbol($1); 
        if(!function) { 
            yyerror(("[EROARE SEMANTICA] Functia " + std::string($1) + " nu este definita!").c_str()); 
            exit(1); 
        }

        // Verifica numarul de parametri
        std::vector<std::string>* args = $3; 
        if(function->paramTypes.size() != args->size()) { 
            yyerror(("[EROARE SINTACTICA] Functia " + std::string($1) + " asteapta " + std::to_string(function->paramTypes.size()) + " argumente, dar i s-au dat " + std::to_string(args->size())).c_str());
            exit(1); 
        }

        // Verifica tipul fiecarui parametru
        for(size_t i = 0; i < args->size(); i++) { 
            if(function->paramTypes[i] != (*args)[i]) { 
                yyerror(("[EROARE SINTACTICA] La functia " + std::string($1) + " , argumentul " + std::to_string(i + 1) + " trebuie sa fie " + function->paramTypes[i] + ", dar a primti" + (*args)[i]).c_str());
                exit(1);  
            }
        }

        $$ = strdup(function->type.c_str()); 
        delete args; 
    } | TOK_ID '.' TOK_ID '(' args_list ')' { 
        $$ = strdup("unknown");  // TODO 
    }
    ;

args_list:
    non_empty_args { $$ = $1; } |
        { $$ = new std::vector<std::string>(); } 
    ;

non_empty_args:
    non_empty_args ',' expression { 
        $1->push_back($3); 
        $$ = $1; 
    } | 
    expression { 
        $$ = new std::vector<std::string> (); 
        $$->push_back($1); 
    }
    ; 

literal:
    LIT_INT { $$ = strdup("basso"); } |
    LIT_FLOAT { $$ = strdup("soprano"); } | 
    LIT_STRING { $$ = strdup("libretto"); } | 
    TOK_TRUE { $$ = strdup("verita"); } | 
    TOK_FALSE { $$ = strdup("verita"); }
%%

void yyerror(const char *s) {
    fprintf(stderr, "Errore di Sintassi line %d: %s\n", yylineno, s);
    exit(1);
}

int main() {
    yyparse();
    return 0;
}