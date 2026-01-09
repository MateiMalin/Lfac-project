#!/bin/bash

rm -f lex.yy.c maestro.tab.c maestro.tab.h maestro
bison -d $1.y
flex $1.l
g++ lex.yy.c $1.tab.c -o $1

