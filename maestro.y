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

// --- TASK IV: VALUE WRAPPER ---
class Value {
public:
    std::string type; // "basso", "soprano", "libretto", "verita"
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
        Value value; // Stores the evaluated value [cite: 4]

        Symbol(std::string _name, std::string _type, std::string _kind) { 
            name = _name; type = _type; kind = _kind; 
            // Initialize default values based on type [cite: 1, 4]
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

// --- TASK IV: AST IMPLEMENTATION ---
enum NodeType { NODE_LITERAL, NODE_ID, NODE_OP, NODE_ASSIGN, NODE_PRINT, NODE_OTHER };

class ASTNode {
public:
    NodeType nodeType;
    std::string op;
    Value literalValue;
    std::string idName;
    std::string exprType;
    ASTNode *left, *right;

    ASTNode(NodeType t) : nodeType(t), left(nullptr), right(nullptr) {}

    Value evaluate() {
        Value res;
        switch(nodeType) {
            case NODE_LITERAL: return literalValue; // 
            case NODE_ID: {
                Symbol* s = currentScope->findSymbol(idName);
                return s ? s->value : res; // 
            }
            case NODE_ASSIGN: {
                Value val = right->evaluate();
                Symbol* s = currentScope->findSymbol(idName);
                if (s) s->value = val; // 
                return val;
            }
            case NODE_PRINT: {
                Value val = left->evaluate();
                if (val.type == "basso") std::cout << val.iVal << "\n";
                else if (val.type == "soprano") std::cout << val.fVal << "\n";
                else if (val.type == "verita") std::cout << (val.bVal ? "vero" : "falso") << "\n";
                return val; // 
            }
            case NODE_OP: {
                Value l = left->evaluate(); Value r = right->evaluate();
                res.type = l.type;
                if (op == "+") {
                    if (l.type == "basso") res.iVal = l.iVal + r.iVal;
                    else res.fVal = l.fVal + r.fVal;
                } else if (op == "==") {
                    res.type = "verita";
                    if (l.type == "basso") res.bVal = (l.iVal == r.iVal);
                    else res.bVal = (l.fVal == r.fVal);
                }
                return res;
            }
            case NODE_OTHER: {
                res.type = exprType; // Returns default for type 
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
%token TOK_CLASS TOK_MAIN TOK_IF TOK_WHILE TOK_PRINT TOK_ASSIGN TOK_EQ TOK_NEQ TOK_LEQ TOK_GEQ TOK_AND TOK_OR TOK_INC TOK_DEC TOK_PLUS_ASSIGN

%type <astNode> expression assignment_stmt print_stmt statement
%type <astList> stmt_list_pure
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
        for (ASTNode* node : *$4) if (node) node->evaluate(); // Evaluate Main Block [cite: 1, 48]
        currentScope->printTable(tableFile); currentScope = currentScope->parent; 
    }
    ;

stmt_list_pure: stmt_list_pure statement { $1->push_back($2); $$ = $1; } | { $$ = new std::vector<ASTNode*>(); } ;

statement: assignment_stmt { $$ = $1; } | print_stmt { $$ = $1; } | func_call ';' { $$ = nullptr; } | control_stmt { $$ = nullptr; } ;

assignment_stmt: TOK_ID TOK_ASSIGN expression ';' { 
        $$ = new ASTNode(NODE_ASSIGN); $$->idName = $1; $$->right = $3; // Construction [cite: 1, 52]
    } ;

print_stmt: TOK_PRINT '(' expression ')' ';' { 
        $$ = new ASTNode(NODE_PRINT); $$->left = $3; // Construction [cite: 1, 61]
    } ;

expression: expression '+' expression { $$ = new ASTNode(NODE_OP); $$->op = "+"; $$->left = $1; $$->right = $3; }
    | LIT_INT { $$ = new ASTNode(NODE_LITERAL); $$->literalValue.type = "basso"; $$->literalValue.iVal = atoi($1); }
    | TOK_TRUE { $$ = new ASTNode(NODE_LITERAL); $$->literalValue.type = "verita"; $$->literalValue.bVal = true; }
    | TOK_ID { $$ = new ASTNode(NODE_ID); $$->idName = $1; }
    | func_call { $$ = new ASTNode(NODE_OTHER); $$->exprType = "unknown"; }
    | '(' expression ')' { $$ = $2; } ;

func_call: TOK_ID '(' args_list ')' { $$ = nullptr; } ;
args_list: non_empty_args | ;
non_empty_args: non_empty_args ',' expression | expression ;
control_stmt: if_stmt | while_stmt ;
if_stmt: TOK_IF '(' expression ')' block_pure ;
while_stmt: TOK_WHILE '(' expression ')' block_pure ;
block_pure: '{' stmt_list_pure '}' ;
class_decl: TOK_CLASS TOK_ID '{' class_body '}' ;
class_body: class_body class_member | ;
class_member: return_type TOK_ID class_member_suffix ;
class_member_suffix: ';' | '(' param_list ')' '{' func_body '}' ;
param_list: non_empty_params | ;
non_empty_params: non_empty_params ',' data_type TOK_ID | data_type TOK_ID ;
func_body: var_decl_list stmt_list_pure ;
var_decl_list: var_decl_list var_decl | ;
var_decl: data_type TOK_ID var_decl_suffix ;
var_decl_suffix: ';' | TOK_ASSIGN expression ';' ;
return_type: data_type | TOK_TYPE_VOID ;
data_type: TOK_TYPE_INT | TOK_TYPE_FLOAT | TOK_TYPE_STRING | TOK_TYPE_BOOL | TOK_ID ;
%%

void yyerror(const char *s) { fprintf(stderr, "Error line %d: %s\n", yylineno, s); exit(1); }
int main() { yyparse(); return 0; }