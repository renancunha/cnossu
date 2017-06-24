#!/bin/bash
yacc -d -Wnone  cnossu.y
lex cnossu.l
g++ -std=c++11 y.tab.c lex.yy.c parser.cpp -o trab
./trab < in.c
rm trab
