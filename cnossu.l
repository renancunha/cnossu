%{
    #include "y.tab.h"
    #include "stdio.h"
    static void comment(void);
    extern void yyerror(const char *);
%}

%s HASH
%option yylineno

string          \"([^\"]|\\.)*\"                                                                                                                                                                        
charLit         \'([^\']|\\.)?\'

alfanum                     [a-zA-Z]
dig                         [0-9]
real                        [+-]?([0-9]*[.])?[0-9]+

%%

[ \t]                       ;
[ \n]                       { yylineno = yylineno + 1;} /* Contador de quebras de linhas */
"int"                       {return INT;}
"float"                     {return FLOAT;}
"void"                      {return VOID;}
"if"                        {return IF;}
"else"                      {return ELSE;}
"while"                     {return WHILE;}
"switch"                    {return SWITCH;}
"default" 					{return DEFAULT;}
"case" 						{return CASE;}
"break" 					{return BREAK;}
{string}                    {return STRING_LITERAL;}
{dig}+                      {return NUM;}
{real}                      {return NUM_REAL;}
{alfanum}({alfanum}|{dig})* {return ID;}
"="                         {return ASGN;}
"=="                        {return EQ;}
"!="                        {return NE;}
"||"                        {return LOR;}
"&&"                        {return LAND;}
\/\/.*                      ; /* Expressao regular para capturar barras */
\/\*(.*\n)*.*\*\/           ; /* Expressao regular para capturar varias barras e varios astericos */
.                           {return yytext[0];}

%%


int yywrap(void)
{
    return 1;
}
