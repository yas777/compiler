%option yylineno
%top{
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "storetokens.hh"
#include "parser.tab.h"
}
/* to-do comments*/
ws [ \t\n]+
letter [_A-Za-z]
digit [0-9]
number {digit}+
id {letter}({letter}|{digit})*
real {digit}+(\.{digit}+)?
comment \/\/[^\n]*
arithop [+\-*\/]
assign_op "="

less_than "<"
greater_than ">"
less_than_equal "<="
greater_than_equal ">="
equal "=="
not_equal "!="
and "&&"
or "||"
not "!"

metachar  [{}(),?:;]
/* {real} {store_token_name("FNUM",yytext,yylineno); return DOUBLE_NUMBER;}*/
%%
{comment} {/*no action, no return*/}
{ws} {/*no action, no return*/}
{number} {store_token_name("NUM",yytext,yylineno);yylval.integer_value =atoi(yytext);return INTEGER_NUMBER;}
{real} {store_token_name("FNUM",yytext,yylineno);yylval.double_value = atof(yytext);return DOUBLE_NUMBER;}
int {store_token_name("INTEGER",yytext,yylineno);return INTEGER;}
float {store_token_name("FLOAT",yytext,yylineno);return FLOAT;}
void {store_token_name("VOID",yytext,yylineno);return VOID;}
return {store_token_name("RETURN",yytext,yylineno);return RETURN;}
do {store_token_name("DO",yytext,yylineno);return DO;}
while {store_token_name("WHILE", yytext,yylineno);return WHILE;}
if {store_token_name("IF",yytext,yylineno);return IF;}
else {store_token_name("ELSE",yytext,yylineno);return ELSE;}
print {store_token_name("PRINT", yytext, yylineno);return PRINT;}

main {store_token_name("NAME", yytext, yylineno);yylval.string_value = new std::string(yytext); *(yylval.string_value);return NAME;}

{id} {store_token_name("NAME",yytext,yylineno); yylval.string_value = new std::string(yytext); *(yylval.string_value) += "_" ;return NAME;}

{arithop} {store_token_name("ARITHOP",yytext,yylineno);return yytext[0];}
{assign_op} {store_token_name("ASSIGN_OP",yytext,yylineno); return ASSIGN;}
{metachar} {store_token_name("META CHAR",yytext,yylineno); return yytext[0];}


{less_than} {store_token_name("LESS_THAN",yytext,yylineno);return LESS_THAN;}
{greater_than} {store_token_name("GREATER_THAN",yytext,yylineno); return GREATER_THAN;}
{less_than_equal} {store_token_name("LESS_THAN_EQUAL",yytext,yylineno);return LESS_THAN_EQUAL;}
{greater_than_equal} {store_token_name("GREATER_THAN_EQUAL",yytext,yylineno);return GREATER_THAN_EQUAL;}
{equal} {store_token_name("EQUAL",yytext,yylineno);return EQUAL;}
{not_equal} {store_token_name("NOT_EQUAL",yytext,yylineno);return NOT_EQUAL;}
{and} {store_token_name("AND",yytext,yylineno);return AND;}
{or} {store_token_name("OR",yytext,yylineno);return OR;}
{not} {store_token_name("NOT",yytext,yylineno);return NOT;}

. {printf("cs316: Error - No matched tokens found.\n");exit(0);}
%%