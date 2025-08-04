%{
#include<stdio.h>

#define ID 1  //identifier token
#define ER 2  //error token
%}

%option noyywrap

%%

[a-zA-Z][a-zA-Z]*[0-9] return ID;

.|\n                      return ER;

%%

int main(){
    int token = yylex();
    if(token == ID){
        printf("Acceptable\n");
    }else if(token == ER){
        printf("Unacceptable\n");
    }
    return 0;
}
