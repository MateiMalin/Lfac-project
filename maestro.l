%{
#include <iostream>
#include <string>
#include <vector>
#include "maestro.tab.h"
using namespace std;
%}

%option yylineno

%%

"basso"         { yylval.stringValue = strdup(yytext); return TOK_TYPE_INT; }
"soprano"       { yylval.stringValue = strdup(yytext); return TOK_TYPE_FLOAT; }
"libretto"      { yylval.stringValue = strdup(yytext); return TOK_TYPE_STRING; }
"verita"        { yylval.stringValue = strdup(yytext); return TOK_TYPE_BOOL; }
"silenzio"      { yylval.stringValue = strdup(yytext); return TOK_TYPE_VOID; }
"ensemble"      { return TOK_CLASS; }
"finale"        { return TOK_MAIN; }
"fortissimo"    { return TOK_IF; }
"crescendo"     { return TOK_WHILE; }
"esegui"        { return TOK_PRINT; }
"vero"          { yylval.stringValue = strdup(yytext); return TOK_TRUE; }
"falso"         { yylval.stringValue = strdup(yytext); return TOK_FALSE; }

":="            { return TOK_ASSIGN; }
"+="            { return TOK_PLUS_ASSIGN; }
"-="            { return TOK_MINUS_ASSIGN; }
"*="            { return TOK_MUL_ASSIGN; }
"/="            { return TOK_DIV_ASSIGN; }
"=="            { return TOK_EQ; }
"!="            { return TOK_NEQ; }
"<="            { return TOK_LEQ; }
">="            { return TOK_GEQ; }
"&&"            { return TOK_AND; }
"||"            { return TOK_OR; }
"++"            { return TOK_INC; }
"--"            { return TOK_DEC; }

[0-9]+\.[0-9]+  { yylval.stringValue = strdup(yytext); return LIT_FLOAT; }
[0-9]+          { yylval.stringValue = strdup(yytext); return LIT_INT; }
\"[^\"]*\"      { yylval.stringValue = strdup(yytext); return LIT_STRING; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.stringValue = strdup(yytext); return TOK_ID; }

[ \t]+          ; 
"//".*          ;
\n              ;

.               { return yytext[0]; } 

.               { fprintf(stderr, "Unknown: %s\n", yytext); exit(1); }
%%