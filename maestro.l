/* aici o sa ii dam inputul si el le transforma in tokenuri ca sa le recunoasca so basically asta e un dictionar */

%{
#include "maestro.tab.h" 
#include <stdio.h>
#include <stdlib.h>
%}


%option noyywrap
%option yylineno

/* aici o sa fie the keywords */
%x COMMENT
%%

"/*"            { BEGIN(COMMENT); }   
<COMMENT>"*/"   { BEGIN(INITIAL); }   
<COMMENT>\n     { ; }                 
<COMMENT>.      { ; }                 
"//".* { ; }

"basso"    {
    yylval.strVal = strdup(yytext); 
    return TOK_TYPE_INT;
}
"soprano"  {
    yylval.strVal = strdup(yytext); 
    return TOK_TYPE_FLOAT;
}
"libretto" {
    yylval.strVal = strdup(yytext); 
    return TOK_TYPE_STRING;
}
"verita"   {
    yylval.strVal = strdup(yytext); 
    return TOK_TYPE_BOOL;
}
"silenzio" {
    yylval.strVal = strdup(yytext); 
    return TOK_TYPE_VOID;
}


"opera"     {return TOK_CLASS;}
"finale"    {return TOK_MAIN;}
"se"        {return TOK_IF;}
"durante"   {return TOK_WHILE;}
"canta"     {return TOK_PRINT;}

"vero"      {return TOK_TRUE;}
"falso"     {return TOK_FALSE;}

[a-zA-Z_][a-zA-Z0-9_]* { 
    yylval.strVal = strdup(yytext); 
    return TOK_ID; 
} // asta pentru numele la variabile 

([0-9]+\.[0-9]+)      { return LIT_FLOAT; }
([0-9]+)               { return LIT_INT; }
\"[^"\n]*\"           { return LIT_STRING; }

":="        { return TOK_ASSIGN; }
"=="        {return TOK_EQ;}
"!="        {return TOK_NEQ;}
"<="        {return TOK_LEQ;}
">="        {return TOK_GEQ;}
"&&"        {return TOK_AND;}
"||"        {return TOK_OR;}
"++"        { return TOK_INC; }          
"--"        { return TOK_DEC; }          
"+="        { return TOK_PLUS_ASSIGN; }  


[+\-*/<>;,(){}.] { return yytext[0]; } /*yytext o sa fie un char* cu lexemul gasit; returnam caracterul */

[ \t\r\n]+ ; /* ignoram whitespace */

. { fprintf(stderr, "Eroare lexica linia %d: %s\n", yylineno, yytext); exit(1); } /* daca o sa avem orice caracter pe care nu il recunoastem, ii dam o eroare */
%%