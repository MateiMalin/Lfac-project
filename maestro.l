%{
#include <iostream>
#include <string>
#include <vector>
#include "maestro.tab.h"
using namespace std;
%}

%option noyywrap
%option yylineno

%%
"basso"     { yylval.stringValue = strdup(yytext); return TOK_TYPE_INT; }
"soprano"   { yylval.stringValue = strdup(yytext); return TOK_TYPE_FLOAT; }
"libretto"  { yylval.stringValue = strdup(yytext); return TOK_TYPE_STRING; }
"verita"    { yylval.stringValue = strdup(yytext); return TOK_TYPE_BOOL; }
"silenzio"  { yylval.stringValue = strdup(yytext); return TOK_TYPE_VOID; }
"opera"     { return TOK_CLASS; }
"finale"    { return TOK_MAIN; }
"se"        { return TOK_IF; }
"durante"   { return TOK_WHILE; }
"canta"     { return TOK_PRINT; }
"vero"      { yylval.stringValue = strdup(yytext); return TOK_TRUE; }
"falso"     { yylval.stringValue = strdup(yytext); return TOK_FALSE; }
":="        { return TOK_ASSIGN; }
"=="        { return TOK_EQ; }
"++"        { return TOK_INC; }
"--"        { return TOK_DEC; }
[0-9]+\.[0-9]+  { yylval.stringValue = strdup(yytext); return LIT_FLOAT; }
[0-9]+          { yylval.stringValue = strdup(yytext); return LIT_INT; }
\"[^\"]*\"      { yylval.stringValue = strdup(yytext); return LIT_STRING; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.stringValue = strdup(yytext); return TOK_ID; }
[ \t\n]+        ;
\/\/.* ;
[+\-*/<>;,(){}.] { return yytext[0]; }
.               { fprintf(stderr, "Unknown: %s\n", yytext); exit(1); }
%%