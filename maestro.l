// aici o sa ii dam inputul si el le transforma in tokenuri ca sa le recunoasca
//so basically asta e un dictionar

%{
#include <iostream>
#include <string>
#include <maestro.tab.h> //ceva fisier generat de bison
using namespace std;
%}

//aici o sa fie cod de c care se compileaza direct
//maesto.tab.h o sa fie headerul care contine codurile pentru tokenuri, ca sa stie ce sa returneze pentru parser

%option noyywrap //citim dintr-un singur fisier
%option yylineno //retinem numarul liniei ca sa putem afisa erori and all that

//aici o sa fie the keywords

%%

"basso"    {return TOK_TYPE_INT;}
"soprano"  {return TOK_TYPE_FLOAT;}
"libretto" {return TOK_TYPE_STRING;}
"verita"   {return TOK_TYPE_BOOL;}
"silenzio" {return TOK_TYPE_VOID;}

// structure keywords

"opera"     {return TOK_CLASS;}
"finale"    {return TOK_MAIN;}
"se"        {return TOK_IF;}
"durante"   {return TOK_WHILE;}
"canta"     {return TOK_PRINT;}

"vero"      {return TOK_TRUE;}
"falso"     {return TOK_FALSE;}

[a-zA-Z_][a-zA-Z0-9_]* { return TOK_ID; } //asta pentru numele la variabile
[0-9]+                 { return LIT_INT; }
[0-9]+\.[0-9]+         { return LIT_FLOAT; }
\"[^"\n]*\"            {return LIT_STRING;}

// fiecare operator in parte
":="        {return TOK_ASSIGN:}
"=="        {return TOK_EQ;}
"!="        {return TOK_NEQ;}
"<="        {return TOK_LEQ;}
">="        {return TOK_GEQ;}
"&&"        {return TOK_AND;}
"||"        {return TOK_OR;}
"++"        { return TOK_INC; }          
"--"        { return TOK_DEC; }          
"+="        { return TOK_PLUS_ASSIGN; }  


[+\-*/<>;,(){}.] { return yytext[0]; }
//yytext o sa fie un char * cu mathced lexeme si o sa dea return fix la caracterul ala

[\t\r\n]+ ;//ignoram tot ce tine de formatare

. {cerr <<"Eroare Lessicale line"<< yylineno << ": "<<yytext <<endl; exit(1);}
// daca o sa avem orice caracter pe care nu il recunoastem, ii dam o eroare
%%