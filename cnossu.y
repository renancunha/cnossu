%{
#include <bits/stdc++.h>
#include "header.h"
#include "parser.hh"

using namespace std;

Parser parser;

%}

%define parse.error verbose

%token INT FLOAT VOID STRING_LITERAL
%token IF ELSE
%token WHILE SWITCH 
%token NUM NUM_REAL ID
%start F_MAIN

%right ASGN 
%left LOR
%left LAND
%left EQ NE 
%left LE GE LT GT
%left '+' '-' 
%left '*' '/'

%%

F_MAIN          : TIPO ID '(' ')' ESCOPO
                ;

ESCOPO          : '{' CORPOS '}' |
                ;

CORPOS          : CORPO  CORPOS
                |
                ;

CORPO           : ST_DECLARACAO    
                | ST_ATRIBUICAO  
                | ST_IF
                | ';'
                ;           

EXP             : EXP   EQ    { parser.stack_push(); } EXP { parser.code_logica(); }                
                | EXP   NE    { parser.stack_push(); } EXP { parser.code_logica(); }
                | EXP   LOR   { parser.stack_push(); } EXP { parser.code_logica(); }
                | EXP   LAND  { parser.stack_push(); } EXP { parser.code_logica(); }
                | EXP   '+'   { parser.stack_push(); } EXP { parser.code_algebrica(); }
                | EXP   '-'   { parser.stack_push(); } EXP { parser.code_algebrica(); }
                | EXP   '*'   { parser.stack_push(); } EXP { parser.code_algebrica(); }
                | EXP   '/'   { parser.stack_push(); } EXP { parser.code_algebrica(); }
                | '(' EXP ')'
                | ID          { parser.existe_var(); parser.stack_push(); }
                | NUM         { parser.stack_push(); }
                | NUM_REAL    { parser.stack_push(); }
                ;

ST_IF           : IF '(' EXP { parser.code_if(); } ')'  ESCOPO ST_ELSE 
                ;

ST_ELSE         : ELSE { parser.code_if_else(); } ESCOPO 
                | { parser.code_if_fim(); }
                ;

ST_DECLARACAO   : TIPO { parser.seta_tipo(); } ID { parser.declara_var(); }  IDS 
                ;

IDS             : ';'
                | ','  ID  IDS 
                ;

ST_ATRIBUICAO   : ID { parser.stack_push(); }  ASGN  { parser.stack_push(); } EXP  { parser.code_atribuicao(); } ';' 
                ;

TIPO            : INT
                | FLOAT
                | VOID
                ;

%%

int main(void)
{
    if(!yyparse())
    {
        printf("Analise lexica/sintatica: OK\n");
        parser.print_int_code();
    }
    else
    {
        printf("Analise lexica/sintatica: DEU BRETE\n");
    }
    return 0;
}

void yyerror(const char *s)
{
    fprintf(stderr, "%s => linha: %d\n", s, yylineno);
}
