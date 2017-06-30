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
%token WHILE SWITCH CASE BREAK DEFAULT
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
                | ST_WHILE
                | ST_SWITCH
                | ';'
                ;           

EXP             : EXP   EQ    { parser.stack_push(); } EXP { parser.code_logica(); }                
                | EXP   NE    { parser.stack_push(); } EXP { parser.code_logica(); }
                | EXP   LE    { parser.stack_push(); } EXP { parser.code_logica(); }
                | EXP   GE    { parser.stack_push(); } EXP { parser.code_logica(); }
                | EXP   LT    { parser.stack_push(); } EXP { parser.code_logica(); }
                | EXP   GT    { parser.stack_push(); } EXP { parser.code_logica(); }
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

ST_WHILE        : { parser.code_while_inicio(); } WHILE '(' EXP { parser.code_while(); } ')' CORPO_WHILE 
                ;

CORPO_WHILE     : ESCOPO { parser.code_while_fim(); }
                | CORPO { parser.code_while_fim(); }
                ;

ST_SWITCH       : { parser.code_switch_inicio(); } SWITCH '(' EXP ')' '{' CORPO_SWITCH '}' { parser.code_switch_fim(); }
                ;

CORPO_SWITCH    : CASES    
                | CASES ST_DEFAULT
                ;

CASES           : { parser.code_switch_label_case(); } CASE NUM { parser.code_switch_case(); } ':' SWITCHEXP BREAKSTMT
                | 
                ;
BREAKSTMT       : { parser.code_switch_break(); } BREAK ';' CASES
                | CASES 
                ;

ST_DEFAULT      : { parser.code_switch_label_case(); } DEFAULT ':' SWITCHEXP DE  
                ;

DE              : BREAK ';'
                |
                ;

SWITCHEXP       : ESCOPO
                | CORPO
                ;

ST_DECLARACAO   : TIPO { parser.seta_tipo(); } ID { parser.declara_var(); }  IDS 
                ;

IDS             : ';'
                | ','  ID  IDS 
                ;

ST_ATRIBUICAO   : ID { parser.verifica_existe(); parser.stack_push(); }  ASGN  { parser.stack_push(); } EXP  { parser.code_atribuicao(); } ';' 
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
