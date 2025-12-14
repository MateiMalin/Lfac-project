//asta o sa fie parserul, cu regulile de gramatica

%{
#include <stdio.h>
#include <stdlib.h>

extern int yylineno;
int yylex(void);
void yyerror(const char *s);

%}

//token definition
%token TOK_TYPE_INT TOK_TYPE_FLOAT TOK_TYPE_STRING TOK_TYPE_BOOL TOK_TYPE_VOID
%token TOK_CLASS TOK_MAIN TOK_IF TOK_WHILE TOK_PRINT
%token TOK_TRUE TOK_FALSE
%token TOK_ID LIT_INT LIT_FLOAT LIT_STRING
%token TOK_ASSIGN TOK_EQ TOK_NEQ TOK_LEQ TOK_GEQ TOK_AND TOK_OR
%token TOK_INC TOK_DEC TOK_PLUS_ASSIGN

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
    global_elements finale_block
    { printf("Sintassi Corretta! (Syntax Correct!)\n"); }
    ;

global_elements:
    global_elements global_decl
    | /* nimic */
    ;

global_decl:
    class_decl
    | return_type TOK_ID global_decl_suffix
    ;

global_decl_suffix:
    '(' param_list ')' '{' func_body '}'   /* Function definition */
    | var_decl_suffix                      /* Global variable */
    ;

//data types & classes

data_type:
    TOK_TYPE_INT | TOK_TYPE_FLOAT | TOK_TYPE_STRING | TOK_TYPE_BOOL 
    | TOK_ID /* Objects */
    ;

/* primitive types for local variable declarations (disambiguation) */
simple_type:
    TOK_TYPE_INT
    | TOK_TYPE_FLOAT
    | TOK_TYPE_STRING
    | TOK_TYPE_BOOL
    ;

class_decl:
    TOK_CLASS TOK_ID '{' class_body '}'
    ;

class_body:
    class_body class_member
    | /* empty */
    ;

class_member:
    return_type TOK_ID class_member_suffix
    ;

return_type:
    data_type
    | TOK_TYPE_VOID
    ;

class_member_suffix:
    ';' /* field */
    | '(' param_list ')' '{' func_body '}' /* method */
    ;

//functions

param_list:
    non_empty_params
    | /* empty */
    ;

non_empty_params:
    non_empty_params ',' data_type TOK_ID
    | data_type TOK_ID
    ;

func_body:
    var_decl_list stmt_list_pure
    ;

var_decl_list:
    var_decl_list var_decl
    | /* empty */
    ;

var_decl:
    simple_type TOK_ID var_decl_suffix
    ;

var_decl_suffix:
    ';'
    | TOK_ASSIGN expression ';'
    | TOK_PLUS_ASSIGN expression ';'
    ;

//main block

finale_block:
    TOK_MAIN '{' stmt_list_pure '}'
    ;

//statements

stmt_list_pure:
    stmt_list_pure statement
    | /* empty */
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
    if_stmt
    | while_stmt
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
    TOK_ID
    | TOK_ID '.' TOK_ID
    ;

func_call:
    TOK_ID '(' args_list ')'
    | TOK_ID '.' TOK_ID '(' args_list ')'
    ;

args_list:
    non_empty_args | /* empty */
    ;

non_empty_args:
    non_empty_args ',' expression
    | expression
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