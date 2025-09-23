%{
#include<stdio.h>

int float_count=0;
int int_count =0;
%}

DIGIT     [0-9]
EXP       ([eE][+-]?{DIGIT}+)
FLOAT     (({DIGIT}+"."{DIGIT}*|{DIGIT}*"."{DIGIT}+){EXP}?|{DIGIT}+{EXP})
INT       [+-]?{DIGIT}+
%option noyywrap

%%

{INT} {
    printf("INTEGER : %s\n", yytext);
    int_count++;
}
{FLOAT} {
    printf("FFLOAT: %s\n", yytext);
    float_count++;
}

[ \t\n]+  ;  // Ignore whitespace
.         ;  

%%

int main(){
    yylex();
    printf("\nTotal Integers     = %d\n", int_count);
    printf("Total Floats       = %d\n", float_count);
    return 0;
}
