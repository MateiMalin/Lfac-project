#!/bin/bash

echo "--- Curatare fisiere vechi ---"
rm -f lex.yy.c maestro.tab.c maestro.tab.h maestro

echo "--- Generare Parser (Bison) ---"
bison -d $1.y

echo "--- Generare Lexer (Flex) ---"
flex $1.l

echo "--- Compilare Finala (G++) ---"
g++ lex.yy.c $1.tab.c -o $1

echo "--- GATA! Ruleaza cu: ./$1 < test_3.txt ---"