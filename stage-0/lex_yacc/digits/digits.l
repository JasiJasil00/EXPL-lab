%{
    #include"y.tab.h"
    #include<stdio.h>
    #include<stdlib.h>
%}

number [0-9]+

%%

{number} {
            yylval = atoi(yytext);
            return DIGITS;
         }
[ \t]     ; // ignore whitespace

\n        return '\n';

.   return *yytext;

%%

yywrap(){
    return 1;
}
