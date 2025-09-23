%{
    #include "y.tab.h"
    #include <stdlib.h>
#include <string.h>
%}

ID [a-zA-Z][a-zA-Z]*

/*** Definition section ***/
%%
{ID}    {
            yylval.c = (char*)malloc(strlen(yytext) + 1);
            strcpy(yylval.c, yytext);
            return ID;
        }
"+"     { return PLUS; }
"-"     { return MINUS; }
"*"     { return MUL; }
"/"     { return DIV; }
[\n]    { return '\n'; }
[ \t]   { /* skip whitespace */ }
.       { printf("Invalid character: %s\n", yytext); }

%%


yywrap(){
    return 1;
}
