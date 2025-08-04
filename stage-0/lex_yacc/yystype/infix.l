%{
#include<stdio.h>
#include "y.tab.h"
%}
%%

[a-z]       { yylval.c = yytext[0]; return ID; }
[+]      { yylval.c = yytext[0]; return ADD; }
[-]      { yylval.c = yytext[0]; return SUB; }
[*]      { yylval.c = yytext[0]; return MUL; }
[/]      { yylval.c = yytext[0]; return DIV; }

[ \t]       ;  // Ignore spaces and tabs
\n          { return '\n'; }
.           ;  // Ignore other characters

%%

yywrap(){
    return 1;
}
