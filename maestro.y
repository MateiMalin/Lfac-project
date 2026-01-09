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
%}

%code requires { 
    #include <vector> 
    #include <string> 
}

%union { 
    char* stringValue; 
    class ASTNode* astNode;
    std::vector<ASTNode*>* astList;
}

%token <stringValue> TOK_TYPE_INT TOK_TYPE_FLOAT TOK_TYPE_STRING TOK_TYPE_BOOL TOK_TYPE_VOID TOK_ID
%token <stringValue> LIT_INT LIT_FLOAT LIT_STRING TOK_TRUE TOK_FALSE
%token TOK_CLASS TOK_MAIN TOK_IF TOK_WHILE TOK_PRINT TOK_ASSIGN TOK_EQ TOK_NEQ TOK_LEQ TOK_GEQ TOK_AND TOK_OR TOK_INC TOK_DEC TOK_PLUS_ASSIGN TOK_MINUS_ASSIGN TOK_MUL_ASSIGN TOK_DIV_ASSIGN

%type <astNode> expression assignment_stmt print_stmt statement  func_call control_stmt if_stmt while_stmt
%type <astList> stmt_list_pure args_list non_empty_args block_pure
%type <stringValue> data_type return_type

%left TOK_OR
%left TOK_AND
%left TOK_EQ TOK_NEQ
%left '<' '>' TOK_LEQ TOK_GEQ
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%%
program: { globalScope = new SymbolTable("Global Scope"); currentScope = globalScope; }
    global_elements finale_block { currentScope->printTable(tableFile); std::cout << "Sintassi Corretta!\n"; }
    ;

global_elements: global_elements global_decl | ;

global_decl: class_decl | return_type TOK_ID { currentType = $1; currentName = $2; } global_decl_suffix ;

global_decl_suffix: '(' { 
        currentScope->addSymbol(currentName, currentType, "function");
        currentScope = new SymbolTable("Function: " + currentName, currentScope); 
    } param_list ')' '{' func_body '}' { currentScope->printTable(tableFile); currentScope = currentScope->parent; }
    | var_decl_suffix { currentScope->addSymbol(currentName, currentType, "global_var"); }
    ;

finale_block: TOK_MAIN { currentScope = new SymbolTable("Main Function", currentScope); }
    '{' stmt_list_pure '}' { 
        for (ASTNode* node : *$4) if (node) node->evaluate(); 
        currentScope->printTable(tableFile); currentScope = currentScope->parent; 
    }
    ;

stmt_list_pure: stmt_list_pure statement { $1->push_back($2); $$ = $1; } | { $$ = new std::vector<ASTNode*>(); } ;

statement: 
      assignment_stmt { $$ = $1; } 
    | print_stmt { $$ = $1; } 
    | func_call ';' { $$ = nullptr; } 
    | control_stmt { $$ = nullptr; }
    | var_decl { $$ = nullptr; }
    | TOK_ID TOK_INC ';' { 
        $$ = new ASTNode(NODE_ASSIGN); $$->idName = $1; 
        
        ASTNode* opNode = new ASTNode(NODE_OP); opNode->op = "+";
        ASTNode* idNode = new ASTNode(NODE_ID); idNode->idName = $1;
        ASTNode* litNode = new ASTNode(NODE_LITERAL); litNode->literalValue.type = "basso"; litNode->literalValue.iVal = 1;

        opNode->left = idNode; opNode->right = litNode;
        $$->right = opNode;
    }
    
    | TOK_ID TOK_DEC ';' { 
        $$ = new ASTNode(NODE_ASSIGN); $$->idName = $1; 
        
        ASTNode* opNode = new ASTNode(NODE_OP); opNode->op = "-";
        ASTNode* idNode = new ASTNode(NODE_ID); idNode->idName = $1;
        ASTNode* litNode = new ASTNode(NODE_LITERAL); litNode->literalValue.type = "basso"; litNode->literalValue.iVal = 1;

        opNode->left = idNode; opNode->right = litNode;
        $$->right = opNode;
    }

    | TOK_ID TOK_PLUS_ASSIGN expression ';' {
        $$ = new ASTNode(NODE_ASSIGN); $$->idName = $1;

        ASTNode* opNode = new ASTNode(NODE_OP); opNode->op = "+";
        ASTNode* idNode = new ASTNode(NODE_ID); idNode->idName = $1;

        opNode->left = idNode; opNode->right = $3; 
        $$->right = opNode;
    }

    | TOK_ID TOK_MINUS_ASSIGN expression ';' {
        $$ = new ASTNode(NODE_ASSIGN); $$->idName = $1;

        ASTNode* opNode = new ASTNode(NODE_OP); opNode->op = "-";
        ASTNode* idNode = new ASTNode(NODE_ID); idNode->idName = $1;

        opNode->left = idNode; opNode->right = $3; 
        $$->right = opNode;
    }

    | TOK_ID TOK_MUL_ASSIGN expression ';' {
        $$ = new ASTNode(NODE_ASSIGN); $$->idName = $1;

        ASTNode* opNode = new ASTNode(NODE_OP); opNode->op = "*";
        ASTNode* idNode = new ASTNode(NODE_ID); idNode->idName = $1;

        opNode->left = idNode; opNode->right = $3; 
        $$->right = opNode;
    }

    | TOK_ID TOK_DIV_ASSIGN expression ';' {
        $$ = new ASTNode(NODE_ASSIGN); $$->idName = $1;

        ASTNode* opNode = new ASTNode(NODE_OP); opNode->op = "/";
        ASTNode* idNode = new ASTNode(NODE_ID); idNode->idName = $1;

        opNode->left = idNode; opNode->right = $3; 
        $$->right = opNode;
    }
    ;

assignment_stmt: 
    TOK_ID TOK_ASSIGN expression ';' { 
        $$ = new ASTNode(NODE_ASSIGN); $$->idName = $1; $$->right = $3; 
    } 
    | TOK_ID '.' TOK_ID TOK_ASSIGN expression ';' {
        $$ = new ASTNode(NODE_ASSIGN);
        $$->idName = std::string($1) + "." + std::string($3);
        $$->right = $5;
    }
    ;

print_stmt: TOK_PRINT '(' expression ')' ';' { 
        $$ = new ASTNode(NODE_PRINT); $$->left = $3;
    } ;

expression: 
    expression '+' expression { $$ = new ASTNode(NODE_OP); $$->op = "+"; $$->left = $1; $$->right = $3; }
    | expression '-' expression { $$ = new ASTNode(NODE_OP); $$->op = "-"; $$->left = $1; $$->right = $3; }
    | expression '*' expression { $$ = new ASTNode(NODE_OP); $$->op = "*"; $$->left = $1; $$->right = $3; }
    | expression '/' expression { $$ = new ASTNode(NODE_OP); $$->op = "/"; $$->left = $1; $$->right = $3; }

    | '-' expression %prec UMINUS { 
            $$ = new ASTNode(NODE_OP); 
            $$->op = "UMINUS"; 
            $$->left = $2; 
            $$->right = nullptr; 
        }
    | TOK_ID '.' TOK_ID { 
        $$ = new ASTNode(NODE_ID);
        $$->idName = std::string($1) + "." + std::string($3);
    }
    
    | expression TOK_EQ expression { $$ = new ASTNode(NODE_OP); $$->op = "=="; $$->left = $1; $$->right = $3; }
    | expression TOK_NEQ expression { $$ = new ASTNode(NODE_OP); $$->op = "!="; $$->left = $1; $$->right = $3; }
    | expression '<' expression { $$ = new ASTNode(NODE_OP); $$->op = "<"; $$->left = $1; $$->right = $3; }
    | expression '>' expression { $$ = new ASTNode(NODE_OP); $$->op = ">"; $$->left = $1; $$->right = $3; }
    | expression TOK_LEQ expression { $$ = new ASTNode(NODE_OP); $$->op = "<="; $$->left = $1; $$->right = $3; }
    | expression TOK_GEQ expression { $$ = new ASTNode(NODE_OP); $$->op = ">="; $$->left = $1; $$->right = $3; }
    
    | expression TOK_AND expression { $$ = new ASTNode(NODE_OP); $$->op = "&&"; $$->left = $1; $$->right = $3; }
    | expression TOK_OR expression { $$ = new ASTNode(NODE_OP); $$->op = "||"; $$->left = $1; $$->right = $3; }

    | LIT_INT { $$ = new ASTNode(NODE_LITERAL); $$->literalValue.type = "basso"; $$->literalValue.iVal = atoi($1); }
    | LIT_FLOAT { $$ = new ASTNode(NODE_LITERAL); $$->literalValue.type = "soprano"; $$->literalValue.fVal = atof($1); }
    | LIT_STRING { $$ = new ASTNode(NODE_LITERAL); $$->literalValue.type = "libretto"; $$->literalValue.sVal = $1; }
    | TOK_TRUE { $$ = new ASTNode(NODE_LITERAL); $$->literalValue.type = "verita"; $$->literalValue.bVal = true; }
    | TOK_FALSE { $$ = new ASTNode(NODE_LITERAL); $$->literalValue.type = "verita"; $$->literalValue.bVal = false; }
    | TOK_ID { $$ = new ASTNode(NODE_ID); $$->idName = $1; }
    | func_call { $$ = new ASTNode(NODE_OTHER); $$->exprType = "unknown"; }
    | '(' expression ')' { $$ = $2; } 
    ;

func_call: 
    TOK_ID '(' args_list ')' { 
        $$ = new ASTNode(NODE_OTHER); 
        $$->idName = $1; 

        if ($3) {
            $$->arguments = *$3; 
            delete $3;          
        }
    }

    | TOK_ID '.' TOK_ID '(' args_list ')' { 
        $$ = new ASTNode(NODE_OTHER); 
        $$->idName = std::string($1) + "." + std::string($3); 
        
        if ($5) {
            $$->arguments = *$5; 
            delete $5; 
        }
    }
    ;

args_list: non_empty_args { $$ = $1; } 
    | { $$ = new std::vector<ASTNode*>(); } ; 

non_empty_args: non_empty_args ',' expression { $1->push_back($3); $$ = $1; }
    | expression { $$ = new std::vector<ASTNode*>(); $$->push_back($1); } ;

control_stmt: if_stmt | while_stmt ;

if_stmt: TOK_IF '(' expression ')' block_pure { $$ = nullptr; } ;

while_stmt: TOK_WHILE '(' expression ')' block_pure { $$ = nullptr; } ;

block_pure: '{' stmt_list_pure '}' { $$ = $2; } ;

class_decl: TOK_CLASS TOK_ID '{' class_body '}' ;

class_body: class_body class_member | ;

class_member: return_type TOK_ID class_member_suffix ;

class_member_suffix: ';' | '(' param_list ')' '{' func_body '}' ;

param_list: non_empty_params | ;

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

func_body: stmt_list_pure ;

var_decl: data_type TOK_ID var_decl_suffix ;

var_decl_suffix: ';' | TOK_ASSIGN expression ';' ;

return_type: data_type | TOK_TYPE_VOID ;

data_type: TOK_TYPE_INT | TOK_TYPE_FLOAT | TOK_TYPE_STRING | TOK_TYPE_BOOL | TOK_ID ;
%%

void yyerror(const char *s) { fprintf(stderr, "Error line %d: %s\n", yylineno, s); exit(1); }
int main() { yyparse(); return 0; }